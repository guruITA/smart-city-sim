# Sprint 1 - Learning Goal 1: REST API with FastAPI

## Learning Question

How do I build a REST API that receives sensor data from ESP32 devices using a framework I've never used before?

## S - Situation

Sprint 1 of City Sim. Five students, each building a physical tile with ESP32 sensors. I picked Backend Developer. In Sprint 0 I chose FastAPI + PostgreSQL + Docker as the stack. I had never used FastAPI before. The parking tile needs a backend that takes ultrasonic distance readings and decides if a spot is occupied.

## T - Task

Build a working FastAPI application with at least 5 REST endpoints. It must accept sensor readings from ESP32 devices over WiFi HTTP POST, validate with Pydantic, store in PostgreSQL, and serve data back via GET. Auto-generated Swagger docs so team members can test without reading code.

### Deliverables

**Design document** - [Design - API structure and endpoint architecture](Design%20-%20API%20structure%20and%20endpoint%20architecture.md)

API endpoint design, router pattern, and Pydantic validation approach.

**Realise document** - [Realise - FastAPI backend implementation](Realise%20-%20FastAPI%20backend%20implementation.md)

The built code: 7 endpoints, 2 routers, Swagger docs, curl testing.

## A - Action

I started by reading FastAPI docs and building a "hello world" endpoint. Then I set up the project structure: `main.py` for the app, `models.py` for SQLAlchemy models, `schemas.py` for Pydantic validation, and a `routers/` folder to separate endpoint groups.

I built the generic `POST /api/v1/readings` endpoint first because all tiles need it. Then I added parking-specific endpoints: `/update/{spot_number}`, `/status`, `/spots`. I tested everything with `curl` and the Swagger UI at `/docs`. I used Pydantic `Field` with examples so Swagger shows realistic payloads without needing extra documentation.

The full endpoint design is in the [Design](Design%20-%20API%20structure%20and%20endpoint%20architecture.md). The implementation details are in the [Realise](Realise%20-%20FastAPI%20backend%20implementation.md).

## R - Result

The [Design](Design%20-%20API%20structure%20and%20endpoint%20architecture.md) produced a router-based structure with 2 router groups and Pydantic validation on all inputs.

The [Realise](Realise%20-%20FastAPI%20backend%20implementation.md) produced a working FastAPI application with 7 REST endpoints. The API accepts POST requests from any tile via the generic endpoint, plus dedicated parking operations. Pydantic catches malformed requests automatically. Swagger docs work at `/docs` with "Try it out" buttons. All data persists in PostgreSQL.

## R - Reflection

FastAPI's learning curve was lower than I expected. Type hints + Pydantic gave me input validation and documentation for free. I did not write extra validation code or extra docs. The biggest risk was choosing an unfamiliar framework under time pressure. It paid off because I could move faster than with Spring Boot.

One thing I did wrong: no automated tests. I tested everything manually with curl and Swagger. That works, but it is not repeatable.

## T - Transfer

The pattern of "type hints generate docs and validation" is something I will use in every future Python API. The router separation pattern scales to any number of tiles. For Sprint 2, I need automated tests instead of only manual testing.

## References

Matin. (2026). Design: API structure and endpoint architecture [Design deliverable]. [Design - API structure and endpoint architecture](Design%20-%20API%20structure%20and%20endpoint%20architecture.md)

Matin. (2026). Realise: FastAPI backend implementation [Realise deliverable]. [Realise - FastAPI backend implementation](Realise%20-%20FastAPI%20backend%20implementation.md)
