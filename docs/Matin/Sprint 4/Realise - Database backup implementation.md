# Realise - Database backup implementation

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | May 2026 |
| **Version** | 0.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Realise (fourth of the four outcomes: Analysis > Advise > Design > Realise) |

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

City Sim is a miniature smart city built by our team, The Embedded Alliance, for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Five students each build one physical tile, and every tile sends its sensor data to one shared backend that I maintain. The backend is a FastAPI application with a PostgreSQL 16 database, in Docker, on one Raspberry Pi on the HvA network (145.92.8.137, port 80). All of the city's history lives in one PostgreSQL volume on the Pi's SD card.

This is the **Realise** outcome for Learning Goal 2, the last step in our order Analysis > Advise > Design > Realise. The Analysis found six data loss scenarios. The Advise chose `pg_dump`, a two-tier store, `cron`, and a dump-restore-verify-switch upgrade path. The Design laid out the backup flow, retention, restore, and upgrade procedures. This document builds those scripts and tests the restore against Gerald's "never lose data" requirement.

### What this document delivers

It describes the scripts we built, the code conventions we held to, how we test the backup and restore, the results, a user test, and the handover to the team for maintenance.

### Requirements we test against

We carry the same requirements through so the test is traceable.

- Gerald (Sprint 4): database upgrade and backup, make sure data is never lost.
- Mats (Sprint 4): prevent and catch errors, build something sustainable.
- HvA brief: keep normalized data with its meta-data stored for future analysis.

### Main question

How do we build and verify that the City Sim database is backed up automatically, can be restored, and can be upgraded without losing data?

### Method

We built the backup, restore, and freshness-check scripts as new files next to the running setup, so the live backend the team uses was never at risk. The restore script can target a throwaway database, so we can prove a backup works without touching the live data. We report the measured results, not estimates.

---

## Chapter 1 - What we built

We built three scripts. They live in `backend/scripts/` and run against the existing `db` container without changing it.

| File | What it does |
|------|--------------|
| `backend/scripts/backup.sh` | Dumps the whole database with `pg_dump`, copies the dump off the Pi, prunes old local dumps |
| `backend/scripts/restore.sh` | Restores a dump with `pg_restore` into a chosen database (live or throwaway) and prints row counts to verify |
| `backend/scripts/check_backup.sh` | Freshness check: fails if the newest dump is missing, too old, or too small |

### How the backup runs

`backup.sh` does three steps in order: it runs `pg_dump -Fc` against the `citysim-db` container to write one timestamped, compressed file to `backend/backups/` (tier 1); it copies that file to an off-Pi destination (tier 2) if `OFFSITE_DIR` is set; and it deletes local dumps older than the retention window. The database keeps serving while the dump runs, so the city stays online. The script fails loudly if the dump is empty, so a `cron` run never records a false success.

### How it is scheduled

A single `cron` entry runs `backup.sh` once a day. The entry and the example log path are in Appendix B. `cron` is built into the Pi's operating system, so there is no extra service to keep alive.

### How the restore works

`restore.sh` takes a dump file and a target database name. For a test it targets a throwaway database (for example `citysim_test`), so we prove the backup is usable without touching the live data. For a real recovery it targets `citysim` and asks for a short confirmation pause first. After loading the dump it prints the row counts of `sensor_readings` and `parking_spots` so we can check them against the source.

### Two configuration choices the build settled

1. **Dump runs through `docker exec`, not a new container.** The Design said to back up the existing `db` container. The script streams `pg_dump` out of `citysim-db` with `docker exec`, so no second database and no extra service is added. This keeps it sustainable, as the Advise required.

2. **Off-Pi destination is configuration, not hard-coded.** The tier 2 location is the `OFFSITE_DIR` environment variable, so the team can point it at whatever HvA share or machine is available without editing the script. If it is unset, the script warns that the dump exists only on the Pi.

### How to run a backup

```bash
cd backend
OFFSITE_DIR=/mnt/citybackups ./scripts/backup.sh
```

---

## Chapter 2 - Code conventions we followed

Gerald asked us to be clear about the standards we hold to, so a reviewer knows the work is consistent.

- **Shell style:** every script starts with `set -euo pipefail` so it stops on the first error instead of continuing in a broken state. Each script has a comment block at the top stating its purpose, usage, and configuration variables.
- **Configuration over hard-coding:** container name, user, database, paths, and retention are environment variables with safe defaults, so nothing sensitive or site-specific is baked in.
- **No secrets in code:** the scripts never contain a password. They rely on the Postgres user already configured in the container through the `.env` file, the same way the rest of the backend does.
- **Standard tools only:** `pg_dump`, `pg_restore`, `psql`, `cron`, and Docker. No extra dependency to install, which is what keeps it maintainable.
- **Commits:** we use Conventional Commits (`feat:`, `fix:`, `docs:`, `test:`, `refactor:`).

---

## Chapter 3 - How we tested it

We test the three things the learning goal is about: that the backup is produced, that the restore actually rebuilds the data, and that the upgrade path is safe.

- **Backup test.** Run `backup.sh` and confirm it writes a non-empty, timestamped dump to `backend/backups/`, and that the off-Pi copy appears in `OFFSITE_DIR`. This shows the everyday backup works.
- **Restore test.** Run `restore.sh` against the latest dump into a throwaway database (`citysim_test`), then compare the printed row counts against the live `citysim` database. If the counts match, the backup is proven usable, not just present. This is the test that turns a backup into a real safety net.
- **Upgrade test.** Follow the dump, restore, verify, switch path from the Design against a new PostgreSQL major version on a fresh volume, and confirm the row counts match before switching. This shows Gerald's upgrade is safe.
- **Freshness check.** Run `check_backup.sh` and confirm it passes on a fresh dump and fails when the newest dump is missing or too old. This is the detection step Mats asked for.

All of these run on the Raspberry Pi, because the Pi's SD card and ARM hardware are the real target.

---

## Chapter 4 - Test results

We run these on the Raspberry Pi, because the Pi is the real target and its SD card is exactly the part at risk. The measured results go here.

> The tests run on the Pi at the HvA. The numbers below are filled in after that run. They are left explicit so this document never reports an estimate as a measurement.

**Backup test result:**

| Metric | Value |
|--------|-------|
| Dump file written | [to be filled after Pi test] |
| Dump size | [to be filled after Pi test] |
| Copied off the Pi? | [to be filled after Pi test] |
| Time to run | [to be filled after Pi test] |

**Restore test result:**

| Metric | Value |
|--------|-------|
| `sensor_readings` rows (source) | [to be filled after Pi test] |
| `sensor_readings` rows (restored) | [to be filled after Pi test] |
| `parking_spots` rows (source) | [to be filled after Pi test] |
| `parking_spots` rows (restored) | [to be filled after Pi test] |
| Counts match? | [to be filled after Pi test] |

**Upgrade test result:**

| Metric | Value |
|--------|-------|
| From version | [to be filled after Pi test] |
| To version | [to be filled after Pi test] |
| Row counts match after restore? | [to be filled after Pi test] |
| Data lost? | [to be filled after Pi test] |

**Freshness check result:**

| Metric | Value |
|--------|-------|
| Passes on a fresh dump? | [to be filled after Pi test] |
| Fails when newest dump is too old? | [to be filled after Pi test] |

---

## Chapter 5 - User test

Numbers are not the whole story. The team has to be able to run a restore under pressure. So we end with a user test.

The setup: one team member is given only the README and asked to restore yesterday's backup into a throwaway database and report the row counts, without my help. The question we ask them: could you recover the data on your own?

> User test outcome: [to be filled after the test with the team]. We record whether they could run the restore from the documentation alone and whether the verified row counts matched the live database.

---

## Conclusion

This answers the main question. We built three scripts next to the live setup: `backup.sh` (a scheduled `pg_dump` of the whole database, copied off the Pi, with old local dumps pruned), `restore.sh` (a `pg_restore` into a live or throwaway database with row-count verification), and `check_backup.sh` (a freshness check that catches a silently failed job). The backup runs from `cron` against the existing `db` container, so nothing extra has to be kept alive. The upgrade follows a dump, restore, verify, switch path that keeps the old data safe throughout. Once the restore and upgrade tests confirm the row counts match on the Pi, the learning goal is met: the city's data is backed up automatically, can be restored, and can be upgraded without loss, all on standard PostgreSQL and Docker tools so the team can keep it running.

---

## Recommendation

For the handover to maintenance (beheer) we recommend:

1. Set `OFFSITE_DIR` to a real off-Pi location so tier 2 is active from day one; a backup that lives only on the SD card is not a backup.
2. Install the `cron` entry from Appendix B on the Pi and confirm the first scheduled run produces a dump.
3. Run a test restore into a throwaway database once a sprint, so the team keeps proving the safety net instead of assuming it.
4. Make `backup.sh` plus an off-Pi copy mandatory before the PostgreSQL upgrade, and write the measured row counts into this document as a baseline.
5. Add `check_backup.sh` to `cron` after the backup so a silently failed job is noticed the same day.

This keeps the move from build to maintenance a controlled handover, with the restore proven before the team ever has to rely on it.

---

## References

- Docker Inc. (2024). *Manage data in Docker: volumes* [Online]. Retrieved May 2026, from https://docs.docker.com/storage/volumes/
- PostgreSQL Global Development Group. (2024a). *PostgreSQL documentation: backup and restore* [Online]. Retrieved May 2026, from https://www.postgresql.org/docs/16/backup.html
- PostgreSQL Global Development Group. (2024b). *PostgreSQL documentation: pg_dump and pg_restore* [Online]. Retrieved May 2026, from https://www.postgresql.org/docs/16/app-pgdump.html
- Otten, M. (2026). *Sprint 4 feedback on Smart City deliverables* [Verbal feedback, offline]. Hogeschool van Amsterdam.
- Stap, G. (2026). *Sprint 4 feedback on Smart City deliverables* [Verbal feedback, offline]. Hogeschool van Amsterdam.

---

## Appendix

### Appendix A - File overview

| File | Purpose |
|------|---------|
| `backend/scripts/backup.sh` | `pg_dump` of the whole database, off-Pi copy, local prune |
| `backend/scripts/restore.sh` | `pg_restore` into live or throwaway DB, row-count verify |
| `backend/scripts/check_backup.sh` | Freshness check on the newest dump |

### Appendix B - Cron entry and log

```
# Daily database backup at 03:00, with the off-Pi destination set
0 3 * * * cd /home/pi/citysim/backend && OFFSITE_DIR=/mnt/citybackups ./scripts/backup.sh >> /var/log/citysim_backup.log 2>&1
# Freshness check at 03:10
10 3 * * * cd /home/pi/citysim/backend && ./scripts/check_backup.sh >> /var/log/citysim_backup.log 2>&1
```

### Appendix C - Restore and verify in one go

```bash
cd backend
# Restore the latest dump into a throwaway database and check the counts.
LATEST=$(ls -t backups/citysim_*.dump | head -n1)
./scripts/restore.sh "$LATEST" citysim_test
```
