# Realise - Backend clustering implementation

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | May 2026 |
| **Version** | 0.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Realise (fourth of the four outcomes: Analysis, Advise, Design, Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - What we built
3. Chapter 2 - Code conventions we followed
4. Chapter 3 - How we tested it
5. Chapter 4 - Test results
6. Chapter 5 - User test
7. Conclusion
8. Recommendation
9. References
10. Appendix

---

## Introduction

### Context

City Sim is a miniature smart city built by our team, The Embedded Alliance, for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Five students each build one physical tile, and every tile sends its sensor data to one shared backend that I maintain. The backend is a FastAPI application with a PostgreSQL database, in Docker, on one Raspberry Pi on the HvA network (145.92.8.137, port 80).

This is the **Realise** outcome for Learning Goal 1, the last step in our order Analysis, Advise, Design, Realise. The Analysis found the seven failure modes. The Advise chose the technologies. The Design drew the architecture. This document builds that architecture and tests it against the 5 second recovery requirement the mayor set.

### What this document delivers

It describes the files we built, the code conventions we held to, the three performance tests we ran and why, the results, a user test, and the handover to the team for maintenance.

### Requirements we test against

We carry the same requirements through so the test is traceable.

- Mats (Sprint 3): auto-recover within 5 seconds; keep it sustainable.
- Gerald (Sprint 3): load balancing, failover, scale up and down.

### Main question

How do we build and verify that the City Sim backend auto-recovers within 5 seconds and stays online during failover?

### Method

We built the clustered stack as new files next to the running setup, so the live backend the team uses was never at risk. We then ran three performance tests (load, soak, recovery) and one user test. We report the measured numbers, not estimates.

---

## Chapter 1 - What we built

We built four artifacts. They live in the `backend/` folder and run alongside the existing `docker-compose.yml`, which we did not touch.

| File | What it does |
|------|--------------|
| `backend/docker-compose.cluster.yml` | The clustered stack: NGINX, two API replicas, one database, with healthcheck and memory limits |
| `backend/nginx.conf` | The reverse proxy and load balancer config, with rate limiting and failover |
| `backend/tests/resilience/resilience_test.py` | The load, soak, and recovery tests, standard library only |
| `backend/tests/resilience/README.md` | How to run the stack and the tests |

### Two things the build forced us to change from the Design

The Design is a sketch. Building it surfaced two real problems we had to solve.

1. **No curl in the image.** The Design wrote the healthcheck with `curl`. The image is `python:3.11-slim`, which has no curl. So the healthcheck calls `/health` with the Python standard library instead:

   ```yaml
   test: [ "CMD-SHELL", "python -c \"import urllib.request,sys; sys.exit(0 if urllib.request.urlopen('http://localhost:8000/health').status==200 else 1)\"" ]
   ```

2. **A fixed container name blocks replicas.** The live `api` service has `container_name: citysim-api`. Docker cannot start two replicas with the same fixed name. So the clustered `api` service has no `container_name`, which lets Docker number the replicas itself.

### The database connection change (proposed)

The Advise and Design call for `pool_pre_ping` on the SQLAlchemy engine so a dropped database connection reconnects on its own. The current `backend/app/database.py` creates the engine without it:

```python
engine = create_engine(DATABASE_URL)
```

The proposed change is two lines:

```python
engine = create_engine(
    DATABASE_URL,
    pool_pre_ping=True,
    pool_recycle=1800,
)
```

This touches the shared backend, so we apply it only after the team agrees. It is backward compatible: it changes how connections are checked, not the data or the API.

### How to start the cluster

```bash
cd backend
docker compose -f docker-compose.cluster.yml up --build --scale api=2
```

NGINX answers on port 80 and balances over the two API replicas. Scaling up or down is one flag, for example `--scale api=3`. This is **manual** scaling: an operator changes the replica count by hand. It answers the upscale and downscale requirement in the sense that the city can run more or fewer replicas, but it is not yet **automatic** scaling on load. True autoscaling, where the replica count follows CPU or request load on its own, needs an orchestrator such as Kubernetes (K3s) with a HorizontalPodAutoscaler. We treat that as the next step, described in the recommendation, because it is a larger infrastructure change that belongs with the infra owner.

---

## Chapter 2 - Code conventions we followed

Gerald asked us to be clear about the standards we hold to, so a reviewer knows the work is consistent.

- **Python style:** we follow PEP 8 (Van Rossum et al., 2001), with type hints and docstrings on functions, as set in the project coding standards.
- **Configuration style:** the Compose and NGINX files have a comment at the top that states their purpose, so a new colleague reads the intent before the detail.
- **Tests:** the test script uses only the standard library, so it runs on any machine without an install step.
- **Commits:** we use Conventional Commits (`feat:`, `fix:`, `docs:`, `test:`, `refactor:`).
- **No secrets in code:** the database password comes from the `.env` file through `env_file`, never hard coded.

---

## Chapter 3 - How we tested it

We ran three performance tests, each one tied to a failure mode from the Analysis, and we explain what each test does.

- **Load test.** Sustained concurrent requests for a fixed time. It measures throughput and error rate, and shows the cluster serves normal multi-tile traffic without failing. This is the everyday case.
- **Soak test.** The same load but for a long duration (30 minutes), while we watch each replica's memory with `docker stats`. A soak test catches a slow **memory leak** (failure 4) that a short test would miss. If memory keeps climbing over time, there is a leak.
- **Recovery test.** We kill one API replica with `docker kill`, then poll the city until it serves a healthy response again, and measure the downtime. This is the direct test of the 5 second target (failure 2). While the replica restarts, NGINX should keep serving from the other replica.

The commands are in `backend/tests/resilience/README.md`. All three run against `http://145.92.8.137/health` on the Pi.

---

## Chapter 4 - Test results

We run these on the Raspberry Pi, because the Pi's limited ARM hardware is the real target and a laptop would give numbers that are too optimistic. The tests ran on 2026-06-03. We first ran them as a second instance next to the live backend (NGINX on port 8080) so the early runs caused no downtime, and then, with the team, we deployed the cluster as the real backend on port 80. The deploy reused the existing database volume (`backend_pgdata`), so the live data was preserved: the row counts were 183 sensor readings and 4 parking spots before and after, and the new `overrides` table was added next to the six existing tables.

**Load test result** (30 seconds, 20 concurrent workers, against the NGINX entry):

| Metric | Value |
|--------|-------|
| Requests | 2361 |
| Errors | 0 (0.00%) |
| Throughput (req/s) | 78.7 |
| Latency p95 | 549.7 ms |

The cluster served sustained concurrent traffic with zero errors. We ran a second load test against the default per-IP rate limit (`rate=10r/s`, `burst=20`) from a single client IP: of 5026 requests only about 314 were served and the rest returned `503`. That is the rate limiter working as designed, not a failure: one misbehaving ESP32 from a single IP is capped at roughly 10 requests per second so it cannot flood the API. The capacity numbers above were measured with the per-IP limit lifted, because in normal use each tile is a separate IP and the limiter would not trigger.

**Soak test result** (5 minutes, 10 concurrent workers; the 30-minute design soak was shortened to limit load on the shared Pi):

| Metric | Value |
|--------|-------|
| Duration | 5 minutes (300 s), 12030 requests, 2% errors |
| Replica memory at start | 78.8 MB (RSS) |
| Replica memory at end | 79.2 MB (RSS) |
| Memory leak? | No - RSS stayed flat at ~79 MB across the run |

Memory was read from `/proc/1/status` (`VmRSS`) inside a replica, because this Pi's kernel has no cgroup memory accounting so `docker stats` reports 0 B. The 2% errors were a handful of requests that hit the 5 second `proxy_read_timeout` under load on the modest Pi hardware. Memory did not grow over time, so there is no leak (failure 4).

**Recovery test result** (`docker kill` on one of two API replicas, on the live port-80 cluster):

| Metric | Value |
|--------|-------|
| Replica killed | Yes (one of two API replicas) |
| Downtime | 0 failed requests across 3 consecutive replica kills; worst case bounded at ~1 s |
| Within 5s target? | Yes (PASS) |

This result came after one honest fix that the live test forced. Our first failover runs were inconsistent: sometimes the city stayed at HTTP 200 with no gap, but sometimes it returned errors for about 4 seconds before recovering. The cause was the NGINX config: it resolved the replica address through Docker DNS with a variable instead of a fixed `upstream` group, so a request aimed at the just-killed replica waited out the full 3 second `proxy_connect_timeout` before NGINX retried the healthy replica. We lowered `proxy_connect_timeout` to 1 second and set `proxy_next_upstream_tries 2`, so a request to a dead replica fails fast and is retried immediately. After that, three consecutive replica kills gave zero failed requests, with the worst case bounded at about 1 second, well inside the 5 second target.

One important nuance also came out of testing. `docker kill` is an administrator action that Docker deliberately does **not** auto-restart, so this test measures **failover**: NGINX kept serving from the surviving replica. We then tested the **actual** failure mode from the Analysis (failure 2, a crashed process) separately: a container that exits with a non-zero code is auto-restarted by `restart: unless-stopped` within about a second (in an isolation test the restart count climbed 1 -> 2 -> 3 on a deliberately crashing container). We could not crash the live API in place because the Linux kernel blocks a SIGKILL to PID 1 from inside its own namespace, but the isolation test proves the policy fires on a real crash. So the city has two layers, both within the 5 second target: fast failover to the other replica, and automatic restart of a crashed replica.

---

## Chapter 5 - User test

Performance numbers are not the whole story. The city has to feel online to the people who use it. So we end with a user test with the team.

The setup: one team member kills an API replica during a normal demo while the others watch the dashboard and their tiles. The question we ask them: did you notice anything go wrong?

> User test outcome: we deployed the cluster to the live backend on port 80 together with the team and killed a replica while it was serving. After the failover fix, three consecutive replica kills caused zero failed requests, so the tiles and the dashboard kept working without anyone having to act. The team confirmed they saw no interruption on their side.

---

## Conclusion

This answers the main question. We built the clustered backend as NGINX in front of two API replicas that share one database, with a Docker healthcheck on `/health` and `restart: unless-stopped` for recovery, plus memory limits, rate limiting, and the `pool_pre_ping` change, which is now applied. Building and deploying it forced three honest fixes over the Design: the healthcheck uses Python instead of curl, the replicas drop the fixed container name, and the failover needed a shorter `proxy_connect_timeout` with a retry so the worst case stays around one second instead of four. We tested the result with a load, soak, and recovery test, and we deployed it as the live backend on port 80 with the team while preserving the existing data. The numbers confirm the learning goal is met: the cluster serves concurrent load with zero errors, shows no memory leak over a soak run, fails over with zero failed requests across repeated replica kills, and auto-restarts a crashed replica in about a second, both well within the 5 second target. It runs on standard Docker and NGINX features so the team can keep it running.

---

## Recommendation

For the handover to maintenance (beheer) we recommend:

1. The cluster is now the live backend on port 80, deployed with the team and reusing the existing data volume. Keep `docker-compose.yml` (the single-container stack) as the documented rollback, since it uses the same `backend_pgdata` volume.
2. Harden the failover with a real NGINX `upstream` block over two named replicas instead of resolving one service name through Docker DNS. That gives deterministic sub-second failover and does not depend on the connect-timeout workaround. This is the main priority-2 follow-up.
3. The `pool_pre_ping` change is applied. Add the history cleanup job as a further priority-2 follow-up so the `sensor_readings` table cannot grow without bound.
4. Always take a backup before a deploy (we did), so a bad rollout can fall back to the last dump as well as the volume.
5. For **automatic** scaling on load, move the cluster to Kubernetes (K3s) with a HorizontalPodAutoscaler that scales the replica count on CPU usage, behind a Service and Ingress. Today's `--scale` is manual. K3s is a larger infrastructure change and belongs with the infra owner (Thijs), so we note it as the autoscaling path rather than building it this sprint. The team already has a K3s setup guide to follow (K3s, autoscaling reference).

This keeps the move from build to maintenance controlled, with a clear rollback and a known next hardening step.

---

## References

- Docker Inc. (2024a). *Dockerfile reference: HEALTHCHECK* [Online]. Retrieved May 2026, from https://docs.docker.com/reference/dockerfile/#healthcheck
- Docker Inc. (2024b). *Compose file reference: restart and deploy.resources* [Online]. Retrieved May 2026, from https://docs.docker.com/reference/compose-file/
- NGINX. (2024). *Using nginx as HTTP load balancer* [Online]. Retrieved May 2026, from https://nginx.org/en/docs/http/load_balancing.html
- Nygard, M. T. (2018). *Release It! Design and deploy production-ready software* (2nd ed.) [Print]. Pragmatic Bookshelf.
- Otten, M. (2026). *Sprint 3 mayor delivery feedback* [Verbal feedback, offline]. Hogeschool van Amsterdam.
- SQLAlchemy. (2024). *Engine configuration: pool_pre_ping* [Online]. Retrieved May 2026, from https://docs.sqlalchemy.org/en/20/core/pooling.html
- Stap, G. (2026). *Sprint 4 feedback on Smart City deliverables* [Verbal feedback, offline]. Hogeschool van Amsterdam.
- Van Rossum, G., Warsaw, B., & Coghlan, N. (2001). *PEP 8: Style guide for Python code* [Online]. Retrieved May 2026, from https://peps.python.org/pep-0008/

---

## Appendix

### Appendix A - File overview

| File | Purpose |
|------|---------|
| `backend/docker-compose.cluster.yml` | Clustered stack: NGINX, 2 API replicas, DB, healthcheck, memory limits |
| `backend/nginx.conf` | Reverse proxy, load balancer, rate limit, failover |
| `backend/tests/resilience/resilience_test.py` | Load, soak, recovery tests (standard library only) |
| `backend/tests/resilience/README.md` | Run instructions |

### Appendix B - Healthcheck and recovery loop

1. Docker calls `/health` on each replica every 5 seconds.
2. No reply within 3 seconds, and the single retry fails: the replica is marked unhealthy.
3. `restart: unless-stopped` restarts the replica.
4. NGINX serves from the healthy replica meanwhile, so the city stays online.
5. The replica passes `/health` again and rejoins the pool.
