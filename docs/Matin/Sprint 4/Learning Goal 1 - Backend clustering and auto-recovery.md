# Sprint 4 - Learning Goal 1: Backend clustering and auto-recovery

## Learning Question

How do I make the backend recover automatically when it crashes, so the city stays online without manual intervention?

## S - Situation

The backend runs as a single Docker container on the Raspberry Pi (145.92.8.137:80). If that container crashes, the entire city goes down. All 5 tiles lose their API, the dashboard goes blank, and sensor data gets lost until someone SSHs in and restarts it manually.

This goal builds directly on the Sprint 3 mayor delivery feedback. At that delivery Mats Otten, the client's representative, said: "cluster the backend, if it crashes, auto-fix within 5 seconds" (Otten, 2026). Mister mayor Gerald Stap added that we should do something with load balancing, failover, and upscaling and downscaling (Stap, 2026). Right now we have none of that, so this learning goal takes that Sprint 3 feedback as its starting point.

Backend reliability is the challenge that runs across this whole sprint. It shows up here as auto-recovery, again in Learning Goal 2 as never losing the stored data, and again in Learning Goal 3 where the backend takes control of the city, so we reuse the same "keep the backend dependable" thread in more than one outcome.

## T - Task

First research what types of failures can happen in our Docker setup and how other projects prevent and handle them. Then design a system that catches these problems automatically. Build a clustering setup where the backend auto-recovers from crashes within 5 seconds. The solution needs to be sustainable, not just a quick fix that works once.

Mats Otten's feedback on this goal was to research how errors happen, how to prevent them, and how to catch them, to design a system that handles our problems automatically, and to keep the solution durable (Otten, 2026).

### Deliverables

**Analysis document** - [Analysis - Backend failure modes and prevention strategies](Analysis%20-%20Backend%20failure%20modes%20and%20prevention%20strategies.md)

Research into what types of crashes and failures can happen, how to prevent them, and how to catch them when they do happen.

**Advise document** - [Advise - Backend resilience technology choices](Advise%20-%20Backend%20resilience%20technology%20choices.md)

Which technologies and approaches to use for detection, replicas, load balancing, and the remaining failures, with the alternatives weighed and the choices justified.

**Design document** - [Design - Backend clustering and failover architecture](Design%20-%20Backend%20clustering%20and%20failover%20architecture.md)

Architecture for health checks, restart policies, replica scaling, and failover strategy. How to set up a sustainable system that catches problems automatically.

**Realise document** - [Realise - Backend clustering implementation](Realise%20-%20Backend%20clustering%20implementation.md)

The built Docker Compose setup with health checks, replicas, and recovery verification.

### Autoscaling follow-up (recommendation #5)

The Realise closed with a recommendation: the Compose cluster scales by hand (`--scale`), so for **automatic** scaling on load, move to Kubernetes (K3s) with a HorizontalPodAutoscaler. We took that recommendation and built it as an extra Design and Realise, so the same learning goal also shows the automatic-scaling path, not only the recommendation.

**Design document** - [Design - Kubernetes autoscaling architecture](Design%20-%20Kubernetes%20autoscaling%20architecture.md)

How to scale the stateless API automatically while keeping the one shared database single, so the shared data cannot split brain. Includes the safe, parallel rollout next to the live backend and the path to more nodes.

**Realise document** - [Realise - Kubernetes autoscaling implementation](Realise%20-%20Kubernetes%20autoscaling%20implementation.md)

The built K3s manifests (`backend/k8s/`) and the autoscaling test. The build is complete and validated; the measured Pi numbers are filled in after a run on the Pi.

## A - Action

We worked through the four outcomes in order. In the **Analysis** we researched seven failure modes of the backend and found the biggest gap: a hung (not crashed) process, which `restart: always` cannot catch because the container is still "up". The api container also had no Docker healthcheck (Khajehfard, 2026a). In the **Advise** we weighed the options and chose native tooling: a Docker healthcheck on the existing `/health` endpoint, NGINX as reverse proxy and load balancer in front of two API replicas, plus memory limits and SQLAlchemy connection settings for the smaller failures (Khajehfard, 2026b). In the **Design** we translated every requirement into one concrete architecture (NGINX on port 80, two replicas, one shared database) and a target `docker-compose.yml` (Khajehfard, 2026c).

In the **Realise** we built that setup next to the live one, so the running city was never at risk:

- `backend/docker-compose.cluster.yml` (NGINX + two API replicas + db, healthcheck, memory limits, `restart: unless-stopped`)
- `backend/nginx.conf` (reverse proxy, `proxy_next_upstream` failover, `limit_req` rate limiting)
- `backend/tests/resilience/resilience_test.py` (standard library only: load, soak, and recovery modes)

Building and deploying it forced three honest corrections versus the Design: `python:3.11-slim` has no `curl`, so the healthcheck uses a small Python `urllib` call instead; a fixed `container_name` blocks replicas, so we dropped it; and the live failover sometimes waited out the full connect timeout on a dead replica, so we lowered `proxy_connect_timeout` to 1 second with a retry. We also added `pool_pre_ping=True` and `pool_recycle=1800` to `database.py`. That change touches the shared database engine, so we held it until the team agreed; with the team present at the live deploy that condition was met and it is now applied.

## R - Result

The cluster setup is built and starts with `docker compose -f docker-compose.cluster.yml up --build --scale api=2`. NGINX owns port 80, balances over the replicas, and routes around an unhealthy one, so a single hung replica no longer takes the city down. The Analysis, Advise, and Design deliverables are finished and submitted in Portflow.

The measured results are now in, and the cluster is live. On 2026-06-03 we ran the resilience tests on the Pi and then, with the team, deployed the cluster as the real backend on port 80. The deploy reused the existing database volume so the live data was preserved (183 readings and 4 parking spots before and after). The cluster served concurrent load with zero errors and a 5 minute soak showed no memory leak (replica RSS flat at ~79 MB). Failover needed one honest fix the live test forced: the first runs were inconsistent (sometimes ~4 seconds of errors) because NGINX waited out the full connect timeout on a dead replica, so we lowered `proxy_connect_timeout` to 1 second with a retry. After that, three replica kills in a row gave zero failed requests. A genuinely crashed process is also auto-restarted by the restart policy in about a second. Both recovery paths are inside the 5 second target Mats Otten asked for (Otten, 2026). The numbers are in the Realise document (Khajehfard, 2026d). The Reflection and Transfer below are written after the sprint review.

## R - Reflection

The cluster is the part of this sprint I am most proud of. We took it live on port 80 with the whole team watching and killed API replicas while it was serving, and the city stayed up. Zero failed requests after a real replica kill. For once the backend was not the thing that broke, and that felt good.

It did not go smoothly though, and I am glad it did not. I was sure my design was correct, so I almost wrote the Realise as if it just worked. On the Pi it did not. The first failover runs gave about four seconds of errors because NGINX sat and waited out the full connect timeout on a dead replica. I had to dig into the config to find that and fix it with a shorter timeout and a retry. That stung a little, but it was the most useful moment of the goal.

What I mainly learned about myself is that I trust my own design too fast. The thing that saved me was testing on the real Pi instead of my laptop. The slow ARM hardware showed a problem a fast machine would have hidden.

## T - Transfer

I take one rule to the next project. A system is not done when the design looks right, it is done when it survives a real test on the real hardware. So I will test failure on the target machine early, and I will not call something "recovers in 5 seconds" until I have actually killed it and measured the gap myself. This is true for any production backend I build later, not just for a school city.

## References

Khajehfard, M. (2026a). *Analysis: Backend failure modes and prevention strategies* [Analysis deliverable]. Hogeschool van Amsterdam. [Online]. [Analysis - Backend failure modes and prevention strategies](Analysis%20-%20Backend%20failure%20modes%20and%20prevention%20strategies.md)

Khajehfard, M. (2026b). *Advise: Backend resilience technology choices* [Advise deliverable]. Hogeschool van Amsterdam. [Online]. [Advise - Backend resilience technology choices](Advise%20-%20Backend%20resilience%20technology%20choices.md)

Khajehfard, M. (2026c). *Design: Backend clustering and failover architecture* [Design deliverable]. Hogeschool van Amsterdam. [Online]. [Design - Backend clustering and failover architecture](Design%20-%20Backend%20clustering%20and%20failover%20architecture.md)

Khajehfard, M. (2026d). *Realise: Backend clustering implementation* [Realise deliverable]. Hogeschool van Amsterdam. [Online]. [Realise - Backend clustering implementation](Realise%20-%20Backend%20clustering%20implementation.md)

Otten, M. (2026). *Sprint 3 mayor delivery feedback (Mats)*. Hogeschool van Amsterdam. [Verbal, offline].

Stap, G. (2026). *Sprint 3 mayor delivery feedback (mister mayor Gerald Stap)*. Hogeschool van Amsterdam. [Verbal, offline].
