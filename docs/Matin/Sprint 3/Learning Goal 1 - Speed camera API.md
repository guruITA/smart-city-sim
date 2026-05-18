# Sprint 3 - Learning Goal 1: Speed camera API

## Learning Question

How do I build an API for someone else's sensor tile when I do not control the hardware?

## S - Situation

Gurpreet has a working speed camera. Two IR sensors on an ESP32-S3, 10 cm apart. He measures speed in km/h, detects violations, and tracks direction. But all of that stays on his Serial monitor. Nothing reaches the backend.

My backend already serves parking (Sprint 1) and railroad crossing (Sprint 2, Thijmen's code). I need to add a third dedicated API for the speed camera.

## T - Task

Build a dedicated speed camera API that stores Gurpreet's measurements and shows them on the dashboard. The API must match his firmware output exactly so he only needs to add one HTTP POST call.

### Deliverables

**Design document** - [Design - Speed camera data model and endpoints](Design%20-%20Speed%20camera%20data%20model%20and%20endpoints.md)

Data model and 5 endpoint designs based on Gurpreet's firmware output.

**Realise document** - [Realise - Speed camera API implementation](Realise%20-%20Speed%20camera%20API%20implementation.md)

The built code, files changed, testing, and current state.

## A - Action

First I read Gurpreet's `speed_camera_s3.ino`. He outputs 4 fields: speed_kmh, direction, is_violation, speed_limit_kmh. That does not fit the generic sensor_readings table. So I designed a dedicated `speed_readings` table with those exact columns. I documented this in the [Design](Design%20-%20Speed%20camera%20data%20model%20and%20endpoints.md).

Then I built everything on April 23: SQLAlchemy model, Pydantic schemas, FastAPI router with 5 endpoints, and a dashboard panel with orange accent. I tested with curl. Committed as `3f6b3e5`. Documented in the [Realise](Realise%20-%20Speed%20camera%20API%20implementation.md).

## R - Result

The [Design](Design%20-%20Speed%20camera%20data%20model%20and%20endpoints.md) produced a dedicated table with 5 columns and 5 endpoints including a `/stats` endpoint with SQL aggregation.

The [Realise](Realise%20-%20Speed%20camera%20API%20implementation.md) produced working code. The backend now serves speed camera data on the dashboard. Gurpreet only needs one `http.POST()` call in his firmware.

## R - Reflection

I learned that building an API for someone else starts with reading their code. In Sprint 1 I designed the generic endpoint and hoped tiles would fit. This time I studied the firmware first and matched the exact field names. That was better.

I committed the API code and docs in one big commit. Should have been separate.

## T - Transfer

For Sprint 4, I will always read the embedded code before designing an API. If Wesley or Betul need endpoints, I read their sketch first. I will also use smaller commits.

## References

Matin. (2026). Design: Speed camera data model and endpoints [Design deliverable]. [Design - Speed camera data model and endpoints](Design%20-%20Speed%20camera%20data%20model%20and%20endpoints.md)

Matin. (2026). Realise: Speed camera API implementation [Realise deliverable]. [Realise - Speed camera API implementation](Realise%20-%20Speed%20camera%20API%20implementation.md)
