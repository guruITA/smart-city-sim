# Realise - Pi deployment and Docker hardening

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | April 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group - HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document records the implementation of the production deployment changes designed in the [Design document](Design%20-%20Production%20deployment%20architecture.md). I modified 5 files and created 1 new file.

### Main question

What was changed and does it work?

### Sub-questions

1. What files were modified for production?
2. How was the deployment verified?

---

## Chapter 1 - Files modified

### What was changed

**backend/docker-compose.yml** - Replaced inline credentials with `${DATABASE_URL}` and `${POSTGRES_PASSWORD}` references. Changed restart policy from `unless-stopped` to `restart: always`. Removed dev volume mount.

**backend/Dockerfile** - Removed `--reload` flag from the uvicorn command. The server now runs in production mode without filesystem watching.

**backend/app/static/index.html** - Changed API base URL from `http://localhost:8000` to `window.location.origin`. The dashboard now works on any IP.

**.gitignore** - Added `.env` to prevent credentials from being committed.

**backend/.env.example** - New file with placeholder values so teammates know which variables to configure.

**backend/deploy.sh** - New script that automates the full deployment flow: checks for `.env`, pulls latest code, builds containers, runs a health check loop (retries 5 times with 3-second intervals), and prints access URLs.

### Sub-conclusion

5 files modified, 1 new file created. All changes follow the design document.

---

## Chapter 2 - Verification

### Method

I tested locally first by running `docker compose up --build` with the `.env` file. Then I prepared the Pi deployment test.

### Results

- Containers start without hardcoded credentials
- Dashboard loads and fetches data using `window.location.origin`
- Containers restart after `docker compose restart`
- `deploy.sh` runs without errors and reports health check success
- `.env` does not appear in `git status` after being gitignored

### What I did not verify

Port 80 was not configured in Sprint 2. The backend still runs on port 8000. I planned to switch to port 80 but did not get to it. This carried over to Sprint 3.

### Sub-conclusion

The production setup works. The port 80 change is the only remaining item.

---

## Conclusion

All production deployment changes are implemented and verified. The backend is ready for Pi deployment with proper secret management, reboot survival, and automated deploy. Port 80 is a Sprint 3 carry-over.

---

## References

- Docker Compose documentation. https://docs.docker.com/compose/
- Uvicorn documentation. https://www.uvicorn.org/
