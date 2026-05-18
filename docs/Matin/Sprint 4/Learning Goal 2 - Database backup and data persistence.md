# Sprint 4 - Learning Goal 2: Database backup and data persistence

## Learning Question

How do I make sure sensor data is never lost, even when the database crashes or the Pi loses power?

## S - Situation

PostgreSQL runs in a Docker container with a named volume. If the container crashes, the volume survives. But if the volume gets corrupted, the SD card fails, or someone runs `docker compose down -v` by accident, all historical sensor data is gone. There are no backups, no export scripts, and no way to restore.

Gerald said in our Sprint 4 conversation: database upgrade and backup, make sure data is never lost. Right now we have zero protection against data loss.

## T - Task

Set up automated database backups that run on a schedule. Build a restore procedure so the city can recover from data loss. Investigate PostgreSQL upgrade strategies so the database can be updated without losing data.

### Deliverables

**Analysis document** - [Analysis - Data persistence risks and backup strategies](Analysis%20-%20Data%20persistence%20risks%20and%20backup%20strategies.md)

What can go wrong with our current setup and what backup strategies exist for PostgreSQL in Docker.

**Design document** - [Design - Backup and restore architecture](Design%20-%20Backup%20and%20restore%20architecture.md)

Backup schedule, storage location, retention policy, and restore procedure.

**Realise document** - [Realise - Database backup implementation](Realise%20-%20Database%20backup%20implementation.md)

The built backup scripts, cron jobs, and tested restore procedure.

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

Matin. (2026). Design: Backup and restore architecture [Design deliverable]. [Design - Backup and restore architecture](Design%20-%20Backup%20and%20restore%20architecture.md)

Matin. (2026). Realise: Database backup implementation [Realise deliverable]. [Realise - Database backup implementation](Realise%20-%20Database%20backup%20implementation.md)
