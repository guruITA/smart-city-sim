# Sprint 4 - Learning Goal 1: Backend clustering and auto-recovery

## Learning Question

How do I make the backend recover automatically when it crashes, so the city stays online without manual intervention?

## S - Situation

The backend runs as a single Docker container on the Raspberry Pi (145.92.8.137:80). If that container crashes, the entire city goes down. All 5 tiles lose their API, the dashboard goes blank, and sensor data gets lost until someone SSHs in and restarts it manually.

In the Sprint 3 mayor delivery, Mats said: "cluster the backend, if it crashes, auto-fix within 5 seconds." Gerald added: do something with load balancing, failover, upscale and downscale. Right now we have none of that.

## T - Task

Build a clustering setup where the backend auto-recovers from crashes within 5 seconds. Implement health checks, automatic restarts, and optionally load balancing across multiple container replicas so the city never goes fully offline.

### Deliverables

**Design document** - [Design - Backend clustering and failover architecture](Design%20-%20Backend%20clustering%20and%20failover%20architecture.md)

Architecture for health checks, restart policies, replica scaling, and failover strategy on a single Raspberry Pi.

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

Matin. (2026). Design: Backend clustering and failover architecture [Design deliverable]. [Design - Backend clustering and failover architecture](Design%20-%20Backend%20clustering%20and%20failover%20architecture.md)

Matin. (2026). Realise: Backend clustering implementation [Realise deliverable]. [Realise - Backend clustering implementation](Realise%20-%20Backend%20clustering%20implementation.md)
