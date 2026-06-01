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

We followed the same four outcomes. In the **Analysis** we went through our own `docker-compose.yml` and listed six realistic data loss scenarios: SD card failure (highest impact, the only copy lives on the card), power loss mid-write, an accidental `docker compose down -v`, accidental deletion, file corruption, and a failed PostgreSQL upgrade. Our setup protected against none of them. In the **Advise** we weighed the options and chose `pg_dump` (logical, compressed) over a raw file copy, a two-tier store with every dump copied off the Pi, `cron` for scheduling, and a dump-restore-verify-switch path for Gerald's upgrade. In the **Design** we laid out the backup flow, the retention table, and the exact restore and upgrade procedures, keeping every requirement traceable.

In the **Realise** we built three scripts in `backend/scripts/`, all running against the existing `db` container so no extra service is added:

- `backup.sh` (`pg_dump` of the whole database via `docker exec`, copy off the Pi, prune old local dumps)
- `restore.sh` (`pg_restore` into a live or throwaway database, then print row counts to verify)
- `check_backup.sh` (freshness check: fails if the newest dump is missing, too old, or too small)

One practical fix came up during the build: the Windows mount wrote the scripts with CRLF line endings, which break `bash`, so we converted them to LF and confirmed each one passes `bash -n`.

## R - Result

The three scripts are built and syntax-checked. `backup.sh` produces one timestamped compressed dump and copies it off the Pi, `restore.sh` rebuilds the data into a throwaway database and prints the row counts so a backup is proven usable and not just present, and `check_backup.sh` catches a silently failed job. A single `cron` entry runs the backup daily. The Analysis, Advise, and Design deliverables are finished and submitted in Portflow.

The measured results (dump size, restore row-count match, upgrade verification) are not in yet, because I need the Pi and its real data to run them. The Realise document keeps explicit `[to be filled after Pi test]` placeholders so it never reports an estimate as a measurement. The plan is to run the backup, a test restore into `citysim_test`, and the upgrade path on the Pi, then paste the real numbers into the Realise before submitting it. The Reflection and Transfer below are written after the sprint review.

## R - Reflection

[To be filled after sprint review]

## T - Transfer

[To be filled after sprint review]

## References

Matin. (2026). Analysis: Data persistence risks and backup strategies [Analysis deliverable]. [Analysis - Data persistence risks and backup strategies](Analysis%20-%20Data%20persistence%20risks%20and%20backup%20strategies.md)

Matin. (2026). Advise: Backup and persistence technology choices [Advise deliverable]. [Advise - Backup and persistence technology choices](Advise%20-%20Backup%20and%20persistence%20technology%20choices.md)

Matin. (2026). Design: Backup and restore architecture [Design deliverable]. [Design - Backup and restore architecture](Design%20-%20Backup%20and%20restore%20architecture.md)

Matin. (2026). Realise: Database backup implementation [Realise deliverable]. [Realise - Database backup implementation](Realise%20-%20Database%20backup%20implementation.md)
