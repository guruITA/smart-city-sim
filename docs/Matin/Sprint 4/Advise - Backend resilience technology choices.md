# Advise - Backend resilience technology choices

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | May 2026 |
| **Version** | 0.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Advise (second of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - How should we detect a hung API
3. Chapter 2 - How should we run replicas and balance load on one Raspberry Pi
4. Chapter 3 - How should we handle the remaining failures
5. Conclusion
6. Recommendation
7. References
8. Appendix

---

## Introduction

### Context

City Sim is a miniature smart city built by our team, The Embedded Alliance, for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Five students each build one physical tile, and every tile sends its sensor data to one shared backend that I maintain. The backend is a FastAPI application with a PostgreSQL database, in Docker, on one Raspberry Pi on the HvA network (145.92.8.137, port 80). If that backend goes down, the whole city goes down. A short reminder of who builds what:

- Matin Khajehfard (me) - parking tile and the shared backend.
- Gurpreet Singh - streetlight and speed camera tile.
- Thijmen Walter - railroad crossing tile.
- Wesley Oerlmans - traffic light tile.
- Betul Aydin - scrum master and backend support.

This is the **Advise** outcome for Learning Goal 1. It follows the Analysis document, which found seven failure modes and named the hung API process as the biggest gap. The Analysis did the research; this Advise document picks the technologies and weighs the alternatives. The next document, Design, turns the chosen technologies into an architecture, and the Realise document builds and tests it. We write at the level of a real engineering team, so a new technical colleague could follow our reasoning.

### The decision we have to make

We have to choose, with reasons, how to make the backend auto-recover within 5 seconds without buying new hardware or adding tools the team cannot maintain.

### Requirements carried from the Analysis

We keep the same requirements so our choices stay traceable.

- HvA: the backend is shared and must serve all five tiles at once; the city works as a whole.
- Mayor (Mats, Sprint 3): cluster the backend so it auto-recovers within 5 seconds; keep it sustainable, not a one-off fix.
- Mayor (Gerald, Sprint 3): add load balancing, failover, and the ability to scale up and down.

### Selection criteria

We score every option against five criteria that come straight from those requirements and from our hardware reality:

1. **Recovery speed** - can it bring the API back within 5 seconds?
2. **Failover** - does the city stay online while one part restarts?
3. **Sustainability** - can a five-student team keep it running after the sprint?
4. **Fits one Raspberry Pi** - does it run on limited ARM hardware without heavy overhead?
5. **Reuse** - does it build on tools we already use (Docker, FastAPI, SQLAlchemy)?

### Main question

Which technologies and approaches should we use to make the City Sim backend auto-recover within 5 seconds, and why?

### Sub-questions

1. How should we detect a hung API?
2. How should we run replicas and balance load on one Raspberry Pi?
3. How should we handle the remaining failures (memory, database drop, overload, disk)?

### Method

For each decision we list the realistic options, score them against the five criteria, and pick one with a justification. We base the options on the Docker, NGINX, and SQLAlchemy documentation and on Nygard (2018).

---

## Chapter 1 - How should we detect a hung API

### Context

The Analysis showed that `restart: always` only catches a process that exits. A hung API keeps running but stops answering, so Docker never restarts it. We need active detection of behaviour, not just liveness.

### Options

| Option | What it is | Recovery | Sustainable | Fits Pi | Reuse |
|--------|-----------|----------|-------------|---------|-------|
| A. Docker healthcheck on `/health` | Docker calls `/health` on a schedule, marks the container unhealthy, restarts it | Within 5s | Yes, native | Yes, no extra service | Yes |
| B. External monitor (Prometheus + blackbox, or Uptime Kuma) | A separate service polls the API and alerts or triggers a restart | Slower, needs extra wiring to act | Extra service to maintain | Adds memory load to the Pi | Partly |
| C. Custom watchdog script | A bash or Python loop that pings the API and runs `docker restart` | Depends on the script | A one-off the team must own | Light | No |

### Choice and justification

We choose **option A, the Docker healthcheck on `/health`**. It is native to the tool we already use, so there is nothing extra to install or keep alive. It restarts the unhealthy container directly, which keeps recovery inside the 5 second target (verified in the Analysis appendix and to be tested in the Realise). Option B adds a monitoring service that itself can fail and does not restart anything on its own without more glue. Option C is exactly the one-off fix Mats warned us against, so it fails the sustainability criterion. The `/health` endpoint already exists in `main.py`, which means option A also scores best on reuse.

### Sub-conclusion

A Docker healthcheck on `/health` is the sustainable, native way to detect a hung API and trigger a restart within 5 seconds.

---

## Chapter 2 - How should we run replicas and balance load on one Raspberry Pi

### Context

A healthcheck restarts a sick container, but during that restart a single container leaves the city offline. Gerald asked for load balancing and failover. That means running more than one API replica and sending traffic to a healthy one.

### Options

| Option | What it is | Failover | Sustainable | Fits Pi | Reuse |
|--------|-----------|----------|-------------|---------|-------|
| A. Docker Compose replicas behind NGINX | Run `api` with several replicas, NGINX in front routes to a healthy one | Yes | Yes, light | Yes | Yes, we use Compose |
| B. Docker Swarm | Docker's own orchestrator with built-in routing | Yes | More moving parts | Heavier | Partly |
| C. Kubernetes (k3s) | A full orchestrator on the Pi | Yes | High maintenance for 5 students | Heavy for one ARM board | No |

### Choice and justification

We choose **option A, Docker Compose replicas behind NGINX**. We already define the whole stack in `docker-compose.yml`, so adding replicas with `deploy.replicas` and an NGINX reverse proxy is a small step, not a new platform. NGINX is a proven, lightweight load balancer that can route around an unhealthy upstream, which gives the failover Gerald asked for (NGINX, 2024). Scaling up and down is one command, `docker compose up --scale api=N`, which answers the "upscale and downscale" request. Swarm and Kubernetes both give failover too, but they add an orchestration layer that a five-student team would have to learn and maintain after the sprint. On a single Raspberry Pi that overhead is not justified, so options B and C fail the sustainability and "fits one Pi" criteria.

We are honest about one limit: all replicas share one PostgreSQL database on the same Pi, so this is redundancy at the application tier, not the database tier. Database resilience is the subject of Learning Goal 2.

### Sub-conclusion

Docker Compose replicas behind an NGINX reverse proxy give failover and easy scaling on one Pi, while staying inside tools we already run.

---

## Chapter 3 - How should we handle the remaining failures

### Context

The Analysis listed four more failures: memory pressure, a dropped database connection, request overload, and a full disk. We choose a light technology for each.

### Options and choices

**Memory pressure (failure 4).** We choose Docker's `deploy.resources.limits.memory` per container (Docker Inc., 2024b). It is native and stops one container from starving the other. The alternative, tuning the Linux OOM score by hand, is fragile and not sustainable.

**Dropped database connection (failure 3).** We choose SQLAlchemy's `pool_pre_ping=True` together with `pool_recycle` (SQLAlchemy, 2024). It checks a connection before use and reconnects a dead one, with no extra service. The alternative, catching errors and retrying by hand in every endpoint, spreads the logic across the code and is easy to get wrong.

**Request overload (failure 6).** We choose NGINX `limit_req` at the edge over an in-app limiter such as SlowAPI. Because NGINX is already our load balancer, the rate limit lives in one place and stops a flood before it ever reaches the API or the database. This directly addresses the lockup we already had when Gurpreet's ESP32 sent parking updates too fast.

**Full disk (failure 7).** We choose a scheduled cleanup that deletes old rows from `sensor_readings`, run by a small scheduled job. We prefer this over storing history forever and over adding a separate time-series database, which would be over-engineering for a school project.

**Restart policy.** We advise `restart: unless-stopped` over `restart: always`. Both auto-recover, but `unless-stopped` lets us stop a container for maintenance without Docker fighting us, which is friendlier for the team.

### Sub-conclusion

Each remaining failure has a light, native answer: memory limits, `pool_pre_ping`, NGINX rate limiting, scheduled history cleanup, and `unless-stopped`. None of them adds a tool the team cannot maintain.

---

## Conclusion

This Advise answers the main question. To make the backend auto-recover within 5 seconds we will use a Docker healthcheck on `/health` to detect a hung API, Docker Compose replicas behind an NGINX reverse proxy for failover and scaling, memory limits to stop the out-of-memory killer, SQLAlchemy `pool_pre_ping` for database drops, NGINX `limit_req` for overload, a scheduled cleanup for the disk, and `restart: unless-stopped` as the policy. Every choice scores well on recovery speed, failover, sustainability, fit on one Raspberry Pi, and reuse of tools we already run. We deliberately rejected external monitors, Swarm, and Kubernetes because they add maintenance a five-student team cannot carry after the sprint, which is the opposite of the sustainable solution the mayor asked for.

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

- Docker Inc. (2024a). *Dockerfile reference: HEALTHCHECK* [Online]. Retrieved May 2026, from https://docs.docker.com/reference/dockerfile/#healthcheck
- Docker Inc. (2024b). *Compose file reference: restart and deploy.resources* [Online]. Retrieved May 2026, from https://docs.docker.com/reference/compose-file/
- NGINX. (2024). *Using nginx as HTTP load balancer* [Online]. Retrieved May 2026, from https://nginx.org/en/docs/http/load_balancing.html
- Nygard, M. T. (2018). *Release It! Design and deploy production-ready software* (2nd ed.) [Print]. Pragmatic Bookshelf.
- Otten, M. (2026). *Sprint 3 mayor delivery feedback* [Verbal feedback, offline]. Hogeschool van Amsterdam.
- SQLAlchemy. (2024). *Engine configuration: pool_pre_ping* [Online]. Retrieved May 2026, from https://docs.sqlalchemy.org/en/20/core/pooling.html
- Stap, G. (2026). *Sprint 4 feedback on Smart City deliverables* [Verbal feedback, offline]. Hogeschool van Amsterdam.

---

## Appendix

### Appendix A - Decision summary

| Decision | Chosen | Rejected | Main reason |
|----------|--------|----------|-------------|
| Detect hung API | Docker healthcheck on `/health` | External monitor, custom script | Native, restarts within 5s, sustainable |
| Replicas + load balancing | Compose replicas + NGINX | Swarm, Kubernetes | Light, failover, fits one Pi |
| Memory | `deploy.resources.limits.memory` | Manual OOM tuning | Native, stops starvation |
| DB drop | `pool_pre_ping` + `pool_recycle` | Per-endpoint retry | Cheap, central |
| Overload | NGINX `limit_req` | SlowAPI in app | One choke point at the edge |
| Disk | Scheduled `sensor_readings` cleanup | Keep forever, time-series DB | Minimal, no new service |
| Restart policy | `unless-stopped` | `always` | Friendlier for maintenance |
