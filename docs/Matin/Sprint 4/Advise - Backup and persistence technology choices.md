# Advise - Backup and persistence technology choices

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | May 2026 |
| **Version** | 0.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Advise (second of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - How to make the backup
3. Chapter 2 - Where to store the backup
4. Chapter 3 - How to schedule and how to upgrade safely
5. Conclusion
6. Recommendation
7. References
8. Appendix

---

## Introduction

### Context

This is the **Advise** outcome for Learning Goal 2 (database backup and data persistence). The Analysis found six ways City Sim can lose its stored sensor data and recommended scheduled backups stored off the Pi, a tested restore, a mandatory backup before the PostgreSQL upgrade, and a freshness check. This document chooses the technologies that deliver those recommendations and justifies each choice against alternatives, so the Design has a settled toolset to draw.

The backend is a FastAPI application with a PostgreSQL 16 database in Docker on one Raspberry Pi (145.92.8.137, port 80). The database keeps its data in a named volume on the Pi's SD card, with no backup anywhere today.

### Main question

Which backup, storage, scheduling, and upgrade technologies best meet Gerald's "never lose data" requirement on our Raspberry Pi setup?

### Sub-questions

1. Which backup method fits a single PostgreSQL container on a Pi?
2. Where should the backups be stored so an SD card failure cannot take them?
3. How do we schedule the backups and run the upgrade without risking the data?

### Method

For each decision we set the criteria from the Analysis, listed the realistic options, and weighed them. We base the choices on the official PostgreSQL and Docker documentation. We deliberately prefer standard, built-in tools over extra services, because the team has to keep this running after Sprint 4 on limited Pi hardware.

### Selection criteria

Every choice in this document is judged against the same five criteria, taken straight from the Analysis:

1. **Never lose data** - the option must make every Analysis scenario survivable.
2. **Survives SD card failure** - backups must not live only on the Pi's card.
3. **Sustainable** - the team can keep it running; no heavy or fragile service.
4. **Fits the Pi** - light on CPU, memory, and disk; ARM-friendly.
5. **Reuses what we have** - prefer PostgreSQL and Docker built-ins over new tools.

---

## Chapter 1 - How to make the backup

### Context

The first decision is how to produce a backup of a running PostgreSQL 16 database without taking the city offline.

### Method

We compared the three realistic ways to back up PostgreSQL against the five criteria.

### Options

**Option A - `pg_dump` logical backup.** PostgreSQL's built-in tool writes a consistent logical copy (the SQL or a compressed archive needed to rebuild the database) to one file while the database keeps serving (PostgreSQL Global Development Group, 2024a). It runs from inside or against the existing `db` container, needs nothing extra, and the output is a single portable file that is easy to copy off the Pi and easy to restore on a new version.

**Option B - File system copy of the volume.** Stop the database, copy the `pgdata` directory, start it again. This is a physical backup. It forces downtime, the copy is tied to the exact PostgreSQL version and platform, and it is larger than a logical dump. It does not fit a city that should stay online.

**Option C - Continuous archiving (WAL archiving / point-in-time recovery).** PostgreSQL can archive its write-ahead log so you can restore to any moment (PostgreSQL Global Development Group, 2024b). It is the most powerful option and gives the smallest possible data loss window, but it is also the most complex to set up and operate, needs continuous storage of log segments, and is more than a five-tile school city on one Pi needs.

### Trade-off

Option A meets every criterion: no downtime, one portable file, built into PostgreSQL, light on the Pi, and trivial to copy off the card. Option B fails "never lose data" in practice because it needs downtime and is version-locked, which also makes it a poor fit for the upgrade. Option C is technically the strongest for the loss window, but it fails "sustainable" and "fits the Pi" for our scale; the operational weight is not justified for this project.

### Sub-conclusion

We choose **Option A, scheduled `pg_dump`**. It is the standard, built-in, online backup that produces a portable file, which is exactly what the off-Pi storage and the safe upgrade both need.

---

## Chapter 2 - Where to store the backup

### Context

The Analysis was clear that the SD card is the single point of failure: a backup that sits next to the database dies with the card. So storage location is its own decision.

### Method

We compared where a `pg_dump` file can land, against the same criteria, with extra weight on "survives SD card failure".

### Options

**Option A - Same SD card as the database.** Simplest, but it fails the one criterion that matters most here. If the card dies, the backup dies with it. Unacceptable as the only copy.

**Option B - Copy off the Pi to another machine or share.** After each dump, copy the file to a second location: a team member's machine, a network share, or HvA infrastructure over the network. This survives the SD card failure and keeps the backup reachable. It needs a destination that is available, which is a small operational dependency.

**Option C - Cloud object storage.** Upload each dump to a cloud bucket. The most durable, but it adds an external account, credentials to manage, and a network dependency the school setup does not really call for, which weakens "sustainable" and "reuses what we have".

### Trade-off

A local-plus-remote approach is the honest answer: keep the most recent dumps on the Pi for a fast restore, and copy every dump off the Pi so an SD card failure cannot erase the safety net. Cloud (Option C) is the most durable but the least sustainable for a student project; we note it as a future option without adopting it now.

### Sub-conclusion

We choose **a two-tier store: recent dumps on the Pi for speed, every dump copied off the Pi (Option B) for safety**. This directly removes the SD card single point of failure the Analysis flagged.

---

## Chapter 3 - How to schedule and how to upgrade safely

### Context

Two decisions remain: how the backup runs automatically, and how Gerald's database upgrade happens without risking the data.

### Method

We compared scheduling options and defined the safe upgrade path, both against the criteria, favouring built-in mechanisms.

### Options - scheduling

**Option A - `cron` on the Pi.** The Linux scheduler runs a backup script at a fixed time. It is built into the operating system, survives reboots, needs no extra service, and is the standard way to run a periodic job. Light and well understood.

**Option B - A scheduler container (for example Ofelia) or an in-app scheduler.** Adds a long-running component to keep alive and monitor. More moving parts for no real gain at our scale.

**Option C - Manual backups by a person.** No automation. Fails "never lose data" the moment someone forgets. Rejected.

Cron (Option A) wins on every criterion: built in, sustainable, light, and reliable across reboots.

### The safe upgrade path

For the PostgreSQL upgrade Gerald asked for, the dump-and-restore path falls straight out of choosing `pg_dump`:

1. Take a full `pg_dump` of the current database.
2. Copy it off the Pi.
3. Bring up the new PostgreSQL major version against an empty volume.
4. Restore the dump into it and verify the row counts.
5. Only then switch the city over.

If anything fails, the old data is still in the dump and on the old volume. This is why the logical dump from Chapter 1 matters: a file system copy (Option B there) would be locked to the old version and could not be restored into the new one.

### Sub-conclusion

We choose **`cron` on the Pi (Option A)** for scheduling and the **dump, copy, restore, verify, switch** path for the upgrade. Both reuse what we already have and need no extra service.

---

## Conclusion

This answers the main question. To meet Gerald's "never lose data" requirement on our Pi, we advise: make backups with scheduled `pg_dump` (a built-in, online, portable logical backup), store them in two tiers with every dump copied off the Pi so an SD card failure cannot erase them, schedule them with `cron`, and perform the upgrade with a dump-restore-verify path that keeps the old data safe throughout. Every choice favours PostgreSQL and Docker built-ins over extra services, which is what keeps it sustainable for the team after Sprint 4. We were honest about the limits: continuous archiving and cloud storage are stronger on paper but too heavy for a five-tile school city on one Pi, so we name them as future options rather than adopt them now.

---

## Recommendation

The Design document should specify:

1. A `pg_dump` backup script, run by `cron` on a fixed schedule, writing a timestamped, compressed file.
2. A two-tier store: a short retention of recent dumps on the Pi, plus a copy of every dump to an off-Pi location.
3. A written restore procedure and a periodic test restore into a throwaway database.
4. The dump-restore-verify-switch path as the standard PostgreSQL upgrade procedure.
5. A freshness check on the latest backup file so a silently failed job is noticed.

The Realise document then builds these and tests the restore against real data.

---

## References

- Docker Inc. (2024). *Manage data in Docker: volumes* [Online]. Retrieved May 2026, from https://docs.docker.com/storage/volumes/
- PostgreSQL Global Development Group. (2024a). *PostgreSQL documentation: backup and restore* [Online]. Retrieved May 2026, from https://www.postgresql.org/docs/16/backup.html
- PostgreSQL Global Development Group. (2024b). *PostgreSQL documentation: continuous archiving and point-in-time recovery* [Online]. Retrieved May 2026, from https://www.postgresql.org/docs/16/continuous-archiving.html
- Stap, G. (2026). *Sprint 4 feedback on Smart City deliverables* [Verbal feedback, offline]. Hogeschool van Amsterdam.

---

## Appendix

### Appendix A - Decision summary

| Decision | Chosen | Rejected | Main reason |
|----------|--------|----------|-------------|
| Backup method | `pg_dump` logical backup | File copy; continuous archiving | Online, portable, built-in, fits upgrade |
| Storage location | Recent on Pi + every dump off Pi | Only on Pi; cloud only | Survives SD card failure, stays sustainable |
| Scheduling | `cron` on the Pi | Scheduler container; manual | Built in, light, reboot-safe |
| Upgrade path | Dump, copy, restore, verify, switch | In-place upgrade | Old data stays recoverable throughout |

### Appendix B - Criteria scoring (summary)

Each chosen option scores well on all five criteria (never lose data, survives SD card failure, sustainable, fits the Pi, reuses what we have). The rejected options each fail at least one hard criterion: file copy needs downtime and is version-locked, continuous archiving is too heavy for the Pi, cloud-only storage adds an external dependency, and manual or in-app scheduling is either fragile or extra weight.
