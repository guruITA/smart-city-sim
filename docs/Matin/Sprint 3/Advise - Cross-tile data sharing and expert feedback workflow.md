# Advise — Cross-tile data sharing and expert feedback workflow

## Introduction

This is the advise deliverable for Sprint 3 Learning Goal 1. It contains my recommendations on cross-tile data sharing, why the speed camera was chosen as the second dedicated API, why port 80 matters, and what the expert feedback workflow should look like. It also reflects on what I was not doing and what needs to change.

## Why a shared backend instead of per-tile backends

I thought about giving each team member their own API. That would be simpler for me: I build parking, Thijmen builds his own railroad API, Gurpreet builds his own speed camera API. But then there is no integration story. Five separate APIs running on five different ports is not a smart city. It is five separate demos.

The whole point of a smart city platform is that data from different sensors lives in one place. A parking sensor alone is useful. A parking sensor combined with traffic light data and speed camera measurements gives a richer picture of how the city moves. If the municipality wants to see "what happened on this street at 3pm", they go to one API, not five.

My recommendation to the team: keep using one shared backend with separate routers per tile. Each tile gets its own router file, its own models if needed, and its own section in the dashboard. But it all runs in one Docker Compose setup on one Pi.

**Risk of this approach:** If one tile's bad code crashes the API, all tiles go down. This has not happened yet. If it becomes a problem, we can add error isolation per router in Sprint 4.

## Why the speed camera as the second dedicated API

I had a choice between three tiles for the next dedicated API:

- **Streetlight (Gurpreet):** LDR sensor sends a single light level value. This fits the generic endpoint perfectly. No need for a dedicated table.
- **Traffic light (Wesley):** Currently sends state changes. Could benefit from a dedicated table eventually, but his data format is still evolving.
- **Speed camera (Gurpreet):** Has 4 structured fields (speed, direction, violation, limit). Does not fit the generic endpoint cleanly.

I chose speed camera because:

1. **Data complexity.** 4 meaningful fields is the strongest case for a dedicated table. Forcing it into `{value: 2.4, unit: "km/h"}` would lose information.
2. **Demo value.** Speed data with violation highlighting makes for a more interesting dashboard panel than a static light level number. Assessors notice interactive data.
3. **Story value.** The smart city detects both where you can park AND whether you are driving safely. Two problems, one platform. That is a stronger narrative for the Mayor Delivery than "we also measure light levels."

From a societal perspective: speed enforcement in residential areas addresses the same residents who suffer from parking search traffic. Cars driving too fast in neighborhoods are a safety problem for pedestrians and children. The speed camera and parking sensor both serve the goal of making neighborhoods safer and quieter.

## Why port 80

I recommended exposing the API on port 80 instead of 8000 for one practical reason: ESP32's HTTPClient library defaults to port 80 for HTTP connections.

When the backend ran on port 8000, every team member had to add `:8000` to their URL:

```
http://145.92.8.137:8000/api/v1/readings
```

With port 80:

```
http://145.92.8.137/api/v1/readings
```

Gurpreet had the port hardcoded in his sketch. Thijmen had it in his config. Every time someone forgot the port, they got a connection timeout and thought the backend was down. Removing the port number removes one source of confusion.

The change is one line in `docker-compose.yml`: `"80:8000"` instead of `"8000:8000"`. The application code does not change. The container still runs on 8000 internally.

## Expert feedback workflow

After talking to Mats on May 6 and Gerald on May 6, I realized I have a feedback problem.

**What I was doing:** Building things, committing code, writing documentation. Getting verbal feedback in team meetings and conversations. Not recording any of it.

**What assessors see:** Zero feedback in Portflow. No comments, no validations, no expert responses. To them, I never asked anyone for input.

**What I should be doing:**

1. Ask for feedback explicitly. Not "hey what do you think" in a casual conversation, but a structured request: "I wrote this analysis, can you check if the stakeholder mapping is realistic?"
2. Record the feedback. In Portflow, not in my head. If Thijmen reviews my speed camera code and says "looks good, but you should add input validation", that needs to be a Portflow comment.
3. Respond to the feedback. Show that I processed it. If Gerald says the problem statement needs more Amsterdam context, I update the document and add a comment saying what I changed.

**My plan for the remaining weeks of Sprint 3:**

| Week | Action | Who |
|------|--------|-----|
| Week 13 | Ask Thijmen to review speed camera router code | Thijmen |
| Week 13 | Ask Gerald to validate problem statement framing | Gerald |
| Week 13 | Ask Mats for 1-on-1 Progress Review prep | Mats |

**My plan for Sprint 4:**

- Week 1: Write learning goals, send to Gerald for review
- Week 1: Record Gerald's response in Portflow
- Week 2: First code delivery, ask teammate for code review
- Week 3: Midpoint check with Mats or Gerald

This is not complicated. It is just discipline. The reason I was not doing it is not that I did not know how. It is that I defaulted to working alone and not making my work visible. Mats and Gerald both called this out. They are right.

## Evaluation of the current architecture

The current backend architecture is good enough for this PoC. The router separation works. The dual-table pattern (generic + dedicated) works. The Docker setup is stable. The dashboard is functional.

Things I would improve before Sprint 4:

- **Input validation:** The speed camera POST endpoint does not validate that `speed_kmh > 0` or that `direction` is one of `"1->2"` or `"2->1"`. Pydantic handles type checking but not business rules. Adding a check constraint (like the `barrier` table has for `input_mode`) would be better.
- **Error handling:** If the database connection fails, the API returns a 500 error with a SQLAlchemy traceback. That is fine for development but not great for production. A generic error handler would be cleaner.
- **Commit discipline:** Smaller, more frequent commits. One commit per feature, one per doc update.

Things I would NOT change:

- The stateless backend design. The ESP32 computes violations, the backend stores them. This is the right separation.
- The router-per-tile structure. It scales and it is easy to understand.
- Docker Compose for deployment. It works. No reason to add Kubernetes complexity for a 5-table PoC.

## Advice for the team

1. **For Gurpreet:** Add one HTTP POST call in your `update()` function. The endpoint is `POST /api/v1/speedcamera/` with the body `{speed_kmh, direction, is_violation, speed_limit_kmh}`. That is all you need.
2. **For Wesley:** When your traffic light needs backend control (receiving green/red commands), we will add a dedicated router. Talk to me before Sprint 4 so we can design it together.
3. **For everyone:** Use port 80. Update your ESP32 sketches to remove `:8000` from URLs. The Pi serves on port 80 now.
