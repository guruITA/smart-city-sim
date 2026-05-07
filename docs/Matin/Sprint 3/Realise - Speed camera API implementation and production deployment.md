# Realise — Speed camera API implementation and production deployment

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
2. [Chapter 1 — Speed camera API implementation](#chapter-1--speed-camera-api-implementation)
3. [Chapter 2 — Port 80 production deployment](#chapter-2--port-80-production-deployment)
4. [Chapter 3 — Problem statement and Portflow restructuring](#chapter-3--problem-statement-and-portflow-restructuring)
5. [Conclusion](#conclusion)
6. [Recommendation](#recommendation)
7. [References](#references)
8. [Appendix](#appendix)

---

## Introduction

This document is the realise deliverable for Sprint 3, Learning Goal 1. It records what was built, when it was built, and which commits contain the work. Sprint 3 had three deliveries: the speed camera API, the port 80 production deployment, and the problem statement with Portflow restructuring.

### Target audience

This document is written for assessors who need to verify what was delivered and where the code lives, and for team members who need to understand the current state of the production system.

### Main question

What was built and deployed in Sprint 3, and what is the current state of the production system?

### Sub-questions

1. How was the speed camera API implemented, and what files were created or changed?
2. How was the port 80 production deployment completed?
3. What documentation and evidence was created to address the Portflow gap?

---

## Chapter 1 — Speed camera API implementation

### Context

On April 23, I read through Gurpreet's `speed_camera_s3.ino` and built the full speed camera backend in one session. Commit: `3f6b3e5`.

### Method

I studied the firmware output, created the SQLAlchemy model, wrote Pydantic schemas, built a FastAPI router with 5 endpoints, and added a dashboard panel. I tested with curl and the Swagger UI.

### What was built

**backend/app/models.py** — Added `SpeedReading` model with 5 columns (speed_kmh, direction, is_violation, speed_limit_kmh, created_at).

**backend/app/schemas.py** — Added 3 Pydantic schemas:

- `SpeedReadingCreate` — request body for POST
- `SpeedReadingResponse` — response with all fields plus id and created_at
- `SpeedCameraStatsResponse` — aggregated stats

**backend/app/routers/speed_camera.py** — New router with 5 endpoints:

- `POST /` — store a reading
- `GET /` — recent readings with `?limit=50`
- `GET /violations` — filtered to violations only
- `GET /latest` — single most recent reading
- `GET /stats` — SQL aggregation (count, avg, max, violation rate)

**backend/app/main.py** — Registered the speed camera router at `/api/v1/speedcamera`.

**backend/app/static/index.html** — Added 5th dashboard panel with orange accent, stats row, scrollable list, and violation highlighting.

### Testing

```bash
curl -X POST http://localhost:8000/api/v1/speedcamera/ \
  -H "Content-Type: application/json" \
  -d '{"speed_kmh": 2.4, "direction": "1->2", "is_violation": true, "speed_limit_kmh": 1.0}'

curl http://localhost:8000/api/v1/speedcamera/stats
curl http://localhost:8000/api/v1/speedcamera/violations
```

All endpoints responded correctly. Swagger docs show the endpoints with "Try it out" buttons.

### What I should have done differently

I committed the API code together with documentation updates in one commit (`3f6b3e5`). That should have been two separate commits. It makes git history harder to review.

### Sub-conclusion

The speed camera API is fully implemented and tested. The backend now serves 5 router groups with 20+ endpoints. Gurpreet only needs to add one HTTP POST call in his firmware to connect.

---

## Chapter 2 — Port 80 production deployment

### Context

This was carry-over from Sprint 2. My Sprint 2 learning goal 3 was "expose on port 80". I scored it as "not achieved". The actual change took about 30 minutes on May 4. The delay was 6 weeks. Commit: `42d99bd`.

### Method

I changed the Docker port mapping, updated the deploy script, moved the dashboard to the root URL, and deployed on the Pi via SSH.

### What was changed

**backend/docker-compose.yml** — Port mapping from `"8000:8000"` to `"80:8000"`. The container still runs on 8000 internally.

**backend/deploy.sh** — Health check URL changed to port 80. Success message updated.

**backend/app/main.py** — Dashboard moved from `/dashboard` to `/`. Opening `http://145.92.8.137/` shows the City Sim Dashboard immediately.

### Deployment verification

Deployed via SSH on May 4. Opened `http://145.92.8.137/` from a different machine on HvA network. Dashboard loaded correctly. API docs available at `/docs`. Health check at `/health` returns 200.

### Sub-conclusion

The Pi is live at port 80. Docker containers auto-restart on reboot. SSH works for remote management. This closes the Sprint 2 carry-over. The change itself was trivial. The delay was a prioritization failure, not a technical one.

---

## Chapter 3 — Problem statement and Portflow restructuring

### Context

After Mats' 1/4 score in Sprint 2, I needed to create a problem statement and restructure all Portflow evidence. This is catch-up work that should have existed from Sprint 1.

### Method

I researched parking search traffic statistics, wrote a problem statement, then rewrote all 12 Portflow evidence items with problem-centered descriptions. I also created learning journal documents.

### What was created

**docs/Matin/problemStatement.md** (commit `dbb1bc8`, May 4)

Frames the parking system in the Amsterdam parking search traffic problem. Key claim: up to 30% of inner-city traffic in busy areas is drivers circling for parking (Shoup, 2017). Amsterdam targets emission-free traffic by 2030. Scope limited to PoC with 3 miniature spots.

**docs/Matin/learningJournalSprint0.md** (commit `faf732d`, May 5)

Sprint 0 STARRT reflection about technology selection (FastAPI over Spring Boot).

**docs/Matin/learningJournal/personalLearningGoalsAllSprints.md** (commit `483abbb`, May 5)

SMART learning goals for all 3 sprints. Honest about Sprint 2 failures: "I did not write these learning goals at the start of the sprint."

**docs/Matin/learningJournal/performanceAnalysisAllSprints.md** (commit `483abbb`, May 5)

Performance metrics per DLO criteria with real data and screenshots. Includes correction: Sprint 2 MR reviews were 3, not 0.

**docs/Matin/portflowMasterSheet.md** (commit `f8fc796`, May 5)

Single copy-paste reference for all 12 Portflow evidence items with commit-hash permalinks.

### Sub-conclusion

All required documentation is committed and merged to main. The problem statement connects the technical work to a real city problem. The learning journal documents are honest about failures. This closes the evidence gap that caused the 1/4 score.

---

## Conclusion

### Sub-conclusion 1 — Speed camera API

Fully implemented with 5 endpoints, dedicated table, and dashboard panel. Commit `3f6b3e5`. Ready for Gurpreet to connect.

### Sub-conclusion 2 — Port 80 deployment

Pi live at `http://145.92.8.137/`. Docker auto-restart, SSH access, dashboard on root. Commit `42d99bd`. Sprint 2 carry-over closed.

### Sub-conclusion 3 — Documentation

Problem statement, learning journal, and Portflow restructuring all committed and merged. Commits `dbb1bc8`, `faf732d`, `483abbb`, `f8fc796`.

### Answer to the main question

Sprint 3 delivered: a speed camera API with 5 endpoints and a dashboard panel, a production deployment on port 80, and a complete documentation restructuring. The Pi now serves 5 router groups with 20+ endpoints. The system state:

| Component | Status |
|-----------|--------|
| Database tables | 5 (sensor_readings, parking_spots, train, barrier, speed_readings) |
| Router groups | 5 (readings, parking, railroad train, railroad barrier, speed camera) |
| Dashboard panels | 5 |
| Host port | 80 |
| Auto-restart | Yes |
| SSH access | Yes |

---

## Recommendation

1. Use smaller commits in Sprint 4. One commit per feature, one per documentation update.
2. Do not carry over small tasks. If something takes less than an hour and you are already touching the same files, just do it.
3. Start time tracking in DLO. Three sprints without formal time logging is a pattern that needs to break.
4. Deploy updates more frequently instead of batching everything at the end of the sprint.

---

## References

- Shoup, D. (2017). *The High Cost of Free Parking*. Routledge.
- FastAPI documentation. https://fastapi.tiangolo.com/
- Docker Compose documentation. https://docs.docker.com/compose/
- Gurpreet Singh. (2026). `speed_camera_s3.ino`. City Sim Learning Group repository.

---

## Appendix

### A. Commit log for Sprint 3

| Commit | Date | Description |
|--------|------|-------------|
| `3f6b3e5` | Apr 23 | Speed camera API, models, schemas, dashboard panel |
| `dbb1bc8` | May 4 | Problem statement |
| `42d99bd` | May 4 | Port 80 fix, dashboard on root |
| `faf732d` | May 5 | Sprint 0 STARRT reflection |
| `483abbb` | May 5 | Learning goals + performance analysis |
| `f8fc796` | May 5 | portflowMasterSheet.md |

### B. Current API endpoint overview

| Method | Path | Description |
|--------|------|-------------|
| POST | /api/v1/readings | Store generic sensor reading |
| GET | /api/v1/readings | Get recent readings |
| GET | /api/v1/readings/latest | Get latest reading |
| POST | /api/v1/parking/update/{spot} | Update parking spot |
| GET | /api/v1/parking/status | Get parking status |
| GET | /api/v1/parking/spots | Get all spots |
| GET | /api/v1/parking/spots/{n} | Get single spot |
| POST | /api/v1/railroad/train | Register train approach |
| PATCH | /api/v1/railroad/train/{id} | Update train with second sensor |
| GET | /api/v1/railroad/train/latest | Get latest train |
| POST | /api/v1/railroad/barrier | Set barrier state |
| GET | /api/v1/railroad/barrier/latest | Get barrier state |
| POST | /api/v1/speedcamera/ | Store speed reading |
| GET | /api/v1/speedcamera/ | Get recent readings |
| GET | /api/v1/speedcamera/violations | Get violations |
| GET | /api/v1/speedcamera/latest | Get latest reading |
| GET | /api/v1/speedcamera/stats | Get aggregated stats |
