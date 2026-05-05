# Database schema

## Overview

The City Sim database uses PostgreSQL and contains five tables:

1. **sensor_readings**: generic table for all sensor data from all tiles
2. **parking_spots**: realtime state per parking spot
3. **train**: train detection state for railroad crossing tile
4. **barrier**: barrier open/close log for railroad crossing tile
5. **speed_readings**: speed camera measurements from Gurpreet's tile

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

## Table: train

Tracks train detection state for the railroad crossing tile.

| Column | Type | Nullable | Default | Description |
|--------|------|----------|---------|-------------|
| id | INTEGER | no | auto increment | Primary key |
| is_approaching | BOOLEAN | no | true | Whether a train is currently approaching |
| first_sensor_time | TIMESTAMP WITH TZ | yes | now() | When the first IR sensor was triggered |
| second_sensor_time | TIMESTAMP WITH TZ | yes | null | When the second IR sensor was triggered |
| predicted_arrival_seconds | FLOAT | yes | null | Predicted time to reach the crossing |
| updated_at | TIMESTAMP WITH TZ | no | now() | Last update timestamp |

**Indexes**: `id` (primary)

---

## Table: barrier

Log of barrier open/close events for the railroad crossing.

| Column | Type | Nullable | Default | Description |
|--------|------|----------|---------|-------------|
| id | INTEGER | no | auto increment | Primary key |
| is_closed | BOOLEAN | no | false | Whether the barrier is closed |
| input_mode | VARCHAR | no | "manual" | How the barrier was triggered (manual or train) |
| train_id | INTEGER | yes | null | Associated train ID if triggered by train detection |
| created_at | TIMESTAMP WITH TZ | no | now() | When the event occurred |

**Indexes**: `id` (primary)

**Constraints**: `input_mode IN ('manual', 'train')`

---

## Table: speed_readings

Speed camera measurements from Gurpreet's tile. Each row represents one vehicle passing through the IR sensor pair.

| Column | Type | Nullable | Default | Description |
|--------|------|----------|---------|-------------|
| id | INTEGER | no | auto increment | Primary key |
| speed_kmh | FLOAT | no | | Measured speed in km/h |
| direction | VARCHAR(10) | no | "1->2" | Which IR sensor triggered first |
| is_violation | BOOLEAN | no | false | Whether the speed limit was exceeded |
| speed_limit_kmh | FLOAT | no | 1.0 | Speed limit at time of measurement |
| created_at | TIMESTAMP WITH TZ | no | now() | When the measurement was taken |

**Indexes**: `id` (primary)

---

## Entity relationship

```
sensor_readings          parking_spots         train                    barrier                  speed_readings
+------------------+     +----------------+    +---------------------+  +------------------+     +------------------+
| id (PK)          |     | id (PK)        |    | id (PK)             |  | id (PK)          |     | id (PK)          |
| tile             |     | spot_number(UQ)|    | is_approaching      |  | is_closed        |     | speed_kmh        |
| sensor_type      |     | is_occupied    |    | first_sensor_time   |  | input_mode       |     | direction        |
| value            |     | distance_cm    |    | second_sensor_time  |  | train_id --------+---->| is_violation     |
| unit             |     | updated_at     |    | predicted_arrival_s |  | created_at       |     | speed_limit_kmh  |
| created_at       |     +----------------+    | updated_at          |  +------------------+     | created_at       |
+------------------+                           +---------------------+                           +------------------+
```

The tables are mostly independent. The `barrier` table has an optional `train_id` reference to the `train` table (not enforced via foreign key). The parking router writes to both `sensor_readings` and `parking_spots` in the same request. Speed camera and train/barrier data go directly to their dedicated tables.

---

## Data flow

1. ESP32 measures distance with sonar sensor
2. ESP32 sends `POST /api/v1/parking/update/1?distance_cm=5.2`
3. Backend checks if spot 1 exists in `parking_spots`, creates it if not
4. Backend updates `is_occupied` based on threshold (< 10 cm = occupied)
5. Backend inserts a new row in `sensor_readings` with the raw value
6. Dashboard reads from `parking_spots` via `GET /api/v1/parking/status`
