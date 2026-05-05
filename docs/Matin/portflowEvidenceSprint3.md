# Portflow Evidence - Sprint 3

## City Sim Learning Group

### Design - Production infrastructure and speed camera API architecture

In Sprint 3, I focused on two major areas: hardening the production Raspberry Pi deployment and designing a new dedicated API for Gurpreet's speed camera tile.

Key design decisions:

- **Port 80 deployment**: Reconfigured Docker Compose to expose the API on port 80 instead of 8000, removing the need for users and ESP32 devices to specify a port number. This simplifies the endpoint URLs from `http://<pi-ip>:8000/api/v1/...` to `http://<pi-ip>/api/v1/...`.
- **Dashboard on root**: Moved the dashboard from `/dashboard` to `/` so the Pi serves the City Sim Dashboard as the landing page. Opening `http://<pi-ip>/` in a browser immediately shows the full tile overview without needing to know a subpath. The API endpoints, Swagger docs (`/docs`), and health check (`/health`) remain unchanged.
- **SSH access hardening**: Fixed SSH connectivity to the Raspberry Pi on the HvA network, enabling remote management without physical access. This was critical for deploying updates during sprint demos without needing to be in the lab.
- **Speed camera data model**: Designed a dedicated `speed_readings` table instead of reusing the generic `sensor_readings` table. The speed camera has structured data (speed in km/h, direction, violation flag, speed limit) that doesn't fit the generic value/unit pattern. This follows the same design pattern established with the `train` and `barrier` tables in Sprint 2.
- **Violation tracking**: The speed camera API stores the speed limit at the time of measurement alongside the actual speed. This means the violation flag is computed on the ESP32 (where the threshold is configured) and stored as-is, keeping the backend stateless and the ESP32 as the source of truth for speed limit configuration.
- **Statistics endpoint**: Added a `/stats` endpoint that computes aggregates (total readings, violations, average/max speed, violation rate) server-side using SQL aggregation functions, avoiding the need to transfer all readings to the client for dashboard statistics.

Deliverables: updated `docker-compose.yml` (port 80), dashboard on root, `speed_readings` model, speed camera router with 5 endpoints, Pydantic schemas

### Realise - Raspberry Pi production deployment and speed camera API implementation

I implemented the full production infrastructure and speed camera feature:

- **Raspberry Pi deployment**: The backend is now running on the HvA campus Pi at port 80. Docker containers auto-restart on reboot. SSH is configured for remote access, enabling deployment and debugging from any machine on the network.
- **Database expansion**: Added the `speed_readings` table (5th table) to the PostgreSQL schema, bringing the total data model to: `sensor_readings`, `parking_spots`, `train`, `barrier`, `speed_readings`.
- **Speed camera router**: Implemented 5 REST endpoints following the established patterns:
  - `POST /api/v1/speedcamera/` — Store a new speed measurement from the ESP32
  - `GET /api/v1/speedcamera/` — Get recent readings (with limit parameter)
  - `GET /api/v1/speedcamera/violations` — Filter to only speed limit violations
  - `GET /api/v1/speedcamera/latest` — Get the most recent reading
  - `GET /api/v1/speedcamera/stats` — Aggregated statistics (total, violations, averages)
- **Dashboard on root**: Moved the dashboard from `/dashboard` to the root URL (`/`). The Pi now serves the City Sim Dashboard as the landing page at `http://145.92.8.137/`. Added a 5th tile panel for the speed camera with real-time statistics (total readings, violations, average speed, max speed) and a scrollable list of recent measurements with violation highlighting. The panel uses orange accent color to differentiate from Gurpreet's streetlight panel (amber).
- **Speed camera photo system**: Analyzed and documented Gurpreet's dual-ESP32 photo capture system. The ESP32-CAM operates independently as a WiFi AP, captures JPEG photos with flash on violation trigger, and serves them via HTTP. This system requires no backend changes — photos are stored locally on the CAM and can be viewed at `192.168.4.1/photo` when connected to the camera's AP network.
- **API integration total**: The backend now serves 5 router groups with 20+ REST endpoints, supporting all 5 team members' tiles.

Deliverables: production Pi running on port 80, speed camera router (`speed_camera.py`), updated dashboard, expanded database schema

### Advise - Cross-tile integration and ESP32 firmware guidance

I analyzed the speed camera hardware implementation to provide integration advice:

- **Gurpreet's speed camera setup**: His ESP32-S3 uses two IR sensors (GPIO 6 and 12) spaced 10cm apart to measure vehicle speed via time-of-flight. Speed is calculated as `(distance / time) * 3.6` to convert m/s to km/h. The current speed limit is 1.0 km/h (appropriate for the miniature scale). A separate ESP32-CAM captures photos of violators.
- **Backend integration path**: Gurpreet's current code outputs speed data to Serial and OLED display but doesn't POST to the backend yet. I created the API endpoint to match his data format exactly: `speed_kmh`, `direction` ("1->2" or "2->1"), `is_violation` flag, and `speed_limit_kmh`. He only needs to add an HTTP POST call in his `update()` loop, similar to how the parking sensor sends data.
- **Multi-ESP32 architecture**: The speed camera uses two ESP32 boards (one for speed detection, one for photo capture) communicating over a local WiFi AP. The ESP32-CAM runs as an access point (SSID: `ESP32CAM_CAPTURE`) and exposes `/capture` to trigger a photo and `/photo` to retrieve the JPEG. When a violation is detected, the speed detection board sends an HTTP GET to `192.168.4.1/capture`, the CAM flashes the LED and stores the frame in memory. The photos stay on the ESP32-CAM and are not sent to the backend — the photo system is self-contained. Only the speed detection board needs to POST measurement data to the backend.
- **API adoption status**: Parking (Matin) and Railroad Crossing (Thijmen) are actively using dedicated endpoints. Streetlight (Gurpreet) and Traffic Light (Wesley) still use the generic readings endpoint. Speed camera will be the third tile with a dedicated API. The generic endpoint remains available as a fallback.

Deliverables: integration guide for Gurpreet, ESP32 HTTP POST example, API adoption status overview

### Analysis - Backend growth and production readiness

I analyzed the project's technical evolution from Sprint 1 to Sprint 3:

- **Schema growth**: From 2 tables (Sprint 1) to 4 tables (Sprint 2) to 5 tables (Sprint 3). Each new dedicated table was added when a tile's data model exceeded the generic sensor_readings pattern. This validates the original architecture: start generic, specialize when needed.
- **Endpoint growth**: From 7 endpoints (Sprint 1) to 15+ (Sprint 2) to 20+ (Sprint 3). The router-based architecture scales well because each tile's API is isolated in its own file. Adding the speed camera router required only 3 touchpoints: model, router file, and main.py registration.
- **Production stability**: The Pi deployment has been running continuously since Sprint 2 with automatic container restarts. Moving to port 80 required no code changes beyond Docker configuration, validating the `window.location.origin` pattern in the dashboard.
- **Dashboard scalability**: The 2x2 grid grew to a 2x3 grid (5 panels) without any layout issues. Each panel independently fetches its data, so performance scales linearly. The dashboard now provides a complete overview of all active tiles in the city simulation.
- **Team velocity**: 3 out of 5 tiles now have dedicated backend endpoints (parking, railroad crossing, speed camera). The remaining 2 tiles (streetlight, traffic light) work via the generic endpoint. Full tile integration is achievable by Sprint 4.

Deliverables: architecture evolution analysis, production metrics, integration roadmap

---

## Learning Journal

### Manage & Control - STARR Reflection

**Situatie:** In Sprint 3 moest de Raspberry Pi productie-klaar worden gemaakt op port 80 en moest ik een nieuwe API bouwen voor Gurpreet's speed camera tile. Tegelijkertijd merkte ik dat ik achterliep met Portflow evidence documentatie.

**Taak:** De Pi deployment optimaliseren (port 80, SSH), een complete speed camera API ontwerpen en implementeren met dedicated database tabel, het dashboard uitbreiden met een 5e tile panel, en alle Sprint 3 evidence documenteren.

**Actie:** Ik heb eerst de Pi configuratie aangepast zodat de API op port 80 draait en SSH remote access werkt. Toen heb ik Gurpreet's embedded code bestudeerd om te begrijpen welke data zijn speed camera produceert (snelheid, richting, overtreding, limiet). Op basis daarvan heb ik een SQLAlchemy model, Pydantic schemas, en een FastAPI router ontworpen met 5 endpoints. Ik heb het dashboard uitgebreid met een speed camera panel dat realtime statistieken en een lijst van metingen toont. Tot slot heb ik alle evidence gedocumenteerd in dezelfde structuur als Sprint 1 en 2.

**Resultaat:** De backend draait stabiel op de Pi met 5 router groepen en 20+ endpoints. Het dashboard toont nu alle 5 tiles. De speed camera API is klaar voor Gurpreet om aan te sluiten vanuit zijn ESP32 firmware. Alle Sprint 3 evidence is gedocumenteerd.

**Reflectie:** Het bestuderen van Gurpreet's embedded code voordat ik de API bouwde heeft geholpen om de data structuur precies af te stemmen op wat zijn ESP32 produceert. Dit is beter dan een generieke API bouwen en achteraf aanpassen. Ik merk ook dat het documenteren van evidence direct na het werk veel efficienter is dan het achteraf reconstrueren. Voor Sprint 4 wil ik een workflow opzetten waarin ik na elke sessie kort mijn voortgang documenteer, zodat de Portflow evidence up-to-date blijft.
