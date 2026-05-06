# Challenges Overview — Matin Khajehfard

## Sprint 1 & 2 — Parking Tile & Shared Backend

### 1. Designing a shared backend for 5 different tiles

**Challenge:** Each team member builds a different tile (parking, streetlight, traffic light, railroad crossing, pedestrian crossing) with different sensors and data formats. I needed to design one backend that accepts data from all of them without requiring changes every time a new tile is added.

**Solution:** I created a generic `sensor_readings` table with a `tile` field that identifies which tile sent the data. Any ESP32 can POST to `/api/v1/readings` with its tile name, sensor type, value, and unit. For tiles that need more complex state tracking (like parking occupancy or train prediction), I added dedicated tables and endpoints alongside the generic one.

**Result:** All 4 tiles can send data to the same API. Thijmen's railroad crossing was the first to need dedicated endpoints (train detection with multi-step state), which validated the flexible architecture.

---

### 2. Merging 5 branches with conflicting code

**Challenge:** All team members work on separate branches and push to `main` independently. When I tried to pull `main` into my `matin` branch, I hit multiple issues at once:
- Windows long filename error (a teammate's markdown filename exceeded 260 characters)
- Untracked local files conflicting with tracked files on `main` (teammate files I had locally but never committed)
- 5 file-level merge conflicts in shared backend files (`main.py`, `models.py`, `schemas.py`, `index.html`, `.gitignore`)
- A stash conflict on top of the merge conflicts

**Solution:** I resolved each issue systematically:
1. Enabled `git config core.longpaths true` for Windows path length support
2. Used `git clean` to remove conflicting untracked files (they would be restored from `main`)
3. Resolved each merge conflict by keeping both sides — my parking code and Thijmen's railroad crossing code
4. Reset to the clean merge commit and dropped the stash after conflicts cascaded

**Result:** Successfully merged 233 commits from `main` into my branch with all team members' code integrated.

---

### 3. Moving from development to production deployment

**Challenge:** The backend was running on my laptop using Docker Compose with development settings (hot-reload, hardcoded credentials, volume-mounted source code). This needed to be production-ready for the Raspberry Pi on the HvA network, where it must:
- Survive reboots without manual intervention
- Be accessible from all ESP32 devices and browsers on the network
- Not expose credentials in version control

**Solution:**
- Removed `--reload` flag from uvicorn (saves CPU on Pi's ARM processor)
- Moved database credentials from `docker-compose.yml` to a `.env` file and added `.env` to `.gitignore`
- Changed restart policy from `unless-stopped` to `always` for automatic recovery after power loss
- Removed the dev volume mount so the container uses the built image instead of live source files
- Changed the dashboard from hardcoded `localhost:8000` to `window.location.origin` so it auto-detects the correct IP address
- Created a `deploy.sh` script that sets up the environment, builds containers, and verifies the health check

**Result:** One-command deployment ready. Run `bash deploy.sh` on the Pi and the API, database, and dashboard are all up and accessible.

---

### 4. Building a real-time multi-tile dashboard

**Challenge:** I needed a single dashboard page that shows live status for all 4 tiles (parking, streetlight, traffic light, railroad crossing), each with different visualization needs. The parking tile shows spot occupancy, the streetlight shows light levels, the traffic light shows signal phases, and the railroad crossing shows an animated train approach timeline.

**Solution:** Built a responsive 2x2 grid dashboard in plain HTML/CSS/JavaScript (no framework — keeps it lightweight for the Pi). Each tile panel independently fetches its data every 2 seconds, so a slow or offline tile doesn't block the others. After merging Thijmen's code, the railroad crossing panel gained an animated approach line showing real-time train position between sensors A, B, and C with predicted arrival time.

**Result:** A live dashboard accessible from any browser on the network at `/dashboard`, showing all tiles with a connection status indicator.

---

### 5. Identifying hardware conflicts across tiles

**Challenge:** While analyzing all team members' code for integration, I discovered that Gurpreet (streetlight) uses GPIO pins 4 and 5 for analog sensor input (LDR), while Wesley (traffic light) uses the exact same GPIO 4 and 5 pins for I2C communication (MCP23017 I/O expander). If both tiles run on the same ESP32 or adjacent boards sharing a bus, this creates a hardware conflict.

**Solution:** Documented the conflict and recommended resolving it before physical integration on the city model. Either one team member needs to remap their pins, or the tiles need completely separate ESP32 boards with no shared I2C bus.

**Result:** Proactive identification of a potential integration blocker before it caused debugging headaches during the physical build.

---

### 6. ESP32 to backend communication

**Challenge:** The ESP32-S3 needs to send ultrasonic sensor readings to the backend API over WiFi using HTTP POST requests, with reliable connectivity handling and local feedback (LEDs) even when the network is down.

**Solution:** Wrote an Arduino sketch that:
- Measures distance with the HC-SR04 sensor using pulse timing
- Immediately updates local LEDs (green = free, red = occupied) for instant feedback
- Sends HTTP POST to the backend at a configurable interval (2 seconds)
- Handles WiFi disconnection gracefully (continues local operation, retries on reconnect)
- Uses a configurable distance threshold (10 cm) to determine occupancy

**Result:** The parking sensor works independently and reports to the backend when connected. The API URL is configured as a constant, ready to be updated to the Pi's IP address.

---

## Technical skills demonstrated

| Skill | Context |
|-------|---------|
| REST API design | Generic multi-tile API with FastAPI |
| Database modeling | SQLAlchemy ORM, multi-table schema with relationships |
| Containerization | Docker Compose with health checks, restart policies, env separation |
| Git workflow | Branch management, merge conflict resolution, conventional commits |
| Embedded C++ | ESP32 sensor reading, WiFi HTTP client, non-blocking timing |
| Frontend | Responsive dashboard with live data polling, CSS grid, vanilla JS |
| DevOps | Production hardening, deploy scripts, environment configuration |
| Team integration | Code review, architecture advice, conflict identification |
