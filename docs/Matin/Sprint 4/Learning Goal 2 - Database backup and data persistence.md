# Sprint 4 - Learning Goal 2: Database backup and data persistence

## Learning Question

How do I make sure sensor data is never lost, even when the database crashes or the Pi loses power?

## S - Situation

PostgreSQL runs in a Docker container with a named volume. If the container crashes, the volume survives. But if the volume gets corrupted, the SD card fails, or someone runs `docker compose down -v` by accident, all historical sensor data is gone. There are no backups, no export scripts, and no way to restore.

Gerald said in our Sprint 4 conversation: database upgrade and backup, make sure data is never lost. Right now we have zero protection against data loss.

## T - Task

First research what data loss scenarios exist in our setup and how other projects handle database durability in Docker. Then design a backup and restore system that is sustainable and prevents data loss in every scenario. Build automated backups, a tested restore procedure, and a safe PostgreSQL upgrade path.

Mats feedback on this goal: research how data loss errors happen, how to prevent them, and how to catch them. Design a durable system, not just a one-time backup script.

### Deliverables

**Analysis document** - [Analysis - Data persistence risks and backup strategies](Analysis%20-%20Data%20persistence%20risks%20and%20backup%20strategies.md)

Research into what data loss scenarios exist (corruption, accidental deletion, SD card failure, power loss), how to prevent them, and what backup strategies work for PostgreSQL in Docker.

**Advise document** - [Advise - Backup and persistence technology choices](Advise%20-%20Backup%20and%20persistence%20technology%20choices.md)

Which backup method, storage layout, scheduling, and upgrade path to use, with the alternatives weighed and the choices justified.

**Design document** - [Design - Backup and restore architecture](Design%20-%20Backup%20and%20restore%20architecture.md)

Backup schedule, storage location, retention policy, restore procedure, and PostgreSQL upgrade strategy. A sustainable system that handles problems automatically.

**Realise document** - [Realise - Database backup implementation](Realise%20-%20Database%20backup%20implementation.md)

The built backup scripts, cron jobs, tested restore procedure, and upgrade verification.

## A - Action

[To be filled after implementation]

## R - Result

[To be filled after implementation]

## R - Reflection

[To be filled after sprint review]

## T - Transfer

[To be filled after sprint review]

## References

Matin. (2026). Analysis: Data persistence risks and backup strategies [Analysis deliverable]. [Analysis - Data persistence risks and backup strategies](Analysis%20-%20Data%20persistence%20risks%20and%20backup%20strategies.md)

Matin. (2026). Advise: Backup and persistence technology choices [Advise deliverable]. [Advise - Backup and persistence technology choices](Advise%20-%20Backup%20and%20persistence%20technology%20choices.md)

Matin. (2026). Design: Backup and restore architecture [Design deliverable]. [Design - Backup and restore architecture](Design%20-%20Backup%20and%20restore%20architecture.md)

Matin. (2026). Realise: Database backup implementation [Realise deliverable]. [Realise - Database backup implementation](Realise%20-%20Database%20backup%20implementation.md)
