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

[To be filled after implementation]

## R - Result

[To be filled after implementation]

## R - Reflection

[To be filled after sprint review]

## T - Transfer

[To be filled after sprint review]

## References

Matin. (2026). Analysis: Backend failure modes and prevention strategies [Analysis deliverable]. [Analysis - Backend failure modes and prevention strategies](Analysis%20-%20Backend%20failure%20modes%20and%20prevention%20strategies.md)

Matin. (2026). Advise: Backend resilience technology choices [Advise deliverable]. [Advise - Backend resilience technology choices](Advise%20-%20Backend%20resilience%20technology%20choices.md)

Matin. (2026). Design: Backend clustering and failover architecture [Design deliverable]. [Design - Backend clustering and failover architecture](Design%20-%20Backend%20clustering%20and%20failover%20architecture.md)

Matin. (2026). Realise: Backend clustering implementation [Realise deliverable]. [Realise - Backend clustering implementation](Realise%20-%20Backend%20clustering%20implementation.md)
