# Analysis - Backend failure modes and prevention strategies

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | June 2026 |
| **Version** | 1.2 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Analysis (first of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - The seven failure modes in our current setup
3. Chapter 2 - Prevention with configuration we already have
4. Chapter 3 - Detection and automatic recovery close the gap
5. Conclusion
6. Recommendation
7. References
8. Appendix

---

## Introduction

### Context

City Sim is a miniature smart city that our team, The Embedded Alliance, builds for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Each of the five students builds one physical city tile with its own embedded sensors. All tiles send their data to one shared backend that we maintain. The team is:

- Matin Khajehfard (me) - parking tile and the shared backend
- Betul Aydin - scrum master and backend support
- Gurpreet Singh, who builds the streetlight and speed camera tile (ESP32 with an LDR light sensor)
- Thijmen Walter, who builds the railroad crossing tile (ESP32 with infrared sensors and an OLED display)
- Wesley Oerlmans, who builds the traffic light tile (ESP32 with LEDs)

The shared backend is a FastAPI application with a PostgreSQL database, packaged in Docker, running on one Raspberry Pi on the HvA network (145.92.8.137, port 80). The dashboard reads from the same backend. Because every tile depends on the backend, the backend is the single most important part of the city. If the backend goes down, the whole city goes dark.

This document is the **Analysis** outcome for Learning Goal 1 (backend clustering and auto-recovery). In our way of working we follow the order Analysis > Advise > Design > Realise. The Analysis does the research. It does not yet pick a solution; choosing the solution is the job of the Advise and Design documents that follow.

We write this document for a technical audience: the City Sim development team (the embedded and backend engineers) and the technical lead on the client side. A detailed, internal, technical document fits that audience because they are the people who implement and maintain the backend, so they need the failure detail, the configuration, and the trade-offs in full. The audience is not the general public and not a teacher, so we do not simplify the engineering away. The author is the backend developer who owns the shared backend, which is why he writes the failure analysis: he knows the `docker-compose.yml` and the code line by line. The client is Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented at the mayor delivery by Mats Otten; the client commissions the city but is not the technical audience, which is why the document stays Internal rather than Public. The version is 1.2 because we revised the document after a second round of writing feedback from mister mayor Gerald Stap.

### Why this matters to the client

For Gemeente Amsterdam a smart city only has value if the data is reliable. A parking sensor that is offline gives a driver the wrong information, which sends them circling for a spot. Circling drivers are exactly the search traffic we are trying to reduce. So backend reliability is not a technical nicety; reliability is what makes the city useful to the department that owns parking and traffic policy.

### Requirements we have to meet

The work in this document is bounded by requirements from two sources. We carry the requirements through into the Advise, Design, and Realise documents so every later choice stays traceable.

The HvA Smart City requirements from the project brief state that the backend is shared and must serve all five tiles at the same time, that the city must work as a whole rather than as five separate demos, and that we document our design decisions and trade-offs as Portflow evidence (Hogeschool van Amsterdam, 2026).

On top of the brief, each mayor delivery added requirements. In Sprint 1 Mats Otten asked us to keep the MVP simple, work as a team, and tell a story. In Sprint 2 he asked that the city be done and stay up, and to surprise him. In Sprint 3 he asked us to cluster the backend so it auto-recovers within 5 seconds (Otten, 2026). At the same Sprint 3 delivery mister mayor Gerald Stap asked us to add load balancing, failover, and the ability to scale up and down (Stap, 2026).

The Sprint 3 mayor delivery is the direct reason for this learning goal. Mats was clear: do not build a quick fix that works once. He asked us to research how errors happen, how to prevent them, and how to catch them, then build something sustainable. The same resilience challenge returns later in the sprint inside the Realise document, where we build and test the cluster, so the challenge appears twice and we reuse the failure list from this Analysis as the test target. This document answers the research request.

### Main question

How can the City Sim backend fail, and how can we prevent and catch those failures inside the 5 second recovery target mister mayor Gerald Stap and the client asked for?

### Sub-questions

1. How can our current Docker setup fail?
2. How do other projects prevent these failures?
3. How can these failures be caught automatically when they still happen?

We work through the three sub-questions in order. For each one we read our own `docker-compose.yml` and backend code line by line, and we compare our setup against the failure categories in Nygard's *Release It!* (Nygard, 2018), a standard reference on production-ready software, and against the official Docker and SQLAlchemy documentation. We use "we" throughout because the backend is team work, even though one person owns the code.

---

## Chapter 1 - The seven failure modes in our current setup

The backend runs as two Docker containers on one Raspberry Pi: one `api` container (FastAPI) and one `db` container (PostgreSQL 16). All five tiles post their sensor data to the single API. To answer how our setup can fail, we walked through our own compose file and code and asked, for each line, how it could break, and whether the current configuration would catch the break. The current `docker-compose.yml` already has `restart: always` on both containers, and the database has a healthcheck, but the API does not. There is one `/health` endpoint in the code, yet Docker does not use it (see Appendix A). We grouped what we found using Nygard's (2018) categories of production failure, which separate a process that crashes from a process that hangs.

The first failure is the cleanest one. The API process can crash when an unhandled exception or a bug kills the Python process. Docker `restart: always` already catches a crash, because a crashed process exits and Docker restarts it, so this failure is handled. The harder cousin is the second failure, where the API hangs but does not crash. The process is still alive, so Docker thinks it is fine, but it stops answering requests because of a deadlock, a blocked database connection, or a stuck event loop. A restart policy cannot catch a hang, because the process never exits, which is exactly why Nygard (2018) argues we should monitor behaviour and not only liveness. The hang is the most dangerous gap in our setup.

The next group sits around the database and the hardware. The third failure is the database not being ready or dropping the connection: on a cold start the API can come up before PostgreSQL is ready, which we already softened with `depends_on: condition: service_healthy`, but a connection that drops while the system is running is still not handled. The fourth failure is the Raspberry Pi running out of memory, because PostgreSQL plus FastAPI plus the operating system can hit the limited RAM and the Linux out-of-memory killer then kills a process; we set no memory limits, so one container can starve the other. The fifth failure is a power loss or reboot, where a cut at HvA or an accidental unplug takes everything down, and `restart: always` only brings the containers back if Docker itself is set to start on boot.

The last two failures we have partly lived through. The sixth failure is high-frequency requests locking up the system, which already happened to us when Gurpreet's ESP32 sent parking updates at high frequency and every update did a double write to the database until it locked up; we fixed the double write, but the setup still has no protection against a tile sending too many requests. The seventh failure is the disk filling up, because the `sensor_readings` table stores history and keeps growing until the `pgdata` volume fills the SD card, at which point PostgreSQL stops accepting writes and the API starts failing. Appendix B collects all seven failures, whether the current setup handles each one, and the direction for a fix, so the rest of the document and the later Advise can work from one shared list.

### Sub-conclusion

Our setup handles a clean crash (failure 1) and a cold database start (failure 3, partly). It does not handle a hung process (failure 2), memory pressure (failure 4), overload (failure 6), or a full disk (failure 7). The single biggest gap is failure 2: a frozen API that Docker still believes is healthy. The single API container is also a single point of failure, which our knowledge graph confirms, because the `FastAPI` node connects to every router in the codebase. While that one container restarts, the whole city is offline.

---

## Chapter 2 - Prevention with configuration we already have

Preventing a failure is cheaper than recovering from one, so before we look at detection we ask how to lower the chance that the failures from Chapter 1 happen at all. To answer how other projects prevent these failures, we read the Docker documentation on healthchecks, restart policies, and resource limits, and the SQLAlchemy documentation on connection pooling. We also looked at how the `db` container in our own project is already configured, because the database is a good example we can copy to the `api` container instead of inventing something new.

Three of the prevention measures harden the hardware and the database. Resource limits prevent the out-of-memory killer (failure 4): Docker Compose can set a memory limit per container with `deploy.resources.limits` (Docker Inc., 2024), so a cap on each container stops one container from eating all the Pi's RAM and starving the other. Connection pre-ping prevents database errors (failure 3): SQLAlchemy can be configured with `pool_pre_ping=True`, which checks a connection before using it and reconnects a dead one (SQLAlchemy, 2024), so a single dropped connection no longer fails requests. Docker on boot prevents downtime after a reboot (failure 5): if the Docker service is enabled to start on boot, then `restart: always` brings the city back automatically after a power cut, with no SSH needed.

The other two measures sit at the application and data layer. Rate limiting prevents overload (failure 6): a limit on how many requests one client can send per second protects the database from a misbehaving ESP32, which is prevention at the application layer and directly addresses the lockup we already lived through. History cleanup prevents a full disk (failure 7): old rows in `sensor_readings` can be deleted on a schedule, which keeps the database from growing forever. None of the five measures is a new tool; each one is a setting that Docker or SQLAlchemy already offers.

### Sub-conclusion

Most failures can be made less likely with configuration that already exists in Docker and SQLAlchemy: memory limits, connection pre-ping, rate limiting, Docker on boot, and history cleanup. None of the prevention is new technology. Prevention is using what we already have correctly, which is what makes the solution sustainable rather than a one-off.

---

## Chapter 3 - Detection and automatic recovery close the gap

Prevention is not enough, because some failures still slip through, and mister mayor Mats Otten asked specifically how we catch errors when they happen and recover within 5 seconds. To answer how failures can be caught automatically, we focused on the one failure our setup cannot catch today, the hung process (failure 2), and we read how Docker healthchecks work and how a load balancer in front of replicas gives failover.

The first answer is a healthcheck. A Docker healthcheck catches a hung API, because Docker can call `/health` on a schedule, and if the endpoint does not answer within the timeout, Docker marks the container unhealthy and restarts it (Docker Inc., 2024). A healthcheck catches failure 2, the gap that `restart: always` misses, and with a short interval and a low retry count the recovery can stay inside the 5 second target. The second answer removes the downtime around that restart. Replicas plus a load balancer give failover: if we run more than one API container behind a load balancer, then when one replica is unhealthy the load balancer sends traffic to the other, so the city stays online during the restart instead of going dark. Running replicas removes the single point of failure and answers mister mayor Gerald Stap's request for load balancing and failover. The third answer ties the two together: a restart policy decides what happens after detection, so `restart: always` brings a container back, and combined with a healthcheck the loop becomes detect unhealthy, restart, healthcheck passes, back in service.

We checked the timing assumption in Appendix C. With a healthcheck interval of 5 seconds, a timeout of 3 seconds, and 1 retry, a hung container is detected and restarted well within the target. The Realise document later verifies the timing with a real test on the Pi rather than leaving it as an assumption.

### Sub-conclusion

The hung-process gap is closed by adding a Docker healthcheck on the API that calls `/health`. Running replicas behind a load balancer turns a restart from "city offline" into "city stays online". Together a healthcheck and replicas meet the 5 second auto-recovery target.

---

## Conclusion

We set out to answer how the City Sim backend can fail, and how we can prevent and catch those failures inside the 5 second recovery target mister mayor Gerald Stap and the client asked for.

First, our backend today handles a clean crash and, partly, a cold database start, but it does not handle a hung process, memory pressure, request overload, or a full disk, and the most urgent gap is the hung process, because `restart: always` cannot see it and the API has no Docker healthcheck. Second, most of those failures can be prevented with configuration we already own: memory limits, connection pre-ping, rate limiting, Docker on boot, and history cleanup. Third, the failures we cannot prevent can be caught with a Docker healthcheck on `/health`, and the downtime during recovery can be removed with replicas behind a load balancer.

Together the three answers show that the failures which can take the city down are known and limited to seven realistic cases, and that each one has either a prevention or a detection answer. So the answer to the main question is that the backend fails in seven ways, that five of them are prevented with existing Docker and SQLAlchemy settings, and that the rest are caught and recovered inside the 5 second target by a healthcheck on `/health` plus replicas behind a load balancer. The combination meets the auto-recovery target the client set and satisfies the load balancing and failover the mayor delivery asked for, and it is sustainable because it builds on standard Docker features instead of a one-off script.

---

## Recommendation

For the Advise and Design outcomes we recommend the following, in priority order:

1. Add a Docker healthcheck to the `api` container that calls `/health`, with an interval of 5 seconds, a timeout of 3 seconds, and 1 retry. The healthcheck closes the biggest gap (failure 2) and meets the 5 second target.
2. Run the API as two or more replicas behind a load balancer so the city stays online during a restart. Replicas remove the single point of failure.
3. Set memory limits on both containers to prevent the out-of-memory killer (failure 4).
4. Enable Docker to start on boot so the city recovers from a power cut without SSH (failure 5).
5. Keep rate limiting and history cleanup as smaller follow-up tasks (failures 6 and 7) if time allows.

The Design document turns these recommendations into a concrete architecture, and the Realise document tests them against the 5 second requirement and hands the result over to the team for maintenance.

---

## References

- Docker Inc. (2024). *Compose Deploy Specification: resources*. [Online]. Retrieved June 5, 2026, from https://docs.docker.com/reference/compose-file/deploy/
- Docker Inc. (2024). *Dockerfile reference: HEALTHCHECK instruction*. [Online]. Retrieved June 5, 2026, from https://docs.docker.com/reference/dockerfile/#healthcheck
- Hogeschool van Amsterdam. (2026). *City Sim project brief: base requirements (General, Embedded, Back-end)*. Studio Smart Cities, HvA. [Print].
- Nygard, M. T. (2018). *Release It! Design and deploy production-ready software* (2nd ed.). Pragmatic Bookshelf. [Print].
- Otten, M. (2026, May 20). *Sprint 4 mayor delivery feedback (Mats)*. Hogeschool van Amsterdam. [Verbal, offline].
- SQLAlchemy. (2024). *Connection pooling: Dealing with disconnects (pool_pre_ping)*. [Online]. Retrieved June 5, 2026, from https://docs.sqlalchemy.org/en/20/core/pooling.html
- Stap, G. (2026, May 20). *Sprint 4 feedback on Smart City deliverables (mister mayor Gerald Stap)*. Hogeschool van Amsterdam. [Verbal, offline].

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

The table below lists the seven failure modes from Chapter 1, whether the current setup handles each one, and the direction for a fix that the Advise picks up. We read it as the shared checklist that the rest of the sprint works from.

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

With a healthcheck of `interval: 5s`, `timeout: 3s`, `retries: 1`, a hung container is marked unhealthy and restarted within roughly one interval plus the restart time. The timing stays inside the 5 second target Mats set. The Realise document verifies the timing with a real test.

### Appendix D - Use of AI

We used an AI assistant (Claude) as a writing aid for this document. It helped restructure the text to the agreed feedback standard, check the APA formatting and the in-text citations, and rephrase passages for clarity. It did not produce the engineering work or the measured results: the architecture, the choices, the code, and the test numbers are our own and were reviewed by the author, who is responsible for the content.
