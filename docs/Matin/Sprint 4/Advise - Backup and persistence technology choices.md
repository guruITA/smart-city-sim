# Advise - Backup and persistence technology choices

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | June 2026 |
| **Version** | 1.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Advise (second of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - We choose pg_dump for an online, portable backup
3. Chapter 2 - A two-tier store keeps a copy off the SD card
4. Chapter 3 - Cron schedules it and a dump-restore path makes the upgrade safe
5. Conclusion
6. Recommendation
7. References
8. Appendix

---

## Introduction

### Context

This is the **Advise** outcome for Learning Goal 2 (database backup and data persistence). The Analysis found six ways City Sim can lose its stored sensor data and recommended scheduled backups stored off the Pi, a tested restore, a mandatory backup before the PostgreSQL upgrade, and a freshness check, all driven by the "never lose data" request from mister mayor Gerald Stap (Stap, 2026). The job of an Advise is to choose, so this document does not stay neutral: it picks the technologies that deliver those recommendations and argues each choice against its alternatives, so the Design has a settled toolset to draw.

The backend is a FastAPI application with a PostgreSQL 16 database in Docker on one Raspberry Pi (145.92.8.137, port 80). The database keeps its data in a named volume on the Pi's SD card, with no backup anywhere today.

### Who this document is for, and why it looks the way it does

We write this Advise for a technical reader: the City Sim development team, the embedded and backend engineers, and the technical lead on the client side, because they are the people who will install and run whatever we choose. They need the trade-offs spelled out, not hidden behind a single recommendation, which is why this is a detailed, internal, engineering document and not a one-line decision memo. The author is Matin Khajehfard, the junior backend developer who owns the shared backend, so the recommendation comes from the engineer who has to operate it. The client is Gemeente Amsterdam, afdeling Verkeer en Openbare Ruimte (V&OR), and Mats Otten represents the client at the mayor delivery; the client is not the audience, since the client cares about the outcome while the audience cares about how to run it. We mark the document Internal because it describes the backup setup of a live system. The version is 1.1 because this is a revision after the round-two writing feedback from mister mayor Gerald Stap.

### Main question

How do we choose the backup, storage, scheduling, and upgrade technologies that best meet the "never lose data" requirement from mister mayor Gerald Stap on our Raspberry Pi setup?

### Sub-questions

1. How should we make a backup of a single PostgreSQL container on a Pi?
2. How should the backups be stored so an SD card failure cannot take them?
3. How do we schedule the backups and run the upgrade without risking the data?

### Method

We answer the sub-questions in order. For each decision we set the criteria from the Analysis, listed the realistic options, weighed them, and then state the choice plainly with its reasons. We base the choices on the official PostgreSQL documentation. We deliberately prefer standard, built-in tools over extra services, because the team has to keep this running after Sprint 4 on limited Pi hardware.

### Selection criteria

Every choice in this document is judged against the same five criteria, taken straight from the Analysis. The "sustainable" criterion comes directly from Mats Otten, who asked for a system the team can keep running rather than a one-off script (Otten, 2026):

1. **Never lose data** - the option must make every Analysis scenario survivable.
2. **Survives SD card failure** - backups must not live only on the Pi's card.
3. **Sustainable** - the team can keep it running; no heavy or fragile service.
4. **Fits the Pi** - light on CPU, memory, and disk; ARM-friendly.
5. **Reuses what we have** - prefer PostgreSQL and Docker built-ins over new tools.

---

## Chapter 1 - We choose pg_dump for an online, portable backup

The first sub-question asks how we should make a backup of a single PostgreSQL container on a Pi. The hard constraint is that the city should stay online, so we need a way to copy a running PostgreSQL 16 database without taking it down, and the copy has to be easy to move off the card and easy to restore on a new version later. With that in mind we compared the three realistic ways to back up PostgreSQL against the five criteria from the introduction. The table below sets the three options side by side so the choice is visible before we argue it.

| Option | What it is | Downtime | Version-locked | Weight on the Pi |
|--------|------------|----------|----------------|------------------|
| A - `pg_dump` logical backup | One portable file written while the database serves | No | No | Light |
| B - File system copy of the volume | Stop, copy `pgdata`, start again | Yes | Yes | Medium |
| C - Continuous archiving (WAL / PITR) | Archive the write-ahead log for point-in-time restore | No | Yes | Heavy |

Reading the table, only one row clears every column. Option A, PostgreSQL's built-in `pg_dump`, writes a consistent logical copy (the SQL or a compressed archive needed to rebuild the database) to a single file while the database keeps serving (PostgreSQL Global Development Group, 2024a); it runs against the existing `db` container, needs nothing extra, and the output is one portable file that copies off the Pi and restores onto a new version cleanly. Option B, a file system copy, means stopping the database, copying the `pgdata` directory, and starting it again; it forces downtime, the copy is tied to the exact PostgreSQL version and platform, and it is larger than a logical dump, so it fails "never lose data" in practice and is a poor fit for the upgrade. Option C, continuous archiving of the write-ahead log for point-in-time recovery, is the most powerful and gives the smallest possible loss window, but it is the most complex to set up and operate and needs continuous storage of log segments, which fails "sustainable" and "fits the Pi" at our scale; the operational weight is not justified for a five-tile school city on one Pi.

### Sub-conclusion

So we choose Option A, scheduled `pg_dump`. It is the standard, built-in, online backup that produces a portable file, which is exactly what the off-Pi storage and the safe upgrade both need, and it is the only option that clears all five criteria.

---

## Chapter 2 - A two-tier store keeps a copy off the SD card

The second sub-question asks how the backups should be stored so an SD card failure cannot take them. The Analysis was clear that the SD card is the single point of failure, because a backup that sits next to the database dies with the card, so where the dump lands is its own decision and we weighed it against the same criteria with extra weight on "survives SD card failure". The table below lays out the three places a `pg_dump` file can live.

| Option | Where the dump lands | Survives SD card failure | Extra dependency |
|--------|----------------------|--------------------------|------------------|
| A - Same SD card | Next to the database | No | None |
| B - Off the Pi (machine / share) | A second machine, network share, or HvA infrastructure | Yes | An available destination |
| C - Cloud object storage | A cloud bucket | Yes | Account, credentials, network |

The table shows the trade-off plainly: Option A is simplest but fails the one criterion that matters most here, since if the card dies the backup dies with it, so it is unacceptable as the only copy. Option C is the most durable, but uploading each dump to a cloud bucket adds an external account, credentials to manage, and a network dependency the school setup does not really call for, which weakens "sustainable" and "reuses what we have". Option B sits in the middle and is the honest answer for us: copy every dump off the Pi to a team machine, a network share, or HvA infrastructure, which survives the card failure and keeps the backup reachable, at the cost of one small operational dependency, a destination that is available. We do not pick one tier over the other, we combine them: keep the most recent dumps on the Pi for a fast restore and copy every dump off the Pi for safety. Cloud stays a future option we note but do not adopt now.

### Sub-conclusion

So we choose a two-tier store: recent dumps on the Pi for speed, and every dump copied off the Pi (Option B) for safety. The combination directly removes the SD card single point of failure the Analysis flagged, without the cloud overhead that would hurt sustainability.

---

## Chapter 3 - Cron schedules it and a dump-restore path makes the upgrade safe

The third sub-question asks how we schedule the backups and run the upgrade without risking the data, so two decisions remain: how the backup runs automatically, and how the database upgrade mister mayor Gerald Stap asked for happens safely. We weighed the scheduling options against the criteria, again favouring built-in mechanisms, and then defined the upgrade path that falls out of the `pg_dump` choice. The table below sets the three ways to trigger the backup against each other.

| Option | Mechanism | Reboot-safe | Extra service | Reliability |
|--------|-----------|-------------|---------------|-------------|
| A - `cron` on the Pi | OS scheduler runs the script at a fixed time | Yes | No | High, well understood |
| B - Scheduler container / in-app scheduler | A long-running component (for example Ofelia) | Depends | Yes | More moving parts |
| C - Manual backups by a person | A human remembers to run it | No | No | Fails on the first forgotten day |

The table makes the winner obvious. Option A, `cron`, is built into the operating system, survives reboots, needs no extra service, and is the standard way to run a periodic job, so it is light and well understood. Option B, a scheduler container such as Ofelia or an in-app scheduler, only adds a long-running component to keep alive and monitor, which is more moving parts for no real gain at our scale and works against "sustainable". Option C, manual backups, has no automation and fails "never lose data" the moment someone forgets, so we reject it. Cron clears every criterion: built in, sustainable, light, and reliable across reboots.

The upgrade path then falls straight out of choosing `pg_dump`, because the dump file restores into a fresh database with `pg_restore` (PostgreSQL Global Development Group, 2024b). For the PostgreSQL upgrade mister mayor Gerald Stap asked for, the dump-and-restore sequence is:

1. Take a full `pg_dump` of the current database.
2. Copy it off the Pi.
3. Bring up the new PostgreSQL major version against an empty volume.
4. Restore the dump into it and verify the row counts.
5. Only then switch the city over.

If anything fails, the old data is still in the dump and on the old volume. The logical dump from Chapter 1 is what makes the sequence work: a file system copy (Option B there) would be locked to the old version and could not be restored into the new one.

### Sub-conclusion

So we choose `cron` on the Pi (Option A) for scheduling and the dump, copy, restore, verify, switch path for the upgrade. Both reuse what we already have and need no extra service, which keeps the whole solution sustainable.

---

## Conclusion

We set out to answer how to choose the backup, storage, scheduling, and upgrade technologies that best meet the "never lose data" requirement from mister mayor Gerald Stap on our Pi. First, for making the backup we choose scheduled `pg_dump`, the only option that is online, portable, and light enough for the Pi. Second, for storing it we choose a two-tier store, recent dumps on the Pi for speed and every dump copied off the Pi for safety, which removes the SD card single point of failure without cloud overhead. Third, for running it we choose `cron` to schedule the job and a dump-restore-verify-switch path for the upgrade, both built in and reboot-safe.

Together those three choices form one coherent toolset, and they share a thread: every decision favours PostgreSQL and Docker built-ins over an extra service, because the team has to keep this alive after Sprint 4 on limited Pi hardware. So the answer to the main question is to back up with scheduled `pg_dump`, store it in two tiers with every dump copied off the Pi, schedule it with `cron`, and upgrade through a dump-restore-verify-switch path that keeps the old data safe throughout. We were honest about the limits: continuous archiving and cloud storage are stronger on paper but too heavy for a five-tile school city on one Pi, so we name them as future options rather than adopt them now.

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

- Hogeschool van Amsterdam. (2026). *City Sim project brief: base requirements (General, Embedded, Back-end)*. Studio Smart Cities, HvA. [Print].
- Otten, M. (2026, May 20). *Sprint 4 mayor delivery feedback (Mats)*. Hogeschool van Amsterdam. [Verbal, offline].
- PostgreSQL Global Development Group. (2024a). *PostgreSQL 16 documentation: pg_dump* [Online]. Retrieved June 5, 2026, from https://www.postgresql.org/docs/16/app-pgdump.html
- PostgreSQL Global Development Group. (2024b). *PostgreSQL 16 documentation: pg_restore* [Online]. Retrieved June 5, 2026, from https://www.postgresql.org/docs/16/app-pgrestore.html
- Stap, G. (2026, May 20). *Sprint 4 feedback on Smart City deliverables (mister mayor Gerald Stap)* [Verbal, offline]. Hogeschool van Amsterdam.

---

## Appendix

### Appendix A - Decision summary

The table below collects the four decisions, the chosen option, what we rejected, and the main reason, so the whole advice can be read at a glance.

| Decision | Chosen | Rejected | Main reason |
|----------|--------|----------|-------------|
| Backup method | `pg_dump` logical backup | File copy; continuous archiving | Online, portable, built-in, fits upgrade |
| Storage location | Recent on Pi + every dump off Pi | Only on Pi; cloud only | Survives SD card failure, stays sustainable |
| Scheduling | `cron` on the Pi | Scheduler container; manual | Built in, light, reboot-safe |
| Upgrade path | Dump, copy, restore, verify, switch | In-place upgrade | Old data stays recoverable throughout |

Every chosen row leans the same way: built-in PostgreSQL or operating-system tools, nothing extra to keep alive.

### Appendix B - Criteria scoring (summary)

Each chosen option scores well on all five criteria (never lose data, survives SD card failure, sustainable, fits the Pi, reuses what we have). The rejected options each fail at least one hard criterion: file copy needs downtime and is version-locked, continuous archiving is too heavy for the Pi, cloud-only storage adds an external dependency, and manual or in-app scheduling is either fragile or extra weight.

### Appendix C - Use of AI

We used an AI assistant (Claude) as a writing aid for this document. It helped restructure the text to the agreed feedback standard, check the APA formatting and the in-text citations, and rephrase passages for clarity. It did not make the technology choices or the trade-offs: the comparison of options, the chosen tools, and the reasoning are our own and were reviewed by the author, who is responsible for the content.
