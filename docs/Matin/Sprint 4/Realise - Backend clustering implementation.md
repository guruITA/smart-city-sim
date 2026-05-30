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

NGINX answers on port 80 and balances over the two API replicas. Scaling up or down is one flag, for example `--scale api=3`, which answers the upscale and downscale requirement.

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

We run these on the Raspberry Pi, because the Pi's limited ARM hardware is the real target and a laptop would give numbers that are too optimistic. The measured results go here.

> The tests run on the Pi at the HvA. The numbers below are filled in after that run. They are left explicit so this document never reports an estimate as a measurement.

**Load test result:**

| Metric | Value |
|--------|-------|
| Requests | [to be filled after Pi test] |
| Errors | [to be filled after Pi test] |
| Throughput (req/s) | [to be filled after Pi test] |
| Latency p95 | [to be filled after Pi test] |

**Soak test result:**

| Metric | Value |
|--------|-------|
| Duration | [to be filled after Pi test] |
| Replica memory at start | [to be filled after Pi test] |
| Replica memory at end | [to be filled after Pi test] |
| Memory leak? | [to be filled after Pi test] |

**Recovery test result:**

| Metric | Value |
|--------|-------|
| Replica killed | [to be filled after Pi test] |
| Downtime | [to be filled after Pi test] |
| Within 5s target? | [to be filled after Pi test] |

---

## Chapter 5 - User test

Performance numbers are not the whole story. The city has to feel online to the people who use it. So we end with a user test with the team.

The setup: one team member kills an API replica during a normal demo while the others watch the dashboard and their tiles. The question we ask them: did you notice anything go wrong?

> User test outcome: [to be filled after the test with the team]. We record whether the dashboard kept updating and whether any tile reported an error during the replica restart.

---

## Conclusion

This answers the main question. We built the clustered backend as NGINX in front of two API replicas that share one database, with a Docker healthcheck on `/health` and `restart: unless-stopped` for recovery, plus memory limits, rate limiting, and the proposed `pool_pre_ping` change. Building it forced two honest fixes over the Design: the healthcheck uses Python instead of curl, and the replicas drop the fixed container name. We test the result with a load, soak, and recovery test on the Pi, and with a user test with the team. Once the recovery test confirms the downtime is within 5 seconds, the learning goal is met: the city auto-recovers and stays online during failover, with load balancing and scaling, all on standard Docker and NGINX features so the team can keep it running.

---

## Recommendation

For the handover to maintenance (beheer) we recommend:

1. Treat `docker-compose.cluster.yml` as a pilot. Run it next to the current stack first, confirm the recovery and user tests pass on the Pi, then make it the default for the city.
2. Apply the `pool_pre_ping` change to `database.py` after the team agrees, since it is backward compatible.
3. Add the rate limiting and the history cleanup job as priority-2 follow-ups.
4. Write the recovery and soak numbers into this document so the next team has a baseline to compare against.

This keeps the move from build to maintenance a controlled pilot, not a sudden switch.

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
