# Performance Analysis — Sprints 1, 2, 3

## Sprint 1

### Commits
**DLO target:** 6 commits/day
**DLO result:** Week 5: Thursday green, rest empty. Week 6: entirely empty. Week 7: Wednesday/Thursday partly.

![Sprint 1 - Commits per dag](screenshots/sprint1Commits.png)

**Total:** 8 meaningful commits on matin branch (excluding CI/mirror setup commits)

Key commits:
- `6b77a87` feat: add shared backend API and parking tile with docs
- `819d51c` feat: add parking dashboard
- `14f3ea4` feat: add city sim dashboard for all tiles
- `0d54767` docs: add initial backend architecture diagram
- `be96ba2` docs: add Sprint 1 Portflow evidence and learning journal

**Reflection:** The DLO data shows I barely hit the daily commit target. Most days are empty, with activity concentrated on 3-4 days total. The 8 commits are real and meaningful, but the distribution is poor. I worked in bursts (March 18-19) instead of consistently throughout the sprint.

**Development plan:** Spread work more evenly across the sprint in Sprint 2. Aim for at least 2 commits per week on separate days.

### Hours spent
**DLO target:** 6 hours/day
**DLO result:** Nothing tracked. Entirely empty for all 3 weeks.

![Sprint 1 - Hours worked](screenshots/sprint1HoursWorked.png)

**Estimated (from git timestamps):** 20-25 hours total

Breakdown estimate:
- Research and technology selection: 4 hours
- Backend development (API + database): 8 hours
- Dashboard development: 4 hours
- Documentation writing: 4 hours
- Docker setup and testing: 2 hours

**Reflection:** Zero hours logged in DLO. I did the work but did not track it at all. The estimate above is a guess based on session lengths. This makes it impossible for assessors to verify effort.

**Development plan:** Start using DLO time tracking in Sprint 2. Even rough logging is better than nothing.

### Tasks completed
**DLO target:** 4 tasks/week
**DLO result:** 0/4, 0/4, 0/4. Zero tasks logged across all 3 weeks.

![Sprint 1 - Tasks per week](screenshots/sprint1Tasks.png)

Actual work delivered (not logged in DLO):
- FastAPI application with 7 REST endpoints
- PostgreSQL database with 2 tables (sensor_readings, parking_spots)
- Docker Compose configuration (API + database containers)
- City Sim Dashboard with 4 tile panels and live refresh
- ESP32 parking sensor sketch (placeholder for testing)
- Architecture diagram (SVG)
- API specification document
- Database schema document

**Reflection:** I completed 8 deliverables but logged 0 tasks in DLO. The work was done but invisible in the tracking system. This is the same pattern Mats identified: the work exists but is not visible because I did not use the tools.

**Development plan:** Use GitLab issues or DLO task board to track work in Sprint 2. Log tasks as I complete them, not retroactively.

### Merge Requests made
**DLO target:** 2 MRs/week
**DLO result:** 1/2, 0/2, 1/2. Total: 2 MRs in sprint.

![Sprint 1 - MRs made](screenshots/sprint1MergeRequestsMade.png)

**Reflection:** 2 MRs total against a target of 6 (2/week x 3 weeks). Below target but at least some MR activity is captured. The main MR was one large batch at the end of the sprint.

**Development plan:** Make smaller, more frequent MRs in Sprint 2 (at least 2 per sprint, ideally matching weekly target).

### Merge Requests reviewed
**DLO target:** 2 MRs reviewed/week
**DLO result:** 0/2, 0/2, 0/2. Total: 0 reviews.

![Sprint 1 - MRs reviewed](screenshots/sprint1MergeRequestsReviewed.png)

**Reflection:** Zero peer reviews in Sprint 1. The team was not yet doing structured code review, but that is an explanation, not an excuse. I should have initiated review culture.

**Development plan:** Review at least 1 MR from a teammate in Sprint 2 to contribute to team quality.

### Documentation written
**DLO target:** 450 words/day
**DLO result:** Week 5: Thursday green. Week 6: entirely empty. Week 7: Wednesday/Thursday green.

![Sprint 1 - Documentation](screenshots/sprint1Documentation.png)

**Total:** 6 documents in docs/Matin/

- system-architecture.md (3-layer architecture overview)
- database-schema.md (table definitions and ER diagram)
- api-specification.md (all endpoints with examples per tile)
- city_sim_backend_architecture_sprint1.svg (visual diagram)
- portflowEvidenceSprint1.md (sprint evidence)
- index.md (documentation homepage)

**Reflection:** Documentation was written alongside code on the days I was active. The DLO shows green on exactly the days I committed code. The days between are empty. Documentation quality is good but consistency is poor.

**Development plan:** Keep documenting alongside development. Try to write at least something every working day, even if it is just updating a section.

---

## Sprint 2

### Commits
**DLO target:** 6 commits/day
**DLO result:** Week 8: nothing. Week 9: Monday/Thursday/Friday partly. Week 10: Wednesday/Thursday partly.

![Sprint 2 - Commits per dag](screenshots/sprint2Commits.png)

**Total:** 4 own commits on matin branch + 3 merge commits (team branches into main)

Key commits:
- `0e851cc` merge: resolve conflicts merging main into matin (233 commits, 5 file conflicts)
- `ea98332` feat: Docker update, backend ready for Raspberry Pi
- `a441175` docs: Raspberry Pi setup

**Reflection:** Week 8 was completely empty for City Sim because I was focused on Aimee (Group Project). Week 9 was my peak week with actual merge and Docker work. Week 10 tapered off again. The pattern from Sprint 1 repeats: I work in bursts instead of consistently. The DLO shows "partly" on active days, meaning I committed but not at the 6/day target level. Improvement over Sprint 1: at least weeks 9-10 show some activity instead of the 2-day burst pattern.

**Development plan:** In Sprint 3, protect dedicated City Sim time blocks (minimum 2 sessions per week). Do not let Aimee crowd out Learning Group work.

### Hours spent
**DLO target:** 6 hours/day
**DLO result:** Week 8: Thursday partly. Week 9: Wednesday/Thursday partly, Friday green. Week 10: Tuesday green, Friday partly.

![Sprint 2 - Hours worked](screenshots/sprint2HoursWorked.png)

**Reflection:** Better than Sprint 1 (which was entirely empty). I logged some hours in weeks 9-10, with Friday week 9 being a full green day. Week 8 is almost empty, confirming that was my Aimee-focused week. Still far from the 6h/day target, but at least the tracking system is not completely blank anymore.

**Development plan:** Start logging hours on every working day in Sprint 3. The DLO data proves I am doing work in weeks 9-10 but losing visibility in quiet weeks.

### Tasks completed
**DLO target:** 4 tasks/week
**DLO result:** Week 8: 0/4. Week 9: 14/4 (over-target). Week 10: 0/4.

![Sprint 2 - Tasks per week](screenshots/sprint2Tasks.png)

Actual work delivered:
- Merged Thijmen's railroad crossing code (233 commits, 5 conflicts resolved)
- Docker Compose hardened for production (.env, restart policy, no hot-reload)
- deploy.sh created for one-command Pi deployment
- Dashboard URL changed from hardcoded localhost to window.location.origin
- Raspberry Pi deployment guide written
- Challenges overview documented

**Tasks NOT completed (carried to Sprint 3):**
- Port 80 configuration (not done)
- Actual deployment on Pi (prep only, not deployed)
- Parking sensor test on Pi network (not done)

**Reflection:** Week 9 was a massive spike (14 tasks, 3.5x target) followed by nothing in week 10. This confirms the burst pattern. I did all merge and Docker work in one concentrated push. The zero weeks (8 and 10) are the problem. A consistent 4/week would have been healthier and more sustainable than 0-14-0.

**Development plan:** In Sprint 3, aim for 3-5 tasks per week consistently. Break large work into smaller logged tasks instead of batching.

### Merge Requests made
**DLO target:** 2 MRs/week
**DLO result:** Week 8: 0/2. Week 9: 2/2 (target hit). Week 10: 1/2. Total: 3 MRs in sprint.

![Sprint 2 - MRs made](screenshots/sprint2MergeRequestsMade.png)

**Reflection:** 3 MRs made in Sprint 2 according to DLO tracking, exceeding my Sprint 1 development plan target of 2/sprint. Most likely related to the railroad crossing merge work which involved multiple integration steps. Week 9 hit the weekly target exactly. This is improvement from Sprint 1 (2 total) and shows the merge work generated multiple MR touchpoints.

**Development plan:** In Sprint 3, maintain this level. Split work into at least 2 MRs: one for carry-over fixes, one for new features.

### Merge Requests reviewed
**DLO target:** 2 MRs reviewed/week
**DLO result:** Week 8: 0/2. Week 9: 1/2. Week 10: 2/2 (target hit). Total: 3 reviews in sprint.

![Sprint 2 - MRs reviewed](screenshots/sprint2MergeRequestsReviewed.png)

**Reflection:** 3 MRs reviewed in Sprint 2 (week 9: 1, week 10: 2). This is clear improvement from Sprint 1 (0 reviews) and shows my team integration work also included peer review activity. I underestimated this in my earlier self-assessment because I did not track it actively. The DLO data captures it accurately. Week 10 even hit the weekly target of 2.

**Development plan:** In Sprint 3, maintain or increase review activity. Continue reviewing teammates' work as it generates visible peer engagement.

### Documentation written
**DLO target:** 450 words/day
**DLO result:** Week 8: nothing. Week 9: Monday/Thursday/Friday green. Week 10: Wednesday/Thursday/Sunday green.

![Sprint 2 - Documentation](screenshots/sprint2Documentation.png)

**Total:** 4 documents in docs/Matin/

- raspberry-pi-deployment.md (deployment guide with SSH, Docker, troubleshooting)
- challenges-overview.md (6 technical challenges encountered and resolved)
- portflowEvidenceSprint2.md (sprint evidence)
- index.md (updated with Sprint 2 links)

**Reflection:** Documentation improved over Sprint 1. Weeks 9-10 show 6 green days compared to Sprint 1's 3 green days. Week 8 is empty (Aimee focus), but once I engaged with City Sim in week 9, documentation was consistent alongside the code work. The Pi deployment guide was written before actual deployment, which is backwards. Documentation quality matters more when it reflects verified reality.

**Development plan:** In Sprint 3, write documentation after verifying it works on the actual Pi. Connect all docs to the problem statement.

### Sprint 2 overall assessment

The DLO data tells a clear story: week 8 was lost to Aimee, week 9 was a productive burst, week 10 was partial. Despite the inconsistency, Sprint 2 shows measurable improvement over Sprint 1 in MRs made (3 vs 2), MRs reviewed (3 vs 0), and hours logged (some vs none). The problem was not lack of work but lack of framing: I did not connect any of this to the parking search problem, did not write learning goals, and Mats' Mayor Delivery feedback ("verras me volgende keer") was a warning I did not sufficiently act on. That is what Sprint 3 fixes.

---

## Sprint 3

### Commits
**DLO target:** 6 commits/day
**DLO result:** [Screenshot to be added at end of week 13]

![Sprint 3 - Commits per dag](screenshots/sprint3Commits.png) (pending)

**Total so far:** 3 own commits merged to main + 1 pending push + 1 upcoming (this document)

Key commits:
- `3f6b3e5` feat: implement speed camera API, database models, and comprehensive project documentation (Apr 23)
- `dbb1bc8` docs: add human-centered problem statement for parking tile (May 4)
- `42d99bd` fix(docker): expose api on port 80 with dashboard on root (May 4)
- `faf732d` docs: add sprint 0 learning journal STARR reflection (May 5, pending push)

**Mid-sprint reflection:** Activity is concentrated in two clusters: week 12 (speed camera) and week 13 (problem statement + port 80 + documentation). Week 11 appears empty in the commit log. The burst pattern from Sprint 1 and 2 continues. However, each commit is a complete deliverable (feature, fix, or document), not fragmented work. I expect the DLO screenshot will show partly/green on April 23, May 4, and May 5, with gaps between.

**Development plan:** In Sprint 4, commit at least every other day. Use smaller commits for incremental progress instead of waiting until a feature is complete.

### Hours spent
**DLO target:** 6 hours/day
**DLO result:** [Screenshot to be added at end of week 13]

![Sprint 3 - Hours worked](screenshots/sprint3HoursWorked.png) (pending)

**Estimated so far:** 18-22 hours total (still no formal time tracking implemented)

Breakdown estimate:
- Speed camera API (model, schemas, router, dashboard panel): 6 hours
- Problem statement research and writing: 3 hours
- Portflow evidence restructuring (12 items, descriptions, permalinks): 5 hours
- Port 80 fix and deploy script updates: 2 hours
- Learning Journal documents (STARR, goals, performance analysis): 4 hours

**Mid-sprint reflection:** I still have not implemented formal time tracking despite committing to it in Sprint 1 and Sprint 2 development plans. Three sprints of the same broken promise. The work is real (20+ hours estimated), but unverifiable. If the DLO screenshot shows empty again, that is entirely my fault for not logging.

**Development plan:** For Sprint 4, this is non-negotiable. Use a spreadsheet with date, start time, end time, task. Fill it in during the session, not after. If I fail to track hours again in Sprint 4, it becomes a pattern I cannot explain away.

### Tasks completed
**DLO target:** 4 tasks/week
**DLO result:** [Screenshot to be added at end of week 13]

![Sprint 3 - Tasks per week](screenshots/sprint3Tasks.png) (pending)

**Completed so far:**
- Speed camera API: 5 REST endpoints, SpeedReading model, 3 Pydantic schemas
- Dashboard: 5th panel (speed camera) with violation highlighting and stats
- Port 80: Docker port mapping fix, deploy.sh updated, dashboard moved to root URL
- Problem statement: human-centered framing of parking search traffic
- Portflow cleanup: 12 evidence items reframed with descriptions and permalinks
- Learning Journal: Sprint 0 STARR, SMART learning goals (all sprints), performance analysis (all sprints)
- SSH access configured for remote Pi management
- portflowMasterSheet.md: single copy-paste document for Portflow editing

**Still in progress:**
- Expert feedback requests (Mats 1-on-1, Gerald validation)
- Mayor Delivery preparation (this week)
- Architecture SVG update

**Mid-sprint reflection:** Task count is higher than Sprint 1 or 2, but much of it is catch-up work (problem statement, learning goals, Portflow restructuring). The actual new feature work is the speed camera API and port 80 fix. Whether this shows in DLO depends on whether I logged tasks there. I suspect I did not.

**Development plan:** In Sprint 4, log every task in DLO as I start it. Reduce scope to pure delivery work (no more catch-up).

### Merge Requests made
**DLO target:** 2 MRs/week
**DLO result:** [Screenshot to be added at end of week 13]

![Sprint 3 - MRs made](screenshots/sprint3MergeRequestsMade.png) (pending)

**Total so far:** 1 MR (matin to main, merged as `3ecfe5e` on May 5)

**Mid-sprint reflection:** One MR so far, same pattern as previous sprints. My development plan from Sprint 2 said "split into at least 2 MRs" but the sprint is not over yet. If I push and merge the learning journal documents as a second MR this week, I hit the self-set target for the first time.

**Development plan:** In Sprint 4, make 2 MRs minimum. First MR early in the sprint (carry-over work), second MR mid-sprint (new features).

### Merge Requests reviewed
**DLO target:** 2 MRs reviewed/week
**DLO result:** [Screenshot to be added at end of week 13]

![Sprint 3 - MRs reviewed](screenshots/sprint3MergeRequestsReviewed.png) (pending)

**Total so far:** 0

**Mid-sprint reflection:** Back to zero after the Sprint 2 improvement (3 reviews). This is disappointing. The sprint is not over, and I can still review a teammate's MR this week before the Progress Review. But if I do not, this becomes a regression.

**Development plan:** In Sprint 4, review 1 MR in the first week. Do not wait. This also generates peer feedback evidence for Portflow.

### Documentation written
**DLO target:** 450 words/day
**DLO result:** [Screenshot to be added at end of week 13]

![Sprint 3 - Documentation](screenshots/sprint3Documentation.png) (pending)

**New documents in Sprint 3:**
- problemStatement.md (human-centered problem framing)
- portflowEvidenceSprint3.md (sprint evidence with STARR)
- learningJournalSprint0.md (Sprint 0 STARR reflection)
- personalLearningGoalsAllSprints.md (SMART goals, 3 sprints)
- performanceAnalysisAllSprints.md (this document)
- portflowMasterSheet.md (Portflow copy-paste reference)

**Updated documents:**
- raspberry-pi-deployment.md (port 80, SSH access)
- api-specification.md (speed camera endpoints)
- database-schema.md (speed_readings table)
- index.md (links to new documents)

**Mid-sprint reflection:** This is my most documentation-heavy sprint by far. Word count easily exceeds Sprint 1 and 2 combined. The DLO should show green on the days I wrote (April 23, May 4, May 5). Most documentation is catch-up work that should have existed earlier. The technical docs (API spec, schema) were updated alongside code, which is the correct pattern. The Portflow/Journal docs are one-time cleanup.

**Development plan:** In Sprint 4, documentation should be minimal and maintenance-only. The story is told. Focus on delivery proof (demo recording, final architecture diagram).

### Sprint 3 overall assessment (mid-sprint)

Sprint 3 is a recovery sprint. The DLO data will likely show the same burst pattern (concentrated activity in weeks 12-13) but the output is qualitatively different from previous sprints. For the first time, all work is connected to a defined city problem. The problem statement exists. The learning goals are written. The evidence is reframed. Whether this translates to a better score from Mats depends on whether the story lands at the Mayor Delivery and whether I collect expert feedback before the Progress Review.

Persistent gaps across all 3 sprints: no formal time tracking, burst work patterns instead of consistent daily activity, zero or low MR reviews. These are honest patterns I need to address in Sprint 4.

[End of sprint reflection - to be added before Progress Review week 13. At that point I will add the actual DLO screenshots and final numbers.]
