# Realise — Speed camera API implementation and production deployment

## Introduction

This is the realise deliverable for Sprint 3 Learning Goal 1. It documents what I built, when I built it, and where the code lives. Sprint 3 had three major deliveries: the speed camera API, the port 80 production deployment, and the problem statement with Portflow restructuring.

## Speed camera API (commit 3f6b3e5, April 23)

On April 23 I read Gurpreet's `speed_camera_s3.ino` and built the full speed camera backend in one session.

### Files created or changed

**backend/app/models.py** — Added `SpeedReading` model:

```python
class SpeedReading(Base):
    __tablename__ = "speed_readings"

    id = Column(Integer, primary_key=True, index=True)
    speed_kmh = Column(Float, nullable=False)
    direction = Column(String(10), nullable=False, default="1->2")
    is_violation = Column(Boolean, default=False)
    speed_limit_kmh = Column(Float, default=1.0)
    created_at = Column(DateTime(timezone=True), server_default=func.now())
```

**backend/app/schemas.py** — Added 3 Pydantic schemas:

- `SpeedReadingCreate` — request body with `speed_kmh`, `direction`, `is_violation`, `speed_limit_kmh`
- `SpeedReadingResponse` — response with all fields plus `id` and `created_at`
- `SpeedCameraStatsResponse` — aggregated stats: `total_readings`, `total_violations`, `average_speed_kmh`, `max_speed_kmh`, `violation_rate_percent`

**backend/app/routers/speed_camera.py** — New router with 5 endpoints:

- `POST /` — Store a reading. Uses `db.add()` + `db.commit()`.
- `GET /` — Recent readings with `?limit=50`. Uses `order_by(desc(created_at)).limit()`.
- `GET /violations` — Filtered to `is_violation == True`.
- `GET /latest` — Single most recent reading using `.first()`.
- `GET /stats` — SQL aggregation:

```python
total = db.query(func.count(SpeedReading.id)).scalar()
violations = db.query(func.count(SpeedReading.id)).filter(
    SpeedReading.is_violation == True
).scalar()
avg_speed = db.query(func.avg(SpeedReading.speed_kmh)).scalar()
max_speed = db.query(func.max(SpeedReading.speed_kmh)).scalar()
```

**backend/app/main.py** — Added router registration:

```python
from routers import speed_camera
app.include_router(speed_camera.router, prefix="/api/v1/speedcamera", tags=["Speed Camera"])
```

**backend/app/static/index.html** — Added 5th dashboard panel with:

- Stats row showing total, violations, avg speed, max speed
- Scrollable list of recent readings
- Violation highlighting in red
- Orange accent color (#f97316)
- Independent 2-second refresh cycle

### Testing

I tested with curl:

```bash
# Store a reading
curl -X POST http://localhost:8000/api/v1/speedcamera/ \
  -H "Content-Type: application/json" \
  -d '{"speed_kmh": 2.4, "direction": "1->2", "is_violation": true, "speed_limit_kmh": 1.0}'

# Get stats
curl http://localhost:8000/api/v1/speedcamera/stats

# Get violations only
curl http://localhost:8000/api/v1/speedcamera/violations
```

All endpoints responded correctly. The Swagger docs at `/docs` show the speed camera endpoints with "Try it out" buttons.

### What I should have done differently

I committed the speed camera API together with documentation updates in one commit (`3f6b3e5`). That should have been separate commits: one for the API code, one for the docs. It makes the git history harder to review. I noticed this too late.

## Port 80 fix (commit 42d99bd, May 4)

This was carry-over from Sprint 2. The actual change took about 30 minutes. The delay was 6 weeks of procrastination.

### Files changed

**backend/docker-compose.yml** — Changed port mapping:

```yaml
# Before
ports:
  - "8000:8000"

# After
ports:
  - "80:8000"
```

The container still runs uvicorn on port 8000 internally. The host maps port 80 to it.

**backend/deploy.sh** — Updated health check and success message:

- Health check URL changed from `http://localhost:8000/health` to `http://localhost/health`
- Success message prints `http://<ip>/` instead of `http://<ip>:8000/dashboard`

**backend/app/main.py** — Moved dashboard from `/dashboard` to `/`:

The dashboard is now served at the root URL. Opening `http://145.92.8.137/` shows the City Sim Dashboard immediately.

### Deployment

Deployed on the Pi via SSH on May 4. Docker pulled the new images, rebuilt, and started on port 80. Verified by opening the dashboard in a browser from a different machine on the HvA network.

## Problem statement (commit dbb1bc8, May 4)

**docs/Matin/problemStatement.md** — New file. Frames the parking system inside the Amsterdam parking search traffic problem:

- 30% of inner-city traffic is drivers circling for parking (Shoup, 2017)
- Amsterdam targets emission-free traffic by 2030
- PoC scope: 3 miniature parking spots, not citywide deployment
- Connection to all 4 learning outcomes

This was the most important file of Sprint 3 from the assessor perspective. Without this file, Mats would have no reason to change the 1/4 score.

## Portflow restructuring (commits faf732d, 483abbb, f8fc796, May 5)

**docs/Matin/learningJournalSprint0.md** — Sprint 0 STARRT reflection about technology selection.

**docs/Matin/learningJournal/personalLearningGoalsAllSprints.md** — SMART learning goals for all 3 sprints. Honest about Sprint 2 failures.

**docs/Matin/learningJournal/performanceAnalysisAllSprints.md** — Performance metrics per DLO criteria with real DLO data and screenshots.

**docs/Matin/portflowMasterSheet.md** — Single copy-paste reference document for all 12 Portflow evidence items with commit-hash permalinks.

## What is running on the Pi right now

As of May 6, 2026:

| Component | Status |
|-----------|--------|
| PostgreSQL 16 | Running in Docker, port 5432 internal |
| FastAPI application | Running in Docker, port 8000 internal |
| Host port mapping | 80 -> 8000 |
| Dashboard | Served at root URL `/` |
| Auto-restart | `restart: always` policy |
| SSH access | Configured for remote management |

**Database tables (5):**

- `sensor_readings` — generic history for all tiles
- `parking_spots` — real-time parking state
- `train` — train detection with sensor timestamps
- `barrier` — barrier control with input mode
- `speed_readings` — speed camera measurements

**Router groups (5):**

- `/api/v1/readings` — generic sensor endpoints
- `/api/v1/parking` — parking-specific endpoints
- `/api/v1/railroad/train` — train detection endpoints
- `/api/v1/railroad/barrier` — barrier control endpoints
- `/api/v1/speedcamera` — speed camera endpoints

**Dashboard panels (5):**

- Parking (green/red occupancy)
- Streetlight (amber light level)
- Traffic Light (state display)
- Railroad Crossing (train approach animation)
- Speed Camera (orange, stats + violation list)

**Total endpoints: 20+**

All accessible at `http://145.92.8.137/`. API docs at `http://145.92.8.137/docs`.
