# Sprint 4 - Learning Goal 2: Database backup and data persistence

## Learning Question

How do I make sure sensor data is never lost, even when the database crashes or the Pi loses power?

## S - Situation

PostgreSQL runs in a Docker container with a named volume. If the container crashes, the volume survives. But if the volume gets corrupted, the SD card fails, or someone runs `docker compose down -v` by accident, all historical sensor data is gone. There are no backups, no export scripts, and no way to restore.

This goal builds on the Sprint 4 feedback. In our Sprint 4 conversation mister mayor Gerald Stap said we needed a database upgrade and backup and had to make sure data is never lost (Stap, 2026). Mats Otten added the same durability point, asking us to research how data loss happens, how to prevent it, and how to catch it (Otten, 2026). Right now we have zero protection against data loss, so this learning goal takes that Sprint 4 feedback as its starting point.

Backend reliability is the challenge that runs across this whole sprint. Learning Goal 1 keeps the backend itself online, this goal keeps the stored data safe, and Learning Goal 3 lets the backend take control of the city, so we reuse the same "keep the backend dependable" thread in more than one outcome.

## T - Task

First research what data loss scenarios exist in our setup and how other projects handle database durability in Docker. Then design a backup and restore system that is sustainable and prevents data loss in every scenario. Build automated backups, a tested restore procedure, and a safe PostgreSQL upgrade path.

Mats Otten's feedback on this goal was to research how data loss errors happen, how to prevent them, and how to catch them, and to design a durable system, not just a one-time backup script (Otten, 2026).

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

We followed the same four outcomes. In the **Analysis** we went through our own `docker-compose.yml` and listed six realistic data loss scenarios: SD card failure (highest impact, the only copy lives on the card), power loss mid-write, an accidental `docker compose down -v`, accidental deletion, file corruption, and a failed PostgreSQL upgrade, and our setup protected against none of them (Khajehfard, 2026a). In the **Advise** we weighed the options and chose `pg_dump` (logical, compressed) over a raw file copy, a two-tier store with every dump copied off the Pi, `cron` for scheduling, and a dump-restore-verify-switch path for the upgrade mister mayor Gerald Stap asked for (Stap, 2026; Khajehfard, 2026b). In the **Design** we laid out the backup flow, the retention table, and the exact restore and upgrade procedures, keeping every requirement traceable (Khajehfard, 2026c).

In the **Realise** we built three scripts in `backend/scripts/`, all running against the existing `db` container so no extra service is added:

- `backup.sh` (`pg_dump` of the whole database via `docker exec`, copy off the Pi, prune old local dumps)
- `restore.sh` (`pg_restore` into a live or throwaway database, then print row counts to verify)
- `check_backup.sh` (freshness check: fails if the newest dump is missing, too old, or too small)

One practical fix came up during the build: the Windows mount wrote the scripts with CRLF line endings, which break `bash`, so we converted them to LF and confirmed each one passes `bash -n`.

## R - Result

The three scripts are built and syntax-checked. `backup.sh` produces one timestamped compressed dump and copies it off the Pi, `restore.sh` rebuilds the data into a throwaway database and prints the row counts so a backup is proven usable and not just present, and `check_backup.sh` catches a silently failed job. A single `cron` entry runs the backup daily. The Analysis, Advise, and Design deliverables are finished and submitted in Portflow.

The measured results are now in. On 2026-06-03 we ran the backup against the live database on the Pi (it kept serving, because `pg_dump` does not lock writes), then restored the dump into a throwaway `citysim_verify` database: the row counts matched exactly (183 sensor readings and 4 parking spots, source and restored), and the throwaway database was dropped afterwards so the live data was never touched. The freshness check passes on a fresh dump and correctly fails when the newest dump is forced too old or too small. The tier-2 off-Pi copy was not exercised in this run, because `OFFSITE_DIR` was not set, so the dump stayed on the Pi; the off-Pi step is built but still has to be run with the second location set. The numbers are in the Realise document (Khajehfard, 2026d). The Reflection and Transfer below are written after the sprint review.

## R - Reflection

This goal was less exciting than the cluster, but it taught me something I did not see coming. Writing a backup script is the easy part. Trusting it is the hard part.

The thing I am happy with is the restore-verify step. Instead of making a dump and hoping it is good, I restore it into a throwaway database and count the rows. The first time the counts matched the live data exactly, 183 readings and 4 parking spots on both sides, I finally believed the backup was real and not just a file sitting on the disk.

What I did not finish is the off-Pi copy. In the test run OFFSITE_DIR was not set, so the dump stayed on the same SD card as the database. The script can copy it off, I just did not wire up the second location in time. So right now the backup still lives next to the thing it is meant to protect, which is the exact risk I started this goal to remove. I am not happy about that, but I would rather write it down honestly than pretend it is done.

## T - Transfer

What I carry forward is one sentence: a backup you have not restored is not a backup, and a backup that lives on the same disk as the original is not safe. On any project after this I will test the restore, not just the dump, and I will make sure a copy lives somewhere other than the machine that holds the data before I call it protected.

## References

Khajehfard, M. (2026a). *Analysis: Data persistence risks and backup strategies* [Analysis deliverable]. Hogeschool van Amsterdam. [Online]. [Analysis - Data persistence risks and backup strategies](Analysis%20-%20Data%20persistence%20risks%20and%20backup%20strategies.md)

Khajehfard, M. (2026b). *Advise: Backup and persistence technology choices* [Advise deliverable]. Hogeschool van Amsterdam. [Online]. [Advise - Backup and persistence technology choices](Advise%20-%20Backup%20and%20persistence%20technology%20choices.md)

Khajehfard, M. (2026c). *Design: Backup and restore architecture* [Design deliverable]. Hogeschool van Amsterdam. [Online]. [Design - Backup and restore architecture](Design%20-%20Backup%20and%20restore%20architecture.md)

Khajehfard, M. (2026d). *Realise: Database backup implementation* [Realise deliverable]. Hogeschool van Amsterdam. [Online]. [Realise - Database backup implementation](Realise%20-%20Database%20backup%20implementation.md)

Otten, M. (2026, May 20). *Sprint 4 mayor delivery feedback (Mats)*. Hogeschool van Amsterdam. [Verbal, offline].

Stap, G. (2026, May 20). *Sprint 4 feedback on Smart City deliverables (mister mayor Gerald Stap)*. Hogeschool van Amsterdam. [Verbal, offline].
