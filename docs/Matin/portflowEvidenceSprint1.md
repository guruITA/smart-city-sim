# Sprint 1 — Portflow Evidence

## Learning Journal

### Challenge 1: Building a REST API with FastAPI for IoT sensor data

**Learning question**
How do I build a REST API that receives sensor data from ESP32 microcontrollers and stores it in a database, using a framework I have never used before?

**S — Situation**
Sprint 1 of the City Sim project. Our team of 5 students is building a miniature smart city where each tile has ESP32 sensors. I chose the role of Backend Developer. In Sprint 0 I selected FastAPI + PostgreSQL + Docker as my tech stack, but I had never used FastAPI before. The parking tile needs a backend that receives ultrasonic distance readings and determines whether a spot is occupied.

**T — Task**
Build a working FastAPI application with at least 5 REST endpoints that accept sensor readings from ESP32 devices over WiFi HTTP POST, validate the data with Pydantic, store it in PostgreSQL, and serve it back via GET endpoints. The API must have auto-generated Swagger documentation so team members can test it without reading code.

**A — Action**
I started by reading the FastAPI documentation and building a minimal "hello world" endpoint. Then I created the project structure: `main.py` for the app, `models.py` for SQLAlchemy models, `schemas.py` for Pydantic validation, and a `routers/` folder to separate endpoint groups. I built the generic `POST /api/v1/readings` endpoint first because all tiles need it. Then I added parking-specific endpoints (`/update/{spot_number}`, `/status`, `/spots`). I tested with `curl` and the Swagger UI at `/docs`. I used Pydantic `Field` with examples so the Swagger docs show realistic payloads without needing separate documentation.

**R — Result**
Working FastAPI application with 7 REST endpoints. The API accepts POST requests from any tile (parking, streetlight, traffic light, railroad crossing) via the generic endpoint, plus dedicated parking operations. Pydantic catches malformed requests automatically. Swagger docs are available at `/docs` with working "Try it out" buttons. All data persists in PostgreSQL.

**R — Reflection**
FastAPI's learning curve was lower than expected. The combination of type hints + Pydantic meant I got input validation and documentation for free, without writing extra code. The biggest risk was choosing an unfamiliar framework under time pressure, but it paid off because I could move faster than with Spring Boot. One thing I did wrong: I did not write automated tests. I tested everything manually with curl and Swagger, which works but is not repeatable.

**T — Transfer**
The pattern of "type hints generate docs and validation" is something I will use in every future Python API. The router separation pattern scales to any number of tiles. For Sprint 2, I need to add automated tests and not rely on manual testing alone.

---

### Challenge 2: Designing a generic database schema for multiple sensor types

**Learning question**
How do I design a database schema that accepts data from 5 different sensor types (sonar, LDR, IR, button, speed) without needing a separate table for each, while still keeping real-time queries fast for specific tiles?

**S — Situation**
Our smart city has 5 tiles, each with different sensors producing different data. Gurpreet sends LDR light values, Thijmen sends IR trigger timestamps, Wesley sends traffic light states, and I send ultrasonic distances. All of this goes to one shared backend. If each tile needs its own table and endpoints, the backend becomes unmaintainable for one person.

**T — Task**
Design a database schema that stores sensor readings from all tiles in a single generic table, plus a separate table for parking spot state that supports fast real-time queries. The schema must handle at least 4 tile types without code changes.

**A — Action**
I designed two tables. `sensor_readings` has columns for `tile` (which tile sent it), `sensor_type` (sonar, ldr, ir, etc.), `value` (float), `unit` (cm, lux, bool), and `created_at` (timestamp). This stores the full history from every tile. For parking, I added a `parking_spots` table that only stores current state: `spot_number`, `is_occupied`, `updated_at`. Every time a new sonar reading comes in, the parking endpoint updates the spot's status based on a distance threshold (< 10 cm = occupied). This dual-table approach separates history from state.

**R — Result**
The generic `sensor_readings` table successfully handled all 4 tile types during testing with simulated data. Parking status queries are instant because they only read the `parking_spots` table (3 rows) instead of scanning the full history. The schema required zero changes when team members started sending different sensor types.

**R — Reflection**
The dual-table pattern (generic history + dedicated state) turned out to be a reusable pattern. In Sprint 2, Thijmen's railroad crossing needed the same thing: generic readings for history, plus dedicated `train` and `barrier` tables for state. I should have documented this pattern explicitly as a design decision at the time, instead of only documenting the tables themselves.

**T — Transfer**
For any future IoT backend: start with a generic table for raw data, add dedicated tables only when a specific use case needs structured state. This avoids over-engineering upfront while keeping the path to specialization open. I applied this pattern again in Sprint 3 for the speed camera.

---

### Challenge 3: Setting up Docker Compose for reproducible team development

**Learning question**
How do I use Docker Compose to run both the API and database so team members can start the backend with one command, without installing Python or PostgreSQL?

**S — Situation**
Our team has 5 members on different machines (Windows, sometimes Mac). If everyone has to install Python 3.11, PostgreSQL 16, and configure database credentials to test the backend, most will never run it. I need a setup where `docker compose up` is the only command needed.

**T — Task**
Create a Docker Compose configuration that runs the FastAPI application and PostgreSQL database in containers, with health checks to ensure the database is ready before the API starts, and automatic table creation on first boot.

**A — Action**
I wrote a `Dockerfile` for the FastAPI app (Python 3.11 slim, pip install requirements, uvicorn with hot-reload for development). Then a `docker-compose.yml` with two services: `db` (PostgreSQL 16 with health check via `pg_isready`) and `api` (depends on db, exposes port 8000, connects to db via environment variables). SQLAlchemy's `Base.metadata.create_all()` runs on app startup, so tables are created automatically on first boot without running migrations manually.

**R — Result**
Team members can clone the repo, run `docker compose up --build`, and access the API at `localhost:8000/docs` within 2 minutes. No Python or PostgreSQL installation needed. The database persists data in a Docker volume. Hot-reload means code changes are reflected without restarting containers.

**R — Reflection**
Docker Compose solved the "works on my machine" problem completely. The health check on PostgreSQL was important because without it, the API would crash on startup trying to connect to a database that was still initializing. One missed opportunity: I should have shared this setup with team members earlier in the sprint. Nobody else ran the backend during Sprint 1 because I did not actively demo it to them.

**T — Transfer**
Every future project should start with Docker Compose, not end with it. Having reproducible infrastructure from day one saves debugging time later. For Sprint 2, the Docker setup needs to be hardened for production on the Raspberry Pi: no hot-reload, .env file for secrets, restart policy for reboot survival.

---

## Learning Group — City Sim

### Analysis — Parking search traffic in Amsterdam: problem identification and data requirements

#### 6W Problem Analysis (Verhoeven, 2010)

**Wat (What)**
Drivers in Amsterdam spend 10 to 20 minutes searching for free parking spots in busy neighborhoods. Up to 30% of inner-city traffic in peak areas consists of cars circling for parking (Shoup, 2017). This search traffic produces unnecessary CO₂ emissions, noise pollution, and fine particles in residential streets.

**Aanleiding (Trigger)**
The City Sim project requires each student to address a human-centered problem in a smart city context. Parking search traffic is a problem that can be addressed with real-time sensor data: if drivers know which spots are free before they start driving, they do not need to circle the block.

**Wie (Who)**
- Residents of dense neighborhoods (De Pijp, Oost, Centrum) who experience noise and pollution from circling cars
- Drivers who waste time and fuel searching for spots
- The municipality of Amsterdam, which spends resources on traffic management that could be avoided with better data availability

**Wanneer (When)**
The problem is worst during peak hours (workday evenings, weekend afternoons) when parking demand exceeds supply. But search traffic occurs at any time in popular areas because drivers have no way of knowing spot availability before arriving.

**Waarom (Why)**
Amsterdam targets emission-free traffic in the city center by 2030. Reducing unnecessary driving from parking search directly contributes to this target. Beyond emissions, search traffic increases congestion and reduces quality of life for residents.

**Waar (Where)**
Amsterdam inner city, specifically residential neighborhoods with high parking demand and limited supply. For this proof of concept: a miniature city tile with 3 parking spots monitored by ultrasonic sensors.

#### Hoofdvraag (main research question)

How can real-time parking occupancy data reduce unnecessary search traffic in urban residential areas?

#### Deelvragen (sub-questions)

1. How much urban traffic consists of cars searching for parking, and what is the environmental impact?
2. What sensor data is needed to determine whether a parking spot is occupied in real time?
3. How should parking occupancy data be collected, stored, and served to make it accessible to drivers?
4. What infrastructure is needed to run a reliable real-time parking detection system?

#### Methodology

For this sprint, I focused on deelvragen 2 and 3 through literature research and prototyping.

**Deelvraag 2 — What sensor data is needed?**
I researched how parking occupancy is detected in existing smart parking systems. The most common approaches are:
- Ultrasonic sensors (distance measurement, detects object above/below threshold)
- Magnetometer sensors (detects metal mass of a vehicle)
- Camera-based detection (image recognition, requires more compute)

For our proof of concept, ultrasonic (HC-SR04) was selected because it is cheap, works with ESP32 GPIO, and gives a clear binary signal: distance < threshold means occupied. The threshold of 10 cm was determined through physical testing with a toy car on the miniature tile.

**Deelvraag 3 — How should data be collected and served?**
I analyzed the communication flow: ESP32 reads sensor → connects to WiFi → sends HTTP POST with JSON → backend validates and stores → dashboard reads via HTTP GET. This is the same pattern as a standard web application, with a microcontroller as the client instead of a browser.

I analyzed each team member's sensor setup to determine whether a single API design could serve all tiles:
- Thijmen (Railroad Crossing): 3 working demos with IR sensors, OLED display, finite state machine. No WiFi connectivity yet.
- Gurpreet (Streetlight): LDR sensor with relay. Functional but no WiFi.
- Wesley (Traffic Light): No code in repository yet.
- Betül: Scrum Master role, no individual tile.

**Finding:** All tiles produce data that fits the pattern `{tile, sensor_type, value, unit}`. A generic API endpoint can serve all of them. Tile-specific endpoints should be deferred until team members have WiFi connectivity.

#### Limitations

- Deelvraag 1 relies on secondary sources (Shoup, 2017). I did not collect primary data on Amsterdam parking search traffic.
- The proof of concept uses 3 parking spots on a miniature model. Real-world deployment would require different sensor types and higher volumes.
- The analysis of team members' code is a snapshot from Sprint 1. Their progress changes each sprint.

#### Sources

- Shoup, D. (2017). *The High Cost of Free Parking*. Routledge. (Cited for the 30% search traffic statistic)
- FastAPI documentation. https://fastapi.tiangolo.com/
- Arduino HTTPClient library. https://docs.arduino.cc/libraries/httpclient/

---

### Advise — Shared API design and technology recommendations for team integration

I designed the backend API to be used by all 5 team members, not just my parking tile. After analyzing each team member's sensor setup and embedded code (see Analysis), I made the following recommendations:

**Technology choice: FastAPI over Spring Boot**
I advised the team to use FastAPI (Python) instead of Java Spring Boot for the shared backend. The reasoning: faster development cycle for a 17-week student project, built-in Swagger docs so team members can test without reading code, and Pydantic validation that catches malformed ESP32 payloads automatically. This choice was validated by Sprint 0 research (see learningJournalSprint0.md).

**Generic endpoint design**
I created a generic `POST /api/v1/readings` endpoint that accepts data from any tile with a simple JSON body: `{tile, sensor_type, value, unit}`. This means Gurpreet, Thijmen, and Wesley can send their sensor data by adding WiFi + HTTP code to their existing sketches, without any backend changes needed. I documented the exact JSON format per tile in the API specification.

**Integration readiness assessment**
Based on my codebase analysis, I identified that Gurpreet and Thijmen have working standalone sketches but no WiFi connectivity. Wesley has not committed code. I advised the team to prioritize WiFi integration in Sprint 2 and offered to help with HTTP POST examples for each tile.

**Docker for team development**
I advised all team members to use Docker Compose to run the backend locally: one command, no dependencies to install. This removes the barrier to testing API integration from their ESP32 sketches.

**Deliverables:** api-specification.md (with usage examples per tile), backend README.md (with team onboarding instructions)

---

### Design — Backend architecture and database schema for real-time parking detection

I designed the system architecture for the shared City Sim backend. The architecture has three layers:

1. **Hardware layer**: ESP32 microcontrollers with sensors (HC-SR04 ultrasonic for parking, LDR for streetlight, IR for railroad crossing)
2. **Application layer**: FastAPI + PostgreSQL running in Docker Compose. The API validates input with Pydantic, stores readings in the database, and serves data via REST endpoints.
3. **Output layer**: LED indicators on the tiles, OLED displays, and a web dashboard showing all tiles with live data refreshing every 2 seconds.

**Database design decisions:**
- Generic `sensor_readings` table for all tiles (history storage)
- Dedicated `parking_spots` table for real-time state (fast status queries)
- REST API with JSON over WiFi HTTP, chosen for simplicity on both ESP32 (HTTPClient library) and browser (native fetch)
- Docker Compose for reproducible deployment, planned to move from laptop to Raspberry Pi in Sprint 2

**Dashboard design:**
- 2x2 grid layout with one panel per tile
- Each panel independently fetches its own data (a slow or offline tile does not block others)
- Auto-refresh every 2 seconds using `setInterval` + `fetch`
- Parking panel shows spot count (occupied/available) with color coding

**Deliverables:** system-architecture.md, database-schema.md, api-specification.md, city_sim_backend_architecture_sprint1.svg

---

### Realise — Working backend API, database, and dashboard

I implemented the full backend stack as designed:

- **FastAPI application** with 7 REST endpoints across 2 router groups (generic readings + parking)
- **PostgreSQL database** with 2 tables (`sensor_readings`, `parking_spots`), auto-created on first boot via SQLAlchemy
- **Docker Compose** running both containers with health checks and volume persistence
- **Parking endpoints**: `POST /update/{spot_number}` (ESP32 sends distance, backend determines occupied/free based on 10 cm threshold), `GET /status` (total/occupied/available count), `GET /spots` (all spots), `GET /spots/{spot_number}` (single spot)
- **City Sim Dashboard** at `/dashboard` showing 4 tile panels (parking, streetlight, traffic light, railroad crossing) with live refresh every 2 seconds
- **ESP32 Arduino sketch** for parking sensor: HC-SR04 ultrasonic reads distance, connects to WiFi, sends HTTP POST with distance value to the backend

The backend was tested with simulated sensor data for all tiles. The dashboard correctly displays status for each tile. The parking tile correctly determines occupancy based on the distance threshold.

**Deliverables:** backend/ folder (FastAPI app, Docker setup), embedded/matin/parking/parking.ino, dashboard at `/dashboard`
