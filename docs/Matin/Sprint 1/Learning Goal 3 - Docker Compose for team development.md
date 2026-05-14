# Sprint 1 - Learning Goal 3: Docker Compose for team development

## Learning Question

How do I use Docker Compose to let team members run the backend with one command?

## S - Situation

Our team has 5 members on different machines. Mostly Windows, sometimes Mac. If everyone has to install Python 3.11, PostgreSQL 16, and configure database credentials to test the backend, most will never run it. I need a setup where `docker compose up` is the only command needed.

## T - Task

Create a Docker Compose configuration that runs FastAPI and PostgreSQL in containers. Health checks must ensure the database is ready before the API starts. Tables must be created automatically on first boot.

### Deliverables

**Realise document** - [Realise - Docker Compose setup](Realise%20-%20Docker%20Compose%20setup.md)

The Dockerfile, docker-compose.yml, health checks, and volume persistence.

**Advise document** - [Advise - Shared API design for team integration](Advise%20-%20Shared%20API%20design%20for%20team%20integration.md)

Technology recommendations and integration advice for the team.

## A - Action

I wrote a `Dockerfile` for the FastAPI app: Python 3.11 slim, pip install requirements, uvicorn with hot-reload for development. Then a `docker-compose.yml` with two services: `db` (PostgreSQL 16 with health check via `pg_isready`) and `api` (depends on db, exposes port 8000, connects via environment variables).

SQLAlchemy's `Base.metadata.create_all()` runs on app startup. Tables are created automatically on first boot. No manual migrations needed. I documented the setup in the [Realise](Realise%20-%20Docker%20Compose%20setup.md).

I also wrote integration advice for the team: why FastAPI over Spring Boot, how to use the generic endpoint, and why Docker matters for team development. That is in the [Advise](Advise%20-%20Shared%20API%20design%20for%20team%20integration.md).

## R - Result

The [Realise](Realise%20-%20Docker%20Compose%20setup.md) produced a working Docker setup. Team members can clone the repo, run `docker compose up --build`, and access the API at `localhost:8000/docs` within 2 minutes. No Python or PostgreSQL installation needed. Data persists in a Docker volume. Hot-reload means code changes reflect without restarting containers.

The [Advise](Advise%20-%20Shared%20API%20design%20for%20team%20integration.md) covered the technology choice, generic endpoint design, integration readiness per tile, and Docker for team development.

## R - Reflection

Docker Compose solved the "works on my machine" problem completely. The health check on PostgreSQL was important. Without it, the API would crash on startup trying to connect to a database that was still initializing.

One missed opportunity: I should have shared this setup with team members earlier in the sprint. Nobody else ran the backend during Sprint 1 because I did not actively demo it to them.

## T - Transfer

Every future project should start with Docker Compose, not end with it. Having reproducible infrastructure from day one saves debugging time later. For Sprint 2, the Docker setup needs hardening for production on the Raspberry Pi: no hot-reload, .env file for secrets, restart policy for reboot survival.

## References

Matin. (2026). Realise: Docker Compose setup [Realise deliverable]. [Realise - Docker Compose setup](Realise%20-%20Docker%20Compose%20setup.md)

Matin. (2026). Advise: Shared API design for team integration [Advise deliverable]. [Advise - Shared API design for team integration](Advise%20-%20Shared%20API%20design%20for%20team%20integration.md)
