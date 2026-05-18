# Design - API structure and endpoint architecture

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

This document describes the API structure I designed for the City Sim backend. The backend must serve 5 tiles with different sensors. I needed a design that handles parking now and other tiles later without rewriting endpoints.

### Main question

What API structure and endpoint design serves both the parking tile and future tile integrations?

### Sub-questions

1. How should endpoints be organized for multiple tiles?
2. How should input be validated?

---

## Chapter 1 - Endpoint organization

### Context

Five tiles, each producing different sensor data. If I build separate endpoint files per tile from day one, I end up with 5 router files before anyone has WiFi working. I needed a structure that starts simple and grows.

### Method

I looked at how FastAPI organizes routes using `APIRouter`. Each router is a separate file that gets mounted on the main app with a prefix.

### Findings

I split the endpoints into two router groups:

**Generic readings router** (`routers/readings.py`) - Handles any tile.

| Method | Path | What it does |
|--------|------|--------------|
| POST | `/api/v1/readings` | Store a reading from any tile |
| GET | `/api/v1/readings` | Get recent readings (all tiles) |
| GET | `/api/v1/readings/{tile}` | Get readings for one tile |

**Parking router** (`routers/parking.py`) - Handles parking-specific logic.

| Method | Path | What it does |
|--------|------|--------------|
| POST | `/api/v1/parking/update/{spot_number}` | ESP32 sends distance, backend determines occupied/free |
| GET | `/api/v1/parking/status` | Total, occupied, available count |
| GET | `/api/v1/parking/spots` | All spots |
| GET | `/api/v1/parking/spots/{spot_number}` | Single spot |

The generic router accepts data from Gurpreet, Thijmen, and Wesley without any backend changes. When a tile needs specialized logic (like parking's threshold check), it gets its own router.

### Sub-conclusion

Two routers: one generic for all tiles, one dedicated for parking. New tiles start with the generic endpoint and get their own router only when they need specialized logic.

---

## Chapter 2 - Input validation

### Context

ESP32 devices send JSON over HTTP. Malformed payloads (missing fields, wrong types) need to be caught before they hit the database.

### Method

I used Pydantic models as FastAPI request bodies. FastAPI validates incoming JSON against the schema automatically and returns 422 with a clear error if validation fails.

### Findings

Example schema for the generic readings endpoint:

```python
class SensorReadingCreate(BaseModel):
    tile: str = Field(..., example="parking")
    sensor_type: str = Field(..., example="sonar")
    value: float = Field(..., example=5.2)
    unit: str = Field(..., example="cm")
```

Pydantic does three things for free:
1. Type validation - sends 422 if `value` is not a float
2. Required field checking - sends 422 if any field is missing
3. Swagger examples - the `example` parameter populates the "Try it out" form

I did not need to write validation logic or error messages. FastAPI + Pydantic handles it.

### Sub-conclusion

Pydantic schemas validate all input. No manual validation code needed. Swagger docs get populated automatically from the schema examples.

---

## Conclusion

The API uses 2 router groups: generic readings for all tiles, dedicated parking for threshold logic. Pydantic validates all input and generates Swagger examples. This structure scales to more tiles by adding routers without touching existing code.

---

## References

- FastAPI documentation. https://fastapi.tiangolo.com/
- Pydantic documentation. https://docs.pydantic.dev/
- SQLAlchemy 2.0 documentation. https://docs.sqlalchemy.org/
