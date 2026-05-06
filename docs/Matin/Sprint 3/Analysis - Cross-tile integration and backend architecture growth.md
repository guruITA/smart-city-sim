# Analysis — Cross-tile integration and backend architecture growth

## Introduction

This is the analysis deliverable for Sprint 3 Learning Goal 1. It examines how the backend architecture grew from Sprint 1 to Sprint 3 and what adding the speed camera means for the shared city data platform.

This document builds on the 6W problem analysis from Sprint 1 (see portflowEvidenceSprint1.md). The core problem remains the same: parking search traffic in Amsterdam wastes fuel and produces CO2. Drivers need real-time data to find free spots without circling the block. Sprint 3 extends this with a second use case: speed enforcement in residential areas.

## Starting point

By the end of Sprint 2, the backend had 4 tables:

- `sensor_readings` — generic table for all tiles (Sprint 1)
- `parking_spots` — real-time parking state (Sprint 1)
- `train` — train detection with temporal sensor data (Sprint 2, Thijmen's code)
- `barrier` — barrier control with input mode validation (Sprint 2, Thijmen's code)

The API served 15+ endpoints across 4 router groups. The dashboard showed 4 tile panels. The system ran in Docker Compose but was not yet deployed on the Pi at port 80 (that was carry-over work).

The architecture had proven itself for two dedicated tiles (parking and railroad crossing) plus the generic endpoint for simple sensor values. The question for Sprint 3 was: does the pattern hold for a third dedicated tile?

## Refined research questions after Mats feedback

In the Sprint 2 Progress Review, Mats pointed out that my analysis focused on the technical "how" but not the human "why does this matter to residents". His exact feedback: "Choose a human-centered problem from the city and describe what you are going to do to address it."

That feedback changed how I approach the analysis. In Sprint 1, my sub-questions were:

1. How much urban traffic consists of parking search?
2. What sensor data is needed for real-time occupancy?
3. How should data be collected, stored, and served?
4. What infrastructure for reliable detection?

These are valid technical questions. But they skip the human side. In Sprint 3 I added:

- **New question:** Who benefits from real-time parking data, and how would they access it? Residents who experience noise and pollution benefit indirectly. Drivers benefit directly. A dashboard works for the PoC, but a phone notification would be more practical in real life.
- **New question:** Does adding more sensor types to the same backend strengthen the case for a city-wide data platform? This is where the speed camera matters. If only parking uses the backend, it is a parking tool. If parking and speed camera both use it, the backend becomes a multi-purpose platform.

## Sub-questions addressed this sprint

**Sub-question 3 (from Sprint 1): How should data be collected, stored, and served?**

Extended with the speed camera API. The answer is now more nuanced than Sprint 1. For simple data (light level, temperature), the generic `sensor_readings` table with `{tile, sensor_type, value, unit}` is enough. For structured data (speed + direction + violation flag + speed limit), a dedicated table works better. The decision point is: does the tile's data have more than one meaningful field? If yes, dedicated table. If no, generic table.

**Sub-question 4 (from Sprint 1): What infrastructure for reliable detection?**

Answered by the Pi deployment. Raspberry Pi on HvA network with static IP (145.92.8.137), Docker Compose with `restart: always`, port 80 mapping, SSH for remote management. The infrastructure question is now settled for this PoC.

## Cross-tile scalability analysis

The backend grew like this:

| Sprint | Tables | Endpoints | Router groups | Dedicated tiles |
|--------|--------|-----------|---------------|-----------------|
| 1 | 2 | 7 | 2 | 1 (parking) |
| 2 | 4 | 15+ | 4 | 2 (parking, railroad) |
| 3 | 5 | 20+ | 5 | 3 (parking, railroad, speed camera) |

Each new dedicated table was added when a tile's data did not fit the generic `{tile, sensor_type, value, unit}` pattern. The speed camera needs `speed_kmh`, `direction`, `is_violation`, `speed_limit_kmh`. That does not map to a single float value.

The router-based architecture scales well. Adding the speed camera required only 3 touchpoints:

1. New model in `models.py`
2. New router file `speed_camera.py`
3. Registration line in `main.py`

No existing code was touched. No existing tests broke. The dashboard added a 5th panel by copy-pasting an existing panel and adjusting the API calls and styling. This confirms that the Sprint 1 architecture decision (router separation, generic + dedicated tables) was correct.

3 out of 5 tiles now have dedicated endpoints. The remaining 2 (streetlight, traffic light) still use the generic endpoint. They could get dedicated endpoints in Sprint 4 if their data becomes more complex.

## Stakeholder mapping

- **Drivers:** Primary users of parking data. They save time and fuel by checking spot availability before driving. For the speed camera: they get fined or warned about speeding. Both systems serve the goal of safer, calmer neighborhoods.
- **Residents:** Indirect beneficiaries. Less circling traffic means less noise and pollution. Fewer speeders means safer streets for pedestrians and children.
- **Municipality of Amsterdam:** Saves on traffic management. Could use the data for parking policy or speed enforcement decisions. Amsterdam targets emission-free inner city traffic by 2030.
- **Team members:** Use the shared backend for their tiles. They benefit from a stable API and clear documentation.
- **Assessors (Mats, Gerald):** Need to see the city problem connection in my evidence. They assess whether I can frame technical work inside a human-centered context.

I did not interview any of these stakeholders. The driver and resident perspectives come from Shoup (2017) and Amsterdam municipality publications. The team member perspective comes from daily project interaction. For Sprint 4 I plan to do at least one informal interview with a student who drives to HvA about their parking experience.

## What counts as a correct analysis for this step

For this sprint, the analysis is correct when:

- The connection to the Sprint 1 6W problem analysis is clear
- New research questions after Mats' feedback are documented
- The cross-tile scalability is shown with concrete data (table growth, endpoint growth)
- Stakeholders are mapped with honest limitations
- The analysis acknowledges what is still missing (primary research, interviews)

## Limitations

- No primary research conducted. All problem data comes from secondary sources (Shoup 2017, Amsterdam municipality publications).
- The proof of concept uses 3 miniature parking spots and a toy car speed camera. Scaling conclusions are speculative.
- No user testing of the dashboard with actual drivers.
- Stakeholder needs are assumed, not validated through interviews.
- The speed camera analysis focuses on backend integration, not on the societal impact of speed enforcement. A proper analysis would include traffic safety statistics for Amsterdam residential areas.

## Sources

- Shoup, D. (2017). *The High Cost of Free Parking*. Routledge.
- Gemeente Amsterdam. (2019). *Agenda Amsterdam Autoluw.*
- FastAPI documentation. https://fastapi.tiangolo.com/
- SQLAlchemy 2.0 documentation. https://docs.sqlalchemy.org/
