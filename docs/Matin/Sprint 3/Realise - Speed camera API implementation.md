# Realise — Speed camera API implementation

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | May 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group — HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document records what I built for the speed camera API and how I tested it. Everything was built on April 23, committed as `3f6b3e5`.

### Main question

What was built, and does it work?

### Sub-questions

1. What files were created or changed?
2. How was it tested?

---

## Chapter 1 — Implementation

### Context

On April 23, I read Gurpreet's firmware and built the full speed camera backend in one session.

### What was built

**backend/app/models.py** — Added `SpeedReading` model with 5 columns (speed_kmh, direction, is_violation, speed_limit_kmh, created_at).

**backend/app/schemas.py** — Added 3 Pydantic schemas: `SpeedReadingCreate` (request body), `SpeedReadingResponse` (full response), `SpeedCameraStatsResponse` (aggregated stats).

**backend/app/routers/speed_camera.py** — New router with 5 endpoints: POST /, GET /, GET /violations, GET /latest, GET /stats.

**backend/app/main.py** — Registered the router at `/api/v1/speedcamera`.

**backend/app/static/index.html** — Added 5th dashboard panel with orange accent, stats row, scrollable list, violation highlighting.

### Sub-conclusion

5 files changed. 1 new router file. The backend now serves 5 router groups with 20+ endpoints.

---

## Chapter 2 — Testing

### Method

I tested with curl and the Swagger UI at `/docs`.

### Commands

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

All endpoints responded correctly. The dashboard showed the new panel with live data after refresh.

### What I should have done differently

I committed the API code together with documentation in one commit (`3f6b3e5`). Should have been separate commits.

### Sub-conclusion

API works. Gurpreet only needs to add one `http.POST()` call in his firmware to connect.

---

## Conclusion

The speed camera API is fully implemented and tested. Commit `3f6b3e5`. The backend serves 5 tiles on the dashboard. Gurpreet can connect by adding one HTTP POST call.

---

## References

- Gurpreet Singh. (2026). `speed_camera_s3.ino`. City Sim Learning Group repository.
