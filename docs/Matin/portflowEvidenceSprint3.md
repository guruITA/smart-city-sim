# Sprint 3 — Portflow Evidence

## Learning Journal Challenge

### Challenge: Building the speed camera API for Gurpreet's tile

**Learning question**
How do I design an API for someone else's sensor when I do not control the hardware or firmware?

**S — Situation**
Sprint 3. Gurpreet built a speed camera with two IR sensors 10 cm apart on an ESP32-S3. His code measures how fast a toy car passes between the sensors and calculates speed in km/h. He also has a separate ESP32-CAM that takes photos of violators. But none of this data reaches the backend yet. He prints to Serial and shows it on an OLED. I need to build the API that his ESP32 will POST to.

**T — Task**
Build a dedicated speed camera API with endpoints for storing readings, listing them, filtering violations, getting the latest reading, and computing statistics. The data model has to match exactly what Gurpreet's firmware produces: speed in km/h, direction (which sensor triggered first), a violation flag, and the speed limit at time of measurement.

**A — Action**
On April 23 I read through Gurpreet's `speed_camera_s3.ino` to understand his data. He calculates speed as `(distance / time) * 3.6` and compares against a threshold of 1.0 km/h (makes sense for miniature scale). I created a `SpeedReading` model in SQLAlchemy, wrote Pydantic schemas for request/response, and built a router with 5 endpoints. The `/stats` endpoint uses SQL aggregation (AVG, MAX, COUNT with filter) so the dashboard does not have to download all readings to show totals. I also added a 5th panel to the dashboard with orange accent color and a scrollable list of recent measurements. Violations show in red.

I committed everything in one go: `3f6b3e5 feat: implement speed camera API, database models, and comprehensive project documentation`.

**R — Result**
The speed camera API works. I tested with curl. Gurpreet only needs to add a WiFi HTTP POST in his `update()` function and the data flows into the backend. The dashboard panel shows stats (total readings, violations, average speed, max speed) and a list with violation highlighting. The `/stats` endpoint responds in under 50ms because the aggregation happens in PostgreSQL, not in Python.

**R — Reflection**
Reading Gurpreet's embedded code before writing the API was the right approach. I matched his exact field names so he does not need to transform anything. In Sprint 1 I designed the generic endpoint first and hoped tiles would fit into it. This time I went the other way: studied the hardware output, then designed the API around it. That felt more solid.

One thing I did wrong: I committed the speed camera API together with documentation updates in one big commit. It should have been two separate commits. Makes git history harder to read.

**T — Transfer**
When building an API for someone else's hardware, always read their code first. Match their data format, do not make them match yours. The dual-table pattern (generic readings for history, dedicated table for structured state) works again here, same as parking in Sprint 1 and railroad in Sprint 2. This is now a proven pattern in our project.

### Connections to Learning Outcomes

This challenge connects to multiple learning outcomes from the Backend Developer profile. The full evidence per outcome is in the Learning Group City Sim section below.

#### Design

I studied Gurpreet's firmware output before designing anything. The speed camera has structured data (speed, direction, violation flag, speed limit) that does not fit the generic `{value, unit}` pattern. I designed a dedicated `speed_readings` table, reusing the dual-table pattern from Sprint 1 parking. See Design section below for the full schema and endpoint architecture.

#### Realise

I built the speed camera router with 5 endpoints and merged it in commit `3f6b3e5`. The `/stats` endpoint uses SQL aggregation instead of pulling all rows into Python. I added a 5th dashboard panel with violation highlighting. See Realise section below for the full implementation list and commit references.

#### Advise

I recommended the speed camera as the second dedicated API because it has the most interesting data structure and tells a stronger demo story than a simple light sensor. I argued for matching Gurpreet's firmware format exactly so he does not need to change his code. See Advise section below for the full reasoning on cross-tile integration.

---

## Learning Group — City Sim

Note: This sprint had three challenges (speed camera API, Pi production deployment with port 80, and problem reframing after the 1/4 score from Mats). Above I have written out only the speed camera challenge in STARRT format because I want to get feedback on this one first before expanding the others. The Learning Group sections below cover all Sprint 3 work, not just speed camera, so the sprint evidence is complete. The two missing challenges (port 80 deployment and problem reframing) will be added after feedback on the speed camera challenge.

### Analysis — Cross-tile integration scalability and refined research questions

This builds on the 6W problem analysis from Sprint 1 (see portflowEvidenceSprint1.md). The core problem remains: parking search traffic in Amsterdam wastes fuel and produces CO2. Drivers need real-time occupancy data.

**Refined research questions after Mats feedback (May 6)**

Mats pointed out that my Sprint 1 analysis addresses the technical "how" but not the human "why does it matter to residents". In Sprint 3 I refined the questions:

- Original deelvraag 1: "How much urban traffic consists of parking search?" → Still answered by Shoup (2017): up to 30% in busy areas.
- New question: "Who benefits from real-time parking data, and how would they access it?" → Residents who experience noise/pollution benefit indirectly. Drivers benefit directly. A dashboard is one access method but a phone notification would be more practical. For this PoC, the dashboard proves the concept.
- New question: "Does adding more sensor types (speed camera, traffic light) to the same backend strengthen the case for a city-wide data platform?" → Yes. The speed camera shares infrastructure and proves the backend is not a single-purpose parking tool.

**Deelvragen addressed this sprint**

- Deelvraag 3 (Sprint 1): "How should data be collected, stored, and served?" → Extended with the speed camera API. The answer is now: shared backend with tile-specific endpoints when data has structure (speed, direction, violation flag), generic endpoint when data is a simple value.
- Deelvraag 4 (Sprint 1): "What infrastructure for reliable detection?" → Answered by the Pi deployment. Raspberry Pi on HvA network, Docker with restart policy, port 80, SSH for remote management.

**Cross-tile scalability analysis**

The backend grew from 2 tables (Sprint 1) to 4 (Sprint 2) to 5 (Sprint 3). Each new dedicated table was added when a tile's data did not fit the generic `{tile, sensor_type, value, unit}` pattern. The speed camera needs `speed_kmh`, `direction`, `is_violation`, `speed_limit_kmh`. That does not map cleanly to a single float value.

The router-based architecture scales well. Adding speed camera required 3 touchpoints: model in `models.py`, router file `speed_camera.py`, registration in `main.py`. No existing code was touched. The dashboard added a 5th panel without layout issues (2x3 grid now).

3 out of 5 tiles have dedicated endpoints (parking, railroad, speed camera). 2 still use the generic endpoint (streetlight, traffic light).

**Stakeholder mapping**

- Drivers: primary users of parking data. Benefit: save time, reduce frustration.
- Residents: indirect beneficiaries. Benefit: less noise and pollution from circling cars.
- Municipality: saves on traffic management. Could use data for parking policy.
- Team members: use the shared backend for their tiles.
- Assessors (Mats, Gerald): need to see the city problem connection in my evidence.

I did not interview any of these stakeholders. The driver/resident perspective comes from Shoup (2017) and Amsterdam municipality publications. For Sprint 4 I plan to do at least one informal interview with a student who drives to HvA about their parking experience.

**Limitations**

- No primary research conducted. All problem data from secondary sources.
- Proof of concept uses 3 miniature parking spots. Scaling conclusions are speculative.
- No user testing of the dashboard with actual drivers.
- Stakeholder needs are assumed, not validated through interviews.

---

### Advise — Cross-tile data sharing, speed camera justification, and expert feedback workflow

**Why a shared backend instead of per-tile backends**

I thought about giving each team member their own API. Simpler for me: I build parking, Thijmen builds railroad, done. But then there is no integration story. The whole point of a smart city is that data from different sensors lives in one place. A parking sensor alone is useful. A parking sensor combined with traffic light data and speed measurements gives a richer picture of how the city moves. I advised the team to keep using one shared backend with separate routers per tile.

**Speed camera as second use case**

I chose to build the speed camera API (not streetlight or traffic light) because it has the most interesting data structure. A speed reading has direction, violation status, and a speed limit that can change. This makes for a better demo than a simple "light level is 342 lux" reading. It also tells a story: the smart city detects both where you can park AND whether you are driving safely. Two problems, one platform.

From a societal perspective: speed enforcement in residential areas addresses the same residents who suffer from search traffic. Cars driving too fast in neighborhoods are a safety problem. The speed camera and parking sensor both serve the goal of making neighborhoods safer and quieter.

**Port 80 choice**

I recommended port 80 because ESP32's HTTPClient library defaults to port 80 for HTTP. If the backend runs on port 8000, every team member has to remember to add `:8000` to their URLs. Gurpreet's code had `http://145.92.8.137:8000/api/v1/readings` hardcoded. With port 80 it becomes `http://145.92.8.137/api/v1/readings`. One less thing to get wrong.

**Expert feedback workflow (after Mats + Gerald conversation May 6)**

Mats and Gerald both said I need visible feedback loops. My plan for the remaining weeks:
- Ask Thijmen for code review on speed camera endpoints (he knows the router pattern)
- Ask Gerald to validate problem statement framing
- Ask Mats for 1-on-1 Progress Review preparation
- Record all feedback as Portflow comments (not just verbal conversations that disappear)

I have not been doing this. In Sprint 1 and 2 I got verbal feedback from Raymond and Thijmen but never recorded it in Portflow. That makes it invisible to assessors. From now on: if it is not in Portflow, it did not happen.

---

### Design — Speed camera API and production network architecture

**Speed camera data model**

I designed a dedicated `speed_readings` table:

| Column | Type | Why |
|--------|------|-----|
| id | Integer PK | Auto-increment |
| speed_kmh | Float | Measured speed from IR sensor time-of-flight |
| direction | String(10) | "1->2" or "2->1" — which sensor triggered first |
| is_violation | Boolean | True if speed > limit at time of measurement |
| speed_limit_kmh | Float | Limit stored with the reading so it survives config changes |
| created_at | DateTime | Server timestamp |

I thought about reusing the generic `sensor_readings` table but the speed camera has 4 meaningful fields, not just one value. Forcing it into `{value: 2.4, unit: "km/h"}` would lose the direction and violation data. Same reasoning as Thijmen's railroad crossing in Sprint 2.

The violation flag is computed on the ESP32, not in the backend. Gurpreet compares speed against his threshold in firmware. The backend just stores the result. This keeps the backend stateless — it does not need to know what the speed limit is.

**API endpoints (5 total)**

1. `POST /api/v1/speedcamera/` — Store a reading. Body: `{speed_kmh, direction, is_violation, speed_limit_kmh}`
2. `GET /api/v1/speedcamera/` — Recent readings with optional `?limit=` parameter
3. `GET /api/v1/speedcamera/violations` — Only readings where `is_violation = true`
4. `GET /api/v1/speedcamera/latest` — Most recent single reading
5. `GET /api/v1/speedcamera/stats` — Aggregates: total, violations, avg speed, max speed, violation rate %

The `/stats` endpoint uses SQL `func.count`, `func.avg`, `func.max` with a filter. This means the database does the math, not Python. For 1000 readings, this is the difference between transferring 1000 rows to Python vs getting back 5 numbers.

**Production network design**

```
[ESP32 tiles] --WiFi--> [HvA network] --port 80--> [Raspberry Pi]
                                                      |
                                                Docker Compose:
                                                  - api (port 8000 internal)
                                                  - db (PostgreSQL, port 5432 internal)
                                                      |
                                                  host maps 80 -> api:8000
```

The Pi has a static IP (145.92.8.137) on the HvA network. Docker containers restart with `restart: always`. SSH is available for remote deployment. The `.env` file stores database credentials and is not in git.

**Dashboard design for 5th panel**

The speed camera panel uses orange accent (#f97316) to differentiate from Gurpreet's streetlight panel (amber). It shows:
- Stats row: total readings, violations, avg speed, max speed
- Scrollable list of recent measurements
- Violation rows highlighted in red
- Auto-refresh every 2 seconds (same as other panels)

---

### Realise — Speed camera API, port 80 deployment, and problem statement

**Speed camera API (commit 3f6b3e5, April 23)**

I built and merged:
- `backend/app/models.py` — added `SpeedReading` model with 5 columns
- `backend/app/schemas.py` — added `SpeedReadingCreate`, `SpeedReadingResponse`, `SpeedCameraStatsResponse`
- `backend/app/routers/speed_camera.py` — 5 endpoints, SQL aggregation in stats
- `backend/app/main.py` — registered speed camera router at `/api/v1/speedcamera`
- `backend/app/static/index.html` — added 5th dashboard panel with stats and violation list

Total: the backend now serves 5 router groups with 20+ REST endpoints.

**Port 80 fix (commit 42d99bd, May 4)**

- `backend/docker-compose.yml` — changed `"8000:8000"` to `"80:8000"`
- `backend/deploy.sh` — updated health check URL and success message to port 80
- `backend/app/main.py` — moved dashboard from `/dashboard` to `/`

Pi is live at `http://145.92.8.137/`. Verified working after deploy via SSH.

**Problem statement (commit dbb1bc8, May 4)**

- `docs/Matin/problemStatement.md` — parking search traffic in Amsterdam, Shoup (2017) reference, connection to all 4 learning outcomes, scope limitations

**Portflow restructuring (commits faf732d, 483abbb, f8fc796, May 5)**

- `docs/Matin/learningJournalSprint0.md` — Sprint 0 STARRT reflection
- `docs/Matin/learningJournal/personalLearningGoalsAllSprints.md` — SMART goals all sprints
- `docs/Matin/learningJournal/performanceAnalysisAllSprints.md` — DLO data analysis with screenshots
- `docs/Matin/portflowMasterSheet.md` — 12-item reference for Portflow browser editing

**What is running on the Pi right now:**
- 5 tables: `sensor_readings`, `parking_spots`, `train`, `barrier`, `speed_readings`
- 5 router groups: readings, parking, railroad_crossing_train, railroad_crossing_barrier, speed_camera
- Dashboard on root with 5 tile panels
- Docker containers with auto-restart
- Port 80, accessible from any device on HvA WiFi
