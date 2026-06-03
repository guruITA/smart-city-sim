# Realise - Kubernetes autoscaling implementation

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | June 2026 |
| **Version** | 0.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Realise (fourth of the four outcomes: Analysis, Advise, Design, Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - What we built
3. Chapter 2 - Conventions we followed
4. Chapter 3 - How we test it
5. Chapter 4 - Test results
6. Chapter 5 - User test
7. Conclusion
8. Recommendation
9. References
10. Appendix

---

## Introduction

### Context

City Sim is our team's miniature smart city for the Studio Smart Cities semester at the HvA. Five tiles send their sensor data to one shared backend I maintain: FastAPI plus PostgreSQL, in containers, on one Raspberry Pi (145.92.8.137).

This is the **Realise** outcome for the autoscaling follow-up to Learning Goal 1. The Learning Goal 1 cluster scales by hand (`--scale`); its recommendation #5 was to move to K3s with a HorizontalPodAutoscaler for **automatic** scaling. The Design drew that architecture with one rule above all: only the stateless API scales, the shared database stays single, so the shared data cannot split brain. This document builds that design and verifies it.

### Status of this document

The build is complete and the manifests are validated. The measured numbers from the Pi (autoscaling, recovery, cutover) are filled in after a run on the Pi, in the same way the Learning Goal 1 Realise was first written with the build and then completed with the live numbers. Sections waiting for that run are marked `[to be filled after Pi test]`.

### Requirements we test against

- Gerald (Sprint 3): scale up and scale down on load, on its own.
- Mats (Sprint 3 and 4): sustainable, and do not break the live backend.
- HvA base requirement: the shared data stays correct (no split brain) through the change.

### Main question

How do we build and verify that the City Sim API scales automatically under load and back down again, while the one shared database stays safe and the live backend keeps serving during the work?

### Method

We built the stack as Kubernetes manifests next to the live Compose backend. We expose the API on a NodePort so the test runs in parallel without touching port 80. We then drive load and watch the autoscaler, test recovery by deleting a pod, and (at cutover) verify the data migrates with matching row counts. We report measured numbers, not estimates.

---

## Chapter 1 - What we built

Everything lives in `backend/k8s/` and `backend/tests/autoscaling/` and runs alongside the existing `docker-compose.yml`, which we did not touch.

| File | What it does |
|------|--------------|
| `backend/k8s/00-namespace.yaml` | the `citysim` namespace |
| `backend/k8s/01-config.yaml` | non-secret DB settings (user, db, host) |
| `backend/k8s/02-secret.example.yaml` | template for the DB password (real one gitignored) |
| `backend/k8s/03-postgres.yaml` | the single database: StatefulSet (replicas 1) + headless Service + PVC |
| `backend/k8s/04-api.yaml` | the API: Deployment + NodePort Service + wait-for-db init container |
| `backend/k8s/05-ingress.yaml` | Traefik Ingress for the port-80 cutover |
| `backend/k8s/06-hpa.yaml` | the HorizontalPodAutoscaler (the autoscaling) |
| `backend/tests/autoscaling/load.yaml` | in-cluster load generator that triggers the autoscaler |
| `backend/k8s/README.md` | the full deploy, test, cutover, rollback and multi-node runbook |

### Three things the build made honest versus the Design

The Design is a sketch. Building it surfaced three real points the reference K3s guide did not cover, because it used a public image and a domain name.

1. **K3s cannot see Docker's images.** K3s uses containerd, not Docker, so our own `citysim-api` image is not visible to it by default. The build step imports it: `docker save citysim-api:0.1 | sudo k3s ctr images import -`, and the Deployment uses `imagePullPolicy: IfNotPresent` so it never tries to pull from the internet. For more nodes we use the GitLab registry instead.
2. **Port 80 is taken during the test.** The live backend owns port 80, and K3s's Traefik wants it too. So for the parallel test we install K3s with Traefik disabled and expose the API on NodePort 30080. Traefik and the Ingress come in only at the cutover, after the live stack is stopped.
3. **The tiles use the IP, not a domain.** The reference guide routed on a hostname. Our ESP32 tiles are hard-coded to the Pi's IP, so the Ingress is hostless and answers on the raw IP.

### The database stays single by construction

As designed, only the API is a scaling target. The database is a StatefulSet with `replicas: 1`, no autoscaler points at it, and it uses a ReadWriteOnce volume so only one node can ever mount the data. This is the built guard against split brain in the shared backend.

---

## Chapter 2 - Conventions we followed

- **Manifests validated:** every YAML file parses cleanly (checked with a YAML parser before commit).
- **Self-documenting config:** each manifest has a comment block at the top stating its purpose and the safety rule, so a colleague reads the intent before the detail.
- **No secrets in git:** the real password lives in `02-secret.yaml`, which is gitignored; only the placeholder template is committed, the same rule as the `.env` file.
- **Namespaced:** all objects live in the `citysim` namespace, so the stack is isolated and easy to remove in one command.
- **Commits:** Conventional Commits (`feat:`, `fix:`, `docs:`).
- **Reuse:** the design and failure modes are reused from the Learning Goal 1 Analysis and Advise, not rewritten.

---

## Chapter 3 - How we test it

Each test ties back to a requirement.

- **Autoscaling test (Gerald: scale up and down).** Start the in-cluster load Job (`tests/autoscaling/load.yaml`), which runs several workers against a real database-backed endpoint so CPU rises. Watch `kubectl get hpa -n citysim -w` and the pod count climb from 2 toward 5. When the load stops, watch it fall back to 2 after the scale-down window. We record the peak replica count and the times.
- **Recovery test (Mats: sustainable, LG1 carry-over).** While load runs, delete one API pod with `kubectl delete pod`. The Service should keep serving from the others with no failed request, and the Deployment recreates the missing pod. This is the orchestrator version of the Learning Goal 1 failover test.
- **Hung-process test (LG1 failure mode 2).** The liveness probe on `/health` should restart a pod that stops answering. We confirm the probe is configured and fires.
- **Data-migration check (HvA base: no data loss at cutover).** At cutover we restore the live backup into the K3s database and confirm the row counts match what the live database had (sensor readings and parking spots), so the shared data survives the move intact.
- **Multi-node failover (optional, if a second node is available).** With two nodes, drain or stop one and confirm the API keeps serving from the other.

The commands are in `backend/k8s/README.md` and `backend/tests/autoscaling/README.md`. The tests run on the Pi, because the Pi's ARM hardware is the real target.

---

## Chapter 4 - Test results

These run on the Raspberry Pi. They are filled in after the Pi run.

**Autoscaling result** (load Job, watch the HPA):

| Metric | Value |
|--------|-------|
| Start replicas | `[to be filled after Pi test]` |
| Peak replicas under load | `[to be filled after Pi test]` |
| Time to first scale-up | `[to be filled after Pi test]` |
| Time to scale back to 2 after load stopped | `[to be filled after Pi test]` |
| Errors during the run | `[to be filled after Pi test]` |
| CPU target used (50%, or lowered for the demo) | `[to be filled after Pi test]` |

**Recovery result** (delete one API pod under load):

| Metric | Value |
|--------|-------|
| Failed requests during the kill | `[to be filled after Pi test]` |
| Pod recreated automatically | `[to be filled after Pi test]` |
| Time to back at desired replicas | `[to be filled after Pi test]` |

**Data-migration result** (at cutover):

| Metric | Before (live Compose DB) | After (K3s DB) |
|--------|--------------------------|----------------|
| sensor_readings rows | `[to be filled after Pi test]` | `[to be filled after Pi test]` |
| parking_spots rows | `[to be filled after Pi test]` | `[to be filled after Pi test]` |

**Multi-node** (only if a second node was available): `[to be filled after Pi test, or note single-node]`

---

## Chapter 5 - User test

The city has to stay online to the people who use it, so we end with a team test.

The setup: with the stack running, one team member starts the load and another deletes an API pod, while the others watch the dashboard and their tiles. The question we ask: did you notice anything go wrong, and did you see the city add capacity?

> User test outcome: `[to be filled after Pi test]`

---

## Conclusion

The autoscaling stack is built, validated, and ready to run on the Pi. It implements the Design: the stateless API is a Deployment scaled by a HorizontalPodAutoscaler between two and five pods behind one Service, and the database is a single StatefulSet pod with its own volume that is never scaled, so the shared data keeps one source of truth and cannot split brain. Building it forced three honest points over the Design: the image is imported into containerd because K3s does not read Docker's store, the API is tested on a NodePort because the live backend holds port 80, and the Ingress is hostless because the tiles use the IP. The work runs entirely next to the live backend, so the team is never at risk until a proven cutover with a backup and a rollback. The measured autoscaling, recovery, and migration numbers are added after the Pi run; once they are in, this closes Learning Goal 1 recommendation #5 with a working, tested automatic-scaling backend.

---

## Recommendation

1. Run the parallel test on the Pi first (NodePort 30080) and fill the result tables before any cutover.
2. Cut over to port 80 only with the team present, a fresh Learning Goal 2 backup taken, and the rollback (Compose) ready, exactly as the runbook describes.
3. Keep database high availability (replicated storage or streaming replication) as future work with its own analysis, because it is the one change that can reintroduce split brain.
4. Hand `backend/k8s/README.md` to maintenance (beheer) as the pilot runbook for the cutover.

---

## References

- Kubernetes. (2024a). *Horizontal Pod Autoscaling* [Online]. Retrieved June 2026, from https://kubernetes.io/docs/tasks/run-application/horizontal-pod-autoscale/
- Kubernetes. (2024b). *StatefulSets* [Online]. Retrieved June 2026, from https://kubernetes.io/docs/concepts/workloads/controllers/statefulset/
- K3s. (2024). *K3s: Lightweight Kubernetes* [Online]. Retrieved June 2026, from https://docs.k3s.io/
- Khajehfard, M. (2026). *Design: Kubernetes autoscaling architecture* [Design deliverable]. The Embedded Alliance.
- Khajehfard, M. (2026). *Realise: Backend clustering implementation* [Realise deliverable, Learning Goal 1]. The Embedded Alliance.
- Otten, M. (2026). *Sprint 3 and Sprint 4 feedback* [Verbal feedback, offline]. Hogeschool van Amsterdam.

---

## Appendix

### Appendix A - Command cheat sheet

```bash
# build + load the image into K3s (containerd)
docker build -t citysim-api:0.1 backend/
docker save citysim-api:0.1 | sudo k3s ctr images import -

# deploy (parallel phase, live backend untouched)
kubectl apply -f backend/k8s/00-namespace.yaml
kubectl apply -f backend/k8s/01-config.yaml -f backend/k8s/02-secret.yaml
kubectl apply -f backend/k8s/03-postgres.yaml -f backend/k8s/04-api.yaml -f backend/k8s/06-hpa.yaml

# test
curl http://<pi-ip>:30080/health
kubectl apply -f backend/tests/autoscaling/load.yaml
kubectl get hpa -n citysim -w
```

### Appendix B - The two tiers

| Tier | Object | Scales? |
|------|--------|---------|
| API (stateless) | Deployment + HPA | Yes, 2 to 5 |
| Database (stateful) | StatefulSet, replicas 1 | No, ever |
