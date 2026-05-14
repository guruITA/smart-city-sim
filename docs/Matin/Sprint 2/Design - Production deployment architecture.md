# Design - Production deployment architecture

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

This document designs the changes needed to move the backend from my dev laptop to a shared Raspberry Pi on the HvA network. The Sprint 1 Docker setup was dev-only. It had hardcoded credentials, hot-reload enabled, and a dashboard that only worked on localhost.

### Main question

What changes are needed to move from dev laptop to production Pi?

### Sub-questions

1. How should secrets and configuration be separated?
2. What ensures the backend survives Pi reboots?

---

## Chapter 1 - Secrets and configuration separation

### Context

The `docker-compose.yml` had database credentials inline: `POSTGRES_PASSWORD=secret123`. This works locally but is a security problem once the code is on GitLab. Anyone with repo access sees the password.

### Method

I followed the 12-factor app methodology. Environment-specific config belongs in environment variables, not in code.

### Findings

I moved all credentials to a `.env` file and added `.env` to `.gitignore`. The `docker-compose.yml` now references variables:

```yaml
environment:
  - DATABASE_URL=${DATABASE_URL}
  - POSTGRES_PASSWORD=${POSTGRES_PASSWORD}
```

I created a `.env.example` with placeholder values so teammates know which variables to set.

The dashboard had `http://localhost:8000` hardcoded as the API base URL. On the Pi, localhost means the Pi itself, which is correct. But team members access the dashboard from their own machines via the Pi's IP. I changed the dashboard to use `window.location.origin`. Now it works on any IP without code changes.

### Sub-conclusion

Secrets live in `.env`, which is gitignored. The dashboard uses the browser's own URL as the API base. No hardcoded addresses remain.

---

## Chapter 2 - Reboot survival

### Context

The Pi will run unattended on the HvA network. Power outages and reboots happen. If Docker does not restart containers automatically, the backend goes down and no one notices until a tile stops working.

### Method

I looked at Docker Compose restart policies and uvicorn production settings.

### Findings

Two changes were needed.

First, the restart policy. I changed from `unless-stopped` to `restart: always`. The difference: `unless-stopped` does not restart containers after a daemon restart (which happens on reboot). `restart: always` does.

Second, the Dockerfile. I removed the `--reload` flag from uvicorn. Hot-reload watches the filesystem for changes and restarts the server. That is useful during development. On the Pi, nobody edits files directly. The reload watcher wastes CPU on the Pi's limited ARM processor. I also removed the dev volume mount that bypassed the Docker image build.

I wrote `deploy.sh` to automate the full deployment:

1. Copy `.env` to the Pi if missing
2. Pull latest code from GitLab
3. Build and start containers
4. Wait for health check to pass
5. Print access URLs

### Sub-conclusion

`restart: always` and a production Dockerfile without `--reload` handle reboots. The deploy script automates the full flow.

---

## Conclusion

Five changes make the backend production-ready: `.env` for secrets, `window.location.origin` for the dashboard, `restart: always` for reboots, no `--reload` in the Dockerfile, and `deploy.sh` for automated deployment. All changes are backward compatible with local development.

---

## References

- Wiggins, A. (2017). *The Twelve-Factor App*. https://12factor.net/
- Docker Compose documentation. https://docs.docker.com/compose/
- Uvicorn documentation. https://www.uvicorn.org/
