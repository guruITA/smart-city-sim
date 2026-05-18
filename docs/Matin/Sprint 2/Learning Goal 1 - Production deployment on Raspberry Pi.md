# Sprint 2 - Learning Goal 1: Production deployment on Raspberry Pi

## Learning Question

How do I make the Docker backend production-ready for a Raspberry Pi on the HvA network?

## S - Situation

In Sprint 1, the backend ran on my laptop. Every ESP32 pointed at my local IP. If I closed my laptop, all tiles lost their connection. The team needed a shared server that stays online. The Pi on the HvA network was available.

But my Docker setup was built for local development. Hardcoded database credentials in `docker-compose.yml`. Hot-reload enabled. No restart policy. The dashboard had `localhost:8000` hardcoded. None of this would survive a Pi deployment.

## T - Task

Make the Docker setup production-ready and prepare everything for the Pi. The backend must survive reboots. Secrets must be separated from code. The dashboard must work on any IP.

### Deliverables

**Design document** - [Design - Production deployment architecture](Design%20-%20Production%20deployment%20architecture.md)

Architecture changes needed to move from dev laptop to production Pi.

**Realise document** - [Realise - Pi deployment and Docker hardening](Realise%20-%20Pi%20deployment%20and%20Docker%20hardening.md)

What was changed, the deploy script, and verification.

## A - Action

I started by listing what breaks if I copy the current setup to the Pi. Hardcoded credentials leak into GitLab. Hot-reload wastes CPU on ARM. No restart policy means a reboot kills everything. Dashboard breaks because `localhost` means nothing on a remote machine.

I designed the fixes. Moved credentials to `.env`. Removed `--reload` from the Dockerfile. Added `restart: always`. Changed the dashboard from `http://localhost:8000` to `window.location.origin`. Created `deploy.sh` to automate the whole flow. Documented all of this in the [Design](Design%20-%20Production%20deployment%20architecture.md).

Then I implemented everything. Each fix was straightforward but the deploy script needed a health check loop. Documented the implementation in the [Realise](Realise%20-%20Pi%20deployment%20and%20Docker%20hardening.md).

## R - Result

The [Design](Design%20-%20Production%20deployment%20architecture.md) produced 5 architecture changes: .env separation, production Dockerfile, restart policy, dynamic dashboard URL, and deploy.sh.

The [Realise](Realise%20-%20Pi%20deployment%20and%20Docker%20hardening.md) confirmed everything works. The backend is production-ready for Pi deployment. Containers restart after reboot. Secrets stay out of version control.

## R - Reflection

The changes were not complicated. The hard part was realizing how many dev-only assumptions I had baked in. I never thought about `localhost` being wrong on a remote machine until I tried it. The .env file should have been there from Sprint 1. I was lazy about it because "it works locally."

I also learned that a deploy script saves time. Without it, I would have to SSH in and run 5 commands every time.

## T - Transfer

For Sprint 3 and beyond, I will start with production config from day one. Dev convenience should be layered on top, not the default. I will also keep deploy scripts updated as the setup evolves.

## References

Matin. (2026). Design: Production deployment architecture [Design deliverable]. [Design - Production deployment architecture](Design%20-%20Production%20deployment%20architecture.md)

Matin. (2026). Realise: Pi deployment and Docker hardening [Realise deliverable]. [Realise - Pi deployment and Docker hardening](Realise%20-%20Pi%20deployment%20and%20Docker%20hardening.md)
