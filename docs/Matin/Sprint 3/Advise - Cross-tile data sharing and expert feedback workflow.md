# Advise — Cross-tile data sharing and expert feedback workflow

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
2. [Chapter 1 — Shared backend vs per-tile backends](#chapter-1--shared-backend-vs-per-tile-backends)
3. [Chapter 2 — Speed camera as the second dedicated API](#chapter-2--speed-camera-as-the-second-dedicated-api)
4. [Chapter 3 — Expert feedback workflow](#chapter-3--expert-feedback-workflow)
5. [Conclusion](#conclusion)
6. [Recommendation](#recommendation)
7. [References](#references)
8. [Appendix](#appendix)

---

## Introduction

This document is the advise deliverable for Sprint 3, Learning Goal 1. It contains my recommendations on cross-tile data sharing, why the speed camera was chosen as the second dedicated API, why port 80 matters, and what the expert feedback workflow should look like going forward.

### Target audience

This document is written for team members who make decisions about tile integration, and for assessors evaluating whether I can advise on architectural and process choices.

### Main question

What recommendations improve cross-tile integration and team workflow for the remaining sprints?

### Sub-questions

1. Why is a shared backend better than per-tile backends for a smart city project?
2. Why was the speed camera the right choice for the second dedicated API?
3. How should the expert feedback workflow be structured to make progress visible?

---

## Chapter 1 — Shared backend vs per-tile backends

### Context

With 5 team members building 5 different tiles, there are two approaches: one shared backend that serves all tiles, or separate backends per tile. I chose shared from Sprint 1, but it is worth evaluating whether that choice still makes sense now that 3 tiles have dedicated endpoints.

### Method

I compared the two approaches on integration story, maintenance cost, and demo value. I also identified the main risk of the shared approach.

### Findings

**Shared backend (current approach):**
- One Docker Compose setup, one Pi, one database
- All tile data in one place — cross-tile queries are possible
- One dashboard shows everything
- One team member (me) maintains it
- Risk: if one tile's bad code crashes the API, all tiles go down

**Per-tile backends:**
- Each team member runs their own API
- No single point of failure
- But: 5 separate deployments, 5 databases, no integration story
- The dashboard would need to call 5 different URLs
- The "smart city" story falls apart — it becomes 5 separate demos

The shared approach is better for this project because the whole point is integration. A smart city with data from different sensors in one place tells a stronger story than 5 isolated demos. The risk of one tile crashing everything has not happened in 3 sprints.

### Sub-conclusion

Keep the shared backend. The integration story is more valuable than the isolation benefit of per-tile backends. If stability becomes a problem, add error isolation per router in Sprint 4.

---

## Chapter 2 — Speed camera as the second dedicated API

### Context

Three tiles could have gotten dedicated APIs next: streetlight (LDR), traffic light (state changes), or speed camera (structured speed data). I needed to choose which one to build first.

### Method

I evaluated each option on data complexity, demo value, and narrative fit with the parking problem.

### Findings

**Streetlight (Gurpreet):** LDR sensor sends a single light level value. This fits the generic endpoint perfectly. A dedicated table adds nothing useful. Simple value, simple endpoint.

**Traffic light (Wesley):** Sends state changes. Could benefit from a dedicated table eventually, but Wesley's data format is still evolving. Building a dedicated API now would likely need redesigning later.

**Speed camera (Gurpreet):** Has 4 structured fields (speed, direction, violation, limit). Does not fit the generic endpoint cleanly. Clear data model, clear use case.

I chose speed camera because:

1. **Data complexity.** 4 fields is the strongest case for a dedicated table. The generic endpoint would lose information.
2. **Demo value.** Speed data with violation highlighting makes a more interesting dashboard than a light level number.
3. **Narrative fit.** The smart city detects both where you can park AND whether you are driving safely. Two problems, one platform. Speed enforcement in residential areas addresses the same residents who suffer from parking search traffic. Cars driving too fast are a safety problem for pedestrians and children.

### Port 80

I also recommended exposing the API on port 80. ESP32's HTTPClient library defaults to port 80. When the backend ran on 8000, every team member had to add `:8000` to their URL. Gurpreet had it hardcoded. Thijmen had it in his config. Every time someone forgot the port, they got a timeout and thought the backend was down.

The change is one line in `docker-compose.yml`. The application code does not change.

### Sub-conclusion

Speed camera was the right choice. It had the most complex data, the best demo value, and the strongest narrative connection to the parking problem. Port 80 removes a common source of connection errors for the team.

---

## Chapter 3 — Expert feedback workflow

### Context

After talking to Mats on May 6 and Gerald on May 6, I realized I have a feedback problem. I build things and commit code, but I do not record feedback. To assessors, it looks like I never asked anyone for input.

### Method

I mapped what I was doing, what assessors see, and what I should be doing. Then I created a concrete plan for the remaining weeks.

### Findings

**What I was doing:** Getting verbal feedback in team meetings. Not recording any of it in Portflow.

**What assessors see:** Zero feedback comments. No validations. No expert responses. It looks like I work alone.

**What I should be doing:**

1. Ask for feedback explicitly. Not "what do you think" casually, but a structured request: "I wrote this analysis, can you check if the stakeholder mapping is realistic?"
2. Record the feedback in Portflow. Not in my head.
3. Respond to the feedback. Show that I processed it. If Gerald says the problem statement needs more context, I update the document and add a comment saying what I changed.

**Plan for remaining Sprint 3 weeks:**

| Week | Action | Who |
|------|--------|-----|
| Week 13 | Ask Thijmen to review speed camera router code | Thijmen |
| Week 13 | Ask Gerald to validate problem statement framing | Gerald |
| Week 13 | Ask Mats for 1-on-1 Progress Review prep | Mats |

**Plan for Sprint 4:**

| When | Action |
|------|--------|
| Week 1 | Write learning goals, send to Gerald |
| Week 1 | Record Gerald's response in Portflow |
| Week 2 | First code delivery, ask teammate for review |
| Week 3 | Midpoint check with Mats or Gerald |

### Sub-conclusion

The feedback gap is not technical. It is a habit problem. I default to working alone and not making work visible. Mats and Gerald both called this out. The fix is simple: ask, record, respond. Every week.

---

## Conclusion

### Sub-conclusion 1 — Shared backend

Keep the shared backend. The integration story matters more than isolation. No stability issues in 3 sprints.

### Sub-conclusion 2 — Speed camera choice

Speed camera was the right second dedicated API: most complex data, best demo value, strongest narrative fit with the parking problem.

### Sub-conclusion 3 — Feedback workflow

I need to ask for feedback explicitly, record it in Portflow, and show that I process it. This is the main non-technical gap in my evidence.

### Answer to the main question

Three recommendations improve cross-tile integration and team workflow:

1. Keep the shared backend with router separation per tile.
2. Build dedicated APIs only when data has structure (more than one meaningful field). The speed camera validates this pattern.
3. Establish a weekly feedback cycle: ask, record, respond. If it is not in Portflow, it did not happen.

---

## Recommendation

1. **For Gurpreet:** Add one HTTP POST call in your `update()` function. Endpoint: `POST /api/v1/speedcamera/` with body `{speed_kmh, direction, is_violation, speed_limit_kmh}`.
2. **For Wesley:** When your traffic light needs backend control, talk to me before Sprint 4 so we design the endpoints together.
3. **For everyone:** Update ESP32 sketches to remove `:8000` from URLs. The Pi serves on port 80 now.
4. **For me:** Review at least 1 teammate MR in the first week of Sprint 4. Record all feedback in Portflow.

---

## References

- Mats Otten. (2026). Sprint 2 Progress Review feedback. Verbal, recorded in portflowMasterSheet.md.
- Gerald Stap. (2026). Sprint 3 feedback on feedback workflow. Verbal, May 6.
- Shoup, D. (2017). *The High Cost of Free Parking*. Routledge.
- Gemeente Amsterdam. (2019). *Agenda Amsterdam Autoluw.*

---

## Appendix

### A. Current API adoption per tile

| Tile | Owner | Endpoint type | Status |
|------|-------|---------------|--------|
| Parking | Matin | Dedicated (`/api/v1/parking`) | Active |
| Railroad crossing | Thijmen | Dedicated (`/api/v1/railroad`) | Active |
| Speed camera | Gurpreet | Dedicated (`/api/v1/speedcamera`) | API ready, ESP32 not connected yet |
| Streetlight | Gurpreet | Generic (`/api/v1/readings`) | Active |
| Traffic light | Wesley | Generic (`/api/v1/readings`) | Intermittent |

### B. Architecture evaluation

| Criterion | Shared backend | Per-tile backends |
|-----------|---------------|-------------------|
| Integration story | Strong | Weak |
| Maintenance cost | 1 deployment | 5 deployments |
| Cross-tile queries | Possible | Not possible |
| Single point of failure | Yes (risk) | No |
| Demo value | High | Low |
