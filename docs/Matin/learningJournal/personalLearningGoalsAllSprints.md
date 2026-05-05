# Personal Learning Goals — Learning Group City Sim

## Sprint 1

### Context
Parking search traffic in Amsterdam wastes fuel and frustrates residents. I am building a backend that collects real time parking sensor data and serves it to a dashboard.

### Learning Goals (start of sprint)

1. **Build a working REST API for IoT sensor data using FastAPI and PostgreSQL.**
   - Specific: Create a FastAPI application with at least 5 REST endpoints that accept sensor readings from ESP32 devices and store them in PostgreSQL.
   - Measurable: API responds to POST and GET requests, data persists in the database, Swagger docs are auto-generated.
   - Achievable: I have Python experience but never used FastAPI. The framework has good documentation and a manageable learning curve.
   - Relevant: The parking detection system needs a backend to receive distance measurements and determine spot occupancy.
   - Time-bound: Working API with database by end of Sprint 1 (2 weeks).

2. **Design a database schema that works for multiple sensor types across the smart city.**
   - Specific: Design and implement a generic sensor_readings table that accepts data from any tile (parking, streetlight, traffic light, railroad crossing) plus a dedicated parking_spots table for real time status.
   - Measurable: Schema handles at least 4 different tile types without code changes. Parking status queries return in under 100ms.
   - Achievable: I have SQL experience from previous semesters. The challenge is making it generic enough for all tiles while keeping parking queries fast.
   - Relevant: Our team has 5 different tiles. If each needs a separate API, the backend becomes unmaintainable.
   - Time-bound: Schema designed and implemented by end of week 1, tested with simulated data by end of Sprint 1.

3. **Deploy the full stack in Docker Compose so team members can run it without installing dependencies.**
   - Specific: Create a docker-compose.yml that runs both the FastAPI application and PostgreSQL database with a single command.
   - Measurable: Team member can clone the repo, run docker compose up, and access the API at localhost:8000 within 2 minutes.
   - Achievable: I have basic Docker knowledge. Docker Compose is new but well-documented.
   - Relevant: Team members should not need to install Python or PostgreSQL to test the backend locally.
   - Time-bound: Docker setup working by end of Sprint 1.

### Reflection (end of sprint)

**Goal 1 (REST API):** Achieved. I built a FastAPI application with 7 endpoints covering generic sensor readings and parking-specific operations. The API accepts POST requests from ESP32 devices and stores data in PostgreSQL. Swagger docs work at /docs. What went well: FastAPI's auto-validation with Pydantic caught malformed requests immediately without extra code. What could be better: I did not write automated tests.

**Goal 2 (Database schema):** Achieved. The generic sensor_readings table successfully handles all 4 tile types. The separate parking_spots table keeps status queries fast. What went well: the dual-table approach (generic for history, dedicated for state) turned out to be a pattern I reused in Sprint 2 for the railroad crossing. What could be better: I did not document the schema decision rationale at the time, only retroactively.

**Goal 3 (Docker):** Achieved. docker-compose.yml runs both containers with one command. Health checks ensure the database is ready before the API starts. What went well: the setup is reproducible and I never had "works on my machine" issues. What could be better: I should have shared the Docker setup with team members earlier. Nobody else ran it during Sprint 1.

---

## Sprint 2

### Context
The parking backend from Sprint 1 runs on my laptop. For the smart city to work, it needs to move to a shared Raspberry Pi on the HvA network so all team members' ESP32 devices can reach it.

### Learning Goals (start of sprint)

1. **Move the backend from my laptop to a Raspberry Pi that runs 24/7 on the HvA network.**
   - Specific: Configure Docker Compose for production on the Pi: .env file for secrets, restart policy for reboot survival, no dev hot-reload, dynamic dashboard URL.
   - Measurable: After a Pi reboot, containers restart automatically. Team members can access the API from any device on the HvA network.
   - Achievable: I have the Docker setup from Sprint 1. The Pi is provided by the lab. SSH access and ARM compatibility are the unknowns.
   - Relevant: Drivers cannot check parking availability if the backend only runs when my laptop is open.
   - Time-bound: Pi deployment working by end of Sprint 2.

2. **Integrate all team members' backend code into one working codebase.**
   - Specific: Merge Thijmen's railroad crossing API (dedicated train and barrier endpoints) into my branch and resolve all conflicts. Ensure the dashboard shows all tiles.
   - Measurable: The backend serves at least 15 endpoints across 4 router groups. All existing tests and health checks pass after the merge.
   - Achievable: Thijmen has working code on a separate branch. The main risk is merge conflicts due to 233 commits of divergence.
   - Relevant: A smart city with only one working tile is not a city. The backend must serve all sensors to demonstrate the parking system in context.
   - Time-bound: Merge completed and tested by mid-Sprint 2.

3. **Expose the API on port 80 so ESP32 devices and browsers do not need to specify a port number.**
   - Specific: Change the Docker port mapping from 8000:8000 to 80:8000. Update deploy.sh and dashboard references.
   - Measurable: Opening http://pi-ip/ in a browser shows the dashboard without typing :8000.
   - Achievable: Simple Docker configuration change. No code changes needed inside the application.
   - Relevant: ESP32 HTTP libraries default to port 80. Removing the port from URLs simplifies firmware for all team members.
   - Time-bound: Working on Pi by end of Sprint 2.

### Reflection (end of sprint)

**Goal 1 (Pi deployment):** Partially achieved. I created the deploy.sh script, moved secrets to .env, added restart policy, and fixed the dashboard URL. The Pi was ready for deployment. However, I did not actually deploy and test it on the campus Pi during Sprint 2. The preparation was done but the execution was not. I prioritized my Group Project (Aimee) over City Sim during weeks 3-4 of the sprint.

**Goal 2 (Team integration):** Achieved. I merged Thijmen's 233 commits into my branch, resolved 5 file-level conflicts, and the backend now serves 15+ endpoints across 4 router groups. The dashboard shows all tiles. What went well: systematic conflict resolution by keeping both sides. What could be better: I should have merged earlier in the sprint instead of waiting until the end, which created time pressure.

**Goal 3 (Port 80):** Not achieved. The configuration change itself is trivial but I ran out of time. This carried over to Sprint 3. The root cause is that I underestimated how much time Aimee would take and did not protect City Sim time.

**Honest assessment of Sprint 2:**
I did not write these learning goals at the start of the sprint. I had no problem statement connecting my work to parking search traffic in Amsterdam. When Mats asked for a story at the Mayor Delivery, I showed him technical infrastructure without a city problem to anchor it. His feedback was clear: "ik wil een verhaal horen" and "verras me volgende keer". I did not act on this feedback between Sprint 1 and Sprint 2 deliveries.

I also received a strike for acting solo without sufficient team communication. I was doing real work but not making it visible to my team or coaches.

The technical work (Pi prep, merge, deploy script) was solid. The framing, communication, and learning goal discipline were absent. This directly caused my 1/4 score from Mats. Sprint 3 must fix this by starting with the problem, writing goals upfront, and asking for expert feedback weekly.

---

## Sprint 3

### Context
After scoring 1/4 from Mats in Sprint 2 because my evidence was not connected to a city problem, Sprint 3 is my recovery sprint. I need to anchor all work in the parking search traffic problem, build the speed camera API for cross-tile integration, and establish a weekly feedback loop.

### Learning Goals (start of sprint)

1. **Define a human-centered problem statement and connect all backend work to it.**
   - Specific: Write a problem statement grounding my parking system in real Amsterdam data (search traffic statistics, CO2 impact, 2030 targets). Reframe all existing Portflow evidence to start from this problem instead of "I built this".
   - Measurable: problemStatement.md committed to GitLab. All 12 Portflow evidence items updated with descriptions that reference the city problem. At least 2 expert validations received (Mats or Gerald).
   - Achievable: The technical evidence already exists. The reframing is writing work, not new development.
   - Relevant: Mats scored 1/4 because evidence was not connected to a human problem. This directly addresses his feedback: "Choose a human-centered problem from the city."
   - Time-bound: Problem statement committed by week 10. Portflow cleanup completed before Progress Review week 13.

2. **Build a dedicated speed camera API so the smart city demonstrates multiple sensor types sharing one backend.**
   - Specific: Design a speed_readings table and implement 5 REST endpoints (store, list, violations, latest, stats) for Gurpreet's speed camera tile. Add a 5th dashboard panel showing speed data with violation highlighting.
   - Measurable: API accepts POST requests matching Gurpreet's ESP32 data format (speed_kmh, direction, is_violation, speed_limit_kmh). Stats endpoint returns aggregates using server-side SQL. Dashboard panel shows live data.
   - Achievable: I built dedicated APIs for parking (Sprint 1) and merged railroad crossing (Sprint 2). The pattern is established. Gurpreet's embedded code is already written, I just need to match his data format.
   - Relevant: A smart city with only parking detection is unconvincing. Speed enforcement is a second use case that shares infrastructure and strengthens the story: real time city data reduces both search traffic and speeding.
   - Time-bound: Speed camera API and dashboard panel working by week 11.

3. **Complete the production deployment on port 80 and establish weekly expert feedback.**
   - Specific: Fix the Docker port mapping (host 80 to container 8000), move dashboard to root URL, update deploy.sh. Request feedback from at least one expert per week (Mats, Gerald, or peer).
   - Measurable: Pi serves dashboard at http://145.92.8.137/ without port number. At least 3 expert feedback comments recorded in Portflow before Progress Review.
   - Achievable: Port 80 is a one-line Docker change carried over from Sprint 2. The feedback loop requires scheduling conversations, which is new for me but logistically simple.
   - Relevant: Port 80 removes friction for all team members connecting ESP32 devices. Weekly feedback prevents the isolation that caused my Sprint 2 strike.
   - Time-bound: Port 80 deployed by week 10. First feedback request sent within 3 days of sprint start.

### Reflection (end of sprint)

[End of sprint reflection - to be added before Progress Review week 13]
