# Analysis - Backend failure modes and prevention strategies

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | May 2026 |
| **Version** | 1.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Analysis (first of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - What failures can happen in our setup
3. Chapter 2 - How to prevent these failures
4. Chapter 3 - How to catch failures when they still happen
5. Conclusion
6. Recommendation
7. References
8. Appendix

---

## Introduction

### Context

City Sim is a miniature smart city that our team, The Embedded Alliance, builds for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Each of the five students builds one physical city tile with its own embedded sensors. All tiles send their data to one shared backend that I maintain. The team is:

- Matin Khajehfard (me) - parking tile and the shared backend
- Betul Aydin - scrum master and backend support
- Gurpreet Singh - streetlight and speed camera tile (ESP32 with an LDR light sensor)
- Thijmen Walter - railroad crossing tile (ESP32 with infrared sensors and an OLED display)
- Wesley Oerlmans - traffic light tile (ESP32 with LEDs)

The shared backend is a FastAPI application with a PostgreSQL database, packaged in Docker, running on one Raspberry Pi on the HvA network (145.92.8.137, port 80). The dashboard reads from this same backend. Because every tile depends on it, the backend is the single most important part of the city. If it goes down, the whole city goes dark.

This document is the **Analysis** outcome for Learning Goal 1 (backend clustering and auto-recovery). In our way of working we follow the order Analysis > Advise > Design > Realise. This first document does the research. It does not yet pick a solution; that is the job of the Advise and Design documents that follow. We write these documents the way an engineering team in a real company would, with enough detail that a new technical colleague could pick up the work.

### Why this matters to the client

For Gemeente Amsterdam a smart city only has value if the data is reliable. A parking sensor that is offline gives a driver the wrong information, which sends them circling for a spot. That is exactly the search traffic we are trying to reduce. So backend reliability is not a technical nicety; it is what makes the city useful to the department that owns parking and traffic policy.

### Requirements we have to meet

The work in this document is bounded by requirements from two sources. We carry these through into the Advise, Design, and Realise documents.

**HvA Smart City requirements (project brief):**

- The backend is shared and must serve all five tiles at the same time.
- The city must work as a whole, not as five separate demos.
- We document our design decisions and trade-offs as Portflow evidence.

**Requirements added per mayor delivery:**

- Sprint 1 (Mats): keep the MVP simple, work as a team, tell a story.
- Sprint 2 (Mats): the city should be done and stay up; surprise me.
- Sprint 3 (Mats): cluster the backend so it auto-recovers within 5 seconds.
- Sprint 3 (Gerald): add load balancing, failover, and the ability to scale up and down.

The Sprint 3 mayor delivery is the direct reason for this learning goal. Mats was clear: do not build a quick fix that works once. Research how errors happen, how to prevent them, and how to catch them, then build something sustainable. This document answers that research request.

### Main question

What backend failures can take the City Sim down, and how can we prevent and catch them within the 5 second recovery target the mayor set?

### Sub-questions

1. What types of failures can happen in our current Docker setup?
2. How do other projects prevent these failures?
3. How can these failures be caught automatically when they still happen?

### Method

We went through our own `docker-compose.yml` and backend code line by line and listed every way it could break. For each failure we checked whether our current configuration catches it. We then compared our setup against the failure categories from Nygard's *Release It!* (2018), a standard reference on production-ready software, and against the official Docker and SQLAlchemy documentation. We use "we" throughout because this is team work, even though I own the backend.

---

## Chapter 1 - What failures can happen in our setup

### Context

The backend runs as two Docker containers on one Raspberry Pi: one `api` container (FastAPI) and one `db` container (PostgreSQL 16). All five tiles post their sensor data to this single API. The current `docker-compose.yml` already has `restart: always` on both containers, and the database has a healthcheck. The API does not have a healthcheck. There is one `/health` endpoint in the code, but Docker does not use it (see Appendix A).

### Method

For each failure mode we asked one question: does our current config catch it, yes or no? We grouped the results using Nygard's (2018) categories of production failure.

### Findings

We found seven realistic failure modes.

**1. The API process crashes.** An unhandled exception or a bug kills the Python process. Docker `restart: always` catches this, because a crashed process exits and Docker restarts it. This one is already handled.

**2. The API hangs but does not crash.** The process is still alive, so Docker thinks it is fine, but it stops answering requests. This happens with a deadlock, a blocked database connection, or an event loop that is stuck. `restart: always` does not catch this, because the process never exits. Nygard (2018) calls this a key reason to monitor behaviour, not just liveness. This is the most dangerous gap in our setup.

**3. The database is not ready or drops the connection.** On a cold start the API can come up before PostgreSQL is ready. We already fixed part of this with `depends_on: condition: service_healthy`. But a connection that drops while the system is running is not handled.

**4. The Raspberry Pi runs out of memory.** The Pi has limited RAM. PostgreSQL plus FastAPI plus the operating system can hit the limit, and the Linux out-of-memory killer then kills a process. We set no memory limits on the containers, so one container can starve the other.

**5. The Pi loses power or reboots.** A power cut at HvA or an accidental unplug takes everything down. `restart: always` brings the containers back after Docker starts, but only if Docker itself starts on boot.

**6. High-frequency requests lock up the system.** This already happened to us. Gurpreet's ESP32 sent parking updates at high frequency, and every update did a double write to the database. The database locked up. We fixed it by removing the double write, but the setup still has no protection against a tile sending too many requests.

**7. The disk fills up.** The `sensor_readings` table stores history and keeps growing. The `pgdata` volume can fill the SD card. When the disk is full, PostgreSQL stops accepting writes and the API starts failing.

### Sub-conclusion

Our setup handles a clean crash (failure 1) and a cold database start (failure 3, partly). It does not handle a hung process (failure 2), memory pressure (failure 4), overload (failure 6), or a full disk (failure 7). The single biggest gap is failure 2: a frozen API that Docker still believes is healthy. The single API container is also a single point of failure, which our knowledge graph confirms: the `FastAPI` node connects to every router in the codebase. While that one container restarts, the whole city is offline.

---

## Chapter 2 - How to prevent these failures

### Context

Preventing a failure is better than recovering from it. This chapter looks at what reduces the chance that the failures from Chapter 1 happen at all.

### Method

We read the Docker documentation on healthchecks, restart policies, and resource limits, and the SQLAlchemy documentation on connection pooling. We looked at how the `db` container in our own project is already configured, because it is a good example we can copy to the `api` container.

### Findings

**Resource limits prevent the out-of-memory killer (failure 4).** Docker Compose can set a memory limit per container with `deploy.resources.limits` (Docker Inc., 2024b). If we cap each container, one container can no longer eat all the Pi's RAM and starve the other.

**Connection pre-ping prevents database errors (failure 3).** SQLAlchemy can be configured with `pool_pre_ping=True`, which checks a connection before using it and reconnects if it is dead (SQLAlchemy, 2024). This prevents a single dropped connection from failing requests.

**Rate limiting prevents overload (failure 6).** A limit on how many requests one client can send per second protects the database from a misbehaving ESP32. This is prevention at the application layer, and it directly addresses the lockup we already lived through.

**Docker on boot prevents downtime after a reboot (failure 5).** If the Docker service is enabled to start on boot, then `restart: always` brings the city back automatically after a power cut, with no SSH needed.

**History cleanup prevents a full disk (failure 7).** Old rows in `sensor_readings` can be deleted on a schedule, which keeps the database from growing forever.

### Sub-conclusion

Most failures can be made less likely with configuration that already exists in Docker and SQLAlchemy: memory limits, connection pre-ping, rate limiting, Docker on boot, and history cleanup. None of this is new technology. It is using what we already have correctly, which is what makes it sustainable.

---

## Chapter 3 - How to catch failures when they still happen

### Context

Prevention is not enough. Mats asked specifically how we catch errors when they happen and recover within 5 seconds. This chapter is about detection and automatic recovery.

### Method

We focused on the one failure our setup cannot catch today: the hung process (failure 2). We read how Docker healthchecks work and how a load balancer in front of replicas gives failover.

### Findings

**A Docker healthcheck catches a hung API.** Docker can call `/health` on a schedule. If the endpoint does not answer within the timeout, Docker marks the container unhealthy and restarts it (Docker Inc., 2024a). This catches failure 2, the gap that `restart: always` misses. With a short interval and a low retry count, recovery can stay inside the 5 second target.

**Replicas plus a load balancer give failover.** If we run more than one API container behind a load balancer, then when one replica is unhealthy the load balancer sends traffic to the other. The city stays online during the restart instead of going dark. This removes the single point of failure and answers Gerald's request for load balancing and failover.

**A restart policy decides what happens after detection.** `restart: always` brings a container back. Combined with a healthcheck, the loop becomes: detect unhealthy, restart, healthcheck passes, back in service.

We tested the timing assumption in Appendix C. With a healthcheck interval of 5 seconds, a timeout of 3 seconds, and 1 retry, a hung container is detected and restarted well within the target. The Realise document will verify this with a real test.

### Sub-conclusion

The hung-process gap is closed by adding a Docker healthcheck on the API that calls `/health`. Running replicas behind a load balancer turns a restart from "city offline" into "city stays online". Together they meet the 5 second auto-recovery target.

---

## Conclusion

Our backend today handles a clean crash and, partly, a cold database start. It does not handle a hung process, memory pressure, request overload, or a full disk. The most urgent gap is the hung process, because `restart: always` cannot see it and the API has no Docker healthcheck.

This answers the main question. The failures that can take the city down are known and limited to seven realistic cases. Most can be prevented with configuration we already have: memory limits, connection pre-ping, rate limiting, Docker on boot, and history cleanup. The failures we cannot prevent can be caught with a Docker healthcheck on `/health`, and the downtime during recovery can be removed with replicas behind a load balancer. This combination meets the 5 second auto-recovery target the mayor set, and it satisfies the load balancing and failover the mayor delivery asked for. It is sustainable because it builds on standard Docker features instead of a one-off script.

---

## Recommendation

For the Advise and Design outcomes we recommend the following, in priority order:

1. Add a Docker healthcheck to the `api` container that calls `/health`, with an interval of 5 seconds, a timeout of 3 seconds, and 1 retry. This closes the biggest gap (failure 2) and meets the 5 second target.
2. Run the API as two or more replicas behind a load balancer so the city stays online during a restart. This removes the single point of failure.
3. Set memory limits on both containers to prevent the out-of-memory killer (failure 4).
4. Enable Docker to start on boot so the city recovers from a power cut without SSH (failure 5).
5. Keep rate limiting and history cleanup as smaller follow-up tasks (failures 6 and 7) if time allows.

The Design document turns these recommendations into a concrete architecture, and the Realise document tests them against the 5 second requirement and hands the result over to the team for maintenance.

---

## References

- Docker Inc. (2024a). *Dockerfile reference: HEALTHCHECK* [Online]. Retrieved May 2026, from https://docs.docker.com/reference/dockerfile/#healthcheck
- Docker Inc. (2024b). *Compose file reference: restart and deploy.resources* [Online]. Retrieved May 2026, from https://docs.docker.com/reference/compose-file/
- Nygard, M. T. (2018). *Release It! Design and deploy production-ready software* (2nd ed.) [Print]. Pragmatic Bookshelf.
- Otten, M. (2026). *Sprint 3 mayor delivery feedback* [Verbal feedback, offline]. Hogeschool van Amsterdam.
- SQLAlchemy. (2024). *Engine configuration: pool_pre_ping* [Online]. Retrieved May 2026, from https://docs.sqlalchemy.org/en/20/core/pooling.html
- Stap, G. (2026). *Sprint 4 feedback on Smart City deliverables* [Verbal feedback, offline]. Hogeschool van Amsterdam.

---

## Appendix

### Appendix A - Current docker-compose.yml (relevant parts)

```yaml
services:
  api:
    build: .
    container_name: citysim-api
    ports:
      - "80:8000"
    depends_on:
      db:
        condition: service_healthy
    restart: always
    # NOTE: no healthcheck. A hung API is not detected.

  db:
    image: postgres:16-alpine
    healthcheck:
      test: [ "CMD-SHELL", "pg_isready -U citysim" ]
      interval: 5s
      timeout: 3s
      retries: 5
    restart: always
```

### Appendix B - Failure mode overview

| # | Failure | Handled today? | How to handle |
|---|---------|----------------|---------------|
| 1 | API process crash | Yes (`restart: always`) | Keep restart policy |
| 2 | API hangs, not crashed | No | Docker healthcheck on `/health` |
| 3 | Database not ready / drops | Partly (`depends_on`) | `pool_pre_ping` + retries |
| 4 | Pi out of memory | No | Memory limits per container |
| 5 | Pi power loss / reboot | Partly | Docker enabled on boot |
| 6 | Request overload | No (fixed once by hand) | Rate limiting |
| 7 | Disk full | No | History cleanup schedule |

### Appendix C - Recovery timing assumption

With a healthcheck of `interval: 5s`, `timeout: 3s`, `retries: 1`, a hung container is marked unhealthy and restarted within roughly one interval plus the restart time. This stays inside the 5 second target Mats set. The Realise document verifies this with a real test.
