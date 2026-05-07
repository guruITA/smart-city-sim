# System architecture

## 1. Overview

The City Sim backend is a shared service that receives sensor data from all tiles in the smart city via WiFi HTTP requests. Each ESP32 microcontroller measures its environment and sends readings to a central FastAPI server, which stores them in PostgreSQL.

The system is designed to be generic: any tile can send data using the same endpoint. Tile specific logic (like parking spot status) is handled by dedicated modules on top of the shared data layer.

## 2. Architecture diagram

The system consists of three layers:

**Hardware layer**: ESP32 microcontrollers with sensors (sonar, LDR, IR) connected to the city tiles. Each ESP32 connects to the local WiFi network and sends HTTP POST requests.

**Application layer**: A Docker Compose setup running two containers. The FastAPI server handles incoming requests and serves the API. PostgreSQL stores all sensor readings and tile specific state.

**Output layer**: LED indicators and OLED displays on the tiles provide local feedback. A web dashboard provides a centralized overview of the entire city.

```
ESP32 sensors ──WiFi POST──> FastAPI ──> PostgreSQL
                                │
                                ├──> LED indicators (via ESP32 GET)
                                ├──> OLED display (via ESP32 GET)
                                └──> Web dashboard (via browser GET)
```

## 3. Communication protocol

All communication between ESP32 devices and the backend uses HTTP REST:

| Direction | Method | Format | Example |
|-----------|--------|--------|---------|
| ESP32 to API | POST | JSON body | `POST /api/v1/readings` |
| ESP32 to API | POST | Query params | `POST /api/v1/parking/update/1?distance_cm=5.2` |
| Dashboard to API | GET | JSON response | `GET /api/v1/parking/status` |

JSON is used because it is easy to parse on both ESP32 (ArduinoJson) and in the browser (native). No authentication is needed for the PoC since the system runs on a local network.

## 4. Technology choices

| Component | Choice | Reason |
|-----------|--------|--------|
| Backend framework | FastAPI (Python) | Fast to develop, auto generated API docs, async support |
| Database | PostgreSQL | Reliable, supports time series queries, team already familiar |
| Containerization | Docker Compose | Reproducible setup, easy to deploy on Raspberry Pi later |
| Microcontroller | ESP32-S3 | WiFi built in, enough GPIO pins, Arduino compatible |
| Sensor | HC-SR04 | Affordable, accurate enough for parking detection (2cm to 400cm range) |

## 5. Deployment plan

**Sprint 1 (current)**: Docker Compose on local laptop for development and demo.

**Sprint 2 and beyond**: Move Docker setup to the Raspberry Pi on the HvA network. This makes the backend accessible to all team members and their ESP32 devices on the same network.

## 6. Design decisions

**Generic readings table**: Instead of making a separate table per tile, all sensor data goes into one `sensor_readings` table with a `tile` column. This means Gurpreet, Thijmen, and Wesley can use the same API without any backend changes. Tile specific logic (like parking status) is built as separate modules that query the shared data.

**No authentication for PoC**: The system runs on a trusted local network. Adding JWT or API keys would slow down development without adding value at this stage. This can be added in Sprint 3 if needed.

**Distance threshold for parking**: A parking spot is considered occupied if the sonar measures less than 10 cm. This threshold is configurable in the backend code and can be adjusted after real world testing.
