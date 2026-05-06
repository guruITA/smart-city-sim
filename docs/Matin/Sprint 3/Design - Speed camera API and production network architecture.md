# Design — Speed camera API and production network architecture

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | May 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group — HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Table of Contents

1. [Introduction](#introduction)
2. [Chapter 1 — Speed camera data model](#chapter-1--speed-camera-data-model)
3. [Chapter 2 — API endpoint design](#chapter-2--api-endpoint-design)
4. [Chapter 3 — Production network and dashboard](#chapter-3--production-network-and-dashboard)
5. [Conclusion](#conclusion)
6. [Recommendation](#recommendation)
7. [References](#references)
8. [Appendix](#appendix)

---

## Introduction

This document is the design deliverable for Sprint 3, Learning Goal 1. It documents the speed camera data model, API endpoints, production network setup, and dashboard panel design.

The design builds on two earlier patterns. In Sprint 1, I designed the generic `sensor_readings` table and the dedicated `parking_spots` table. In Sprint 2, Thijmen added dedicated `train` and `barrier` tables. The speed camera follows the same pattern: when a tile's data has more than one meaningful field, it gets a dedicated table.

### Target audience

This document is written for team members who need to understand the API design (especially Gurpreet, who will integrate his ESP32), and for assessors evaluating the design decisions.

### Main question

How should the speed camera API be designed to match Gurpreet's firmware output and integrate into the existing backend without changing existing code?

### Sub-questions

1. What data model fits the speed camera's structured output?
2. What endpoints does the speed camera need, and how should statistics be computed?
3. How should the production network and dashboard be configured for the expanded system?

---

## Chapter 1 — Speed camera data model

### Context

Before designing anything, I read Gurpreet's `speed_camera_s3.ino`. His firmware produces 4 fields per measurement:

- `speed_kmh` — calculated as `(distance / time) * 3.6`, distance is the 10 cm gap between two IR sensors
- `direction` — `"1->2"` or `"2->1"` depending on which sensor triggers first
- `is_violation` — `true` if speed exceeds the limit
- `speed_limit_kmh` — currently 1.0 km/h for miniature scale

He prints this to Serial and shows it on an OLED. He does not POST to the backend yet.

### Method

I compared two approaches: reusing the generic `sensor_readings` table vs creating a dedicated `speed_readings` table. I evaluated which approach preserves all the data and keeps queries simple.

### Findings

**Option 1: Generic table.** Store as `{tile: "speedcamera", sensor_type: "speed", value: 2.4, unit: "km/h"}`. This loses the direction, violation flag, and speed limit. I would need 4 separate rows per measurement. Querying "all violations" means joining rows by timestamp. This is ugly and slow.

**Option 2: Dedicated table.** One row per measurement with all 4 fields plus a server timestamp. Clean, queryable, matches the firmware output directly.

I chose option 2. The dedicated table:

| Column | Type | Why |
|--------|------|-----|
| id | Integer PK | Auto-increment |
| speed_kmh | Float | Measured speed |
| direction | String(10) | Which sensor triggered first |
| is_violation | Boolean | Exceeded limit at time of measurement |
| speed_limit_kmh | Float | Stored with reading so it survives config changes |
| created_at | DateTime | Server timestamp via `func.now()` |

**Why the violation flag is computed on the ESP32:** I made the backend stateless. Gurpreet compares speed against his threshold in firmware. If I compute violations server-side, the backend needs to know the speed limit. If Gurpreet changes his threshold, the backend needs updating too. Letting the ESP32 decide keeps the firmware as the source of truth.

**Why the speed limit is stored per reading:** If Gurpreet changes the limit mid-experiment from 1.0 to 2.0 km/h, old readings should still show whether they were violations at the original limit. Each row tells the full story without external context.

### Sub-conclusion

A dedicated `speed_readings` table is the right choice. It preserves all 4 fields, matches the firmware output, and keeps the backend stateless. The violation flag stays on the ESP32 side.

---

## Chapter 2 — API endpoint design

### Context

The speed camera needs endpoints for storing data (POST from ESP32), retrieving data (GET for dashboard), and computing statistics (GET for stats panel). I followed the same router pattern used for parking and railroad crossing.

### Method

I listed what the ESP32 needs to send, what the dashboard needs to display, and what aggregated views would be useful. Then I designed the minimal set of endpoints that covers all three.

### Findings

5 endpoints under `/api/v1/speedcamera`:

**1. POST /** — Store a new speed measurement.

Request body: `{speed_kmh, direction, is_violation, speed_limit_kmh}`. Returns the stored reading with `id` and `created_at`. This is what Gurpreet's ESP32 calls.

**2. GET /** — Get recent readings.

Optional `?limit=50` parameter. Returns most recent readings ordered by `created_at` descending. The dashboard uses this for the scrollable list.

**3. GET /violations** — Get only violations.

Same as GET / but filtered to `is_violation = true`. Useful for a "violations only" view.

**4. GET /latest** — Get the single most recent reading.

Returns one reading. The dashboard uses this for the header display.

**5. GET /stats** — Get aggregated statistics.

Returns `{total_readings, total_violations, average_speed_kmh, max_speed_kmh, violation_rate_percent}`. Uses SQL aggregation: `func.count`, `func.avg`, `func.max`, and a filtered count for violations. The database computes all 5 numbers in one query. For 1000 readings, this transfers 5 numbers instead of 1000 rows.

### Sub-conclusion

5 endpoints cover all use cases: ESP32 storage, dashboard display, violation filtering, and aggregated statistics. The `/stats` endpoint uses SQL aggregation to keep the response fast regardless of data volume.

---

## Chapter 3 — Production network and dashboard

### Context

The Pi deployment was carry-over from Sprint 2. The actual change (port 80) was small, but it affects how all ESP32 devices and browsers connect to the backend. The dashboard also needed a 5th panel for the speed camera.

### Method

I mapped the network path from ESP32 to backend and identified where port 80 simplifies the connection. For the dashboard, I designed the panel to be visually distinct while following the same pattern as existing panels.

### Findings

**Network architecture:**

```
[ESP32 tiles] --WiFi--> [HvA network] --port 80--> [Raspberry Pi 145.92.8.137]
                                                      |
                                                Docker Compose:
                                                  - api (port 8000 internal)
                                                  - db (PostgreSQL 16, port 5432 internal)
                                                      |
                                                  host maps 80 -> api:8000
```

The Dockerfile stays on port 8000 (container internal). `docker-compose.yml` maps `"80:8000"`. ESP32 devices POST to `http://145.92.8.137/api/v1/speedcamera/` without specifying a port. HTTP defaults to 80.

Docker containers restart with `restart: always`. SSH is configured for remote deployment. The `.env` file stores database credentials and is in `.gitignore`.

**Dashboard 5th panel:**

- Orange accent (`#f97316`) to differentiate from other panels
- Stats row: total readings, violations, avg speed, max speed
- Scrollable list of recent measurements
- Violation rows highlighted in red
- Auto-refresh every 2 seconds, independent of other panels

### Sub-conclusion

Port 80 removes one source of confusion for all team members. The dashboard scales from 4 to 5 panels without layout issues. Each panel fetches data independently so a slow tile does not block others.

---

## Conclusion

### Sub-conclusion 1 — Data model

A dedicated `speed_readings` table with 5 columns matches Gurpreet's firmware output. The backend stays stateless: violations are computed on the ESP32, the backend stores the result.

### Sub-conclusion 2 — Endpoints

5 endpoints cover storage, retrieval, filtering, and aggregation. The `/stats` endpoint uses SQL aggregation for performance.

### Sub-conclusion 3 — Network and dashboard

Port 80 simplifies ESP32 connections. The dashboard grows from 4 to 5 panels without structural changes.

### Answer to the main question

The speed camera API integrates into the existing backend by following the same patterns: dedicated table for structured data, separate router file, registration in main.py. No existing code changes. The design matches Gurpreet's firmware exactly so he only needs to add one HTTP POST call. The production network on port 80 and the 5-panel dashboard complete the system.

---

## Recommendation

1. Add input validation: check that `speed_kmh > 0` and `direction` is one of `"1->2"` or `"2->1"`. Use a Pydantic validator or a database check constraint.
2. Consider adding a time-series chart to the dashboard panel. A list shows individual readings but a chart would show trends over time.
3. If the ESP32 sends readings very frequently, add rate limiting or batch insertion to prevent database fill-up.
4. The photo system (ESP32-CAM) is not integrated. If photo evidence is needed in the backend, design a file upload endpoint in Sprint 4.

---

## References

- FastAPI documentation. https://fastapi.tiangolo.com/
- SQLAlchemy 2.0 documentation. https://docs.sqlalchemy.org/
- Pydantic V2 documentation. https://docs.pydantic.dev/
- Gurpreet Singh. (2026). `speed_camera_s3.ino`. City Sim Learning Group repository.

---

## Appendix

### A. Speed camera data model (SQLAlchemy)

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

### B. Stats endpoint SQL aggregation

```python
total = db.query(func.count(SpeedReading.id)).scalar()
violations = db.query(func.count(SpeedReading.id)).filter(
    SpeedReading.is_violation == True
).scalar()
avg_speed = db.query(func.avg(SpeedReading.speed_kmh)).scalar()
max_speed = db.query(func.max(SpeedReading.speed_kmh)).scalar()
```

### C. Pydantic schemas

```python
class SpeedReadingCreate(BaseModel):
    speed_kmh: float = Field(..., example=2.4)
    direction: str = Field(default="1->2", example="1->2")
    is_violation: bool = Field(default=False, example=True)
    speed_limit_kmh: float = Field(default=1.0, example=1.0)

class SpeedCameraStatsResponse(BaseModel):
    total_readings: int
    total_violations: int
    average_speed_kmh: float
    max_speed_kmh: float
    violation_rate_percent: float
```
