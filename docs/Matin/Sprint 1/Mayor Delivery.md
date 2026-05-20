# Mayor Delivery - Sprint 1

## My Challenges

I built the shared backend infrastructure for the city: a FastAPI REST API with PostgreSQL database running in Docker. 7 API endpoints serving all tiles, tested in Swagger UI. Built a dashboard showing 4 tile panels (parking, streetlight, traffic light, railroad crossing). Created the ESP32 parking sensor sketch with HC-SR04 ultrasonic sensor.

## The City

We delivered a working backend with a generic API that any tile can use. The parking tile has a working sensor sketch. The dashboard shows all 4 tiles live. Thijmen built 3 railroad crossing demos, Gurpreet has a working LDR sensor. The API was tested and documented in Swagger. Docker Compose lets any team member run the backend with one command.

## Agreements Next Period

Focus on the backend and deploy it on the Raspberry Pi on the HvA network. Get the ESP32 tiles connected via WiFi to the shared backend.

## Learning Group Feedback

Mats said: keep the MVP simple first, work more as a team than individually, and next time tell a story instead of just showing code.

## Personal Feedback

Focus on backend infrastructure and making sure the shared API works for all tiles.

## Reflection

**What went well:** Backend is up and running, API works for all tiles, Docker makes it easy for the team to get started.

**What can be improved:** Worked too individually, not enough as a team. Only showed code without a story behind it.

**What I take to the next sprint:** Collaborate more as a team and tell a story during the delivery, not just show the technical side.
