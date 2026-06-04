# Advise - Backend resilience technology choices

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | June 2026 |
| **Version** | 1.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Advise (second of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - A native Docker healthcheck detects the hung API
3. Chapter 2 - Compose replicas behind NGINX give failover on one Pi
4. Chapter 3 - Light native settings cover the remaining failures
5. Conclusion
6. Recommendation
7. References
8. Appendix

---

## Introduction

### Context

City Sim is a miniature smart city built by our team, The Embedded Alliance, for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Five students each build one physical tile, and every tile sends its sensor data to one shared backend that we maintain. The backend is a FastAPI application with a PostgreSQL database, in Docker, on one Raspberry Pi on the HvA network (145.92.8.137, port 80). If the backend goes down, the whole city goes down. A short reminder of who builds what:

- Matin Khajehfard (me) - parking tile and the shared backend.
- Gurpreet Singh, who builds the streetlight and speed camera tile.
- Thijmen Walter, who builds the railroad crossing tile.
- Wesley Oerlmans, who builds the traffic light tile.
- Betul Aydin - scrum master and backend support.

This is the **Advise** outcome for Learning Goal 1. It follows the Analysis document, which found seven failure modes and named the hung API process as the biggest gap. The Analysis did the research; the Advise document picks the technologies and weighs the alternatives. The next document, Design, turns the chosen technologies into an architecture, and the Realise document builds and tests it.

We write for a technical audience: the City Sim development team (the embedded and backend engineers) and the technical lead on the client side. A detailed, internal, technical document fits that audience because they are the people who will run the chosen technologies, so they need the option comparison and the reasons in full, not a summary for the public or a teacher. The author is the junior backend developer who owns the shared backend, so he is the one who has to live with the maintenance burden of whatever we choose, which is why the sustainability criterion weighs so heavily below. The client is Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented at the mayor delivery by Mats Otten; the client sets the goal but is not the technical audience, so the document stays Internal. The version is 1.1 because we revised it after the second round of writing feedback from mister mayor Gerald Stap.

### The decision we have to make

We have to choose, with reasons, how to make the backend auto-recover within 5 seconds without buying new hardware or adding tools the team cannot maintain.

### Requirements carried from the Analysis

We keep the same requirements so our choices stay traceable. The HvA brief says the backend is shared and must serve all five tiles at once, and that the city works as a whole (Hogeschool van Amsterdam, 2026). At the Sprint 3 mayor delivery Mats Otten asked us to cluster the backend so it auto-recovers within 5 seconds and to keep it sustainable rather than a one-off fix (Otten, 2026), and mister mayor Gerald Stap asked us to add load balancing, failover, and the ability to scale up and down (Stap, 2026).

### Selection criteria

We score every option against five criteria that come straight from those requirements and from our hardware reality:

1. **Recovery speed** - can it bring the API back within 5 seconds?
2. **Failover** - does the city stay online while one part restarts?
3. **Sustainability** - can a five-student team keep it running after the sprint?
4. **Fits one Raspberry Pi** - does it run on limited ARM hardware without heavy overhead?
5. **Reuse** - does it build on tools we already use (Docker, FastAPI, SQLAlchemy)?

### Main question

How should we make the City Sim backend auto-recover within 5 seconds with technologies a five-student team can sustain?

### Sub-questions

1. How should we detect a hung API?
2. How should we run replicas and balance load on one Raspberry Pi?
3. How should we handle the remaining failures (memory, database drop, overload, disk)?

We work through the three sub-questions in order. For each decision we list the realistic options, score them against the five criteria, and pick one with a justification, so the choice is never hidden inside the question. We base the options on the Docker, NGINX, and SQLAlchemy documentation and on Nygard (2018).

---

## Chapter 1 - A native Docker healthcheck detects the hung API

The first decision answers how we detect a hung API. The Analysis showed that `restart: always` only catches a process that exits, while a hung API keeps running but stops answering, so Docker never restarts it (Nygard, 2018). We need active detection of behaviour, not just liveness, and we have three realistic ways to get it.

The table below lists the three options and scores each against the criteria that matter for detection: recovery speed, sustainability, fit on the Pi, and reuse of tools we already run.

| Option | What it is | Recovery | Sustainable | Fits Pi | Reuse |
|--------|-----------|----------|-------------|---------|-------|
| A. Docker healthcheck on `/health` | Docker calls `/health` on a schedule, marks the container unhealthy, restarts it | Within 5s | Yes, native | Yes, no extra service | Yes |
| B. External monitor (Prometheus + blackbox, or Uptime Kuma) | A separate service polls the API and alerts or triggers a restart | Slower, needs extra wiring to act | Extra service to maintain | Adds memory load to the Pi | Partly |
| C. Custom watchdog script | A bash or Python loop that pings the API and runs `docker restart` | Depends on the script | A one-off the team must own | Light | No |

We choose **option A, the Docker healthcheck on `/health`**. A healthcheck is native to the tool we already use, so there is nothing extra to install or keep alive, and Docker restarts the unhealthy container directly, which keeps recovery inside the 5 second target (Docker Inc., 2024), verified in the Analysis appendix and tested in the Realise. Option B adds a monitoring service that itself can fail and does not restart anything on its own without more glue. Option C is exactly the one-off fix Mats warned us against, so the watchdog script fails the sustainability criterion. The `/health` endpoint already exists in `main.py`, which means option A also scores best on reuse, so the choice lines up with both the speed and the sustainability the client asked for.

### Sub-conclusion

A Docker healthcheck on `/health` is the sustainable, native way to detect a hung API and trigger a restart within 5 seconds, so it is the chosen option for detection.

---

## Chapter 2 - Compose replicas behind NGINX give failover on one Pi

The second decision answers how we run replicas and balance load on one Raspberry Pi. A healthcheck restarts a sick container, but during that restart a single container leaves the city offline, and mister mayor Gerald Stap asked for load balancing and failover. Failover means running more than one API replica and sending traffic to a healthy one, and there are three platforms that can do it.

The table below sets the three platforms against the criteria that matter here: failover, sustainability for a five-student team, fit on one Pi, and reuse.

| Option | What it is | Failover | Sustainable | Fits Pi | Reuse |
|--------|-----------|----------|-------------|---------|-------|
| A. Docker Compose replicas behind NGINX | Run `api` with several replicas, NGINX in front routes to a healthy one | Yes | Yes, light | Yes | Yes, we use Compose |
| B. Docker Swarm | Docker's own orchestrator with built-in routing | Yes | More moving parts | Heavier | Partly |
| C. Kubernetes (k3s) | A full orchestrator on the Pi | Yes | High maintenance for 5 students | Heavy for one ARM board | No |

We choose **option A, Docker Compose replicas behind NGINX**. We already define the whole stack in `docker-compose.yml`, so adding replicas with `deploy.replicas` and an NGINX reverse proxy is a small step, not a new platform. NGINX is a proven, lightweight load balancer that can route around an unhealthy upstream, which gives the failover mister mayor Gerald Stap asked for (NGINX, 2024). Scaling up and down is one command, `docker compose up --scale api=N`, which answers the upscale and downscale request. Swarm and Kubernetes both give failover too, but they add an orchestration layer that a five-student team would have to learn and maintain after the sprint. On a single Raspberry Pi that overhead is not justified, so options B and C fail the sustainability and "fits one Pi" criteria.

We are honest about one limit: all replicas share one PostgreSQL database on the same Pi, so the redundancy sits at the application tier, not the database tier (Kleppmann, 2017). Database resilience is the subject of Learning Goal 2.

### Sub-conclusion

Docker Compose replicas behind an NGINX reverse proxy give failover and easy scaling on one Pi while staying inside tools we already run, so they are the chosen platform.

---

## Chapter 3 - Light native settings cover the remaining failures

The third decision answers how we handle the remaining failures the Analysis listed: memory pressure, a dropped database connection, request overload, and a full disk. None of these is large enough to need its own platform, so for each one we weigh the native option against the hand-rolled alternative and pick the native one, to keep the maintenance burden low.

For memory pressure (failure 4) we choose Docker's `deploy.resources.limits.memory` per container (Docker Inc., 2024). The limit is native and stops one container from starving the other, while the alternative of tuning the Linux OOM score by hand is fragile and not sustainable. For a dropped database connection (failure 3) we choose SQLAlchemy's `pool_pre_ping=True` together with `pool_recycle` (SQLAlchemy, 2024), which checks a connection before use and reconnects a dead one with no extra service, where the alternative of catching errors and retrying by hand in every endpoint spreads the logic across the code and is easy to get wrong.

For request overload (failure 6) we choose NGINX `limit_req` at the edge over an in-app limiter such as SlowAPI (NGINX, 2024). Because NGINX is already our load balancer, the rate limit lives in one place and stops a flood before it ever reaches the API or the database, which directly addresses the lockup we already had when Gurpreet's ESP32 sent parking updates too fast. For a full disk (failure 7) we choose a scheduled cleanup that deletes old rows from `sensor_readings`, run by a small scheduled job, over storing history forever and over adding a separate time-series database, which would be over-engineering for a school project. Finally, for the restart policy we advise `restart: unless-stopped` over `restart: always`: both auto-recover, but `unless-stopped` lets us stop a container for maintenance without Docker fighting us, which is friendlier for the team.

### Sub-conclusion

Each remaining failure has a light, native answer: memory limits, `pool_pre_ping`, NGINX rate limiting, scheduled history cleanup, and `unless-stopped`. None of the answers adds a tool the team cannot maintain, so they are the chosen settings.

---

## Conclusion

We set out to answer how we should make the City Sim backend auto-recover within 5 seconds with technologies a five-student team can sustain.

First, a Docker healthcheck on `/health` is the native, sustainable way to detect a hung API and restart it inside the target. Second, Docker Compose replicas behind an NGINX reverse proxy give the failover and the easy scaling mister mayor Gerald Stap asked for, without the orchestration overhead of Swarm or Kubernetes. Third, the remaining failures each get a light native setting: memory limits, SQLAlchemy `pool_pre_ping`, NGINX `limit_req`, a scheduled cleanup, and `restart: unless-stopped`.

Together the three answers form one stack where every part is native to a tool we already run. So the answer to the main question is to make the backend auto-recover with a Docker healthcheck on `/health`, Compose replicas behind NGINX, and the five light settings, because every one of those choices scores well on recovery speed, failover, sustainability, fit on one Raspberry Pi, and reuse. We deliberately rejected external monitors, Swarm, and Kubernetes because they add maintenance a five-student team cannot carry after the sprint, which is the opposite of the sustainable solution the client asked for.

---

## Recommendation

We recommend the Design document work out this chosen stack into a concrete architecture:

1. An NGINX reverse proxy as the single entry on port 80, balancing across the API replicas.
2. The `api` service with two or more replicas, each with a Docker healthcheck on `/health` (interval 5s, timeout 3s, 1 retry) and a memory limit.
3. The `db` service unchanged except for a memory limit.
4. SQLAlchemy configured with `pool_pre_ping` and `pool_recycle`.
5. NGINX `limit_req` and the history cleanup job as priority-2 items if time allows.

The Realise document then builds this, tests recovery against the 5 second requirement with a stress, load, and soak test, ends with a user test, and hands the result to the team for maintenance.

---

## References

- Docker Inc. (2024). *Compose Deploy Specification: resources*. [Online]. Retrieved June 5, 2026, from https://docs.docker.com/reference/compose-file/deploy/
- Docker Inc. (2024). *Dockerfile reference: HEALTHCHECK instruction*. [Online]. Retrieved June 5, 2026, from https://docs.docker.com/reference/dockerfile/#healthcheck
- Hogeschool van Amsterdam. (2026). *City Sim project brief: base requirements (General, Embedded, Back-end)*. Studio Smart Cities, HvA. [Print].
- Kleppmann, M. (2017). *Designing data-intensive applications*. O'Reilly Media. [Print].
- NGINX. (2024). *Using nginx as HTTP load balancer*. [Online]. Retrieved June 5, 2026, from https://nginx.org/en/docs/http/load_balancing.html
- Nygard, M. T. (2018). *Release It! Design and deploy production-ready software* (2nd ed.). Pragmatic Bookshelf. [Print].
- Otten, M. (2026, May 20). *Sprint 4 mayor delivery feedback (Mats)*. Hogeschool van Amsterdam. [Verbal, offline].
- SQLAlchemy. (2024). *Connection pooling: Dealing with disconnects (pool_pre_ping)*. [Online]. Retrieved June 5, 2026, from https://docs.sqlalchemy.org/en/20/core/pooling.html
- Stap, G. (2026, May 20). *Sprint 4 feedback on Smart City deliverables (mister mayor Gerald Stap)*. Hogeschool van Amsterdam. [Verbal, offline].

---

## Appendix

### Appendix A - Decision summary

The table below gathers every decision from the three chapters with the chosen option, the rejected alternatives, and the main reason, so a reviewer can check the whole stack at a glance. Each row is argued in full in its chapter.

| Decision | Chosen | Rejected | Main reason |
|----------|--------|----------|-------------|
| Detect hung API | Docker healthcheck on `/health` | External monitor, custom script | Native, restarts within 5s, sustainable |
| Replicas + load balancing | Compose replicas + NGINX | Swarm, Kubernetes | Light, failover, fits one Pi |
| Memory | `deploy.resources.limits.memory` | Manual OOM tuning | Native, stops starvation |
| DB drop | `pool_pre_ping` + `pool_recycle` | Per-endpoint retry | Cheap, central |
| Overload | NGINX `limit_req` | SlowAPI in app | One choke point at the edge |
| Disk | Scheduled `sensor_readings` cleanup | Keep forever, time-series DB | Minimal, no new service |
| Restart policy | `unless-stopped` | `always` | Friendlier for maintenance |

### Appendix B - Use of AI

We used an AI assistant (Claude) as a writing aid for this document. It helped restructure the text to the agreed feedback standard, check the APA formatting and the in-text citations, and rephrase passages for clarity. It did not produce the engineering work or the measured results: the architecture, the choices, the code, and the test numbers are our own and were reviewed by the author, who is responsible for the content.
