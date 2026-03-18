# Database schema

## Overview

The City Sim database uses PostgreSQL and contains two tables:

1. **sensor_readings**: generic table for all sensor data from all tiles
2. **parking_spots**: realtime state per parking spot

The generic table stores the history (every reading ever received). The parking spots table stores only the current state (last reading per spot). This separation keeps queries fast: the dashboard reads from `parking_spots` (small table), while historical analysis uses `sensor_readings` (large table).

---

## Table: sensor_readings

Stores every sensor reading received from any tile.

| Column | Type | Nullable | Default | Description |
|--------|------|----------|---------|-------------|
| id | INTEGER | no | auto increment | Primary key |
| tile | VARCHAR(50) | no | | Which tile sent this (parking, streetlight, etc.) |
| sensor_type | VARCHAR(50) | no | | Type of sensor (sonar, ldr, ir, button) |
| value | FLOAT | no | | Raw sensor value |
| unit | VARCHAR(20) | yes | "" | Unit of measurement (cm, lux, bool) |
| created_at | TIMESTAMP WITH TZ | no | now() | When the reading was received |

**Indexes**: `id` (primary), `tile` (for filtering)

---

## Table: parking_spots

Current status per parking spot. Updated on every new sonar reading.

| Column | Type | Nullable | Default | Description |
|--------|------|----------|---------|-------------|
| id | INTEGER | no | auto increment | Primary key |
| spot_number | INTEGER | no | | Unique spot identifier (1, 2, 3, ...) |
| is_occupied | BOOLEAN | no | false | Whether a car is detected |
| distance_cm | FLOAT | no | 0.0 | Last measured distance from sonar |
| updated_at | TIMESTAMP WITH TZ | no | now() | When this spot was last updated |

**Indexes**: `id` (primary), `spot_number` (unique)

---

## Entity relationship

```
sensor_readings          parking_spots
+------------------+     +------------------+
| id (PK)          |     | id (PK)          |
| tile             |     | spot_number (UQ)  |
| sensor_type      |     | is_occupied      |
| value            |     | distance_cm      |
| unit             |     | updated_at       |
| created_at       |     +------------------+
+------------------+
```

The two tables are not directly linked with a foreign key. The parking router writes to both tables in the same request: it updates `parking_spots` for the current state and inserts a row into `sensor_readings` for the history.

---

## Data flow

1. ESP32 measures distance with sonar sensor
2. ESP32 sends `POST /api/v1/parking/update/1?distance_cm=5.2`
3. Backend checks if spot 1 exists in `parking_spots`, creates it if not
4. Backend updates `is_occupied` based on threshold (< 10 cm = occupied)
5. Backend inserts a new row in `sensor_readings` with the raw value
6. Dashboard reads from `parking_spots` via `GET /api/v1/parking/status`
