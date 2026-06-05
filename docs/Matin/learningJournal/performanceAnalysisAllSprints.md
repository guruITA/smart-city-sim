# Performance Analysis - Sprints 1, 2, 3, 4

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
**DLO result:** Week 11: Mon-Tue active, Thu green. Week 12: Wed-Thu active (speed camera built Apr 23). May Holiday: empty. Week 13: Mon-Thu green — most active week of the semester.

![Sprint 3 - Commits per dag](screenshots/sprint3Commits.png)

Key commits:
- `3f6b3e5` feat: implement speed camera API, database models, and comprehensive project documentation (Apr 23)
- `dbb1bc8` docs: add human-centered problem statement for parking tile (May 4)
- `42d99bd` fix(docker): expose api on port 80 with dashboard on root (May 4)
- `faf732d` docs: add sprint 0 learning journal STARR reflection (May 5)
- `483abbb` docs: learning goals + performance analysis (May 5)
- Multiple commits week 13: Portflow restructuring, Sprint 3 deliverables, dashboard bug fixes, traffic light integration

**Reflection:** The burst pattern continues but with more spread. Week 11 had light activity, week 12 concentrated on the speed camera, and week 13 was a sustained push across 4 days. That last week is the most consistent I have been all semester. The improvement is visible: Sprint 1 had 2 active days, Sprint 2 had 4-5, Sprint 3 has about 8.

**Development plan:** In Sprint 4, commit at least every other day. Use smaller commits for incremental progress.

### Hours spent
**DLO target:** 6 hours/day
**DLO result:** Week 11: Mon red, Wed green, Thu red. Week 12: Tue/Wed/Fri red, Thu green. May Holiday: Mon red only. Week 13: Mon-Wed green.

![Sprint 3 - Hours worked](screenshots/sprint3HoursWorked.png)

**Reflection:** Mixed results. Some tracking happened (better than Sprint 1 which was empty), but most days are red — meaning I logged hours but below the 6h target. Week 13 Mon-Wed are green, matching the commit data: that was the most productive stretch. The May Holiday week is almost empty. Overall: hours are being tracked now (improvement), but still below target on most days.

**Development plan:** Sprint 4: log hours on every working day. Use a spreadsheet with start/end times if DLO is not convenient enough.

### Tasks completed
**DLO target:** 4 tasks/week
**DLO result:** 0/4 across all 4 weeks. Zero tasks logged in DLO.

![Sprint 3 - Tasks per week](screenshots/sprint3Tasks.png)

Actual work delivered (not logged in DLO):
- Speed camera API: 5 REST endpoints, SpeedReading model, 3 Pydantic schemas, dashboard panel
- Traffic light integration: GET endpoints, dashboard panel updated for Wesley's dedicated API
- Port 80: Docker port mapping, deploy.sh updated, dashboard moved to root URL
- Dashboard bug fixes: distance_cm crash fix, traffic double prefix fix
- Problem statement: human-centered framing of parking search traffic
- Portflow cleanup: 12 evidence items reframed with descriptions and permalinks
- Sprint 3 deliverable docs: 3 learning goals + 4 deliverable documents (Analysis, Design, Realise, Advise)
- Learning Journal: Sprint 0 STARR, SMART learning goals, performance analysis

**Reflection:** Same problem as Sprint 1: lots of work delivered, zero tasks logged. Three sprints in a row with 0/4 on task tracking. This is a pattern I keep promising to fix but have not fixed. The work is real but invisible in DLO.

**Development plan:** Sprint 4: this is non-negotiable. Log tasks in DLO as I start them. If I fail again, I cannot explain it away.

### Merge Requests made
**DLO target:** 2 MRs/week
**DLO result:** Week 11: 1/2. Week 12: 5/2. May Holiday: 0/2. Week 13: 9/2. Total: 15 MRs.

![Sprint 3 - MRs made](screenshots/sprint3MergeRequestsMade.png)

**Reflection:** Massive improvement. Sprint 1: 2 MRs total. Sprint 2: 3 MRs total. Sprint 3: 15 MRs total. Week 12 (5) was the speed camera and docs work. Week 13 (9) was the Portflow restructuring, dashboard fixes, and traffic integration — each pushed and merged separately. This matches my Sprint 2 development plan: "split work into at least 2 MRs." I exceeded that by a lot. The smaller, more frequent MR pattern worked.

**Development plan:** Maintain this in Sprint 4. Keep making small, focused MRs instead of batching.

### Merge Requests reviewed
**DLO target:** 2 MRs reviewed/week
**DLO result:** Week 11: 0/2. Week 12: 0/2. May Holiday: 0/2. Week 13: 3/2. Total: 3 reviews.

![Sprint 3 - MRs reviewed](screenshots/sprint3MergeRequestsReviewed.png)

**Reflection:** Same total as Sprint 2 (3 reviews), all concentrated in week 13. Weeks 11-12 and the holiday had zero reviews. The improvement happened late but it happened. Week 13 exceeded the weekly target (3/2). The reviews in week 13 were on Thijmen's eink display merge and Wesley's traffic light code — both directly relevant to the dashboard integration I was doing.

**Development plan:** Sprint 4: review at least 1 MR in the first week. Do not wait until the last week.

### Documentation written
**DLO target:** 450 words/day
**DLO result:** Week 11: Tue and Thu green. Week 12: Wed and Thu green. May Holiday: empty. Week 13: Mon-Wed green. Total: 7 green days.

![Sprint 3 - Documentation](screenshots/sprint3Documentation.png)

Documents written in Sprint 3:
- problemStatement.md
- portflowEvidenceSprint3.md (restructured)
- portflowEvidenceSprint1.md (restructured)
- learningJournalSprint0.md
- personalLearningGoalsAllSprints.md
- performanceAnalysisAllSprints.md (this document)
- portflowMasterSheet.md
- Sprint 3 folder: 3 learning goal docs + 4 deliverable docs (Analysis, Design, Realise, Advise)

Updated documents:
- api-specification.md (speed camera + traffic endpoints)
- database-schema.md (speed_readings + traffic tables)
- index.md (links to new documents)

**Reflection:** Most documentation-heavy sprint by far. 7 green days compared to Sprint 1 (3 days) and Sprint 2 (6 days). Much of it is catch-up work (problem statement, learning goals, Portflow restructuring). The Sprint 3 deliverable docs are the first time I wrote structured Analysis/Design/Realise/Advise documents per challenge. Documentation was consistent on active days.

**Development plan:** Sprint 4: documentation should be incremental. Write deliverable docs as I work, not after.

### Sprint 3 overall assessment

Sprint 3 was a recovery sprint. The DLO data confirms both improvement and persistent gaps:

**Improved:**
- MRs made: 15 total (vs 2 in S1, 3 in S2) — biggest improvement of the semester
- MRs reviewed: 3 in week 13, hitting the weekly target
- Documentation: 7 green days, most consistent spread yet
- Commits: week 13 had 4 consecutive active days — a first
- Hours: some tracking happened (better than S1 which was blank)

**Still broken:**
- Tasks: 0/4 across all weeks, third sprint in a row
- Hours: most days red, still below target
- Burst pattern: May Holiday week was empty, bulk of work in week 13
- Reviews concentrated in final week instead of throughout

**What changed qualitatively:** For the first time, all work is connected to a defined city problem (parking search traffic). Problem statement exists. Learning goals are written. Evidence is structured per challenge with only relevant outcomes. Expert feedback was requested (Bernardo, Gerald). This addresses Mats' core feedback from Sprint 2.

**Persistent gaps for Sprint 4:** Task tracking (must start logging in DLO), hour tracking (use spreadsheet if DLO fails), review distribution (week 1 not week 4).

---

## Sprint 4

Sprint 4 ran over four weeks: week 14 (11-17 May) to week 17 (01-07 Jun). This is the sprint where I tested every Sprint 3 development plan against real DLO data.

### Commits
**DLO target:** 6 commits/day
**DLO result:** Week 14: Mon, Tue, Thu active. Week 15: Mon and Tue only, light. Week 16: Thu, Fri, Sat, Sun active, so I worked into the weekend. Week 17: Mon to Fri active, five days in a row.

![Sprint 4 - Commits per dag](screenshots/sprint4Commits.png)

**Total:** 34 commits by me in the sprint window (11 May to 5 Jun), the most of any sprint.

Key commits:
- `6a2039c` docs: add Sprint 4 LG1 backend clustering deliverables and resilience setup
- `821ffed` docs: add Sprint 4 LG2 database backup deliverables and backup scripts
- `0cfeb3b` feat: add backend override (emergency corridor) as Sprint 4 surprise feature
- `5de1553` feat: deploy clustered backend live and harden failover
- `cf5301e` feat: add K3s autoscaling deployment for the backend
- `1c2ea26` docs: apply the round-2 feedback to all Sprint 4 deliverables

**Reflection:** The burst pattern is finally breaking. Week 14 had three active days, week 15 dipped, week 16 picked up across the weekend, and week 17 was five active days in a row, Mon to Fri. That last week is the most consistent stretch of the whole semester, better than the four days I was proud of in Sprint 3. I still do not hit six commits on every day, but the spread across weeks is the real change. Sprint 1 had two active days, Sprint 4 has about fourteen.

**Development plan:** Keep the every-day rhythm from week 17 going into the internship. Small commits on more days beats a big push at the end.

### Hours spent
**DLO target:** 6 hours/day
**DLO result:** Week 14: Mon green, Tue red, Thu green. Week 15: Mon red, Tue light, Fri green. Week 16: Mon red, Thu red, Fri red, Sat green. Week 17: Tue red, Wed green, Thu red.

![Sprint 4 - Hours worked](screenshots/sprint4HoursWorked.png)

**Reflection:** This is the first sprint where I logged hours in every single week. That is the improvement I kept promising since Sprint 1. The honest part is that most days are red, which means I logged time but under the six hour target, with a few full green days (week 14 Mon and Thu, week 15 Fri, week 16 Sat, week 17 Wed). So the habit is finally there, but the daily target is not. I track now, I just need the days to be fuller.

**Development plan:** Log start and end times on the same day, not from memory afterwards. A red day with real hours is still better than an empty one, but I want more green.

### Tasks completed
**DLO target:** 4 tasks/week
**DLO result:** Week 14: 13/4. Week 15: 0/4. Week 16: 0/4. Week 17: 20/4. Total: 33 tasks logged.

![Sprint 4 - Tasks per week](screenshots/sprint4Tasks.png)

Actual work delivered:
- LG1 backend clustering: `docker-compose.cluster.yml`, `nginx.conf`, `tests/resilience/`, deployed live on port 80 with a hardened failover
- LG1 K3s autoscaling: `backend/k8s/` manifests, HorizontalPodAutoscaler, parallel rollout
- LG2 database backup: `backup.sh`, `restore.sh`, `check_backup.sh`, cron, restore-verify
- LG3 surprise: backend override (`Override` model, schemas, `routers/override.py`, `/emergency`), flow test
- Dashboard rebuilt as a command center with a per-hub override panel
- 12 deliverable documents, 3 STARRT learning goal docs, the feedback document
- All deliverables revised twice to the mayor feedback standard

**Reflection:** This is the metric I am most happy about. For three sprints in a row I logged 0/4 tasks every week, and I kept writing "this is non-negotiable" in my development plan and then not doing it. In Sprint 4 I finally did it: 13 tasks in week 14 and 20 in week 17, 33 in total. The work was always real, but for the first time it is also visible in the system the assessors look at. The catch is the shape: 13, 0, 0, 20. Weeks 15 and 16 are empty again, so the burst pattern moved from commits to tasks. But going from "always zero" to "logged and over target in two weeks" is the single biggest behaviour change of my semester.

**Development plan:** Keep logging, and spread it. Aim for a few tasks every week instead of two big weeks. The hard part (starting at all) is solved.

### Merge Requests made
**DLO target:** 2 MRs/week
**DLO result:** Week 14: 3/2. Week 15: 2/2. Week 16: 2/2. Week 17: 6/2. Total: 13 MRs.

![Sprint 4 - MRs made](screenshots/sprint4MergeRequestsMade.png)

**Reflection:** This is my most consistent MR sprint. Every single week hit or beat the target of two: 3, 2, 2, 6. Sprint 3 had more in total (15) but it was lumpy, with nine in the last week. Sprint 4 spreads thirteen MRs across all four weeks, which is exactly the "small, focused MRs" plan I set after Sprint 2 and 3. The week 17 spike (6) is the clustering deploy, the K3s work, and the round-2 documentation revision, each merged separately instead of in one batch.

**Development plan:** This habit is solid now. Carry the one-feature-one-MR rhythm into team work after the semester.

### Merge Requests reviewed
**DLO target:** 2 MRs reviewed/week
**DLO result:** Week 14: 0/2. Week 15: 0/2. Week 16: 1/2. Week 17: 0/2. Total: 1 review.

![Sprint 4 - MRs reviewed](screenshots/sprint4MergeRequestsReviewed.png)

**Reflection:** This is my weakest metric and I am not going to dress it up. One review in the whole sprint, in week 16. My Sprint 3 plan said "review at least 1 MR in the first week, do not wait." I did the opposite again. Across the semester my reviews go 0, 3, 3, 1, so Sprint 4 is actually my second worst. The reason is real but it is not an excuse: I was deep in my own backend work and the embedded tiles were not producing much for me to review near the end. Still, reviewing is how I show I carry the team, and one review does not show that.

**Development plan:** Make review a fixed habit, not an afterthought. Block 30 minutes at the start of each week to review a teammate's MR, before I open my own work.

### Documentation written
**DLO target:** 450 words/day
**DLO result:** Week 14: Tue and Thu green. Week 15: Mon and Tue green. Week 16: Thu and Sun green. Week 17: Mon, Wed, Thu, Fri green. Total: 10 green days.

![Sprint 4 - Documentation](screenshots/sprint4Documentation.png)

**Documents written in Sprint 4:**
- LG1: Analysis, Advise, Design (clustering), Design (K3s autoscaling), Realise (clustering), Realise (K3s autoscaling)
- LG2: Analysis, Advise, Design, Realise (database backup)
- LG3: Design and Realise (surprise / backend override)
- 3 STARRT learning goal documents (LG1, LG2, LG3)
- Feedback document (Mats and mister mayor Gerald Stap)
- Diagrams: failover topology, backup flow, emergency override, K3s topology, overrides ERD, failover sequence
- This Sprint 4 performance analysis

**Reflection:** My most documentation-heavy sprint by a clear margin: 10 green days, against 3 in Sprint 1, 6 in Sprint 2, and 7 in Sprint 3, and spread across all four weeks instead of bunched at the end. The deliverable docs were also written as I built, not after, which was my Sprint 3 plan. The biggest documentation work was rewriting every deliverable twice to the mayor feedback standard, so the docs improved in quality, not only in word count.

**Development plan:** Keep writing alongside the build. The thing to add next is writing the test results into the Realise on the same day I run them, instead of leaving placeholders.

### Sprint 4 overall assessment

Sprint 4 is my strongest sprint on almost every metric, and the DLO data finally backs that up instead of hiding it.

**Improved (with proof):**
- Tasks: 33 logged (13 and 20 in weeks 14 and 17) after three sprints of 0/4. The biggest behaviour change of my semester.
- MRs made: 13, and for the first time every week met or beat the target, so the work is spread, not lumped.
- Documentation: 10 green days, the most ever, across all four weeks.
- Commits: 34 total, with week 17 giving five active days in a row, my most consistent stretch.
- Hours: logged in every week for the first time, so the tracking habit is finally real.

**Still broken:**
- MRs reviewed: only 1 in the whole sprint, my second worst. Review is the habit I still have not fixed.
- Hours: most days are red, so I track but stay under the six hour target.
- Burst pattern: it did not disappear, it moved. Tasks went 13, 0, 0, 20 instead of being spread.

**What changed qualitatively:** Sprint 4 was the most technical sprint, with real clustering, K3s autoscaling, automated backups, and a backend override built and tested on the Pi. Every claim in the documents is tied to a requirement and to mayor feedback, and the deliverables were revised twice to a strict feedback standard. The one real disappointment was not a metric: I built the surprise override and tested it green, but the embedded tiles were not working on demo day, so I could not show the emergency corridor in front of the mayor. The backend was ready, the city was not.

**What I take beyond this semester:** the task and MR habits are solved and I want to keep them. The two things to carry as deliberate work are reviewing teammates' MRs early in the week, and filling fuller, greener hour days instead of red ones. The data shows I can change a habit when I decide to, because the task metric proves it.
