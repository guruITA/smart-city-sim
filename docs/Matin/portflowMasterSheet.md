# Portflow Master Sheet — Matin Khajehfard

Kopieer beschrijvingen en permalinks direct uit dit document naar Portflow.
Gemaakt op 4 mei 2026. Alle permalinks zijn commit-hash gebaseerd (permanent).

---

## Sprint 1 — Problem Analysis (Goal: Analysis)
**Versie:** V1
**Loose item merge:** "GitLab repository analyze" (Thijmen comment "Good job!" 8 apr komt automatisch mee)
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint1.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/be96ba28/docs/Matin/portflowEvidenceSprint1.md
- problemStatement.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/dbb1bc8a/docs/Matin/problemStatement.md

**Beschrijving (kopieer in description veld):**
"Parking search traffic wastes fuel, produces CO2, and frustrates residents in Amsterdam neighborhoods. To address this, I first analyzed what data a smart parking system needs: real time distance measurements per spot, a threshold to determine occupancy, and a way to store and serve this data to multiple consumers. I also assessed all team members' code and hardware to understand integration needs. Each tile uses different sensors and data formats, so the backend had to be generic enough to accept any tile's data without changes. I learned that starting with a clear data model before writing code prevents rework later."

**Expert feedback (al gevalideerd):**
- Thijmen Walter: Portflow comment "Good job!" op 8 april 2026 (VALIDATED, komt mee via loose item merge)

**Feedback nog te vragen:**
- Gerald Stap of Mats Otten: retroactieve feedback op Sprint 1 analysis [datum]

**Version note:** "Sprint 1"

---

## Sprint 1 — Problem Solution Design (Goal: Design)
**Versie:** V1
**Loose item merge:** "backend architecture" (Thijmen comment "It's great to see..." 8 apr komt automatisch mee)
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint1.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/be96ba28/docs/Matin/portflowEvidenceSprint1.md
- system-architecture.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/c7c95974/docs/Matin/system-architecture.md
- database-schema.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/database-schema.md
- api-specification.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/api-specification.md
- city_sim_backend_architecture_sprint1.svg: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/0d547672/docs/Matin/city_sim_backend_architecture_sprint1.svg

**Beschrijving (kopieer in description veld):**
"To reduce parking search traffic, drivers need real time information about which spots are free. I designed a three layer architecture for this: ESP32 sensors measure distance per spot, a FastAPI backend stores readings in PostgreSQL, and a web dashboard shows live status. I chose a generic sensor_readings table so all tiles in the city sim can use the same API, plus a dedicated parking_spots table for fast status queries. The architecture diagram, database schema, and full API specification are documented in GitLab. I learned that separating generic storage from tile specific state keeps the system flexible without sacrificing query speed."

**Expert feedback (al gevalideerd):**
- Thijmen Walter: Portflow comment "It's great to see..." op 8 april 2026 (VALIDATED, komt mee via loose item merge)

**Feedback nog te vragen:**
- Gerald Stap: validatie op architecture + schema docs [datum]

**Version note:** "Sprint 1"

---

## Sprint 1 — Prototype (Goal: Realise)
**Versie:** V1
**Loose item merge:** "Backend setup" (Thijmen comment "Nicely done..." 8 apr komt automatisch mee)
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint1.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/be96ba28/docs/Matin/portflowEvidenceSprint1.md
- main.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/main.py
- models.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/models.py
- schemas.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/schemas.py
- readings.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/c7c95974/backend/app/routers/readings.py
- parking.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/c7c95974/backend/app/routers/parking.py
- docker-compose.yml: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/42d99bd6/backend/docker-compose.yml
- parking.ino: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/c7c95974/embedded/matin/parking/parking.ino
- index.html: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/static/index.html

**Beschrijving (kopieer in description veld):**
"I built a working proof of concept that detects parking spot occupancy and shows it on a live dashboard. The ESP32 measures distance with an HC-SR04 ultrasonic sensor every 2 seconds and sends the reading to the backend via WiFi HTTP POST. The FastAPI backend stores each reading in PostgreSQL and determines if the spot is occupied (distance below 10 cm). A web dashboard polls the API and displays all spots with green (free) or red (occupied) indicators. The full stack runs in Docker Compose. I learned that getting a working end to end prototype early is more useful than perfecting individual components."

**Expert feedback (al gevalideerd):**
- Thijmen Walter: Portflow comment "Nicely done..." op 8 april 2026 (VALIDATED, komt mee via loose item merge)

**Feedback nog te vragen:**
- Gurpreet Singh: peer feedback op parking API [datum]

**Version note:** "Sprint 1"

---

## Sprint 1 — Show & Tell (Goal: Advise)
**Versie:** V1
**Loose item merge:** "shared backend" (Thijmen comment "Everything is well documented..." 8 apr komt automatisch mee)
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint1.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/be96ba28/docs/Matin/portflowEvidenceSprint1.md
- api-specification.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/api-specification.md

**Beschrijving (kopieer in description veld):**
"I designed the backend API so all team members can use it, not just my parking tile. The generic POST /api/v1/readings endpoint accepts any tile's data with a JSON body containing tile name, sensor type, value, and unit. I documented the exact format per tile in the API specification, so Gurpreet (streetlight), Thijmen (railroad crossing), and Wesley (traffic light) know exactly how to connect. I presented this at the Sprint 1 Mayor Delivery to Mats Otten, where he told us to keep the MVP simple, work more as a team than individually, and tell a story next time. His personal feedback to me was to focus on the backend and the Raspberry Pi and test with real data. I learned that documentation and a working API are a good start, but I need to frame my work as a story about the city problem, not just a technical demo."

**Expert feedback (al gevalideerd):**
- Thijmen Walter: Portflow comment "Everything is well documented..." op 8 april 2026 (VALIDATED, komt mee via loose item merge)
- Mats Otten (Mayor): Mayor Delivery Sprint 1. Group: "MVP eerst simple houden, meer als team dan individueel, een verhaal vertellen volgende keer." Personal: "Focus vooral op backend en de Raspberry Pi en test de backend met echte data." (mondeling ontvangen)

**Feedback nog te vragen:**
- geen, beide experts hebben al feedback gegeven

**Version note:** "Sprint 1"

---

## Sprint 2 — Problem Analysis (Goal: Analysis)
**Versie:** V2
**Loose item merge:** nieuw evidence item (geen bestaand loose item)
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint2.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/ea983320/docs/Matin/portflowEvidenceSprint2.md
- challenges-overview.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/a4411750/docs/Matin/challenges-overview.md

**Beschrijving (kopieer in description veld):**
"A smart parking system only works if all tiles in the city can share data through one backend. In Sprint 2, I analyzed the full codebase to assess integration readiness for each team member's tile. I found that Thijmen's railroad crossing needed dedicated database tables because multi step train detection does not fit the generic sensor_readings pattern. I also identified a GPIO pin conflict between Gurpreet (pins 4/5 for LDR) and Wesley (same pins for I2C), which would block physical integration. What was not done: I did not connect this analysis to the parking search problem explicitly, and I had no learning goals written down. I learned that analyzing teammates' code before integration saves debugging time, but without a problem framing the analysis looks like tech work without purpose."

**Expert feedback (al gevalideerd):**
- Thijmen Walter: Portflow feedback op 8 april 2026 met sub-goal feedback (VALIDATED)

**Feedback nog te vragen:**
- geen extra nodig, Thijmen feedback is voldoende voor V2

**Version note:** "Sprint 2"

---

## Sprint 2 — Problem Solution Design (Goal: Design)
**Versie:** V2
**Loose item merge:** "System Architecture design backend sprint 2" (23 april, Add link)
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint2.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/ea983320/docs/Matin/portflowEvidenceSprint2.md
- system-architecture.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/c7c95974/docs/Matin/system-architecture.md
- database-schema.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/database-schema.md
- raspberry-pi-deployment.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/raspberry-pi-deployment.md
- docker-compose.yml: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/42d99bd6/backend/docker-compose.yml
- deploy.sh: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/42d99bd6/backend/deploy.sh

**Beschrijving (kopieer in description veld):**
"For the parking system to be useful beyond my laptop, the backend had to move to a shared server that all ESP32 devices and browsers can reach on the HvA network. I redesigned the deployment for a Raspberry Pi: moved credentials from docker-compose.yml into a .env file, removed the dev hot reload flag to save CPU on the Pi's ARM processor, set restart policy to always so containers survive reboots, and changed the dashboard URL from hardcoded localhost to window.location.origin so it works on any IP. I also designed the database schema expansion to include train and barrier tables for Thijmen's railroad crossing, following the same pattern as the parking tables. What was not done: I did not document why this deployment change matters for the parking search problem specifically. I learned that separating dev and production configuration early prevents surprises when you deploy on constrained hardware."

**Expert feedback (al gevalideerd):**
- Thijmen Walter: Portflow feedback op 8 april 2026, sub-goal feedback for Design (VALIDATED)

**Feedback nog te vragen:**
- geen extra nodig

**Version note:** "Sprint 2"

---

## Sprint 2 — Prototype (Goal: Realise)
**Versie:** V2
**Loose item merge:** nieuw evidence item (geen bestaand loose item)
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint2.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/ea983320/docs/Matin/portflowEvidenceSprint2.md
- main.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/main.py
- models.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/models.py
- railroad_crossing_train.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/901ea014/backend/app/routers/railroad_crossing_train.py
- railroad_crossing_barrier.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/2b22937a/backend/app/routers/railroad_crossing_barrier.py
- index.html: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/static/index.html
- deploy.sh: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/42d99bd6/backend/deploy.sh

**Beschrijving (kopieer in description veld):**
"The parking backend needed to grow from a single tile prototype into a shared production system. I merged Thijmen's railroad crossing code into my branch, resolving 5 file level conflicts across main.py, models.py, schemas.py, index.html, and .gitignore. The merge brought in 233 commits and added train detection and barrier control endpoints alongside my parking API. I created a deploy script that automates the full setup on the Raspberry Pi: environment file creation, container build, and health check verification. The backend went from 7 endpoints serving 1 tile to 15+ endpoints serving 2 tiles. What was not done: the Pi was not yet running on port 80, and I did not test the parking sensor on the actual Pi network. I learned that merging large branches is painful but necessary, and that doing it earlier in the sprint would have been less stressful."

**Expert feedback (al gevalideerd):**
- Thijmen Walter: Portflow feedback op 8 april 2026, sub-goal feedback for Realise (VALIDATED)

**Feedback nog te vragen:**
- geen extra nodig

**Version note:** "Sprint 2"

---

## Sprint 2 — Show & Tell (Goal: Advise)
**Versie:** V2
**Loose item merge:** nieuw evidence item (geen bestaand loose item)
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint2.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/ea983320/docs/Matin/portflowEvidenceSprint2.md
- api-specification.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/api-specification.md
- challenges-overview.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/a4411750/docs/Matin/challenges-overview.md

**Beschrijving (kopieer in description veld):**
"To make the smart city work as one system, I analyzed each teammate's integration status and advised on a phased deployment plan: parking and railroad crossing first on the Pi, then streetlight and traffic light once they add WiFi. I reported a GPIO pin conflict between Gurpreet and Wesley that would block physical assembly. At the Sprint 2 Mayor Delivery, Mats told us he still wanted to hear a story, the city must be ready, aim for MVP not perfection, and surprise him next time. His personal feedback was to focus on the backend and make sure the whole city works together. I presented the technical integration plan but did not frame it around the parking search problem or any city problem at all. I learned that Mats' repeated request for a story was not about presentation style. It was about connecting my technical work to a human problem in the city. That is what Sprint 3 addresses with the problem statement and learning goals."

**Expert feedback (al gevalideerd):**
- Thijmen Walter: Portflow feedback op 8 april 2026, sub-goal feedback for Advise (VALIDATED)
- Mats Otten (Mayor): Mayor Delivery Sprint 2. Group: "Ik wil een verhaal horen nogsteeds en de stad moet klaar zijn, MVP geen perfectie, en verras me volgende keer." Personal: "Focus on backend and make sure that the whole city works together." (mondeling ontvangen)

**Feedback nog te vragen:**
- geen, beide experts hebben al feedback gegeven

**Version note:** "Sprint 2"

---

## Sprint 3 — Problem Analysis (Goal: Analysis)
**Versie:** V3
**Loose item merge:** nieuw evidence item
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint3.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/portflowEvidenceSprint3.md
- problemStatement.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/dbb1bc8a/docs/Matin/problemStatement.md
- database-schema.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/database-schema.md

**Beschrijving (kopieer in description veld):**
"In Sprint 1 and 2, I built a technically working parking system but never defined what city problem it solves. Mats' Mayor feedback kept asking for a story, and his Sprint 2 review scored 1/4 because evidence was not connected to a human problem. In Sprint 3, I wrote a problem statement grounded in real Amsterdam data: drivers spend 10 to 20 minutes searching for parking in busy neighborhoods, up to 30% of inner city traffic is cars circling for spots (Shoup, 2017), and Amsterdam targets emission free traffic by 2030. I also analyzed the backend's growth from 2 tables and 7 endpoints in Sprint 1 to 5 tables and 20+ endpoints in Sprint 3, validating the original architecture decision to start generic and add dedicated tables when a tile's data model outgrows the generic pattern. I learned that technical analysis without a problem framing is invisible to assessors, and that defining the problem retroactively is harder but still necessary."

**Expert feedback (al gevalideerd):**
- Mats Otten: Sprint 2 review feedback (24 april): "Choose a human-centered problem from the city and describe what you are going to do to address it." en "Come talk to me ASAP." Problem statement is het directe antwoord op deze feedback.

**Feedback nog te vragen:**
- Mats Otten: 1-on-1 [datum] — validatie op problem statement
- Gerald Stap: [datum] — validatie als Learning Group teacher

**Version note:** "Sprint 3"

---

## Sprint 3 — Problem Solution Design (Goal: Design)
**Versie:** V3
**Loose item merge:** nieuw evidence item
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint3.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/portflowEvidenceSprint3.md
- system-architecture.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/c7c95974/docs/Matin/system-architecture.md
- database-schema.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/database-schema.md
- api-specification.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/api-specification.md
- raspberry-pi-deployment.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/raspberry-pi-deployment.md
- docker-compose.yml: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/42d99bd6/backend/docker-compose.yml
- deploy.sh: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/42d99bd6/backend/deploy.sh

**Beschrijving (kopieer in description veld):**
"Reducing parking search traffic requires real time data from multiple sensor types across the city, not just parking. In Sprint 3, I designed the backend to support Gurpreet's speed camera tile alongside my parking system. The speed camera produces structured data (speed in km/h, direction, violation flag, speed limit) that does not fit the generic sensor_readings table, so I designed a dedicated speed_readings table following the same pattern used for the railroad crossing in Sprint 2. The violation detection stays on the ESP32 (Gurpreet's embedded code), keeping the backend stateless and the sensor as the source of truth. I added a stats endpoint that computes aggregates (total readings, violations, average speed) using SQL functions server side, so the dashboard does not need to transfer all readings to calculate statistics. For production, I chose to map host port 80 to container port 8000 instead of running the container as privileged on port 80, and moved the dashboard to the root URL so opening the Pi's IP in a browser shows the city overview immediately. I learned that designing APIs by studying the embedded code first (what data the ESP32 actually produces) results in a better fit than designing a generic endpoint and hoping it works."

**Expert feedback (al gevalideerd):**
- geen (Sprint 3 Design is nieuw)

**Feedback nog te vragen:**
- Gerald Stap: [datum] — validatie op system-architecture.md en database-schema.md
- Mats Otten: 1-on-1 [datum]

**Version note:** "Sprint 3"

---

## Sprint 3 — Prototype (Goal: Realise)
**Versie:** V3
**Loose item merge:** nieuw evidence item
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint3.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/portflowEvidenceSprint3.md
- speed_camera.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/routers/speed_camera.py
- models.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/models.py
- schemas.py: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/schemas.py
- index.html: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/backend/app/static/index.html
- docker-compose.yml: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/42d99bd6/backend/docker-compose.yml
- deploy.sh: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/42d99bd6/backend/deploy.sh
- raspberry-pi-deployment.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/raspberry-pi-deployment.md

**Beschrijving (kopieer in description veld):**
"A smart parking system is more convincing when it runs alongside other city sensors on the same infrastructure. In Sprint 3, I implemented a speed camera API for Gurpreet's tile: 5 REST endpoints covering storage, retrieval, violation filtering, latest reading, and server side statistics. I added the SpeedReading model and three Pydantic schemas to the shared backend. The dashboard now has a 5th panel showing speed camera data in a 2x3 grid, and serves from the root URL instead of a subpath. On the production side, I configured the Pi to expose the API on port 80, updated the deploy script, and documented SSH access and troubleshooting in the deployment guide. The backend grew from 7 endpoints and 2 tables in Sprint 1 to 20+ endpoints and 5 tables now, with 3 out of 5 tiles using dedicated APIs. Gurpreet builds the embedded code for both the parking sensor and the speed camera. I build the backend that receives, stores, and serves the data from all tiles. I learned that building an API by matching it exactly to the data format the ESP32 produces (instead of guessing) means the integration works on the first try."

**Expert feedback (al gevalideerd):**
- geen (Sprint 3 Prototype is nieuw)

**Feedback nog te vragen:**
- Mats Otten: 1-on-1 [datum] — live demo van alle tiles op Pi
- Gurpreet Singh: peer feedback [datum] — werkt zijn embedded code met mijn speed camera API?
- Gerald Stap: [datum]

**Version note:** "Sprint 3"

---

## Sprint 3 — Show & Tell (Goal: Advise)
**Versie:** V3
**Loose item merge:** nieuw evidence item
**Type:** Add link

**Permalinks (kopieer deze in Portflow):**
- portflowEvidenceSprint3.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/portflowEvidenceSprint3.md
- problemStatement.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/dbb1bc8a/docs/Matin/problemStatement.md
- system-architecture.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/c7c95974/docs/Matin/system-architecture.md
- raspberry-pi-deployment.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/raspberry-pi-deployment.md
- api-specification.md: https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3f6b3e59/docs/Matin/api-specification.md

**Beschrijving (kopieer in description veld):**
"In Sprint 1 and 2, Mats asked me to tell a story instead of just showing technology. In Sprint 3, I built that story. At the Mayor Delivery, I present the parking search traffic problem in Amsterdam as the starting point: drivers waste time and fuel circling for spots, producing unnecessary CO2 in a city that targets emission free traffic by 2030. I then show how our smart city addresses this. The Pi runs on port 80 as a production server. The dashboard on the root URL shows 5 tile panels with live data. Three tiles (parking, railroad crossing, speed camera) have dedicated APIs that I built, two more use the generic endpoint. The city works together on shared infrastructure. I also advise Gurpreet on integrating his speed camera ESP32 with my API by documenting the exact JSON format and endpoint his firmware needs to call. This is a working MVP, not a finished product. The problem statement, the shared backend, and the live demo are my answer to Mats' Sprint 2 feedback: a story about why this system matters, not just how it works. I learned that the technical work was always there, but without a problem to anchor it, nobody could see what it was for."

**Expert feedback (al gevalideerd):**
- geen (Mayor Delivery is deze week)

**Feedback nog te vragen:**
- Mats Otten (Mayor): Mayor Delivery Sprint 3 feedback [deze week]
- Mats Otten: 1-on-1 [datum]
- Gerald Stap: [datum]
- Thijmen Walter: peer feedback [datum]

**Version note:** "Sprint 3"

---

## Loose items — acties

| Loose item in Portflow | Actie | Doel |
|------------------------|-------|------|
| GitLab repository analyze | Merge als V1 van Problem Analysis | Sprint 1 |
| backend architecture | Merge als V1 van Problem Solution Design | Sprint 1 |
| Backend setup | Merge als V1 van Prototype | Sprint 1 |
| shared backend | Merge als V1 van Show & Tell | Sprint 1 |
| System Architecture design backend sprint 2 | Merge als V2 van Problem Solution Design | Sprint 2 |
| Learning Journal - Sprint 1 | Verplaats naar Learning Journal collection | n.v.t. |
