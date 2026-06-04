# Realise - Kubernetes autoscaling implementation

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | June 2026 |
| **Version** | 1.1 |
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
    - Appendix A - Command cheat sheet
    - Appendix B - The two tiers
    - Appendix C - Cutover stappenplan
    - Appendix D - Use of AI

---

## Introduction

### Context

City Sim is our team's miniature smart city for the Studio Smart Cities semester at the HvA. Five tiles send their sensor data to one shared backend we maintain: FastAPI plus PostgreSQL, in containers, on one Raspberry Pi (145.92.8.137).

This is the **Realise** outcome for the autoscaling follow-up to Learning Goal 1, and it closes the Analysis, Advise, Design, Realise order for that work. The Learning Goal 1 cluster scales by hand (`--scale`); its recommendation #5 was to move to K3s with a HorizontalPodAutoscaler for **automatic** scaling. The Design drew that architecture with one rule above all: only the stateless API scales, the shared database stays single, so the shared data cannot split brain. This document builds that design and verifies it.

### Who this document is for, and why this form

We write this build report for a technical audience: the City Sim development team, that is the embedded and backend engineers, and the technical lead on the client side. We chose a detailed, internal engineering document because that audience runs the deploy, the test, and the cutover, so it needs the manifests, the commands, and the measured numbers in full rather than a summary. A public note or a teacher-facing report would drop the operational detail the people doing the cutover rely on, so it would be the wrong fit.

The document properties at the top back that choice. The author is Matin Khajehfard, the backend developer who owns the shared backend, so building and testing how it scales is his to report. The client is Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented at the mayor delivery by Mats Otten, and the client is not the audience: the client commissions the city while the engineers above build and run it. The classification stays Internal, because the report names the live Pi and its address. The version is 1.1 because this is a revision after the second round of written feedback from mister mayor Gerald Stap.

### Status of this document

The build is complete and the manifests are validated. The measured numbers from the Pi (autoscaling, recovery, cutover) are filled in after a run on the Pi, in the same way the Learning Goal 1 Realise was first written with the build and then completed with the live numbers. Sections waiting for that run are marked `[to be filled after Pi test]`.

### Requirements we test against

- Mister mayor Gerald Stap (Sprint 3): scale up and scale down on load, on its own.
- Mats Otten, the client representative (Sprint 3 and 4): sustainable, and do not break the live backend.
- HvA base requirement: the shared data stays correct (no split brain) through the change.

### Main question

How do we build and verify that the City Sim API scales automatically under load and back down again, while the one shared database stays safe and the live backend keeps serving during the work?

### Method

We built the stack as Kubernetes manifests next to the live Compose backend. We expose the API on a NodePort so the test runs in parallel without touching port 80. We then drive load and watch the autoscaler, test recovery by deleting a pod, and (at cutover) verify the data migrates with matching row counts. We report measured numbers, not estimates.

---

## Chapter 1 - What we built

We built the design as a set of Kubernetes manifests. Everything lives in `backend/k8s/` and `backend/tests/autoscaling/` and runs alongside the existing `docker-compose.yml`, which we did not touch, so the live backend keeps running while we build. The table below lists every file we added and what it does, in the order the runbook applies them.

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

The file list is the design made real: one manifest per Kubernetes object, with the single database in `03-postgres.yaml` kept apart from the scaling API in `04-api.yaml` and `06-hpa.yaml`, which is the two-tier split the design is built on. The same FastAPI application from Learning Goal 1 runs unchanged inside the API pods, packaged the same multi-file way (FastAPI, 2024); only the platform around it changed.

### Three things the build made honest versus the Design

The Design is a sketch, and building it surfaced three real points the reference K3s guide did not cover, because that guide used a public image and a domain name while we run our own image on the raw IP. We name the three points here because each one changed a line in the manifests.

1. **K3s cannot see Docker's images.** K3s uses containerd, not Docker, so our own `citysim-api` image is not visible to it by default. The build step imports it: `docker save citysim-api:0.1 | sudo k3s ctr images import -`, and the Deployment uses `imagePullPolicy: IfNotPresent` so it never tries to pull from the internet (K3s, 2024). For more nodes we use the GitLab registry instead.
2. **Port 80 is taken during the test.** The live backend owns port 80, and K3s's Traefik wants it too. So for the parallel test we install K3s with Traefik disabled and expose the API on NodePort 30080. Traefik and the Ingress come in only at the cutover, after the live stack is stopped.
3. **The tiles use the IP, not a domain.** The reference guide routed on a hostname. Our ESP32 tiles are hard-coded to the Pi's IP, so the Ingress is hostless and answers on the raw IP.

The three points share a theme: the design held, but a real Pi with a private image forced honest build choices the sketch could skip.

### The database stays single by construction

As designed, only the API is a scaling target. The database is a StatefulSet with `replicas: 1`, no autoscaler points at it, and it uses a ReadWriteOnce volume so only one node can ever mount the data (Kubernetes, 2024). The single, never-scaled database is the built guard against split brain in the shared backend, and it is the one part of the design we were most careful not to compromise while building.

---

## Chapter 2 - Conventions we followed

We kept the build to the same conventions as the rest of the backend, so the autoscaling stack reads like the code the team already maintains. The Python in the API image follows PEP 8 (Van Rossum et al., 2001) and the project coding standards in the repository's CLAUDE.md, which set type hints, English comments, conventional commits, and no hard-coded secrets (Hogeschool van Amsterdam, 2026). The list below records how those rules land on the manifests.

- **Manifests validated:** every YAML file parses cleanly (checked with a YAML parser before commit).
- **Self-documenting config:** each manifest has a comment block at the top stating its purpose and the safety rule, so a colleague reads the intent before the detail.
- **No secrets in git:** the real password lives in `02-secret.yaml`, which is gitignored; only the placeholder template is committed, the same rule as the `.env` file.
- **Namespaced:** all objects live in the `citysim` namespace, so the stack is isolated and easy to remove in one command.
- **Commits:** Conventional Commits (`feat:`, `fix:`, `docs:`).
- **Reuse:** the design and failure modes are reused from the Learning Goal 1 Analysis and Advise, not rewritten.

The conventions are not new for this stack: they are the same standards we apply to the FastAPI code and the Compose setup, which is what keeps the autoscaling work maintainable by the same team.

---

## Chapter 3 - How we test it

We test the build with one performance test and a set of behaviour checks, each tied back to a requirement. The performance test is a **load test**: it drives normal concurrent traffic at a real database-backed endpoint to see whether the autoscaler adds capacity and how the system holds up. We did not run a **soak test** (a long run to catch a memory leak) or a **stress test** (push past normal load to find the breaking point) for this stack, because the open question for autoscaling is whether scaling triggers and recovers, not the long-run or breaking-point behaviour, and because the Learning Goal 1 cluster already covered the soak case for the same FastAPI code. The behaviour checks around the load test cover recovery, the hung-process case, the data migration, and optional multi-node failover.

Each test ties back to a requirement:

- **Autoscaling test (mister mayor Gerald Stap: scale up and down).** Start the in-cluster load Job (`tests/autoscaling/load.yaml`), which runs several workers against a real database-backed endpoint so CPU rises. Watch `kubectl get hpa -n citysim -w` and the pod count climb from 2 toward 5. When the load stops, watch it fall back to 2 after the scale-down window. We record the peak replica count and the times.
- **Recovery test (Mats Otten: sustainable, LG1 carry-over).** While load runs, delete one API pod with `kubectl delete pod`. The Service should keep serving from the others with no failed request, and the Deployment recreates the missing pod. The recovery test is the orchestrator version of the Learning Goal 1 failover test.
- **Hung-process test (LG1 failure mode 2).** The liveness probe on `/health` should restart a pod that stops answering. We confirm the probe is configured and fires.
- **Data-migration check (HvA base: no data loss at cutover).** At cutover we restore the live backup into the K3s database and confirm the row counts match what the live database had (sensor readings and parking spots), so the shared data survives the move intact.
- **Multi-node failover (optional, if a second node is available).** With two nodes, drain or stop one and confirm the API keeps serving from the other.

Before the results, we set out the scenarios in one table: the steps to run, the result we expect, the result we measured, and the verdict. The "Actual result" cell holds the measured value once a run is done, and keeps the `[to be filled after Pi test]` placeholder where no number is measured yet, so the table is honest about what is still open.

| # | Scenario | Steps | Expected result | Actual result | Pass/Fail |
|---|----------|-------|-----------------|---------------|-----------|
| 1 | Scale up under load | Apply `load.yaml`, watch `kubectl get hpa -n citysim -w` | Replicas climb from 2 toward 5 as CPU passes the target | `[to be filled after Pi test]` | `[to be filled after Pi test]` |
| 2 | Scale back down | Stop the load Job, wait the scale-down window | Replicas fall back to 2 after the calm window | `[to be filled after Pi test]` | `[to be filled after Pi test]` |
| 3 | Recovery under load | `kubectl delete pod` on one API pod while load runs | No failed request; Deployment recreates the pod | `[to be filled after Pi test]` | `[to be filled after Pi test]` |
| 4 | Hung-process restart | Confirm the `/health` liveness probe fires on a non-answering pod | Pod is killed and recreated by the probe | `[to be filled after Pi test]` | `[to be filled after Pi test]` |
| 5 | Data migration at cutover | Restore the live backup into the K3s database, compare row counts | `sensor_readings` and `parking_spots` counts match the live database | `[to be filled after Pi test]` | `[to be filled after Pi test]` |
| 6 | Multi-node failover (optional) | With two nodes, drain or stop one node | API keeps serving from the other node | `[to be filled after Pi test, or note single-node]` | `[to be filled after Pi test]` |

The table reads as the plan we run on the Pi: scenarios 1 and 2 prove autoscaling, scenario 3 proves failover, scenario 4 proves the hung-process guard, scenario 5 proves the lossless cutover, and scenario 6 proves machine-level failover when a second node is available. The measured numbers go into the results tables in Chapter 4. The commands are in `backend/k8s/README.md` and `backend/tests/autoscaling/README.md`, and the tests run on the Pi, because the Pi's ARM hardware is the real target.

---

## Chapter 4 - Test results

### Deployment constraint: the demo Pi has too little memory for K3s

When we went to run the test on the live Raspberry Pi (2026-06-04) we hit a real hardware limit and recorded it as a finding, because it shapes the result.

- The demo Pi has about **906 MB of total RAM**. During demo preparation the live Docker stack (PostgreSQL + two API replicas + NGINX) already used about **440 MB**, leaving roughly **467 MB free**.
- A K3s server needs about **512 MB on its own**, before any workload, and our stack then adds a PostgreSQL pod and two to five API pods on top.
- Running K3s next to the live Docker cluster would exceed the Pi's memory and trigger the kernel OOM killer, which could kill the live backend the whole team depends on for the demo.

So we deliberately did **not** install K3s on the demo Pi. This is itself a result that matches the Design: autoscaling needs spare capacity to scale into, and a single 1 GB Pi that is already serving the city has none. The production path is therefore the multi-node setup the Design describes (several nodes with more RAM), not one small Pi. The manifests, the HPA, and the load test are built and validated; the measured numbers below wait on a node with adequate memory (about 2 GB or more), for example a laptop VM or a larger Pi, run next to nothing else.

The result tables below map one-to-one onto the scenarios in Chapter 3 and stay open until that run on adequate hardware.

The first table holds the autoscaling numbers from scenarios 1 and 2, captured by watching the HPA while the load Job runs and then stops:

| Metric | Value |
|--------|-------|
| Start replicas | `[to be filled after Pi test]` |
| Peak replicas under load | `[to be filled after Pi test]` |
| Time to first scale-up | `[to be filled after Pi test]` |
| Time to scale back to 2 after load stopped | `[to be filled after Pi test]` |
| Errors during the run | `[to be filled after Pi test]` |
| CPU target used (50%, or lowered for the demo) | `[to be filled after Pi test]` |

The second table holds the recovery numbers from scenario 3, captured by deleting one API pod while load runs:

| Metric | Value |
|--------|-------|
| Failed requests during the kill | `[to be filled after Pi test]` |
| Pod recreated automatically | `[to be filled after Pi test]` |
| Time to back at desired replicas | `[to be filled after Pi test]` |

The third table holds the data-migration counts from scenario 5, captured at cutover by comparing the live database with the K3s database:

| Metric | Before (live Compose DB) | After (K3s DB) |
|--------|--------------------------|----------------|
| sensor_readings rows | `[to be filled after Pi test]` | `[to be filled after Pi test]` |
| parking_spots rows | `[to be filled after Pi test]` | `[to be filled after Pi test]` |

The multi-node result from scenario 6 (only if a second node was available) is: `[to be filled after Pi test, or note single-node]`.

We leave every cell as a placeholder on purpose, because we will not write a number we did not measure. The numbers land here straight from the Pi run once the stack runs on a node with enough memory, exactly as the Learning Goal 1 Realise was first written with the build and then completed with its real run.

---

## Chapter 5 - User test

The city has to stay online to the people who use it, so we end with a team test.

The setup: with the stack running, one team member starts the load and another deletes an API pod, while the others watch the dashboard and their tiles. The question we ask: did you notice anything go wrong, and did you see the city add capacity?

> User test outcome: `[to be filled after Pi test]`

---

## Conclusion

We set out to build and verify that the City Sim API scales automatically under load and back down again, while the one shared database stays safe and the live backend keeps serving during the work. The autoscaling stack is built, validated, and ready to run on the Pi. It implements the Design: the stateless API is a Deployment scaled by a HorizontalPodAutoscaler between two and five pods behind one Service, and the database is a single StatefulSet pod with its own volume that is never scaled, so the shared data keeps one source of truth and cannot split brain. Building it forced three honest points over the Design: the image is imported into containerd because K3s does not read Docker's store, the API is tested on a NodePort because the live backend holds port 80, and the Ingress is hostless because the tiles use the IP. The work runs entirely next to the live backend, so the team is never at risk until a proven cutover with a backup and a rollback. Going to the live Pi surfaced a real constraint (Chapter 4): the 1 GB demo Pi has no spare memory to run K3s next to the city, so the autoscaling numbers wait on a node with about 2 GB or more. So the answer to the main question is that the automatic-scaling backend is built and verifiable by the scenarios in Chapter 3, the database stays single by construction so the shared data cannot split brain, and the parallel rollout keeps the live backend serving throughout; once the measured numbers are filled on adequate hardware, this closes Learning Goal 1 recommendation #5 with a working, tested automatic-scaling backend.

---

## Recommendation

1. Run the parallel test on the Pi first (NodePort 30080) and fill the result tables before any cutover.
2. Cut over to port 80 only with the team present, a fresh Learning Goal 2 backup taken, and the rollback (Compose) ready, exactly as the runbook describes.
3. Keep database high availability (replicated storage or streaming replication) as future work with its own analysis, because it is the one change that can reintroduce split brain.
4. Hand `backend/k8s/README.md` to maintenance (beheer) as the pilot runbook for the cutover.

---

## References

- FastAPI. (2024). *Bigger applications: Multiple files*. [Online]. Retrieved June 5, 2026, from https://fastapi.tiangolo.com/tutorial/bigger-applications/
- Hogeschool van Amsterdam. (2026). *City Sim project brief: base requirements (General, Embedded, Back-end)*. Studio Smart Cities, HvA. [Print].
- K3s. (2024). *K3s: Lightweight Kubernetes*. [Online]. Retrieved June 5, 2026, from https://docs.k3s.io/
- Khajehfard, M. (2026). *Design: Kubernetes autoscaling architecture* [Design deliverable]. The Embedded Alliance. [Print].
- Khajehfard, M. (2026). *Realise: Backend clustering implementation* [Realise deliverable, Learning Goal 1]. The Embedded Alliance. [Print].
- Kubernetes. (2024). *Horizontal Pod Autoscaling*. [Online]. Retrieved June 5, 2026, from https://kubernetes.io/docs/tasks/run-application/horizontal-pod-autoscale/
- Kubernetes. (2024). *StatefulSets*. [Online]. Retrieved June 5, 2026, from https://kubernetes.io/docs/concepts/workloads/controllers/statefulset/
- Otten, M. (2026, May 20). *Sprint 4 mayor delivery feedback (Mats)*. Hogeschool van Amsterdam. [Verbal, offline].
- Stap, G. (2026, May 20). *Sprint 4 feedback on Smart City deliverables (mister mayor Gerald Stap)*. Hogeschool van Amsterdam. [Verbal, offline].
- Van Rossum, G., Warsaw, B., & Coghlan, N. (2001). *PEP 8: Style guide for Python code*. [Online]. Retrieved June 5, 2026, from https://peps.python.org/pep-0008/

---

## Appendix

### Appendix A - Command cheat sheet

The commands below build the image, deploy the stack in the parallel phase, and start the autoscaling test, in the order a colleague runs them:

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

The table restates the one safety rule of the build, so it is in the report once more next to the commands:

| Tier | Object | Scales? |
|------|--------|---------|
| API (stateless) | Deployment + HPA | Yes, 2 to 5 |
| Database (stateful) | StatefulSet, replicas 1 | No, ever |

The API tier scales because it is stateless, and the database tier never does because it is the one writer, which is the line that keeps the shared data from splitting brain.

### Appendix C - Cutover stappenplan

When the autoscaling test passes and the team agrees to cut over to port 80, run the steps below in order. The cutover is reversible: if any step misbehaves, stop and follow the rollback at the end.

1. Take a fresh Learning Goal 2 backup of the live database (`backend/scripts/backup.sh`) and confirm it with `backend/scripts/check_backup.sh`.
2. Record the live row counts (`sensor_readings`, `parking_spots`) so they can be compared after the migration.
3. Stop the live Compose stack (`docker compose down`, without `-v` so the data volume stays), which frees port 80 and keeps the original data.
4. Restore the backup into the K3s database with `backend/scripts/restore.sh` and verify the row counts match step 2.
5. Enable Traefik and apply the Ingress (`kubectl apply -f backend/k8s/05-ingress.yaml`) so the city answers on the raw IP again.
6. Smoke-test the city on port 80: hit `/health`, check the dashboard, and confirm a tile reading lands in the database.
7. Rollback if needed: release port 80 from Traefik, then bring the Compose stack back up (`docker compose up -d`). The original backend returns with its original data, because the live volume was never touched.

### Appendix D - Use of AI

We used an AI assistant (Claude) as a writing aid for this document. It helped restructure the text to the agreed feedback standard, check the APA formatting and the in-text citations, and rephrase passages for clarity. It did not produce the engineering work or the measured results: the architecture, the choices, the code, and the test numbers are our own and were reviewed by the author, who is responsible for the content. The Pi measurements are our real runs from the project hardware, so the `[to be filled after Pi test]` placeholders stay empty until we measure them, and no number in this document is generated.
