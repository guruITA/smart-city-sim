# Analysis — Cross-tile integration and backend architecture growth

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | May 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group — HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Table of Contents

1. [Introduction](#introduction)
2. [Chapter 1 — Backend architecture growth](#chapter-1--backend-architecture-growth)
3. [Chapter 2 — Refined research questions after assessor feedback](#chapter-2--refined-research-questions-after-assessor-feedback)
4. [Chapter 3 — Stakeholder mapping](#chapter-3--stakeholder-mapping)
5. [Conclusion](#conclusion)
6. [Recommendation](#recommendation)
7. [References](#references)
8. [Appendix](#appendix)

---

## Introduction

This document is the analysis deliverable for Sprint 3, Learning Goal 1. It examines how the shared City Sim backend grew from Sprint 1 to Sprint 3 and what adding the speed camera tile means for the architecture.

This analysis builds on the 6W problem analysis from Sprint 1 (see portflowEvidenceSprint1.md). The core problem remains: parking search traffic in Amsterdam wastes fuel and produces CO2. Drivers need real-time data to find free spots without circling the block. Sprint 3 extends this with a second use case: speed enforcement in residential areas.

### Target audience

This document is written for the City Sim assessors (Mats Otten, Gerald Stap) and team members who need to understand how the backend architecture scales as more tiles are added.

### Main question

How does adding a third dedicated API (speed camera) affect the scalability and architecture of the shared City Sim backend?

### Sub-questions

1. How has the backend architecture grown from Sprint 1 to Sprint 3, and does the original design pattern still hold?
2. Which research questions from the Sprint 1 problem analysis are addressed by the Sprint 3 work?
3. Who are the stakeholders of the smart city platform, and what do they need from the system?

---

## Chapter 1 — Backend architecture growth

### Context

By the end of Sprint 2, the backend had 4 tables:

- `sensor_readings` — generic table for all tiles (Sprint 1)
- `parking_spots` — real-time parking state (Sprint 1)
- `train` — train detection with temporal sensor data (Sprint 2, Thijmen's code)
- `barrier` — barrier control with input mode validation (Sprint 2, Thijmen's code)

The API served 15+ endpoints across 4 router groups. The dashboard showed 4 tile panels. The architecture had proven itself for two dedicated tiles (parking and railroad crossing) plus the generic endpoint for simple sensor values.

The question for Sprint 3 was: does the pattern hold for a third dedicated tile?

### Method

I compared the backend state across all three sprints by counting tables, endpoints, router groups, and dedicated tiles. I also tracked what it took to add the speed camera: how many files changed, whether existing code was affected, and whether the dashboard needed structural changes.

### Findings

The backend grew like this:

| Sprint | Tables | Endpoints | Router groups | Dedicated tiles |
|--------|--------|-----------|---------------|-----------------|
| 1 | 2 | 7 | 2 | 1 (parking) |
| 2 | 4 | 15+ | 4 | 2 (parking, railroad) |
| 3 | 5 | 20+ | 5 | 3 (parking, railroad, speed camera) |

Each new dedicated table was added when a tile's data did not fit the generic `{tile, sensor_type, value, unit}` pattern. The speed camera needs `speed_kmh`, `direction`, `is_violation`, `speed_limit_kmh`. That does not map to a single float value.

Adding the speed camera required only 3 touchpoints:

1. New model in `models.py`
2. New router file `speed_camera.py`
3. Registration line in `main.py`

No existing code was touched. No existing endpoints changed behavior. The dashboard added a 5th panel without layout issues (2x3 grid now). 3 out of 5 tiles now have dedicated endpoints. The remaining 2 (streetlight, traffic light) still use the generic endpoint.

### Sub-conclusion

The original architecture pattern holds. The dual-table approach (generic for simple data, dedicated for structured data) scales to at least 3 dedicated tiles without requiring changes to existing code. The router-based separation means each tile's API is isolated. Adding a new tile is a 3-file change, not a rewrite.

---

## Chapter 2 — Refined research questions after assessor feedback

### Context

In the Sprint 2 Progress Review, Mats gave me 1/4 on all learning goals. His feedback: "Choose a human-centered problem from the city and describe what you are going to do to address it." My Sprint 1 analysis focused on the technical "how" but not the human "why does this matter to residents."

### Method

I revisited my Sprint 1 sub-questions and checked whether they address the human side of the problem. Where they did not, I added new questions. I then checked which sub-questions Sprint 3 work actually answers.

### Sprint 1 sub-questions (original)

1. How much urban traffic consists of parking search, and what is the environmental impact?
2. What sensor data is needed to determine parking spot occupancy in real time?
3. How should parking occupancy data be collected, stored, and served to make it accessible to drivers?
4. What infrastructure is needed to run a reliable real-time parking detection system?

These are valid technical questions. But they skip the human perspective.

### New questions added in Sprint 3

- **Who benefits from real-time parking data, and how would they access it?** Residents who experience noise and pollution benefit indirectly. Drivers benefit directly. A dashboard works for the PoC, but a phone notification would be more practical in real life.
- **Does adding more sensor types to the same backend strengthen the case for a city-wide data platform?** If only parking uses the backend, it is a parking tool. If parking and speed camera both use it, the backend becomes a multi-purpose platform.

### Sub-questions addressed this sprint

**Sub-question 3: How should data be collected, stored, and served?**

Extended with the speed camera API. The answer is now more nuanced. For simple data (light level, temperature), the generic table is enough. For structured data (speed + direction + violation + limit), a dedicated table works better. The decision point is: does the tile's data have more than one meaningful field? If yes, dedicated table. If no, generic table.

**Sub-question 4: What infrastructure for reliable detection?**

Answered by the Pi deployment. Raspberry Pi on HvA network with static IP (145.92.8.137), Docker Compose with `restart: always`, port 80, SSH for remote management.

### Sub-conclusion

The Sprint 1 sub-questions were technically correct but missed the human perspective. After Mats' feedback, I added questions about who benefits and whether cross-tile integration strengthens the platform. Sprint 3 work addresses sub-questions 3 and 4 with concrete implementation. Sub-question 1 still relies on secondary sources only.

---

## Chapter 3 — Stakeholder mapping

### Context

A proper analysis should identify who is affected by the problem and who would use the solution. I mapped stakeholders based on secondary sources and project interaction. I have not done primary research (interviews, surveys).

### Method

I listed all groups that interact with or are affected by the parking search traffic problem and the smart city platform. For each group I noted their relationship to the system and what they would need from it.

### Findings

- **Drivers:** Primary users of parking data. They save time and fuel by checking spot availability before driving. For the speed camera: they get warned about speeding. Both systems serve the goal of safer, calmer neighborhoods.
- **Residents:** Indirect beneficiaries. Less circling traffic means less noise and pollution. Fewer speeders means safer streets for pedestrians and children.
- **Municipality of Amsterdam:** Saves on traffic management. Could use the data for parking policy or speed enforcement decisions. Amsterdam targets emission-free inner city traffic by 2030.
- **Team members:** Use the shared backend for their tiles. They benefit from a stable API and clear documentation.
- **Assessors (Mats, Gerald):** Need to see the city problem connection in my evidence. They assess whether I can frame technical work inside a human-centered context.

### Limitations

- I did not interview any stakeholders. Driver and resident perspectives come from Shoup (2017) and Amsterdam municipality publications.
- The proof of concept uses 3 miniature parking spots and a toy car speed camera. Scaling conclusions are speculative.
- No user testing of the dashboard with actual drivers.
- Stakeholder needs are assumed, not validated.
- The speed camera analysis focuses on backend integration, not on the societal impact of speed enforcement.

### Sub-conclusion

Five stakeholder groups are identified. The most important gap is that all stakeholder needs are assumed from secondary sources. No primary research has been conducted. For Sprint 4, I plan at least one informal interview with a student who drives to HvA about their parking experience.

---

## Conclusion

### Sub-conclusion 1 — Architecture growth

The backend grew from 2 tables and 7 endpoints in Sprint 1 to 5 tables and 20+ endpoints in Sprint 3. The original design pattern (generic table for simple data, dedicated tables for structured data, router separation per tile) scales without requiring changes to existing code. Adding the speed camera was a 3-file change.

### Sub-conclusion 2 — Research questions

The Sprint 1 sub-questions were technically focused. After Mats' feedback, I added human-centered questions about who benefits and whether cross-tile data strengthens the platform. Sprint 3 addresses sub-questions 3 and 4. Sub-question 1 still needs primary research.

### Sub-conclusion 3 — Stakeholders

Five stakeholder groups are identified but none have been interviewed. All needs are assumed from secondary sources. This is the biggest gap in the analysis.

### Answer to the main question

Adding the speed camera as a third dedicated API confirms that the shared backend architecture scales. The router-based design isolates each tile's API. The dual-table pattern handles both simple and structured sensor data. The backend is no longer a single-purpose parking tool — it is a multi-sensor city data platform that can grow as more tiles are added. The main weakness is not technical but methodological: the analysis lacks primary stakeholder research.

---

## Recommendation

1. Conduct at least one stakeholder interview in Sprint 4 to validate assumed needs.
2. If Wesley's traffic light or Betul's pedestrian crossing need structured data, apply the same dedicated-table pattern.
3. Add input validation (check constraints) to the speed camera table, following the pattern already used in the barrier table.
4. Update the architecture diagram (SVG) to reflect the current 5-table, 5-router state.

---

## References

- Shoup, D. (2017). *The High Cost of Free Parking*. Routledge.
- Gemeente Amsterdam. (2019). *Agenda Amsterdam Autoluw.*
- FastAPI documentation. https://fastapi.tiangolo.com/
- SQLAlchemy 2.0 documentation. https://docs.sqlalchemy.org/

---

## Appendix

### A. Backend growth table

| Sprint | Tables | Endpoints | Router groups | Dedicated tiles |
|--------|--------|-----------|---------------|-----------------|
| 1 | 2 | 7 | 2 | 1 (parking) |
| 2 | 4 | 15+ | 4 | 2 (parking, railroad) |
| 3 | 5 | 20+ | 5 | 3 (parking, railroad, speed camera) |

### B. Files changed to add speed camera

1. `backend/app/models.py` — added SpeedReading model
2. `backend/app/routers/speed_camera.py` — new router with 5 endpoints
3. `backend/app/main.py` — registered speed camera router
