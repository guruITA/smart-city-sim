# Matin's documentation

## Parking tile and shared backend

This section contains all documentation for the **Parking** tile and the **shared backend API** that serves sensor data for the entire City Sim.

## Contents

- [System architecture](./system-architecture.md) — overview of how hardware, backend, and frontend connect
- [API specification](./api-specification.md) — all REST endpoints with examples
- [Database schema](./database-schema.md) — table design and data model

## Quick start

Start the backend locally:

```bash
cd backend
docker compose up --build
```

The API will be available at `http://localhost:8000`.
Interactive docs (Swagger UI) at `http://localhost:8000/docs`.

## Tile overview

| Component | Technology | Description |
|-----------|-----------|-------------|
| Sensor | HC-SR04 ultrasonic | Measures distance to detect parked cars |
| Controller | ESP32-S3 | Reads sensor, sends data via WiFi |
| Backend | FastAPI + PostgreSQL | Receives, stores, and serves sensor data |
| Output | LED (red/green) + OLED | Local visual feedback on the tile |
| Dashboard | HTML + JavaScript | Web interface for realtime parking status |
