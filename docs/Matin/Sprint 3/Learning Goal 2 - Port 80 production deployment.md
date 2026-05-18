# Sprint 3 - Learning Goal 2: Port 80 production deployment

## Learning Question

How do I make the backend accessible to all team members without them needing to remember a port number?

## S - Situation

The Pi ran on port 8000 since Sprint 2. Every team member had to add `:8000` to their URLs. Gurpreet had it hardcoded. Thijmen had it in his config. Every time someone forgot the port, they got a timeout and thought the backend was down.

This was supposed to be done in Sprint 2. I scored it as "not achieved" in my Sprint 2 learning goals. It carried over to Sprint 3.

## T - Task

Switch the Pi from port 8000 to port 80. HTTP defaults to 80, so no one needs to specify a port anymore.

### Deliverables

**Realise document** - [Realise - Port 80 deployment](Realise%20-%20Port%2080%20deployment.md)

What was changed, how it was deployed, and verification.

## A - Action

On May 4 I changed `docker-compose.yml` from `"8000:8000"` to `"80:8000"`, updated `deploy.sh` health check URLs, and moved the dashboard from `/dashboard` to `/`. Deployed via SSH. The whole thing took 30 minutes. Documented in the [Realise](Realise%20-%20Port%2080%20deployment.md).

## R - Result

The [Realise](Realise%20-%20Port%2080%20deployment.md) confirmed the Pi is live at `http://145.92.8.137/`. All team members can connect without specifying a port. Dashboard shows on the root URL.

## R - Reflection

This change was trivial. One line in docker-compose, one line in deploy.sh. But it took 6 weeks because I kept prioritizing other things. That is a prioritization failure. Small tasks that remove friction for the whole team should not carry over.

## T - Transfer

In Sprint 4, if something takes less than an hour and unblocks the team, I do it immediately. No carry-over for small fixes.

## References

Matin. (2026). Realise: Port 80 deployment [Realise deliverable]. [Realise - Port 80 deployment](Realise%20-%20Port%2080%20deployment.md)
