# Realise - Docker Compose setup

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

This document records the Docker Compose setup I built in Sprint 1. The goal was simple: team members run one command and the backend works.

### Main question

How does the Docker setup enable team development?

### Sub-questions

1. What containers are needed?
2. How does first-boot table creation work?

---

## Chapter 1 - Container setup

### Context

Five team members on different machines. If everyone has to install Python 3.11 and PostgreSQL 16, nobody will bother. I needed `docker compose up` to be the only command.

### What was built

**backend/Dockerfile** - Python 3.11 slim base. Copies requirements.txt, runs pip install, copies the app code, starts uvicorn with hot-reload.

```dockerfile
FROM python:3.11-slim
WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY ./app ./app
CMD ["uvicorn", "app.main:app", "--host", "0.0.0.0", "--port", "8000", "--reload"]
```

**backend/docker-compose.yml** - Two services:

| Service | Image | Purpose |
|---------|-------|---------|
| `db` | postgres:16 | Database with health check via `pg_isready` |
| `api` | Built from Dockerfile | FastAPI app, depends on db, exposes port 8000 |

The `api` service has `depends_on` with a health check condition. It waits until PostgreSQL responds to `pg_isready` before starting. Without this, the API crashes on startup because the database is still initializing.

A Docker volume persists the database. Stopping and restarting containers does not lose data.

### Sub-conclusion

Two containers. Health check prevents startup race condition. Volume keeps data between restarts.

---

## Chapter 2 - First-boot table creation

### Context

On a fresh clone, there are no database tables. Running SQL migrations manually defeats the purpose of a one-command setup.

### Method

SQLAlchemy's `Base.metadata.create_all(bind=engine)` runs in `main.py` on app startup. It checks which tables exist and creates any that are missing.

### How it works

1. Team member clones the repo
2. Runs `docker compose up --build`
3. PostgreSQL starts, passes health check
4. FastAPI starts, `create_all()` runs
5. Tables `sensor_readings` and `parking_spots` are created
6. API is ready at `localhost:8000/docs`

Total time from clone to working API: about 2 minutes.

### Limitation

`create_all()` only creates tables. It does not handle schema migrations. If I change a column, I need to drop and recreate the table or use Alembic. For Sprint 1 this was fine because the schema was new. For later sprints, this became a problem.

### Sub-conclusion

Automatic table creation on first boot. No manual SQL needed. Migration support is missing but not needed yet.

---

## Conclusion

The Docker setup lets team members run the backend with one command. Two containers, health check, volume persistence, automatic table creation. The main limitation is no migration support, which I will need in later sprints when the schema evolves.

---

## References

- Docker Compose documentation. https://docs.docker.com/compose/
- PostgreSQL Docker image. https://hub.docker.com/_/postgres
- SQLAlchemy `create_all()`. https://docs.sqlalchemy.org/
