# Analysis - Data persistence risks and backup strategies

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | May 2026 |
| **Version** | 0.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Analysis (first of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - What can cause data loss in our setup
3. Chapter 2 - How to prevent data loss
4. Chapter 3 - How to catch data loss and recover from it
5. Conclusion
6. Recommendation
7. References
8. Appendix

---

## Introduction

### Context

City Sim is a miniature smart city that our team, The Embedded Alliance, builds for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Each of the five students builds one physical city tile with its own embedded sensors. All tiles send their data to one shared backend that I maintain. The team is:

- Matin Khajehfard (me) - parking tile and the shared backend
- Betul Aydin - scrum master and backend support
- Gurpreet Singh - streetlight and speed camera tile (ESP32 with an LDR light sensor)
- Thijmen Walter - railroad crossing tile (ESP32 with infrared sensors and an OLED display)
- Wesley Oerlmans - traffic light tile (ESP32 with LEDs)

The shared backend is a FastAPI application with a PostgreSQL 16 database, packaged in Docker, running on one Raspberry Pi on the HvA network (145.92.8.137, port 80). PostgreSQL keeps its data in a Docker named volume (`pgdata`) on the Pi's SD card. Every reading from every tile ends up in this one database.

This document is the **Analysis** outcome for Learning Goal 2 (database backup and data persistence). In our way of working we follow the order Analysis > Advise > Design > Realise. This first document does the research into what can destroy our data and how to protect it. It does not yet pick a solution; that is the job of the Advise and Design documents that follow.

### Why this matters to the client

For Gemeente Amsterdam the value of a smart city is in the data it collects over time. The HvA project brief requires that city data is stored for future analysis, normalized, and saved with relevant meta-data such as date, time, device, IP, and location. If that history is lost, the department loses the very thing the city was built to produce: a record it can analyse to make parking and traffic policy. A live sensor that works is not enough; the stored history has to survive crashes, power cuts, and human mistakes.

### Requirements we have to meet

The work in this document is bounded by requirements from two sources. We carry these through into the Advise, Design, and Realise documents.

**HvA Smart City requirements (project brief):**

- City data is stored for future analysis. The data structure is normalized and saved with relevant meta-data (date/time, device, IP, location).
- The backend is shared and collects all data from the hubs (sensors and actuators).
- All back-end services run in Docker.
- We document our design decisions and trade-offs as Portflow evidence.

**Requirements added per mayor delivery:**

- Sprint 4 (Gerald): database upgrade and backup, make sure data is never lost.
- Sprint 4 (Mats): research how errors happen, how to prevent them, and how to catch them; design something sustainable, not a one-time backup script.

Gerald's Sprint 4 request is the direct reason for this learning goal. Mats then sharpened it: do not write a single backup script and call it done. Research the failure scenarios, prevent what we can, and catch the rest. This document answers that research request for the data side of the backend, the same way Learning Goal 1 answered it for the service side.

### Main question

What can cause City Sim to lose stored sensor data, and how can we prevent that loss and recover from it when it still happens?

### Sub-questions

1. What data loss scenarios exist in our current Docker and Raspberry Pi setup?
2. How do other projects prevent data loss with PostgreSQL in Docker?
3. How can data loss be caught and recovered from when prevention is not enough?

### Method

We went through our own `docker-compose.yml`, the database configuration, and the way the data is stored, and we listed every way the stored data could be lost. For each scenario we checked whether our current setup protects against it. We then compared our setup against the official PostgreSQL documentation on backup and recovery and the Docker documentation on volumes, which are the standard references for this topic. We use "we" throughout because this is team work, even though I own the backend.

---

## Chapter 1 - What can cause data loss in our setup

### Context

The database runs as one `db` container (PostgreSQL 16) with its data in a Docker named volume called `pgdata`, stored on the Pi's SD card. There are no backups, no export scripts, and no copy of the data anywhere off the Pi. If the `pgdata` volume is gone, the history is gone.

### Method

For each scenario we asked one question: if this happens, do we still have the data? We grouped the scenarios by cause: hardware, software, and human.

### Findings

We found six realistic data loss scenarios.

**1. The SD card fails (hardware).** The Pi boots and stores everything on an SD card. SD cards wear out, especially with the constant small writes a database makes. When the card dies, the volume dies with it, and there is no copy anywhere else. This is the scenario with the highest impact, because it takes everything at once.

**2. The Pi loses power mid-write (hardware).** A power cut at HvA while PostgreSQL is writing can leave the database files in an inconsistent state. PostgreSQL uses a write-ahead log to protect against this and usually recovers on its own (PostgreSQL Global Development Group, 2024b), but a power cut combined with a worn SD card can still corrupt files beyond automatic recovery.

**3. Someone runs `docker compose down -v` (human).** The `-v` flag deletes named volumes. One person cleaning up containers with the wrong flag deletes `pgdata` and the whole history in one command. Nothing warns them, and nothing brings it back.

**4. Accidental data deletion through the API or SQL (human).** A wrong `DELETE` during testing, a bad migration, or a mistaken admin action removes rows. The volume is fine, but the data inside it is wrong. A backup from before the mistake is the only way back.

**5. Volume or file corruption (software).** A bug, a disk-full event, or a failed write can corrupt the database files even while the SD card is physically fine. PostgreSQL may then refuse to start.

**6. A failed PostgreSQL upgrade (software).** Gerald asked for a database upgrade. Moving from one major PostgreSQL version to the next changes the on-disk format. If the upgrade goes wrong without a backup first, the data can be left unreadable by both the old and the new version.

### Sub-conclusion

Our setup has zero protection against any of these six scenarios. The named volume survives a container crash, which is good, but every scenario here destroys or corrupts the volume itself or the data inside it. The single biggest risk is the SD card (scenario 1), because the only copy of the data lives on it. The most likely human risk is `docker compose down -v` (scenario 3), because it is one keystroke away during normal work. We also note a related exposure: the `db` container publishes port 5432 to the host, which widens who can reach the database (see Appendix A).

---

## Chapter 2 - How to prevent data loss

### Context

Preventing loss is better than recovering from it. This chapter looks at what reduces the chance that the scenarios from Chapter 1 happen at all.

### Method

We read the PostgreSQL documentation on backup methods and the Docker documentation on volumes, and we looked at how our own `db` container is configured today so we know what we are starting from.

### Findings

**Regular backups make every scenario survivable.** A backup is not really prevention, it is the safety net underneath all six scenarios. PostgreSQL ships `pg_dump`, which writes a consistent logical copy of the database to a single file while the database keeps running (PostgreSQL Global Development Group, 2024a). A scheduled `pg_dump` means the worst case is losing the data since the last backup, not all of it.

**Storing the backup off the Pi prevents the SD card single point of failure (scenario 1).** A backup that sits on the same SD card dies with the card. Copying the backup to another machine, a network share, or the HvA infrastructure removes that single point of failure.

**A named volume already prevents container-crash loss.** Our use of a named volume (`pgdata`) instead of writing inside the container is correct: when the container is removed, the data stays. This is the one thing we already do right, and we keep it.

**Restricting the destructive command prevents the human mistake (scenario 3).** Documenting that `docker compose down -v` must never be run on the Pi, and not exposing that flag in any helper script, lowers the chance of an accidental wipe.

**The write-ahead log already softens power loss (scenario 2).** PostgreSQL's write-ahead log lets it recover a consistent state after an unclean shutdown (PostgreSQL Global Development Group, 2024b). This is on by default. It does not replace backups, but it reduces how often a power cut leads to real loss.

### Sub-conclusion

Most data loss is made survivable by one thing we do not have yet: scheduled backups stored off the Pi. The named volume and the write-ahead log already give us a base layer. Prevention here is not exotic technology; it is `pg_dump` on a schedule plus a copy that does not live on the same SD card. That is what makes it sustainable.

---

## Chapter 3 - How to catch data loss and recover from it

### Context

Backups are worthless if nobody knows they failed, or if the restore has never been tried. Mats asked specifically how we catch problems and recover. This chapter is about knowing a backup is good and getting the data back.

### Method

We read the PostgreSQL documentation on restoring from a dump and looked at what it takes to verify a backup rather than just produce one.

### Findings

**A restore procedure turns a backup into a recovery.** `pg_dump` produces a file; `pg_restore` or `psql` loads it back into a database (PostgreSQL Global Development Group, 2024a). Without a written, tested restore procedure, a backup is only a hope. The restore is the half that actually saves the city, so it has to be proven, not assumed.

**Testing the restore catches a silent backup failure.** A backup job can run every night and write a broken or empty file, and nobody notices until the day they need it. Restoring a backup into a throwaway database and checking the row counts is how we catch that the backup is actually usable. This is the detection step Mats asked for, applied to data.

**A backup before an upgrade catches the upgrade risk (scenario 6).** The safe upgrade path is: take a full `pg_dump`, bring up the new PostgreSQL version, restore the dump, and verify. If anything goes wrong, the old data is still in the dump file. This makes Gerald's database upgrade safe instead of a gamble.

**A simple check confirms the backup ran.** Looking at the age and size of the latest backup file tells us whether the schedule is still working. A backup that suddenly stopped growing or stopped appearing is a caught failure before it becomes a disaster.

### Sub-conclusion

Catching data loss is about proving the safety net works: a written and tested restore procedure, a periodic test restore to confirm the backup is usable, a mandatory backup before any upgrade, and a basic freshness check on the latest backup file. Together these turn backups from a checkbox into a recovery the team can trust.

---

## Conclusion

Our backend today stores all city history in one PostgreSQL volume on one SD card, with no backup anywhere. This answers the main question. The data can be lost in six realistic ways: SD card failure, power loss mid-write, an accidental `docker compose down -v`, accidental deletion, file corruption, and a failed upgrade. Our current setup protects against none of them; only the named volume and the write-ahead log give any base safety.

Most of this loss becomes survivable with one thing we do not have: scheduled `pg_dump` backups stored off the Pi. The loss we cannot prevent can be caught and undone with a written, tested restore procedure, a periodic test restore, a mandatory backup before the upgrade Gerald asked for, and a freshness check on the latest backup. This combination meets Gerald's "make sure data is never lost" and Mats's request for a sustainable system that prevents and catches errors, because it builds on standard PostgreSQL tools instead of a one-off script.

---

## Recommendation

For the Advise and Design outcomes we recommend the following, in priority order:

1. Add scheduled `pg_dump` backups of the database, running automatically on the Pi. This is the single most important missing piece.
2. Copy each backup off the Pi so an SD card failure does not take the backups with it (scenario 1).
3. Write and test a restore procedure, and run a periodic test restore so we know the backups are usable, not just present.
4. Make a full backup mandatory before the PostgreSQL upgrade, and verify the restore on the new version (scenario 6).
5. Document that `docker compose down -v` must never be run on the Pi, and add a freshness check on the latest backup file (scenarios 3 and detection).

The Design document turns these recommendations into a concrete backup, restore, and upgrade architecture, and the Realise document builds and tests it against Gerald's "never lose data" requirement and hands the result over to the team for maintenance.

---

## References

- Docker Inc. (2024). *Manage data in Docker: volumes* [Online]. Retrieved May 2026, from https://docs.docker.com/storage/volumes/
- Otten, M. (2026). *Sprint 4 feedback on Smart City deliverables* [Verbal feedback, offline]. Hogeschool van Amsterdam.
- PostgreSQL Global Development Group. (2024a). *PostgreSQL documentation: backup and restore* [Online]. Retrieved May 2026, from https://www.postgresql.org/docs/16/backup.html
- PostgreSQL Global Development Group. (2024b). *PostgreSQL documentation: reliability and the write-ahead log* [Online]. Retrieved May 2026, from https://www.postgresql.org/docs/16/wal-intro.html
- Stap, G. (2026). *Sprint 4 feedback on Smart City deliverables* [Verbal feedback, offline]. Hogeschool van Amsterdam.

---

## Appendix

### Appendix A - Current db service (relevant parts)

```yaml
services:
  db:
    image: postgres:16-alpine
    container_name: citysim-db
    env_file:
      - .env
    ports:
      - "5432:5432"        # NOTE: database port published to the host.
    volumes:
      - pgdata:/var/lib/postgresql/data   # named volume on the SD card.
    restart: always
    # NOTE: no backup of pgdata anywhere. One SD card = one copy.

volumes:
  pgdata:
```

### Appendix B - Data loss scenario overview

| # | Scenario | Cause | Protected today? | How to handle |
|---|----------|-------|------------------|---------------|
| 1 | SD card failure | Hardware | No | Off-Pi backups |
| 2 | Power loss mid-write | Hardware | Partly (write-ahead log) | Backups + clean shutdown |
| 3 | `docker compose down -v` | Human | No | Document + no `-v` in scripts |
| 4 | Accidental deletion | Human | No | Restore from backup |
| 5 | File / volume corruption | Software | No | Restore from backup |
| 6 | Failed upgrade | Software | No | Mandatory backup before upgrade |

### Appendix C - Why a backup alone is not enough

A backup that is never restored is a guess. The Realise document will prove the restore by loading a backup into a throwaway database and checking that the row counts match the source, so the team knows the safety net actually holds before they ever need it.
