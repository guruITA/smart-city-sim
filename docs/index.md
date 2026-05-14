# City of The Embedded Alliance

## Project Overview

Welcome to **The Embedded Alliance** project repository. We are building a Smart City simulator as part of Studio Smart Cities at Hogeschool van Amsterdam (semester 2, 2025-2026). Each team member builds a physical tile with embedded sensors that connect to a shared backend API.

## Team Members

| Name | Role | Tile |
|------|------|------|
| **Betul Aydin** | Scrum Master | Pedestrian Crossing |
| **Gurpreet Singh** | Developer | Streetlight (ESP32 + LDR + relay) |
| **Thijmen Walter** | Developer | Railroad Crossing (ESP32 + IR + OLED) |
| **Wesley Oerlmans** | Developer | Traffic Light (ESP32 + MCP23017 + ULN2803) |
| **Matin Khajehfard** | Developer / Backend | Parking (ESP32 + HC-SR04 sonar) + shared backend |

## Architecture

All tiles send sensor data to a shared **FastAPI** backend over WiFi HTTP. The backend stores data in **PostgreSQL** and serves a live dashboard.

```
ESP32 sensors  ──HTTP POST──>  FastAPI backend  ──SQLAlchemy──>  PostgreSQL
                                    │
                              /dashboard (HTML)
```

## Technology Stack

- **Hardware**: ESP32-S3, HC-SR04, LDR, IR sensors, MCP23017, ULN2803, LEDs, OLED displays
- **Backend**: Python 3.11, FastAPI, SQLAlchemy 2.0, PostgreSQL 16, Docker Compose
- **Embedded**: Arduino C++ (Arduino IDE / Wokwi)
- **Documentation**: mkdocs with Material theme (deployed via GitLab CI)
- **Version Control**: GitLab (HvA)

## Quick Start

### Run the backend

```bash
cd backend
docker compose up --build
```

- API: `http://localhost:8000`
- Swagger docs: `http://localhost:8000/docs`
- Dashboard: `http://localhost:8000/dashboard`

### Deploy on Raspberry Pi (Sprint 2+)

```bash
cd backend
bash deploy.sh
```

### Build documentation locally

```bash
pip install -r requirements.txt
mkdocs serve
```

## Project Timeline

| Phase | Description | Timeline |
|-------|-------------|----------|
| Sprint 0 | Planning & Requirements | Week 1-3 |
| Sprint 1 | Analysis, Design, Prototype, Show & Tell | Week 3-6 |
| Sprint 2 | Integration, Pi deployment, Show & Tell | Week 6-9 |
| Sprint 3 | Iteration, improvements | Week 9-12 |
| Sprint 4 | Iteration, improvements | Week 12-15 |
| Sprint 5 | Final Product Delivery | Week 15-18 |

## Documentation per team member

- [Matin - Parking & Backend](Matin/index.md)
- [Thijmen - Railroad Crossing](Thijmen/index.md)

## Commit Guidelines

Follow conventional commit messages:
- `feat:` - New feature
- `fix:` - Bug fix
- `docs:` - Documentation changes
- `test:` - Test additions or changes
- `refactor:` - Code refactoring

## Acknowledgments

- Coaches: Mats Otten, Gerald Stap
- Assessors: Raymond Bookelman, Bernardo Villalba Frias, Dolinde van Beek, Wilko Oskam

---

**Last Updated**: 2026-04-09
**Project Status**: In Development (Sprint 2)
