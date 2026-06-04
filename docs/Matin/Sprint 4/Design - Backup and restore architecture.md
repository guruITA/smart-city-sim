# Design - Backup and restore architecture

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | June 2026 |
| **Version** | 1.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Design (third of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - One cron-driven script produces a portable backup
3. Chapter 2 - A two-tier layout keeps the card from filling and from being the only copy
4. Chapter 3 - A five-step restore underpins the safe upgrade
5. Conclusion
6. Recommendation
7. References
8. Appendix

---

## Introduction

### Context

This is the **Design** outcome for Learning Goal 2 (database backup and data persistence). The Analysis found six data loss scenarios and the Advise chose the tools: scheduled `pg_dump`, a two-tier store with every dump copied off the Pi, `cron` scheduling, and a dump-restore-verify-switch upgrade path. This document turns those choices into a concrete architecture the Realise can build: the backup flow, the retention and storage layout, the restore procedure, and the upgrade procedure.

The backend is FastAPI plus PostgreSQL 16 in Docker on one Raspberry Pi (145.92.8.137, port 80). The database stores its data in a named volume (`pgdata`) on the SD card, with no backup today.

### Who this document is for, and why it looks the way it does

We write this Design for a technical reader: the City Sim development team, the embedded and backend engineers, and the technical lead on the client side, because they are the people who build and run the backup setup straight from this document. They need the flow, the directory layout, and the exact procedures, so it is a detailed, internal, engineering document and not a high-level sketch for a teacher or the public. The author is Matin Khajehfard, the junior backend developer who owns the shared backend, so the design comes from the engineer who will run it. The client is Gemeente Amsterdam, afdeling Verkeer en Openbare Ruimte (V&OR), with Mats Otten as the client's representative at the mayor delivery; the client sets the goal while the audience implements it. We mark the document Internal because it lays out the backup setup of a live system. The version is 1.1 because this is a revision after the round-two writing feedback from mister mayor Gerald Stap.

### Design question

How should we lay out the backup, restore, and upgrade so City Sim never loses data, the restore is proven, and the team can keep it running?

### Sub-questions

1. How does the backup flow run, step by step?
2. How are backups retained and stored across the two tiers?
3. How do the restore and upgrade procedures work, exactly?

### Method

We answer the sub-questions in order. We translate each Advise choice into a component, show how the components connect through a data flow diagram, and keep every requirement traceable from the Analysis through to this design (see the traceability table below and in Appendix C). The design uses only PostgreSQL and Docker built-ins plus `cron`, as the Advise settled.

### Requirements traceability

The table below maps each requirement, with its source, to the design element that satisfies it. The "never lose data" and safe-upgrade rows come from mister mayor Gerald Stap (Stap, 2026), the sustainable and catch-failure rows from Mats Otten (Otten, 2026), and the normalized-data row from the HvA brief (Hogeschool van Amsterdam, 2026). We carry it forward from the Analysis so nothing the client asked for is dropped, and we confirm in the conclusion that every row is covered.

| Requirement (source) | Design answer |
|----------------------|---------------|
| Never lose data (mister mayor Gerald Stap) | Scheduled `pg_dump` + off-Pi copy of every dump |
| Survives SD card failure (Analysis 1) | Tier 2: every dump copied off the Pi |
| Catch a silent backup failure (Mats Otten) | Freshness check on the latest dump |
| Safe database upgrade (mister mayor Gerald Stap) | Dump, restore on new version, verify, switch |
| Sustainable, no extra service (Mats Otten) | `pg_dump` + `cron`, both built in |
| Normalized data with meta-data kept (HvA brief) | Full-database dump preserves every table and its meta-data columns |

Every requirement above points to a concrete element, which is the requirement-to-design mapping the Design outcome has to show.

---

## Chapter 1 - One cron-driven script produces a portable backup

The first sub-question asks how the backup flow runs, step by step. The core of the design is a single scheduled job that produces one consistent, portable backup file per run without taking the city offline, so we lay it out as a sequence of steps, each one a thing the Realise script will do, mapped onto the `db` container that already exists. We model that movement of data with a data flow diagram (DFD) in Gane-Sarson notation (Gane & Sarson, 1979), because a DFD is the standard way to show a process and the data it moves between a source, a transform, and a store.

![Data flow diagram (Gane-Sarson notation) of the backup flow: cron triggers the backup script, which dumps the database, copies the dump off the Pi, and prunes old local dumps](backupFlow.png)

The diagram above reads top to bottom and is the picture the steps below describe in words.

```
cron (on the Pi, daily)
   |
   v
backup script
   |  1. pg_dump the citysim database from the running db container
   |     (custom format, compressed) -> /backups/citysim_<timestamp>.dump
   |
   |  2. copy the new dump off the Pi (tier 2)
   |
   |  3. delete local dumps older than the retention window (tier 1)
   |
   v
result: one fresh dump on the Pi + one copy off the Pi
```

Walking the diagram, the flow is three steps with one purpose each. Step 1 runs `pg_dump` against the existing `db` container, so no second database or extra service is needed; the custom, compressed format keeps the file small on the SD card and restores cleanly with `pg_restore` (PostgreSQL Global Development Group, 2024a). Step 2 is the off-Pi copy that removes the SD card single point of failure the Analysis flagged. Step 3 deletes local dumps past the retention window, which keeps the card from filling up, the disk-full risk the design must not reintroduce. One design choice runs underneath all three steps: we dump the entire `citysim` database, not selected tables. Dumping the whole database guarantees the normalized schema and the meta-data columns (date/time, device, IP, location) the HvA brief requires are captured together (Hogeschool van Amsterdam, 2026), and it keeps the restore simple, because one file rebuilds everything.

### Sub-conclusion

So the backup flow is one `cron`-driven script doing three steps: dump, copy off the Pi, prune locally. It runs against the existing `db` container and produces one portable file per run, and the DFD shows the data moving cleanly from the live database to a fresh dump on the Pi plus a copy off it.

---

## Chapter 2 - A two-tier layout keeps the card from filling and from being the only copy

The second sub-question asks how backups are retained and stored across the two tiers. The Advise chose a two-tier store, so this chapter fixes how many backups we keep and where, balancing recovery reach against the limited SD card space, so the card never fills and a card failure never erases the safety net. The table below sets the two tiers side by side with their location, retention, and purpose.

| Tier | Location | Retention | Purpose |
|------|----------|-----------|---------|
| Tier 1 | `/backups` on the Pi | Last 7 daily dumps | Fast local restore |
| Tier 2 | Off-Pi (team machine / share) | Longer history | Survives SD card failure |

The two tiers split the work between speed and safety. Tier 1 gives a one-week window for a quick restore directly on the Pi, which covers the common case of an accidental deletion noticed within days. Tier 2 holds the longer history off the card, so even a total SD card loss leaves the city's data recoverable. The retention numbers are a starting point the team can tune; the design fixes the shape, not unchangeable values. Where the dumps physically live matters too: they sit in a dedicated `/backups` directory mounted into the backup process, kept separate from the `pgdata` volume so a problem with the database volume does not touch the backups, and the off-Pi destination is a configuration value rather than a hard-coded path, so the team can point it at whatever HvA infrastructure is available.

### Sub-conclusion

So the layout is seven daily dumps on the Pi for speed and a longer history off the Pi for safety, in a directory separate from the database volume. The split keeps the SD card from filling while removing it as the single point of failure.

---

## Chapter 3 - A five-step restore underpins the safe upgrade

The third sub-question asks how the restore and upgrade procedures work, exactly. A backup only counts once it is restored, so this chapter fixes the exact restore procedure and the upgrade procedure that depends on it, written as ordered steps a team member can follow with `pg_restore` and the existing Docker setup, both of which the Realise will run and record.

The restore is a five-step procedure. It picks the dump to restore (the latest, or a specific timestamp before a mistake), creates an empty target database (a throwaway one for a test restore, or the real one for a real recovery), runs `pg_restore` to load the dump into the target (PostgreSQL Global Development Group, 2024b), verifies that the key tables exist and the row counts match what we expect, and, for a real recovery, points the API at the restored database and confirms the dashboard reads correctly.

1. Pick the dump to restore (latest, or a specific timestamp before a mistake).
2. Create an empty target database (a throwaway one for a test restore, or the real one for a real recovery).
3. Run `pg_restore` to load the dump into the target.
4. Verify: check that the key tables exist and the row counts match what we expect.
5. For a real recovery, point the API at the restored database and confirm the dashboard reads correctly.

The test version of the procedure, run on a throwaway database, is what proves the backup is usable, and the Realise document runs it and records the row counts so the team has evidence the safety net holds. The database upgrade mister mayor Gerald Stap asked for then wraps that same restore in a dump-first, switch-last sequence: produce a full current dump, confirm it copied off the Pi, bring up the new PostgreSQL major version against a fresh empty volume, restore and verify the row counts using the restore procedure above, switch the city over only after the verify passes, and keep the old volume and the dump until the new version has run cleanly for a while.

1. Run the backup script to produce a full, current dump.
2. Confirm the dump copied off the Pi (tier 2).
3. Bring up the new PostgreSQL major version against a fresh, empty volume.
4. Restore the dump into the new version and verify the row counts (the restore procedure above).
5. Switch the city over to the new version only after the verify passes.
6. Keep the old volume and the dump until the new version has run cleanly for a while.

If any step fails, the old data is untouched in both the dump and the old volume, so the upgrade can never lose data. Around both procedures sits a small freshness check that reports the age and size of the newest dump in `/backups`; if the newest dump is older than expected or unexpectedly small, the schedule has silently failed and the team is alerted, which is the detection step Mats Otten asked for, applied to backups.

### Sub-conclusion

So the restore is a five-step `pg_restore` procedure proven on a throwaway database, the upgrade wraps that restore in a dump-first, switch-last sequence that keeps the old data safe, and a freshness check catches a silently failed backup.

---

## Conclusion

We set out to answer how to lay out the backup, restore, and upgrade so City Sim never loses data, the restore is proven, and the team can keep it running. First, the backup is one `cron`-driven script that dumps the whole database, copies the dump off the Pi, and prunes old local dumps, shown as a data flow diagram. Second, the storage is a two-tier layout of seven daily dumps on the Pi for speed and a longer history off it for safety, in a directory separate from the database volume. Third, the recovery is a five-step, tested restore procedure that also underpins a dump-restore-verify-switch upgrade for the PostgreSQL upgrade mister mayor Gerald Stap asked for, with a freshness check that catches a silent backup failure.

Put together, those three parts are one architecture, and it maps cleanly back to the requirements: every row of the traceability table in the introduction and in Appendix C points to a component above, including the HvA brief's normalized-data-with-meta-data requirement, which the full-database dump covers. So the answer to the design question is a single backup script plus a two-tier store, a proven five-step restore, a safe dump-restore-verify-switch upgrade, and a freshness check, built on only `pg_dump`, `pg_restore`, `cron`, and Docker, which keeps it sustainable for the team. The Realise document builds these artifacts and tests the restore against real data.

---

## Recommendation

For the Realise outcome we recommend building, in this order:

1. The backup script (`pg_dump` of the whole database, custom compressed format, timestamped).
2. The `cron` entry that runs it daily and the `/backups` directory with 7-day local retention.
3. The off-Pi copy step (tier 2), with the destination as configuration.
4. The restore procedure as a documented, runnable script, and a test restore into a throwaway database with row-count verification.
5. The freshness check on the latest dump.

Build these next to the live setup, the same way Learning Goal 1 did, so the running city is never at risk while we test.

---

## References

- Gane, C., & Sarson, T. (1979). *Structured systems analysis: Tools and techniques*. Prentice-Hall. [Print].
- Hogeschool van Amsterdam. (2026). *City Sim project brief: base requirements (General, Embedded, Back-end)*. Studio Smart Cities, HvA. [Print].
- Otten, M. (2026, May 20). *Sprint 4 mayor delivery feedback (Mats)*. Hogeschool van Amsterdam. [Verbal, offline].
- PostgreSQL Global Development Group. (2024a). *PostgreSQL 16 documentation: pg_dump* [Online]. Retrieved June 5, 2026, from https://www.postgresql.org/docs/16/app-pgdump.html
- PostgreSQL Global Development Group. (2024b). *PostgreSQL 16 documentation: pg_restore* [Online]. Retrieved June 5, 2026, from https://www.postgresql.org/docs/16/app-pgrestore.html
- Stap, G. (2026, May 20). *Sprint 4 feedback on Smart City deliverables (mister mayor Gerald Stap)* [Verbal, offline]. Hogeschool van Amsterdam.

---

## Appendix

### Appendix A - Backup directory layout (target)

```
backend/
  backups/                         # tier 1, on the Pi (7 daily dumps)
    citysim_2026-05-30_0300.dump
    citysim_2026-05-29_0300.dump
    ...
  scripts/
    backup.sh                      # pg_dump + copy off Pi + prune
    restore.sh                     # pg_restore + row-count verify
    check_backup.sh                # freshness check on latest dump
```

### Appendix B - Example cron entry (target)

```
# Daily database backup at 03:00
0 3 * * * /home/pi/citysim/backend/scripts/backup.sh >> /var/log/citysim_backup.log 2>&1
```

### Appendix C - Requirement to component traceability

The table below maps each requirement to the concrete component in this design that delivers it, so a reviewer can check that nothing was dropped between the Analysis and the build.

| Requirement | Component in this design |
|-------------|--------------------------|
| Never lose data | backup.sh + off-Pi copy |
| Survives SD card failure | tier 2 off-Pi store |
| Proven restore | restore.sh + test restore with row counts |
| Safe upgrade | upgrade procedure (Chapter 3) |
| Catch silent failure | check_backup.sh freshness check |
| Sustainable | only pg_dump, pg_restore, cron, Docker |
| Keep normalized data + meta-data | full-database dump |

Every requirement has exactly one component answering it, which is the requirement-to-design mapping the conclusion confirms.

### Appendix D - Use of AI

We used an AI assistant (Claude) as a writing aid for this document. It helped restructure the text to the agreed feedback standard, check the APA formatting and the in-text citations, and rephrase passages for clarity. It did not produce the design: the backup flow, the storage layout, the restore and upgrade procedures, and the diagram are our own and were reviewed by the author, who is responsible for the content.
