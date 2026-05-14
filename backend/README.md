# City Sim Backend

Shared backend API for The Embedded Alliance smart city project. Receives sensor data from all ESP32 tiles via WiFi HTTP and serves it to dashboards and displays.

## Quick start

```bash
# Start the backend (API + PostgreSQL)
docker compose up --build

# API is now running at http://localhost:8000
# Swagger docs at http://localhost:8000/docs
```

## Test with curl

```bash
# Send a parking sensor reading
curl -X POST "http://localhost:8000/api/v1/parking/update/1?distance_cm=5.2"

# Check parking status
curl http://localhost:8000/api/v1/parking/status

# Send a generic reading (any tile)
curl -X POST http://localhost:8000/api/v1/readings \
  -H "Content-Type: application/json" \
  -d '{"tile": "streetlight", "sensor_type": "ldr", "value": 650, "unit": "lux"}'

# Get all readings for a tile
curl "http://localhost:8000/api/v1/readings?tile=parking&limit=10"
```

## Project structure

```
backend/
  app/
    main.py           # FastAPI app setup and root routes
    database.py        # SQLAlchemy engine and session
    models.py          # Database table definitions
    schemas.py         # Pydantic request/response models
    routers/
      readings.py      # Generic sensor data endpoints
      parking.py       # Parking-specific endpoints
  Dockerfile           # Python container image
  docker-compose.yml   # API + PostgreSQL setup
  requirements.txt     # Python dependencies
  .env.example         # Environment variables template
```

## For team members

You can send data from your ESP32 using the generic readings endpoint. Just change the `tile` and `sensor_type` values:

| Tile | tile | sensor_type | unit |
|------|------|-------------|------|
| Parking (Matin) | parking | sonar | cm |
| Streetlight (Gurpreet) | streetlight | ldr | lux |
| Traffic light (Wesley) | trafficLight | button | bool |
| Railroad crossing (Thijmen) | railroadCrossing | ir | bool |

## Deployment

Currently runs on Docker locally. Will be moved to the Raspberry Pi on the HvA network in Sprint 2.
