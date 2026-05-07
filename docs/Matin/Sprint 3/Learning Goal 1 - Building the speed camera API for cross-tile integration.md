# Sprint 3 — Learning Goal 1: Building the speed camera API for cross-tile integration

## Learning Question

How do I design and build an API for someone else's sensor tile when I do not control the hardware or firmware, and how does adding a second dedicated API strengthen the shared backend as a city-wide data platform?

## S — Situation

At the start of Sprint 3, my backend already serves two types of tiles. Parking has a dedicated table and endpoints since Sprint 1. Railroad crossing was merged from Thijmen's branch in Sprint 2 with its own dedicated tables for train detection and barrier control. The generic `sensor_readings` table handles everything else. The whole system runs in Docker Compose with PostgreSQL. The dashboard shows 4 tile panels with live refresh.

But I am coming off a bad Sprint 2. Mats gave me 1/4 on all learning goals. His feedback: "Choose a human-centered problem from the city." I had no problem statement, no learning goals written upfront, and I got a strike for working solo without enough team communication. The technical work was solid. The framing was absent.

Sprint 3 is my recovery sprint. I need to prove two things at once: that I can deliver new technical work (speed camera API for Gurpreet's tile), and that I can frame it inside a real city problem (parking search traffic in Amsterdam, extended with speed enforcement).

Gurpreet has a working speed camera. Two IR sensors on an ESP32-S3, 10 cm apart, measure how fast a toy car passes between them. He calculates speed in km/h. He also has a separate ESP32-CAM that takes photos of violations. But none of this data reaches the backend. He prints to Serial and shows it on an OLED. I need to build the API that his ESP32 will POST to.

This is the first time I build an API for someone else's hardware. In Sprint 1 I built parking for my own sensor. In Sprint 2 I merged Thijmen's code that he wrote himself. Now I need to study Gurpreet's firmware, understand his data format, and design an API that matches it exactly.

## T — Task

My task is to build a dedicated speed camera API that turns Gurpreet's standalone speed measurements into backend-stored data visible on the shared dashboard. The API must match his firmware output exactly so he only needs to add one HTTP POST call to connect.

Beyond the speed camera, Sprint 3 also includes finishing the port 80 deployment (carry-over from Sprint 2) and writing the problem statement that Mats required. But this learning goal focuses on the speed camera as the main technical challenge.

I want to answer whether adding a second dedicated API (after parking) makes the shared backend more convincing as a smart city platform. If only parking uses dedicated endpoints, the backend is a parking tool. If parking and speed camera both have dedicated APIs with structured data, the backend becomes a multi-purpose city data platform.

### Deliverables

**Analysis document**

In this document, I analyze how the backend has grown from Sprint 1 to Sprint 3 and what adding the speed camera means for the architecture. I look at which sub-questions from Sprint 1 this sprint addresses, what new research questions emerged after Mats' feedback, and how the cross-tile integration scales. I also map the stakeholders and am honest about the limitations of my analysis.

**Design document**

In this document, I show the speed camera data model, the 5 API endpoints, the production network architecture, and the dashboard design for the 5th panel. I explain why I chose a dedicated table instead of reusing the generic sensor_readings table, and why the violation flag is computed on the ESP32 instead of in the backend.

**Realise document**

In this document, I record what I built, when I built it, and which commits contain the work. This covers the speed camera router, the port 80 fix, the problem statement, and the Portflow restructuring. I list every file I changed and show what is running on the Pi right now.

**Advise document**

In this document, I explain my recommendations for cross-tile data sharing, why I chose the speed camera as the second dedicated API, why I recommended port 80, and what the expert feedback workflow should look like going forward. I also reflect on what I was not doing (recording feedback in Portflow) and what needs to change.

## A — Action

I worked on this learning goal in four steps.

First, I made the analysis deliverable [Analysis - Cross-tile integration and backend architecture growth](). In that document, I examined how the backend grew from 2 tables in Sprint 1 to 5 tables in Sprint 3, and what adding the speed camera means for scalability. I refined the research questions from Sprint 1 after Mats' feedback about the human "why". I also mapped stakeholders and was honest that I have not done any primary research yet.

After that, I made the design deliverable [Design - Speed camera API and production network architecture](). On April 23 I read through Gurpreet's `speed_camera_s3.ino`. He calculates speed as `(distance / time) * 3.6` and compares against a 1.0 km/h threshold. I designed a `speed_readings` table with 5 columns that match his firmware output exactly. I designed 5 endpoints including a `/stats` endpoint that uses SQL aggregation. I also documented the production network setup and the dashboard panel design.

Then I worked on the realise deliverable [Realise - Speed camera API implementation and production deployment](). I built the SpeedReading model, Pydantic schemas, and a FastAPI router with 5 endpoints. I added a 5th dashboard panel with orange accent color and violation highlighting. I committed everything on April 23 as `3f6b3e5`. On May 4 I finished the port 80 fix (`42d99bd`) and committed the problem statement (`dbb1bc8`). On May 5 I restructured all Portflow evidence and learning journal documents.

Finally, I made the advise deliverable [Advise - Cross-tile data sharing and expert feedback workflow](). I explained why a shared backend is better than per-tile backends, why the speed camera was the right second use case, and why port 80 matters for ESP32 integration. After talking to Mats and Gerald on May 6, I documented the feedback workflow I should have been following all along.

## R — Result

This learning goal resulted in a set of deliverables that together document the full speed camera integration and Sprint 3 work.

The first result was my [Analysis - Cross-tile integration and backend architecture growth](). In this document, I showed how the backend architecture scales and which Sprint 1 sub-questions were addressed. I also identified that my analysis still lacks primary research and planned an informal interview for Sprint 4.

The second result was my [Design - Speed camera API and production network architecture](). In this document, I worked out the data model, endpoint design, network architecture, and dashboard panel. The key design decision was making the backend stateless: the ESP32 computes the violation flag, the backend just stores it.

The third result was my [Realise - Speed camera API implementation and production deployment](). This document lists all code changes, commits, and the current state of the Pi. The backend now serves 5 router groups with 20+ endpoints. The Pi runs at `http://145.92.8.137/` on port 80 with auto-restart.

The fourth result was my [Advise - Cross-tile data sharing and expert feedback workflow](). This document contains my recommendations for the team and my honest reflection on what was missing: recorded expert feedback. The plan for weekly feedback is documented here.

Together, these results show that I completed the learning goal by analyzing the integration challenge, designing the API, building and deploying it, and advising on the broader implications. Each step is documented in a separate deliverable.

## R — Reflection

This learning goal helped me answer my learning question. I learned that building an API for someone else's hardware requires reading their code first, not guessing. In Sprint 1 I designed the generic endpoint and hoped tiles would fit. This time I studied Gurpreet's `speed_camera_s3.ino`, matched his exact field names, and designed the API around his output. That was a better approach.

The most important result is not the speed camera itself but what it proves about the architecture. In Sprint 1, the backend served parking. Now it serves parking, railroad crossing, and speed camera. Each tile has its own dedicated endpoints when the data has structure, and the generic endpoint handles simple values. That pattern holds. Adding the speed camera required only 3 touchpoints in existing code: model, new router file, and registration in main.py. Nothing else changed.

I also learned something about framing. Mats did not care about the technical quality. He cared about the story. "Why does this matter?" is more important than "How does this work?" for the assessors. Building the speed camera without connecting it to a city problem would have been the same mistake as Sprint 2. This time I wrote the problem statement first and framed the speed camera as part of the story: a smart city that detects both parking availability and speeding in neighborhoods.

A weakness I noticed: I committed the speed camera code together with documentation in one big commit (`3f6b3e5`). That should have been separate commits. It makes git history harder to review.

Another weakness: I still work in bursts. The speed camera was built on April 23. The port 80 fix and problem statement came on May 4. The Portflow restructuring on May 5-6. Nothing in between. That pattern has been consistent across all three sprints and I have not fixed it.

## T — Transfer

I will transfer this learning to Sprint 4 in three ways.

First, I will always read the embedded code before designing an API. The approach of studying Gurpreet's firmware first and matching his data format saved both of us time. If Betul or Wesley need backend endpoints, I will read their ESP32 sketches first.

Second, I will use smaller commits. One commit per feature, one per documentation update. Not one big batch.

Third, I will write the learning goal at the start of the sprint, not at the end. This learning goal was written retroactively. The work was real but the documentation came after. In Sprint 4, I will write the learning question and task description in week 1, then fill in the action and result as I go.

The dual-table pattern (generic for simple data, dedicated for structured data) is now proven across 3 tiles. For Sprint 4, if a new tile needs a dedicated API, I can apply the same pattern without redesigning anything.

## References

Matin. (2026). Analysis: Cross-tile integration and backend architecture growth [Analysis deliverable]. [Analysis - Cross-tile integration and backend architecture growth]()

Matin. (2026). Design: Speed camera API and production network architecture [Design deliverable]. [Design - Speed camera API and production network architecture]()

Matin. (2026). Realise: Speed camera API implementation and production deployment [Realise deliverable]. [Realise - Speed camera API implementation and production deployment]()

Matin. (2026). Advise: Cross-tile data sharing and expert feedback workflow [Advise deliverable]. [Advise - Cross-tile data sharing and expert feedback workflow]()
