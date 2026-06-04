# Analysis - Data persistence risks and backup strategies

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | June 2026 |
| **Version** | 1.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Analysis (first of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - Six realistic ways the city loses its stored data
3. Chapter 2 - Off-Pi backups make every scenario survivable
4. Chapter 3 - A tested restore turns a backup into a recovery
5. Conclusion
6. Recommendation
7. References
8. Appendix

---

## Introduction

### Context

City Sim is a miniature smart city that our team, The Embedded Alliance, builds for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Each of the five students builds one physical city tile with its own embedded sensors. All tiles send their data to one shared backend that we maintain. The team is:

- Matin Khajehfard, who builds the parking tile and owns the shared backend
- Betul Aydin, the scrum master, who also gives backend support
- Gurpreet Singh, who builds the streetlight and speed-camera tile (ESP32 with an LDR light sensor)
- Thijmen Walter, who builds the railroad crossing tile (ESP32 with infrared sensors and an OLED display)
- Wesley Oerlmans, who builds the traffic light tile (ESP32 with LEDs)

The shared backend is a FastAPI application with a PostgreSQL 16 database, packaged in Docker, running on one Raspberry Pi on the HvA network (145.92.8.137, port 80). PostgreSQL keeps its data in a Docker named volume (`pgdata`) on the Pi's SD card. Every reading from every tile ends up in this one database.

This document is the **Analysis** outcome for Learning Goal 2 (database backup and data persistence). In our way of working we follow the order Analysis > Advise > Design > Realise. This first document does the research into what can destroy our data and how to protect it. It does not yet pick a solution; choosing the tools is the job of the Advise document, and laying out the architecture is the job of the Design document that follow.

### Who this document is for, and why it looks the way it does

We write this Analysis for a technical reader: the City Sim development team, meaning the embedded and backend engineers, and the technical lead on the client side. Those are the people who run the backend and have to keep the data alive, so they need the detail in full, including the Docker config and the database internals. A teacher or the wider public is not the audience, which is why the document is a detailed, internal, engineering text and not a glossy summary. The author is Matin Khajehfard, the junior backend developer who owns the shared backend, so the failure analysis comes from the person who knows the running setup. The client is Gemeente Amsterdam, afdeling Verkeer en Openbare Ruimte (V&OR); the client is not the same as the audience, and Mats Otten represents the client at the mayor delivery. We mark the document Internal because it exposes the database setup of a live system, which we do not want to share outside the project. The version is 1.1 because this is a revision after the round-two writing feedback from mister mayor Gerald Stap, not a first draft.

### Why this matters to the client

For Gemeente Amsterdam the value of a smart city is in the data it collects over time. The HvA project brief requires that city data is stored for future analysis, that the data structure is normalized, and that every record is saved with relevant meta-data such as date, time, device, IP, and location (Hogeschool van Amsterdam, 2026). Translated to our case that means the brief does not just ask us to store readings; it asks us to keep a normalized history with its meta-data so the department can analyse it later. The stored history is therefore exactly the asset we must not lose. If the history is gone, the department loses the very thing the city was built to produce: a record it can analyse to make parking and traffic policy. A live sensor that works is not enough; the stored history has to survive crashes, power cuts, and human mistakes.

### Requirements we have to meet

The work in this document is bounded by requirements from two sources. We carry these through into the Advise, Design, and Realise documents.

**HvA Smart City requirements (project brief) (Hogeschool van Amsterdam, 2026):**

- City data is stored for future analysis. The data structure is normalized and saved with relevant meta-data (date/time, device, IP, location). For us this is the load-bearing requirement: the normalized tables and their meta-data columns are the thing the backup has to protect.
- The backend is shared and collects all data from the hubs (sensors and actuators).
- All back-end services run in Docker.
- We document our design decisions and trade-offs as Portflow evidence.

**Requirements added per mayor delivery:**

- Sprint 4 (mister mayor Gerald Stap): a database upgrade and a backup, so the data is never lost (Stap, 2026).
- Sprint 4 (Mats Otten, client representative): research how errors happen, how to prevent them, and how to catch them; design something sustainable, not a one-time backup script (Otten, 2026).

The Sprint 4 request from mister mayor Gerald Stap is the direct reason for this learning goal. Mats then sharpened it: do not write a single backup script and call it done. Research the failure scenarios, prevent what we can, and catch the rest. This document answers that research request for the data side of the backend, the same way Learning Goal 1 answered it for the service side. That reuse is deliberate: the same challenge, "keep the city running and its data safe", shows up in Learning Goal 1 for the running service and here for the stored data, and we carry the prevent-and-catch arc across both.

### Main question

How can City Sim lose its stored sensor data, and how can we prevent that loss and recover from it when it still happens?

### Sub-questions

1. How can our current Docker and Raspberry Pi setup lose stored data?
2. How do other projects prevent data loss with PostgreSQL in Docker?
3. How can data loss be caught and recovered from when prevention is not enough?

We answer the sub-questions in order. We went through our own `docker-compose.yml`, the database configuration, and the way the data is stored, and we listed every way the stored data could be lost. For each scenario we checked whether our current setup protects against it. We then compared our setup against the official PostgreSQL documentation on backup and recovery, which is the standard reference for this topic. We use "we" throughout because this is team work, even though the backend is ours to own.

---

## Chapter 1 - Six realistic ways the city loses its stored data

The first sub-question asks how our current Docker and Raspberry Pi setup can lose stored data. To answer it we started from what the setup actually is. The database runs as one `db` container (PostgreSQL 16) with its data in a Docker named volume called `pgdata`, stored on the Pi's SD card. There are no backups, no export scripts, and no copy of the data anywhere off the Pi. If the `pgdata` volume is gone, the history is gone. So we walked through the setup line by line and, for each scenario we could think of, asked one question: if this happens, do we still have the data? We grouped what we found by cause, hardware, software, and human, because the cause shapes how we would later defend against it.

Across those three causes we found six realistic ways the data disappears, and they get worse the closer they sit to the SD card. The hardware causes are the heaviest. The SD card itself can simply fail; the Pi boots and stores everything on that card, and SD cards wear out, especially under the constant small writes a database makes. When the card dies the volume dies with it, and there is no copy anywhere else, so this is the scenario with the highest impact because it takes everything at once. Close to it is a power cut while PostgreSQL is writing. A power loss at HvA mid-write can leave the database files inconsistent; PostgreSQL uses a write-ahead log so it can replay committed changes and recover a consistent state after a crash, which is the standard way a database survives an unclean shutdown (Kleppmann, 2017), but a power cut on a worn card can still corrupt files past automatic recovery.

The human causes are the most likely to happen on any normal day. The sharpest is someone running `docker compose down -v`: the `-v` flag deletes named volumes, so one person cleaning up containers with the wrong flag wipes `pgdata` and the whole history in a single command, with nothing to warn them and nothing to bring it back. Softer but just as real is accidental deletion through the API or SQL, a wrong `DELETE` during testing, a bad migration, or a mistaken admin action; here the volume is fine but the data inside it is wrong, and only a backup from before the mistake gets it back. The software causes round out the list. File or volume corruption from a bug, a disk-full event, or a failed write can damage the database files even while the card is physically healthy, after which PostgreSQL may refuse to start. Finally, the database upgrade that mister mayor Gerald Stap asked for is its own risk: moving from one major PostgreSQL version to the next changes the on-disk format, and an upgrade gone wrong without a backup first can leave the data unreadable by both the old and the new version.

The overview below collects the six scenarios with their cause, whether our setup protects against them today, and the direction of the fix we carry into the Advise. We read it as a ranking of exposure, not just a list.

| # | Scenario | Cause | Protected today? | Direction of the fix |
|---|----------|-------|------------------|----------------------|
| 1 | SD card failure | Hardware | No | Off-Pi backups |
| 2 | Power loss mid-write | Hardware | Partly (write-ahead log) | Backups + clean shutdown |
| 3 | `docker compose down -v` | Human | No | Document + no `-v` in scripts |
| 4 | Accidental deletion | Human | No | Restore from backup |
| 5 | File / volume corruption | Software | No | Restore from backup |
| 6 | Failed upgrade | Software | No | Mandatory backup before upgrade |

Reading down the "protected today" column tells the story: only scenario 2 has any defence, and even that is partial. Every other row is an open hole, and almost every fix points back to the same missing thing, a backup that does not live on the SD card.

### Sub-conclusion

So our setup has effectively zero protection against these six scenarios. The named volume survives a container crash, which is good, but every scenario here destroys or corrupts the volume itself or the data inside it. The single biggest risk is the SD card (scenario 1), because the only copy of the data lives on it. The most likely human risk is `docker compose down -v` (scenario 3), because it is one keystroke away during normal work. We also note a related exposure: the `db` container publishes port 5432 to the host, which widens who can reach the database (see Appendix A).

---

## Chapter 2 - Off-Pi backups make every scenario survivable

The second sub-question asks how other projects prevent data loss with PostgreSQL in Docker, and how much of that we already have. Preventing loss is better than recovering from it, so before looking at recovery we wanted to know what reduces the chance the Chapter 1 scenarios happen at all. To find out we read the PostgreSQL documentation on backup methods, the standard reference for this stack, and we held it against how our own `db` container is configured today so we knew what we were starting from.

The clearest finding is that one mechanism sits underneath all six scenarios: regular backups. A backup is not really prevention, it is the safety net beneath every row of the table. PostgreSQL ships `pg_dump`, which writes a consistent logical copy of the database to a single file while the database keeps running (PostgreSQL Global Development Group, 2024a), so a scheduled `pg_dump` means the worst case is losing the data since the last backup, not all of it. The dump is a consistent snapshot taken without locking out writers, which is why the city can stay online while it runs (PostgreSQL Global Development Group, 2024a). The safety net only holds, though, if the copy does not share the fate of the original. A backup that sits on the same SD card dies with the card, so copying each backup off the Pi, to another machine, a network share, or the HvA infrastructure, is what actually removes the single point of failure behind scenario 1.

On top of that net, two smaller defences already exist and a third is free to add. Our use of a named volume (`pgdata`) instead of writing inside the container is correct: when the container is removed the data stays, so container-crash loss is already handled, and we keep that. PostgreSQL's write-ahead log is on by default and lets the database recover a consistent state after an unclean shutdown by replaying its committed log (Kleppmann, 2017), which softens the power-cut scenario 2; it does not replace backups, but it lowers how often a power cut turns into real loss. The cheapest defence of all is procedural: documenting that `docker compose down -v` must never be run on the Pi, and never exposing that flag in a helper script, directly lowers the chance of the accidental wipe in scenario 3.

### Sub-conclusion

So most data loss is made survivable by one thing we do not have yet: scheduled backups stored off the Pi. The named volume and the write-ahead log already give us a base layer, and a documented rule guards the worst human mistake. Prevention here is not exotic technology; it is `pg_dump` on a schedule plus a copy that does not live on the same SD card. Because it leans on tools that ship with PostgreSQL and Docker rather than a new service to babysit, it is the sustainable direction the client asked for, not a one-off.

---

## Chapter 3 - A tested restore turns a backup into a recovery

The third sub-question asks how data loss can be caught and recovered from when prevention is not enough. Backups are worthless if nobody knows they failed or if the restore has never been tried, and Mats Otten asked specifically how we catch problems and recover, so this is the half of the work about knowing a backup is good and actually getting the data back. To answer it we read the PostgreSQL documentation on restoring from a dump and looked at what it takes to verify a backup rather than just produce one.

What we found is that a backup is only ever half a safety net; the restore is the other half. `pg_dump` produces a file, but it is `pg_restore` or `psql` loading that file back into a database that turns the backup into a recovery (PostgreSQL Global Development Group, 2024b), and without a written, tested restore procedure a backup is only a hope. Because the restore is the half that actually saves the city, it has to be proven, not assumed, which leads straight to detection. A backup job can run every night and quietly write a broken or empty file that nobody notices until the day they need it, so restoring a backup into a throwaway database and checking the row counts is how we catch that the backup is genuinely usable. That row-count check is the detection step Mats asked for, applied to data.

The same restore covers the two remaining holes. For the upgrade risk in scenario 6, the safe path is to take a full `pg_dump`, bring up the new PostgreSQL version, restore the dump, and verify; if anything goes wrong the old data is still in the dump file, which turns the database upgrade mister mayor Gerald Stap asked for from a gamble into a controlled step. And to know the schedule itself is alive, a simple look at the age and size of the latest backup file tells us whether backups are still being produced, so a backup that suddenly stopped growing or stopped appearing is a caught failure before it becomes a disaster.

### Sub-conclusion

So catching data loss is about proving the safety net works: a written and tested restore procedure, a periodic test restore to confirm the backup is usable, a mandatory backup before any upgrade, and a basic freshness check on the latest backup file. Together these turn backups from a checkbox into a recovery the team can trust.

---

## Conclusion

We set out to answer how City Sim can lose its stored sensor data, and how we can prevent that loss and recover from it when it still happens. First, our setup can lose the data in six realistic ways, ranging from SD card failure and a power cut to an accidental `docker compose down -v`, accidental deletion, file corruption, and a failed upgrade, and today only the named volume and the write-ahead log give any base safety. Second, most of those scenarios become survivable through one mechanism we do not have yet: scheduled `pg_dump` backups with a copy stored off the Pi, layered on the defences we already have. Third, the loss we cannot prevent can still be caught and undone with a written, tested restore procedure, a periodic test restore, a mandatory backup before the upgrade mister mayor Gerald Stap asked for, and a freshness check on the latest backup file.

Together those three findings point one way. The data is the asset the HvA brief tells us to keep, the normalized history with its meta-data, and the single thing standing between that asset and loss is an off-Pi backup we can prove restores. So the answer to the main question is that City Sim loses its data mainly through its one SD card and a handful of human and software mistakes, and we prevent and recover from that with scheduled off-Pi `pg_dump` backups plus a tested restore, freshness check, and backup-before-upgrade. That combination meets the "never lose data" request from mister mayor Gerald Stap and the request from Mats Otten for a sustainable system that prevents and catches errors, because it builds on standard PostgreSQL tools instead of a one-off script.

---

## Recommendation

For the Advise and Design outcomes we recommend the following, in priority order:

1. Add scheduled `pg_dump` backups of the database, running automatically on the Pi. This is the single most important missing piece.
2. Copy each backup off the Pi so an SD card failure does not take the backups with it (scenario 1).
3. Write and test a restore procedure, and run a periodic test restore so we know the backups are usable, not just present.
4. Make a full backup mandatory before the PostgreSQL upgrade, and verify the restore on the new version (scenario 6).
5. Document that `docker compose down -v` must never be run on the Pi, and add a freshness check on the latest backup file (scenario 3 and detection).

The Design document turns these recommendations into a concrete backup, restore, and upgrade architecture, and the Realise document builds and tests it against the "never lose data" requirement from mister mayor Gerald Stap and hands the result over to the team for maintenance.

---

## References

- Hogeschool van Amsterdam. (2026). *City Sim project brief: base requirements (General, Embedded, Back-end)*. Studio Smart Cities, HvA. [Print].
- Kleppmann, M. (2017). *Designing data-intensive applications*. O'Reilly Media. [Print].
- Otten, M. (2026, May 20). *Sprint 4 mayor delivery feedback (Mats)*. Hogeschool van Amsterdam. [Verbal, offline].
- PostgreSQL Global Development Group. (2024a). *PostgreSQL 16 documentation: pg_dump* [Online]. Retrieved June 5, 2026, from https://www.postgresql.org/docs/16/app-pgdump.html
- PostgreSQL Global Development Group. (2024b). *PostgreSQL 16 documentation: pg_restore* [Online]. Retrieved June 5, 2026, from https://www.postgresql.org/docs/16/app-pgrestore.html
- Stap, G. (2026, May 20). *Sprint 4 feedback on Smart City deliverables (mister mayor Gerald Stap)* [Verbal, offline]. Hogeschool van Amsterdam.

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

### Appendix B - Why a backup alone is not enough

A backup that is never restored is a guess. The Realise document proves the restore by loading a backup into a throwaway database and checking that the row counts match the source, so the team knows the safety net actually holds before they ever need it.

### Appendix C - Use of AI

We used an AI assistant (Claude) as a writing aid for this document. It helped restructure the text to the agreed feedback standard, check the APA formatting and the in-text citations, and rephrase passages for clarity. It did not produce the engineering work or the analysis: the failure scenarios, the assessment of our own setup, and the recommendations are our own and were reviewed by the author, who is responsible for the content.
