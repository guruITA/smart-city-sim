# Realise — Port 80 deployment

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | May 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group — HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document records the port 80 deployment on the Raspberry Pi. This was a Sprint 2 carry-over. The change itself took 30 minutes on May 4. Commit: `42d99bd`.

### Main question

What was changed to make the backend accessible on port 80?

---

## Chapter 1 — What was changed

### Files

**backend/docker-compose.yml** — Port mapping from `"8000:8000"` to `"80:8000"`. The container still runs on 8000 internally. The host maps port 80 to the container.

**backend/deploy.sh** — Health check URL updated from `localhost:8000` to `localhost:80`. Success message updated.

**backend/app/main.py** — Dashboard moved from `/dashboard` to `/`. Opening `http://145.92.8.137/` now shows the dashboard directly.

### Deployment

Deployed via SSH on May 4. Ran `git pull origin main && docker compose up --build -d`. Verified from a different machine on HvA network.

### Verification

- `http://145.92.8.137/` — dashboard loads
- `http://145.92.8.137/docs` — Swagger UI works
- `http://145.92.8.137/health` — returns 200
- Docker containers restart automatically with `restart: always`

---

## Conclusion

The Pi is live at port 80. No team member needs to specify `:8000` anymore. Docker auto-restart on reboot. This closes the Sprint 2 carry-over. The delay was 6 weeks for a 30-minute change.

---

## References

- Docker Compose documentation. https://docs.docker.com/compose/
