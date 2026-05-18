# Design - Speed camera data model and endpoints

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | May 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group - HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document designs the speed camera data model and API endpoints. Gurpreet's ESP32 produces 4 fields per measurement. I need a backend that stores them and shows them on the dashboard.

### Main question

What data model and endpoints fit Gurpreet's firmware output?

### Sub-questions

1. Should I use the generic sensor_readings table or a dedicated table?
2. What endpoints does the dashboard need?

---

## Chapter 1 - Data model

### Context

Gurpreet's `speed_camera_s3.ino` outputs 4 fields per measurement: `speed_kmh`, `direction`, `is_violation`, `speed_limit_kmh`. The generic `sensor_readings` table stores `{tile, sensor_type, value, unit}` - one float value per row.

### Method

I compared storing 4 fields in the generic table (4 separate rows per measurement, joined by timestamp) vs a dedicated table (1 row with all fields).

### Findings

The generic table loses structure. To query "all violations" I would need to join rows by timestamp. That is slow and fragile.

A dedicated `speed_readings` table:

| Column | Type | Why |
|--------|------|-----|
| id | Integer PK | Auto-increment |
| speed_kmh | Float | Measured speed |
| direction | String(10) | "1->2" or "2->1" |
| is_violation | Boolean | Exceeded limit at measurement time |
| speed_limit_kmh | Float | Stored per reading so it survives config changes |
| created_at | DateTime | Server timestamp |

The violation flag is computed on the ESP32, not in the backend. This keeps the backend stateless. If Gurpreet changes his threshold, the backend does not need updating.

### Sub-conclusion

Dedicated table. One row per measurement with all 4 fields. Backend stays stateless.

---

## Chapter 2 - Endpoints

### Context

The ESP32 needs to store data. The dashboard needs to display data and stats.

### Method

I listed what each consumer needs and designed the minimal set of endpoints.

### Findings

5 endpoints under `/api/v1/speedcamera`:

| Method | Path | What it does |
|--------|------|--------------|
| POST | `/` | Store a reading from ESP32 |
| GET | `/` | Recent readings (default 50) |
| GET | `/violations` | Only violations |
| GET | `/latest` | Single most recent reading |
| GET | `/stats` | Aggregated: total, violations, avg speed, max speed, violation rate |

The `/stats` endpoint uses SQL aggregation (`func.count`, `func.avg`, `func.max`). The database computes 5 numbers in one query instead of transferring all rows to Python.

### Sub-conclusion

5 endpoints cover storage, display, filtering, and aggregation. The stats endpoint keeps the response fast regardless of data volume.

---

## Conclusion

A dedicated `speed_readings` table with 5 columns matches the firmware output. 5 endpoints cover all use cases. The backend stays stateless: violations are computed on the ESP32.

---

## References

- Gurpreet Singh. (2026). `speed_camera_s3.ino`. City Sim Learning Group repository.
- FastAPI documentation. https://fastapi.tiangolo.com/
- SQLAlchemy 2.0 documentation. https://docs.sqlalchemy.org/
