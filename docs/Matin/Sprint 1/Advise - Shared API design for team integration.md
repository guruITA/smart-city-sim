# Advise - Shared API design for team integration

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | March 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group - HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document explains the technology and integration recommendations I made for the team. I designed the backend for all 5 team members, not just my parking tile. After analyzing each member's sensor setup, I made choices that affect everyone.

### Main question

How should the shared backend be designed so all team members can use it?

### Sub-questions

1. Which framework fits a 17-week student project?
2. How should tiles send data without needing backend changes?

---

## Chapter 1 - Framework choice

### Context

In Sprint 0, I evaluated two options: Java Spring Boot and Python FastAPI. The team had no strong preference. I needed to pick one and justify it.

### Findings

| Criteria | Spring Boot | FastAPI |
|----------|-------------|---------|
| Setup time | Slow. Maven/Gradle, annotations, boilerplate. | Fast. One file gets you an API. |
| Learning curve | High. I would need weeks to be productive. | Low. Python type hints generate most things. |
| Documentation | Manual Swagger config or SpringDoc. | Auto-generated Swagger at `/docs`. |
| Validation | Manual or Bean Validation annotations. | Pydantic does it from type hints. |
| Team testing | Team members need to read code or docs. | Swagger "Try it out" buttons. No code reading needed. |

I picked FastAPI. The project is 17 weeks. I cannot spend 4 of them learning a framework. FastAPI gave me a working API on day one.

### Sub-conclusion

FastAPI. Faster development cycle. Built-in Swagger docs. Pydantic validation for free. Right choice for a time-constrained student project.

---

## Chapter 2 - Generic endpoint for all tiles

### Context

Each tile produces different data. If every tile needs custom endpoints, I become a bottleneck. I needed a design where team members can send data without me changing the backend.

### Method

I created a generic `POST /api/v1/readings` endpoint that accepts a simple JSON body:

```json
{
  "tile": "streetlight",
  "sensor_type": "ldr",
  "value": 450.0,
  "unit": "lux"
}
```

Any tile can use this by filling in their own values. No backend changes needed.

### Integration readiness

Based on my codebase analysis in Sprint 1:

| Team member | Can integrate now? | What they need |
|-------------|-------------------|----------------|
| Thijmen | No | WiFi + HTTP code in his Arduino sketch |
| Gurpreet | No | WiFi + HTTP code in his Arduino sketch |
| Wesley | No | Needs to write code first |
| Betul | N/A | Scrum Master role |

I advised the team to prioritize WiFi integration in Sprint 2. I offered to help with HTTP POST examples for each tile.

### Sub-conclusion

One generic endpoint serves all tiles. Team members add WiFi + one HTTP POST call. No backend changes required per tile.

---

## Chapter 3 - Docker for team development

### Context

Not everyone will install Python and PostgreSQL. I needed to remove that barrier.

### Recommendation

Use Docker Compose to run the backend. One command: `docker compose up --build`. API available at `localhost:8000/docs` within 2 minutes. No dependencies to install.

I documented this in the backend README with step-by-step instructions. I also created an API specification document with exact JSON examples per tile so team members know what to send.

### Sub-conclusion

Docker removes the setup barrier. Team members test locally with one command.

---

## Conclusion

FastAPI was the right framework choice for a 17-week student project. The generic endpoint lets all tiles send data without backend changes. Docker Compose removes the setup barrier. The biggest risk is that nobody on the team ran the backend during Sprint 1 because I did not actively demo it. For Sprint 2, I need to do a live demo and help team members connect their ESP32 devices.

---

## References

- FastAPI documentation. https://fastapi.tiangolo.com/
- Docker Compose documentation. https://docs.docker.com/compose/
- Arduino HTTPClient library. https://docs.arduino.cc/libraries/httpclient/
