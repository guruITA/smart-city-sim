# Design — Speed camera API and production network architecture

## Introduction

This is the design deliverable for Sprint 3 Learning Goal 1. It documents the speed camera data model, API endpoints, production network setup, and dashboard panel design.

The design builds on two earlier patterns. In Sprint 1, I designed the generic `sensor_readings` table and the dedicated `parking_spots` table. In Sprint 2, Thijmen added dedicated `train` and `barrier` tables for structured state data. The speed camera follows the same pattern: when a tile's data has more than one meaningful field, it gets a dedicated table.

## Design goal

The goal is to design a backend API for Gurpreet's speed camera tile that:

- stores structured speed measurements (not just a single float value)
- matches the data format that Gurpreet's ESP32-S3 firmware already produces
- computes statistics server-side so the dashboard does not need to process all readings
- integrates into the existing router-based architecture without changing existing code
- works with the production Pi deployment on port 80

## Starting point: Gurpreet's firmware output

Before designing anything, I read Gurpreet's `speed_camera_s3.ino`. His firmware produces:

- **speed_kmh** — calculated as `(distance / time) * 3.6`, where distance is the 10 cm gap between two IR sensors and time is measured in microseconds
- **direction** — `"1->2"` or `"2->1"` depending on which IR sensor triggers first
- **is_violation** — `true` if `speed_kmh > speed_limit`
- **speed_limit_kmh** — currently 1.0 km/h, appropriate for the miniature scale

He prints all of this to Serial and shows it on an OLED display. He does NOT currently POST to the backend. My API must accept exactly these 4 fields so Gurpreet only needs to add one HTTP POST call in his `update()` function.

## Why a dedicated table

I thought about reusing the generic `sensor_readings` table. It has `{tile, sensor_type, value, unit}`. I could store `{tile: "speedcamera", sensor_type: "speed", value: 2.4, unit: "km/h"}`. But then I lose the direction, violation flag, and speed limit. I would need 4 separate rows per measurement, which is ugly and slow to query.

The speed camera has 4 meaningful fields, not 1. Same reasoning as when Thijmen needed dedicated tables for train and barrier in Sprint 2. When the data has structure, use a dedicated table.

## Speed camera data model

| Column | Type | Why |
|--------|------|-----|
| id | Integer PK | Auto-increment |
| speed_kmh | Float | Measured speed from IR sensor time-of-flight |
| direction | String(10) | `"1->2"` or `"2->1"` — which sensor triggered first |
| is_violation | Boolean | True if speed exceeded the limit at time of measurement |
| speed_limit_kmh | Float | Limit stored with the reading so it survives config changes on the ESP32 |
| created_at | DateTime | Server-side timestamp using `func.now()` |

### Why the violation flag is computed on the ESP32

I made the backend stateless. Gurpreet compares speed against his threshold in firmware and sends the result. The backend just stores it. I considered computing violations server-side (store speed + limit, calculate in the backend), but that would mean the backend needs to know the speed limit. If Gurpreet changes his threshold, the backend would need updating too. By letting the ESP32 decide, the firmware stays the source of truth for speed limit configuration.

### Why the speed limit is stored with each reading

If Gurpreet changes the speed limit mid-experiment from 1.0 to 2.0 km/h, old readings should still show whether they were violations at the time of measurement. Storing the limit alongside each reading makes the data self-contained. Each row tells the full story without needing external context.

## API endpoints

5 endpoints in total, all under `/api/v1/speedcamera`:

**1. POST /** — Store a new speed measurement.

Request body: `{speed_kmh, direction, is_violation, speed_limit_kmh}`

This is what Gurpreet's ESP32 will call. Returns the stored reading with id and timestamp.

**2. GET /** — Get recent readings.

Optional query parameter `?limit=50` (default 50). Returns most recent readings ordered by created_at descending. Used by the dashboard for the scrollable list.

**3. GET /violations** — Get only violation readings.

Same as GET / but filtered to `is_violation = true`. Useful for a "violations only" view.

**4. GET /latest** — Get the single most recent reading.

Returns one reading. Useful for real-time display on the dashboard header.

**5. GET /stats** — Get aggregated statistics.

Returns: `{total_readings, total_violations, average_speed_kmh, max_speed_kmh, violation_rate_percent}`

This uses SQL aggregation: `func.count`, `func.avg`, `func.max`, and a filtered count for violations. The database computes all 5 numbers in one query. For 1000 readings, this transfers 5 numbers instead of 1000 rows. The dashboard calls this endpoint for the stats row.

## Production network design

```
[ESP32 tiles] --WiFi--> [HvA network] --port 80--> [Raspberry Pi 145.92.8.137]
                                                      |
                                                Docker Compose:
                                                  - api container (port 8000 internal)
                                                  - db container (PostgreSQL 16, port 5432 internal)
                                                      |
                                                  host maps 80 -> api:8000
```

The Dockerfile stays on port 8000 (container internal). `docker-compose.yml` maps `"80:8000"` so the host exposes port 80. ESP32 devices POST to `http://145.92.8.137/api/v1/speedcamera/` without needing a port number. HTTP defaults to port 80.

Docker containers restart with `restart: always`. The Pi auto-starts Docker on boot. SSH is configured for remote deployment without physical access.

The `.env` file stores `POSTGRES_USER`, `POSTGRES_PASSWORD`, `POSTGRES_DB`. It is in `.gitignore`. The `deploy.sh` script creates the `.env` from the example file if it does not exist.

## Dashboard design for the 5th panel

The speed camera panel sits in the third row of the dashboard grid (which went from 2x2 to 2x3).

Design choices:

- **Color:** Orange accent (`#f97316`) to differentiate from Gurpreet's streetlight panel (amber) and my parking panel (green/red).
- **Stats row:** Shows total readings, total violations, average speed, max speed. Updated every 2 seconds by calling `/stats`.
- **List:** Scrollable list of recent measurements from `GET /`. Each row shows speed, direction, and timestamp. Violation rows are highlighted in red.
- **Auto-refresh:** Same 2-second interval as other panels, using `setInterval` + `fetch`. Each panel fetches independently so a slow tile does not block others.

## Correct design result for this step

The design is correct when:

- The data model matches Gurpreet's firmware output exactly
- The `/stats` endpoint uses SQL aggregation, not Python-side computation
- The design integrates into the existing architecture without touching existing code
- The production network works on port 80 without code changes to the application
- The dashboard panel is visually distinct and functionally consistent with existing panels

## Limits of this design

- The photo system (ESP32-CAM) is not integrated into the backend. Photos stay on the CAM device. A future version could add photo upload if needed.
- The speed limit is hardcoded in firmware. There is no API to change the speed limit remotely. That could be a Sprint 4 feature.
- The dashboard does not show a time-series graph of speeds. It is a list only. A chart would make trends more visible.
- No rate limiting on the POST endpoint. If the ESP32 sends readings very frequently, the database could fill up. For the PoC scale this is not a problem.
