# Realise - FastAPI backend implementation

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | March 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group - HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document records what I built for the City Sim backend in Sprint 1. A FastAPI application with 7 endpoints, 2 routers, PostgreSQL storage, and a dashboard.

### Main question

What was built and how does it work?

### Sub-questions

1. What endpoints were implemented?
2. How was it tested?

---

## Chapter 1 - Implementation

### Context

I needed a backend that accepts sensor data from ESP32 devices, stores it in PostgreSQL, and displays it on a dashboard. I had never used FastAPI before this sprint.

### What was built

**backend/app/main.py** - FastAPI application entry point. Mounts routers, serves the dashboard, and runs `Base.metadata.create_all()` on startup to create tables.

**backend/app/models.py** - SQLAlchemy models for `sensor_readings` and `parking_spots` tables.

**backend/app/schemas.py** - Pydantic schemas for request validation and response serialization.

**backend/app/routers/readings.py** - Generic sensor data router. 3 endpoints:

| Method | Path | What it does |
|--------|------|--------------|
| POST | `/api/v1/readings` | Store a reading from any tile |
| GET | `/api/v1/readings` | Recent readings (all tiles) |
| GET | `/api/v1/readings/{tile}` | Readings for one tile |

**backend/app/routers/parking.py** - Parking-specific router. 4 endpoints:

| Method | Path | What it does |
|--------|------|--------------|
| POST | `/api/v1/parking/update/{spot_number}` | ESP32 sends distance, backend checks threshold |
| GET | `/api/v1/parking/status` | Total, occupied, available count |
| GET | `/api/v1/parking/spots` | All spots |
| GET | `/api/v1/parking/spots/{spot_number}` | Single spot |

**backend/app/static/index.html** - Dashboard at `/dashboard` with 4 tile panels (parking, streetlight, traffic light, railroad crossing). Live refresh every 2 seconds using `setInterval` + `fetch`. Each panel fetches independently so a slow tile does not block others.

**embedded/Matin/parking/parking.ino** - ESP32 Arduino sketch. HC-SR04 reads distance, connects to WiFi, sends HTTP POST with distance value.

### Sub-conclusion

7 endpoints across 2 routers. Dashboard with 4 panels. ESP32 sketch for the parking sensor. All data persists in PostgreSQL.

---

## Chapter 2 - Testing

### Method

I tested with `curl` and the Swagger UI at `/docs`. No automated tests.

### Commands

```bash
# Store a generic reading
curl -X POST http://localhost:8000/api/v1/readings \
  -H "Content-Type: application/json" \
  -d '{"tile": "parking", "sensor_type": "sonar", "value": 5.2, "unit": "cm"}'

# Update parking spot
curl -X POST "http://localhost:8000/api/v1/parking/update/1?distance_cm=5.2"

# Check parking status
curl http://localhost:8000/api/v1/parking/status
```

All endpoints returned correct responses. The dashboard showed live data after refresh. Parking correctly showed occupied when distance < 10 cm and free when > 10 cm.

I also tested with simulated data for other tiles (streetlight, railroad crossing) using the generic endpoint. All data stored and displayed correctly.

### What I should have done differently

No automated tests. All testing was manual with curl and Swagger. That works once, but it is not repeatable. For Sprint 2, I need to add pytest tests.

### Sub-conclusion

API works. Tested manually. No automated tests is a gap.

---

## Conclusion

The Sprint 1 backend is fully implemented: 7 endpoints, 2 routers, PostgreSQL with 2 tables, a dashboard with 4 panels, and an ESP32 sketch. Everything tested manually. The main gap is the lack of automated tests.

---

## References

- FastAPI documentation. https://fastapi.tiangolo.com/
- SQLAlchemy 2.0 documentation. https://docs.sqlalchemy.org/
- Arduino HTTPClient library. https://docs.arduino.cc/libraries/httpclient/
