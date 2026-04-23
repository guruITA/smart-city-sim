# API specification

## Base URL

```
http://145.92.8.137/api/v1
```

For local development: `http://localhost:8000/api/v1`

The dashboard is served at the root (`/`). Interactive documentation (Swagger UI) is available at `/docs`.

---

## Generic endpoints (all tiles)

### POST /readings

Receive a sensor reading from any ESP32 tile.

**Request body (JSON):**

```json
{
  "tile": "parking",
  "sensor_type": "sonar",
  "value": 12.5,
  "unit": "cm"
}
```

**Response (201 Created):**

```json
{
  "id": 1,
  "tile": "parking",
  "sensor_type": "sonar",
  "value": 12.5,
  "unit": "cm",
  "created_at": "2026-03-18T14:30:00Z"
}
```

**Usage per tile:**

| Tile | tile value | sensor_type | unit | Example value |
|------|-----------|-------------|------|---------------|
| Parking | `parking` | `sonar` | `cm` | 5.2 (occupied), 150.0 (free) |
| Streetlight | `streetlight` | `ldr` | `lux` | 200 (dark), 800 (bright) |
| Traffic light | `trafficLight` | `button` | `bool` | 1 (car detected), 0 (no car) |
| Railroad crossing | `railroadCrossing` | `ir` | `bool` | 1 (train detected), 0 (clear) |

### GET /readings

Get sensor readings with optional filters.

**Query parameters:**

| Param | Type | Default | Description |
|-------|------|---------|-------------|
| tile | string | none | Filter by tile name |
| sensor_type | string | none | Filter by sensor type |
| limit | int | 50 | Max number of results (1 to 500) |

**Example:** `GET /api/v1/readings?tile=parking&limit=10`

### GET /readings/latest/{tile}

Get the most recent reading for a specific tile.

**Example:** `GET /api/v1/readings/latest/streetlight`

---

## Parking endpoints

### GET /parking/status

Get the current parking overview: total spots, how many are occupied, and how many are available.

**Response:**

```json
{
  "total_spots": 3,
  "occupied": 1,
  "available": 2,
  "spots": [
    {
      "spot_number": 1,
      "is_occupied": true,
      "distance_cm": 5.2,
      "updated_at": "2026-03-18T14:30:00Z"
    },
    {
      "spot_number": 2,
      "is_occupied": false,
      "distance_cm": 150.0,
      "updated_at": "2026-03-18T14:29:55Z"
    }
  ]
}
```

### POST /parking/update/{spot_number}

Update a parking spot with a new distance reading. The spot is automatically created if it does not exist yet.

**Query parameters:**

| Param | Type | Description |
|-------|------|-------------|
| distance_cm | float | Distance measured by the sonar sensor |

**Example:** `POST /api/v1/parking/update/1?distance_cm=5.2`

A spot is marked as occupied when `distance_cm < 10.0`.

### GET /parking/spots

Get all parking spots and their current status.

### GET /parking/spots/{spot_number}

Get status of a specific parking spot.

---

## Speed camera endpoints

### POST /speedcamera/

Store a new speed measurement from the ESP32 speed camera.

**Request body (JSON):**

```json
{
  "speed_kmh": 2.4,
  "direction": "1->2",
  "is_violation": true,
  "speed_limit_kmh": 1.0
}
```

**Response (201 Created):**

```json
{
  "id": 1,
  "speed_kmh": 2.4,
  "direction": "1->2",
  "is_violation": true,
  "speed_limit_kmh": 1.0,
  "created_at": "2026-04-23T10:30:00Z"
}
```

### GET /speedcamera/

Get recent speed camera readings, ordered by most recent first.

**Query parameters:**

| Param | Type | Default | Description |
|-------|------|---------|-------------|
| limit | int | 50 | Max number of results (1 to 500) |

### GET /speedcamera/violations

Get only speed limit violations.

**Query parameters:**

| Param | Type | Default | Description |
|-------|------|---------|-------------|
| limit | int | 50 | Max number of results (1 to 500) |

### GET /speedcamera/latest

Get the most recent speed camera reading. Returns `null` if no readings exist.

### GET /speedcamera/stats

Get aggregated speed camera statistics.

**Response:**

```json
{
  "total_readings": 42,
  "total_violations": 8,
  "average_speed_kmh": 1.35,
  "max_speed_kmh": 3.72,
  "violation_rate_percent": 19.0
}
```

---

## Testing with curl

Send a parking reading:

```bash
curl -X POST "http://localhost:8000/api/v1/parking/update/1?distance_cm=5.2"
```

Send a generic reading:

```bash
curl -X POST http://localhost:8000/api/v1/readings \
  -H "Content-Type: application/json" \
  -d '{"tile": "streetlight", "sensor_type": "ldr", "value": 650, "unit": "lux"}'
```

Get parking status:

```bash
curl http://localhost:8000/api/v1/parking/status
```

Send a speed camera reading:

```bash
curl -X POST http://localhost:8000/api/v1/speedcamera/ \
  -H "Content-Type: application/json" \
  -d '{"speed_kmh": 2.4, "direction": "1->2", "is_violation": true, "speed_limit_kmh": 1.0}'
```

Get speed camera stats:

```bash
curl http://localhost:8000/api/v1/speedcamera/stats
```

---

## Error responses

| Code | Meaning |
|------|---------|
| 201 | Created successfully |
| 200 | Request successful |
| 404 | Resource not found (e.g. spot does not exist) |
| 422 | Validation error (missing or wrong field types) |
