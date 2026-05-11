# Analysis - Backend scalability and multi-tile architecture

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | April 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group - HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document analyzes how the backend architecture handles the growing number of tiles. In Sprint 1, I built a generic `sensor_readings` table and hoped every tile would fit. In Sprint 2, Thijmen's railroad crossing proved that some tiles need their own tables. I wanted to understand when the generic approach works and when it breaks.

### Main question

How well does the backend architecture scale as more tiles are added?

### Sub-questions

1. When should a tile get its own table vs use the generic table?
2. How does the dashboard handle growing numbers of tiles?

---

## Chapter 1 - Generic vs dedicated tables

### Context

The `sensor_readings` table stores `{tile, sensor_type, value, unit}`. One float value per row. This works for simple sensors: parking sonar sends distance, streetlight LDR sends brightness, traffic light sends state.

### Findings

Thijmen's railroad crossing does not fit. A single train detection involves 3 sensor triggers (A, B, C) across time, a predicted arrival calculation, and a barrier state that depends on the detection sequence. Storing this as separate rows in `sensor_readings` would require joining by timestamp to reconstruct the event. That is slow and fragile.

The dedicated `Train` and `Barrier` tables store the full event in one row. Querying "show me the last 10 train approaches" is a simple `SELECT` with `ORDER BY` instead of a multi-join reconstruction.

**Pattern that emerged:**

| Tile complexity | Storage approach | Example |
|----------------|-----------------|---------|
| Single value per event | Generic table | Parking distance, LDR brightness |
| Multi-field event with temporal state | Dedicated table | Train detection (3 sensors + prediction) |
| State machine with input validation | Dedicated table + check constraints | Barrier (manual/train mode) |

### Sub-conclusion

The generic table works for simple sensors. When a tile has multi-step state or temporal relationships, it needs its own table. The Sprint 1 design decision to allow both was the right call.

---

## Chapter 2 - Dashboard scalability

### Context

The dashboard started as a single parking panel in Sprint 1. In Sprint 2, it grew to 4 panels in a 2x2 grid.

### Findings

Each panel independently fetches its own data from its own API endpoint. A slow or offline tile does not block the others. This is a good pattern. No panel depends on another panel's data.

The railroad crossing panel needed a more complex visualization than the other panels. Parking shows a list. Railroad crossing shows an animated progress line between 3 sensors with predicted arrival time. This required more JavaScript but did not affect the other panels.

**Schema growth:**

| Sprint | Tables | Models |
|--------|--------|--------|
| Sprint 1 | 2 (sensor_readings, parking_spots) | 2 |
| Sprint 2 | 4 (+train, +barrier) | 4 |

At this rate, Sprint 3 could add 1-2 more tables if Wesley or Gurpreet need dedicated storage. The database handles this fine. PostgreSQL does not care about 6 tables.

The 2x2 grid layout works for 4 tiles. If a 5th tile is added, the layout needs adjustment. But that is a CSS problem, not an architecture problem.

### Sub-conclusion

The dashboard scales well with independent panels. Each panel is self-contained. The main risk is layout, not performance.

---

## Conclusion

The backend scales by using generic tables for simple sensors and dedicated tables for complex state. The dashboard scales with independent panels that each own their data fetching. Schema growth from 2 to 4 tables in one sprint is manageable. The architecture supports adding more tiles without breaking existing ones.

---

## References

- FastAPI documentation. https://fastapi.tiangolo.com/
- SQLAlchemy 2.0 documentation. https://docs.sqlalchemy.org/
- PostgreSQL documentation. https://www.postgresql.org/docs/
