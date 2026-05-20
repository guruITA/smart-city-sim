# Mayor Delivery - Sprint 3

## My Challenges

Built the speed camera API for Gurpreet's tile: 5 endpoints with a dedicated table for speed readings and violation tracking. Deployed the backend on port 80 so ESP32s connect without needing a port number. Updated the dashboard to 5 tile panels. Reframed my problem statement after Mats' feedback, connecting the backend work to the parking search traffic problem and Amsterdam's 2030 emission goals.

## The City

Backend now serves all 5 tiles: parking, railroad crossing, speed camera, streetlight, and traffic light. Dashboard at port 80 shows live data from every tile. 20+ REST endpoints across 7 routers and 6 database tables. The dual-table pattern (generic readings + dedicated state) is proven 3 times: parking, railroad, speed camera.

## Agreements Next Period

Cluster the backend so if it crashes it auto-fixes within 5 seconds. Add 1 surprise feature in the backend.

## Learning Group Feedback

Mats said: knallen (go all out this sprint).

## Personal Feedback

Cluster the backend for reliability. Add something unexpected that shows creativity in the backend.

## Reflection

**What went well:** Speed camera API built quickly, port 80 deployment makes integration simple, problem statement finally in order after Mats' feedback.

**What can be improved:** Should have started with problem framing in sprint 1. Wesley and Betul tiles still not fully integrated.

**What I take to the next sprint:** Cluster the backend for reliability and add a creative surprise. All tiles need to be running.
