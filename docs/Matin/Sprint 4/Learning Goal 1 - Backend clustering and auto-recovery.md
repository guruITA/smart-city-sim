# Sprint 4 - Learning Goal 1: Backend clustering and auto-recovery

## Learning Question

How do I make the backend recover automatically when it crashes, so the city stays online without manual intervention?

## S - Situation

The backend runs as a single Docker container on the Raspberry Pi (145.92.8.137:80). If that container crashes, the entire city goes down. All 5 tiles lose their API, the dashboard goes blank, and sensor data gets lost until someone SSHs in and restarts it manually.

In the Sprint 3 mayor delivery, Mats said: "cluster the backend, if it crashes, auto-fix within 5 seconds." Gerald added: do something with load balancing, failover, upscale and downscale. Right now we have none of that.

## T - Task

First research what types of failures can happen in our Docker setup and how other projects prevent and handle them. Then design a system that catches these problems automatically. Build a clustering setup where the backend auto-recovers from crashes within 5 seconds. The solution needs to be sustainable, not just a quick fix that works once.

Mats feedback on this goal: research how errors happen, how to prevent them, and how to catch them. Design a system that handles our problems automatically. Make sure the solution is durable.

### Deliverables

**Analysis document** - [Analysis - Backend failure modes and prevention strategies](Analysis%20-%20Backend%20failure%20modes%20and%20prevention%20strategies.md)

Research into what types of crashes and failures can happen, how to prevent them, and how to catch them when they do happen.

**Advise document** - [Advise - Backend resilience technology choices](Advise%20-%20Backend%20resilience%20technology%20choices.md)

Which technologies and approaches to use for detection, replicas, load balancing, and the remaining failures, with the alternatives weighed and the choices justified.

**Design document** - [Design - Backend clustering and failover architecture](Design%20-%20Backend%20clustering%20and%20failover%20architecture.md)

Architecture for health checks, restart policies, replica scaling, and failover strategy. How to set up a sustainable system that catches problems automatically.

**Realise document** - [Realise - Backend clustering implementation](Realise%20-%20Backend%20clustering%20implementation.md)

The built Docker Compose setup with health checks, replicas, and recovery verification.

## A - Action

We worked through the four outcomes in order. In the **Analysis** we researched seven failure modes of the backend and found the biggest gap: a hung (not crashed) process, which `restart: always` cannot catch because the container is still "up". The api container also had no Docker healthcheck. In the **Advise** we weighed the options and chose native tooling: a Docker healthcheck on the existing `/health` endpoint, NGINX as reverse proxy and load balancer in front of two API replicas, plus memory limits and SQLAlchemy connection settings for the smaller failures. In the **Design** we translated every requirement into one concrete architecture (NGINX on port 80, two replicas, one shared database) and a target `docker-compose.yml`.

In the **Realise** we built that setup next to the live one, so the running city was never at risk:

- `backend/docker-compose.cluster.yml` (NGINX + two API replicas + db, healthcheck, memory limits, `restart: unless-stopped`)
- `backend/nginx.conf` (reverse proxy, `proxy_next_upstream` failover, `limit_req` rate limiting)
- `backend/tests/resilience/resilience_test.py` (standard library only: load, soak, and recovery modes)

Building and deploying it forced three honest corrections versus the Design: `python:3.11-slim` has no `curl`, so the healthcheck uses a small Python `urllib` call instead; a fixed `container_name` blocks replicas, so we dropped it; and the live failover sometimes waited out the full connect timeout on a dead replica, so we lowered `proxy_connect_timeout` to 1 second with a retry. We also added `pool_pre_ping=True` and `pool_recycle=1800` to `database.py`. That change touches the shared database engine, so we held it until the team agreed; with the team present at the live deploy that condition was met and it is now applied.

## R - Result

The cluster setup is built and starts with `docker compose -f docker-compose.cluster.yml up --build --scale api=2`. NGINX owns port 80, balances over the replicas, and routes around an unhealthy one, so a single hung replica no longer takes the city down. The Analysis, Advise, and Design deliverables are finished and submitted in Portflow.

The measured results are now in, and the cluster is live. On 2026-06-03 I ran the resilience tests on the Pi and then, with the team, deployed the cluster as the real backend on port 80. The deploy reused the existing database volume so the live data was preserved (183 readings and 4 parking spots before and after). The cluster served concurrent load with zero errors and a 5 minute soak showed no memory leak (replica RSS flat at ~79 MB). Failover needed one honest fix the live test forced: the first runs were inconsistent (sometimes ~4 seconds of errors) because NGINX waited out the full connect timeout on a dead replica, so I lowered `proxy_connect_timeout` to 1 second with a retry. After that, three replica kills in a row gave zero failed requests. A genuinely crashed process is also auto-restarted by the restart policy in about a second. Both recovery paths are inside the 5 second target Mats asked for. The numbers are in the Realise document. The Reflection and Transfer below are written after the sprint review.

## R - Reflection

[To be filled after sprint review]

## T - Transfer

[To be filled after sprint review]

## References

Matin. (2026). Analysis: Backend failure modes and prevention strategies [Analysis deliverable]. [Analysis - Backend failure modes and prevention strategies](Analysis%20-%20Backend%20failure%20modes%20and%20prevention%20strategies.md)

Matin. (2026). Advise: Backend resilience technology choices [Advise deliverable]. [Advise - Backend resilience technology choices](Advise%20-%20Backend%20resilience%20technology%20choices.md)

Matin. (2026). Design: Backend clustering and failover architecture [Design deliverable]. [Design - Backend clustering and failover architecture](Design%20-%20Backend%20clustering%20and%20failover%20architecture.md)

Matin. (2026). Realise: Backend clustering implementation [Realise deliverable]. [Realise - Backend clustering implementation](Realise%20-%20Backend%20clustering%20implementation.md)
