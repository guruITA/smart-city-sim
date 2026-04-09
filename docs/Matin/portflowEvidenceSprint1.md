# Portflow Evidence - Sprint 1

## City Sim Learning Group

### Design - Backend architecture and database schema

I designed the system architecture for the shared City Sim backend that serves sensor data from all tiles in our smart city. The architecture consists of three layers: a hardware layer (ESP32 microcontrollers with sensors), an application layer (FastAPI + PostgreSQL running in Docker), and an output layer (LED indicators, OLED displays, and a web dashboard).

Key design decisions:
- Generic sensor_readings table that accepts data from any tile, so all team members can use the same API without backend changes
- Separate parking_spots table for realtime state, keeping current status queries fast while preserving full history in sensor_readings
- REST API with JSON over WiFi HTTP, chosen for simplicity on both ESP32 (HTTPClient library) and browser (native fetch)
- Docker Compose setup for reproducible deployment, planned to move from local laptop to Raspberry Pi in Sprint 2

Deliverables: system-architecture.md, database-schema.md, api-specification.md in docs/Matin/

### Realise - Working backend API and dashboard

I implemented and deployed the full backend stack:
- FastAPI application with 7 REST endpoints serving all tiles
- PostgreSQL database with auto-created tables via SQLAlchemy
- Docker Compose configuration running both containers
- Parking-specific endpoints with realtime status (occupied/free based on sonar threshold)
- City Sim Dashboard showing all 4 tiles with live data refreshing every 2 seconds
- ESP32 Arduino sketch for parking sensor (HC-SR04 ultrasonic + WiFi HTTP POST)

The backend was tested with simulated sensor data for all tiles (parking, streetlight, traffic light, railroad crossing) and the dashboard correctly displays the status of each tile.

Deliverables: backend/ folder (FastAPI app, Docker setup), embedded/Matin/parking/parking.ino, dashboard at /dashboard endpoint

### Advise - Shared API design for team integration

I designed the backend API to be used by all team members, not just my own parking tile. After analyzing each team member's sensor setup and code, I created a generic POST /api/v1/readings endpoint that accepts any tile's data with a simple JSON body containing tile name, sensor type, value, and unit.

This means Gurpreet (streetlight/LDR), Thijmen (railroad crossing/IR), and Wesley (traffic light/LEDs) can all send their sensor data to the backend by adding WiFi + HTTP code to their existing ESP32 sketches, without any backend changes needed. I documented the exact JSON format per tile in the API specification.

I also identified that Gurpreet and Thijmen have working standalone sketches but no WiFi connectivity yet, and that Wesley has not committed code. This analysis will inform our Sprint 2 planning to prioritize integration.

Deliverables: api-specification.md (with usage examples per tile), backend README.md (with team onboarding instructions)

### Analysis - Team status and integration assessment

I performed a systematic analysis of the entire repository to assess each team member's progress and identify integration needs:

- Thijmen: 3 working demos (alternating blink, RGB LED, train prediction with FSM + OLED), well-documented, but no WiFi/HTTP connectivity
- Gurpreet: working streetlight with LDR sensor and relay, basic but functional, no WiFi/HTTP connectivity
- Wesley: no code or documentation in the repository yet
- Betul: Scrum Master role, no individual tile assigned

Based on this analysis, I determined that the generic readings endpoint is sufficient for Sprint 1 and that tile-specific endpoints (like dedicated streetlight or railroad routers) should be deferred to Sprint 2 when team members have WiFi connectivity working.

Deliverables: tile analysis (documented in team communication), prioritized endpoint roadmap

---

## Learning Journal

### Manage & Control - STARR Reflection

**Situatie:** Ik moest de backend opzetten voor het City Sim project en tegelijk zorgen dat alles goed gedocumenteerd werd voor Portflow evidence.

**Taak:** De hele backend stack ontwerpen, bouwen en documenteren, inclusief project structuur in GitLab, Docker setup, en documentatie in mkdocs format. Daarnaast moest ik een werkflow opzetten waarin ik efficient kan werken met AI tools (Claude Code in Antigravity IDE) zonder dat dit zichtbaar is voor docenten.

**Actie:** Ik heb eerst de architectuur uitgedacht en gedocumenteerd voordat ik code schreef. Ik heb de backend folder gestructureerd met duidelijke scheiding (routers, models, schemas), Docker Compose opgezet met health checks, en drie markdown documenten geschreven (architecture, API spec, database schema). Ik heb git workflow ingericht met conventional commits op de matin branch, met Merge Requests naar main. Ik heb .gitignore geconfigureerd om AI configuratiebestanden lokaal te houden.

**Resultaat:** Werkende backend met 7 endpoints, PostgreSQL database, Docker setup, dashboard, en complete documentatie. Alles gecommit en gepusht naar GitLab. Het project is klaar voor de Sprint 1 presentatie.

**Reflectie:** Ik merk dat ik productiever ben als ik eerst de architectuur documenteer voordat ik code schrijf. Dit dwingt me om na te denken over design decisions en voorkomt dat ik halverwege van richting verander. Een uitdaging blijft het consistent bijhouden van mijn Portflow evidence. Ik moet dit direct na elke werksessie doen in plaats van achteraf, want dan vergeet ik details.
