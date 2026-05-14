# Design - Dual-table database pattern

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

This document explains the database design for the City Sim backend. Five tiles produce different sensor data. I needed a schema that stores everything without needing a new table for each tile, while still making parking status queries fast.

### Main question

Should all sensor data go in one table, or should tiles have dedicated tables?

### Sub-questions

1. What are the tradeoffs of generic vs dedicated tables?
2. How should parking state be queried efficiently?

---

## Chapter 1 - Generic vs dedicated tables

### Context

Gurpreet sends LDR light values. Thijmen sends IR trigger timestamps. Wesley sends traffic light states. I send ultrasonic distances. All different shapes of data going to one backend.

### Method

I compared two approaches:
- **Option A**: One table per tile. Each tile gets columns matching its sensor output.
- **Option B**: One generic table for all tiles. Each row stores `{tile, sensor_type, value, unit}`.

### Findings

| Criteria | One table per tile | One generic table |
|----------|-------------------|-------------------|
| Adding a new tile | New migration, new model, new endpoints | Just POST with a new `tile` value |
| Query complexity | Simple per tile | Need to filter by `tile` |
| Schema maintenance | 5+ tables to maintain | 1 table |
| Structured multi-field data | Natural (dedicated columns) | Awkward (one value per row) |

Option A is better when a tile has multiple related fields per reading. Option B is better for raw sensor data where each reading is a single value with a unit.

For Sprint 1, most tiles produce single values: a distance, a light level, a boolean state. One generic table works.

### Sub-conclusion

Generic table for raw sensor history. Dedicated tables only when a tile needs structured multi-field state.

---

## Chapter 2 - Parking state queries

### Context

The dashboard needs to show "2 out of 3 spots occupied" instantly. If I query the history table for this, I need to find the latest reading per spot and check its value against the threshold. That is slow and fragile.

### Method

I added a separate `parking_spots` table that only stores current state. Every time a new distance reading arrives, the parking endpoint updates the corresponding spot.

### Findings

**`sensor_readings` table** (history):

| Column | Type | Purpose |
|--------|------|---------|
| id | Integer PK | Auto-increment |
| tile | String | Which tile sent it |
| sensor_type | String | sonar, ldr, ir, etc. |
| value | Float | Sensor value |
| unit | String | cm, lux, bool |
| created_at | DateTime | Timestamp |

**`parking_spots` table** (current state):

| Column | Type | Purpose |
|--------|------|---------|
| spot_number | Integer PK | 1, 2, or 3 |
| is_occupied | Boolean | Based on 10 cm threshold |
| updated_at | DateTime | Last update time |

The dashboard queries `parking_spots` (3 rows). The history table grows unbounded but is only queried for analytics or debugging. This separation means status queries stay fast no matter how much history accumulates.

The 10 cm threshold was determined by physical testing with a toy car on the miniature tile.

### Sub-conclusion

Dual-table pattern: generic history for all tiles, dedicated state table for parking. Status queries read 3 rows instead of scanning history.

---

## Conclusion

The dual-table pattern works: one generic `sensor_readings` table stores history from all tiles, one dedicated `parking_spots` table stores current state. Adding new tiles requires no schema changes. Parking status queries are instant. I reused this pattern in Sprint 2 for railroad crossing and in Sprint 3 for speed camera.

---

## References

- SQLAlchemy 2.0 documentation. https://docs.sqlalchemy.org/
- FastAPI documentation. https://fastapi.tiangolo.com/
