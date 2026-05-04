# Matin's documentation

## Problem and learning goals
- [Problem statement](./problemStatement.md) — human-centered problem: parking search traffic in Amsterdam

## Parking tile and shared backend

This section contains all documentation for the **Parking** tile and the **shared backend API** that serves sensor data for the entire City Sim.

## Contents

### Architecture & API
- [System architecture](./system-architecture.md) — overview of how hardware, backend, and frontend connect
- [API specification](./api-specification.md) — all REST endpoints with examples
- [Database schema](./database-schema.md) — table design and data model

### Deployment
- [Raspberry Pi deployment](./raspberry-pi-deployment.md) — network setup, Docker, SSH, maintenance
- [Challenges overview](./challenges-overview.md) — problems solved across all sprints

### Portflow Evidence
- [Sprint 1 evidence](./portflowEvidenceSprint1.md) — backend architecture, API implementation, team integration
- [Sprint 2 evidence](./portflowEvidenceSprint2.md) — production deployment, Pi setup, team code integration
- [Sprint 3 evidence](./portflowEvidenceSprint3.md) — Pi port 80, speed camera API, dashboard expansion

## Quick start

### Local development
```bash
cd backend
docker compose up --build
```

### Deploy on Raspberry Pi
```bash
cd backend
bash deploy.sh
```

Dashboard at `http://<host-ip>/` (root, port 80).
API at `http://<host-ip>/api/v1/...`.
Interactive docs (Swagger UI) at `http://<host-ip>/docs`.

## Tile overview

| Component | Technology | Description |
|-----------|-----------|-------------|
| Sensor | HC-SR04 ultrasonic | Measures distance to detect parked cars |
| Controller | ESP32-S3 | Reads sensor, sends data via WiFi |
| Backend | FastAPI + PostgreSQL | Receives, stores, and serves sensor data |
| Output | LED (red/green) + OLED | Local visual feedback on the tile |
| Dashboard | HTML + JavaScript | Web interface for realtime parking status |
