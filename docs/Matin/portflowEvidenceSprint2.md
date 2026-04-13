# Portflow Evidence - Sprint 2

## City Sim Learning Group

### Design - Production deployment architecture for Raspberry Pi

In Sprint 2, I redesigned the backend deployment to move from a local laptop to a shared Raspberry Pi on the HvA network. This addresses the Sprint 1 limitation where all ESP32 devices depended on my laptop being online.

Key design decisions:

- **Environment separation**: Moved hardcoded database credentials from `docker-compose.yml` into a `.env` file, added `.env` to `.gitignore`. This follows the 12-factor app methodology and prevents secrets from leaking into version control.
- **Production Dockerfile**: Removed `--reload` flag from uvicorn (hot-reload is dev-only and wastes CPU on the Pi's limited ARM resources) and removed the dev volume mount that bypassed the Docker build.
- **Restart policy**: Changed from `unless-stopped` to `restart: always` so Docker automatically restarts containers after a Pi reboot, meeting the acceptance criteria for reboot survival.
- **Dynamic dashboard URL**: Changed the dashboard's API endpoint from hardcoded `http://localhost:8000` to `window.location.origin`, so the dashboard works on any IP address without code changes. This means team members can access `http://<pi-ip>:8000/dashboard` from any browser on the HvA network.
- **Deploy script**: Created `deploy.sh` that handles `.env` setup, builds containers, and waits for the health check before printing the access URLs.

Deliverables: updated `docker-compose.yml`, `Dockerfile`, `deploy.sh`, `.env` template, dashboard fix in `index.html`

### Realise - Backend hardening and team code integration

I implemented all production deployment changes and integrated Thijmen's railroad crossing tile into the shared backend:

- **Merge resolution**: Merged `main` into `matin` branch, resolving 5 merge conflicts across `main.py`, `models.py`, `schemas.py`, `index.html`, and `.gitignore`. The merge brought in Thijmen's railroad crossing API (train detection + barrier control) alongside my parking endpoints.
- **Git workflow challenges**: The merge was complicated by Windows long filename issues (`core.longpaths`), untracked teammate files conflicting with tracked files on main, and stash conflicts. I resolved each issue systematically: enabled long paths, cleaned untracked files with `git clean`, resolved merge conflicts keeping both sides, and handled stash pop conflicts.
- **New API endpoints integrated**: The backend now serves 4 router groups — generic readings, parking, railroad crossing train, and railroad crossing barrier — totaling 15+ REST endpoints.
- **Database models expanded**: Added `Train` and `Barrier` SQLAlchemy models with check constraints for input validation (`input_mode IN ('manual', 'train')`).
- **Dashboard update**: The railroad crossing panel now shows real-time train approach visualization with animated progress line between sensors A, B, and C, including predicted arrival time calculation.

Deliverables: merged codebase with all tiles integrated, production-ready Docker setup, working deploy script

### Advise - Team integration analysis and deployment strategy

I analyzed the full project codebase to assess integration readiness for all team members and advise on deployment:

- **Thijmen (Railroad Crossing)**: Has fully working API integration with dedicated endpoints for train detection and barrier control. His Sprint 2 work refactors the standalone sketch into a modular library with 4 components (TrainDetector, SignalController, CommunicationHandler, CrossingController). Ready for Pi deployment.
- **Gurpreet (Streetlight)**: Working prototype with LDR sensor and relay. Sprint 2 adds speed camera functionality and UML design. Sends data via generic readings endpoint. **GPIO conflict identified**: Gurpreet uses GPIO 4/5 for analog input, but Wesley uses the same pins for I2C (MCP23017). Recommended resolving this before physical integration.
- **Wesley (Traffic Light)**: Significant Sprint 2 progress with MCP23017 I/O expander and ULN2803 driver arrays for a four-way crossing. Has comprehensive documentation. Needs backend integration for receiving traffic states.
- **Betül (Pedestrian Crossing)**: Analysis and design documents for smart pedestrian crossing behavior. No hardware prototype yet.

**Deployment advice**: I recommended a phased deployment approach:
1. Deploy backend on Pi with existing parking + railroad crossing endpoints
2. Team members update their ESP32 sketches to use Pi's IP address
3. Add tile-specific endpoints as each team member achieves WiFi connectivity

Deliverables: team status assessment, GPIO conflict report, phased deployment plan

### Analysis - Shared backend scalability and multi-tile architecture

I analyzed how the backend architecture handles the growing number of tiles and identified areas for improvement:

- **Generic vs. specific endpoints**: The `sensor_readings` table successfully handles data from all tiles (parking sonar, streetlight LDR, traffic light state, railroad IR). However, Thijmen's railroad crossing required dedicated tables (`train`, `barrier`) because his use case involves multi-step state tracking (sensor A → sensor B → prediction → crossing). This validates the Sprint 1 design decision to keep the generic table for simple readings while allowing tile-specific tables when needed.
- **Database schema growth**: From 2 tables (Sprint 1) to 4 tables (Sprint 2). The `Train` model introduces temporal relationships (first/second sensor timestamps) and derived data (predicted arrival). The `Barrier` model uses a check constraint for input validation, which is a pattern we should adopt for other models.
- **Dashboard scalability**: The 2x2 grid layout works well for 4 tiles. Each tile panel independently fetches its data, so a slow or offline tile doesn't block others. The railroad crossing panel required a more complex visualization (animated approach line) compared to the simple list-based panels.
- **Deployment readiness**: The Docker Compose setup now properly separates dev and production concerns. The `.env` file pattern allows different credentials per environment (local dev vs Pi vs future cloud deployment).

Deliverables: architecture analysis, schema evolution documentation, production deployment comparison

---

## Learning Journal

### Manage & Control - STARR Reflection

**Situatie:** In Sprint 2 moest de backend van een lokale laptop naar een gedeelde Raspberry Pi op het HvA netwerk verhuizen. Tegelijk moesten de bijdragen van alle teamleden geïntegreerd worden in één werkende codebase.

**Taak:** De Docker setup production-ready maken, merge conflicts oplossen tussen 5 branches, en een deployment strategie opzetten voor de Pi. Daarnaast moest ik de hele codebase analyseren om te adviseren over integratie.

**Actie:** Ik heb eerst de Docker configuratie aangepast voor productie (geen hot-reload, .env file, restart policy). Toen heb ik systematisch de merge van main in mijn branch uitgevoerd, waarbij ik 5 file-level conflicts heb opgelost door beide kanten te behouden. Ik heb een deploy script geschreven dat de hele setup automatiseert op de Pi. Voor de team analyse heb ik alle code en documentatie van elk teamlid doorgelezen om de status te bepalen en een GPIO conflict te identificeren.

**Resultaat:** De backend is volledig klaar voor Pi deployment. Alle 4 tile endpoints zijn geïntegreerd en werkend. Het dashboard detecteert automatisch het juiste IP adres. Er is een one-command deploy script. Ik heb een duidelijk overzicht van elke tile's status en een geïdentificeerd hardware conflict dat opgelost moet worden.

**Reflectie:** Merge conflicts zijn tijdrovend maar leerzaam. Ik heb geleerd dat het belangrijk is om regelmatig main in je branch te mergen om grote conflicten te voorkomen. De overstap van dev naar productie configuratie was een goede oefening in het scheiden van concerns. Een verbetering voor volgende sprints: eerder beginnen met integratie in plaats van alles op het einde te doen. De GPIO conflict ontdekking laat zien waarom een hardware pin allocation plan vanaf het begin nodig is.
