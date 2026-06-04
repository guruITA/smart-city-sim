# Design - Kubernetes autoscaling architecture

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | June 2026 |
| **Version** | 0.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Design (third of the four outcomes: Analysis, Advise, Design, Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - Requirements translated to design
3. Chapter 2 - The architecture
4. Chapter 3 - Keeping the shared database safe (no split brain)
5. Chapter 4 - Self-healing and sustainability
6. Chapter 5 - A safe rollout next to the live backend
7. Chapter 6 - Growing to more nodes
8. Chapter 7 - Capacity and tuning choices
9. Conclusion
10. Recommendation
11. References
12. Appendix

---

## Introduction

### Context

City Sim is a miniature smart city our team, The Embedded Alliance, builds for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Five students each build a physical tile, and every tile sends its sensor data to one shared backend that I maintain: a FastAPI application with a PostgreSQL database, in Docker, on one Raspberry Pi on the HvA network (145.92.8.137, port 80).

In Learning Goal 1 we made that backend auto-recover. We built a Docker Compose cluster with NGINX in front of two API replicas and tested it live: it fails over with zero failed requests and restarts a crashed replica in about a second, both inside the 5 second target the mayor set. That setup scales by hand: an operator types `--scale api=3`. The Realise document for Learning Goal 1 closed with recommendation #5: for **automatic** scaling on load, move to Kubernetes (K3s) with a HorizontalPodAutoscaler. This document is the design of that recommendation.

### Where this fits in the four outcomes

This is the **Design** outcome for the autoscaling follow-up to Learning Goal 1. The Analysis (failure modes) and Advise (technology choices) from Learning Goal 1 still hold and are reused here: the same failure modes, the same preference for native tooling. What changes is the platform, from Docker Compose to K3s, so this design focuses on the new architecture and, above all, on doing it without putting the shared backend at risk.

### Requirements we design against

We carry the same requirements through so the design stays traceable.

- Gerald (Sprint 3): load balancing, failover, scale up and scale down. The "scale up and down on its own" part is what is still open after Learning Goal 1.
- Mats (Sprint 3 and 4): keep it sustainable, design a durable system, do not break what works.
- HvA base requirement: the backend is shared by all five tiles, so its data must stay correct and available. A scaling change must not corrupt or split that shared state.
- Project constraint: the ESP32 tiles are hard-coded to the Pi's IP address, not a domain name, so the public entry must answer on the raw IP.

### Design question

This is a design question, and the choice is deliberately not baked into the question: **How do we design a backend deployment that scales the API automatically under load, and can grow to more machines, without putting the one shared database at risk of corruption or split brain?**

### Method

We start from the Learning Goal 1 architecture and the K3s autoscaling guide the team adopted as a reference. We separate the backend into a stateless tier and a stateful tier, design autoscaling for the stateless tier only, and design explicit guards for the stateful tier. We then design the rollout so the live backend keeps serving until a proven cutover. We justify each choice against the requirements above.

---

## Chapter 1 - Requirements translated to design

We turn each requirement into one concrete design decision, so the architecture is a direct answer to the brief and not a copy of the reference guide.

| Requirement | Design decision |
|-------------|-----------------|
| Scale up and down on load, on its own (Gerald) | A HorizontalPodAutoscaler on the API Deployment, scaling on CPU between a minimum and a maximum |
| Load balancing (Gerald) | A Kubernetes Service in front of the API pods spreads requests across them |
| Failover (Gerald, LG1) | The Service routes only to ready pods; a failed pod is dropped and rescheduled |
| Sustainable, durable (Mats) | Liveness and readiness probes on `/health`, self-healing reschedules, fixed resource requests and limits |
| Shared data stays correct (HvA base) | The database is one pod, never scaled, with its own ReadWriteOnce volume |
| Tiles use the IP, not a domain (project) | A hostless Ingress (and a NodePort for testing) so the entry answers on the raw IP |
| Do not break the live backend (Mats) | K3s runs next to Compose; test on a NodePort; cut over to port 80 only when proven |

---

## Chapter 2 - The architecture

The backend splits into two tiers.

- **Stateless tier (scales):** the FastAPI application. Every pod is identical and keeps no local state. All request state lives in the database, so any pod can serve any request, and we can add or remove pods at will.
- **Stateful tier (fixed):** the PostgreSQL database. It holds the one copy of the city's data. It is a single pod with its own persistent volume.

The pieces that make this run on K3s:

| Component | Kubernetes object | Role |
|-----------|-------------------|------|
| Database | StatefulSet (replicas 1) + headless Service + PersistentVolumeClaim | the single source of truth, with stable storage and DNS |
| API | Deployment + Service | the stateless backend, load balanced behind one internal address |
| Autoscaler | HorizontalPodAutoscaler | adds and removes API pods on CPU load |
| Test entry | NodePort (30080) | reach the API in parallel with the live backend, off port 80 |
| Live entry | Ingress (Traefik, hostless) | serve on port 80 at cutover, on the raw IP |
| Config and secret | ConfigMap + Secret | DB settings and password, kept out of the image |

> Diagram placeholder: insert a topology diagram here (for example `diagrams/k3s_autoscaling_topology.svg`). It should show: clients and tiles on the left, the Ingress or NodePort entry, the API Service spreading over two-to-five API pods, the HPA watching the API pods and changing their count, and one pinned database pod with its volume that every API pod reads and writes.

Data flow: a tile or the dashboard calls the entry, the Service load balances across the ready API pods, each pod reads or writes the one database through the `postgres` Service name. The autoscaler watches the API pods' CPU and changes their count. The database is outside that loop on purpose.

---

## Chapter 3 - Keeping the shared database safe (no split brain)

This is the core of the design, because it is where a careless scaling change does the most damage.

**What split brain is.** Split brain is when two parts of a system both believe they are in charge of the same data and write to it independently, so the data diverges and there is no longer one correct version. For a shared backend that five tiles depend on, that is the worst outcome: the parking tile and the dashboard could read different truths.

**How naive scaling causes it.** The tempting mistake is to scale the database the same way as the API, for example setting the database to two or more pods so it is "more available". Two PostgreSQL pods writing to their own storage are two primaries, and they diverge. Two pods sharing one read-write-many volume corrupt each other's files. Either way you get split brain or corruption.

**Our guard.** We never scale the stateful tier. Concretely:

- The database is a StatefulSet with `replicas: 1`, and that number is fixed.
- No HorizontalPodAutoscaler points at the database. Only the API Deployment is a scaling target.
- The database uses a ReadWriteOnce PersistentVolumeClaim, so only one node can mount the data at a time. Even if the pod moves to another node, the volume detaches first and attaches once, so there is never a second writer.
- Availability of the database is handled by **backups** (Learning Goal 2), not by cloning the database. Recovering from a backup keeps a single source of truth; running a second live database would break it.

So the system scales where it is safe to scale (the stateless API) and stays single where scaling is dangerous (the database). Many API pods all talk to the one database, which is exactly one writer of record. That is the design answer to the shared-backend risk in the design question.

---

## Chapter 4 - Self-healing and sustainability

Mats asked for a durable system, not a one-off fix. The design reuses the Learning Goal 1 Analysis, where the biggest gap was failure mode 2: a process that is up but hung, which a simple restart policy cannot catch. K3s closes that gap at the orchestrator level.

- **Liveness probe on `/health`.** If a pod stops answering `/health`, Kubernetes kills and recreates it. This is the orchestrator version of the Docker healthcheck from Learning Goal 1, and it catches the hung-process case that `restart: always` missed.
- **Readiness probe on `/health`.** A pod receives traffic only once it is ready, so a starting or unhealthy pod is kept out of the load balancer instead of serving errors.
- **Self-healing.** If a pod or a whole node is lost, the Deployment recreates the missing API pods, on another node when there is one. The desired replica count is restored automatically.
- **Ordered start.** An init container waits for the database to accept connections before the API starts, so pods do not crash-loop on first boot while PostgreSQL is still initialising.
- **Resource requests and limits.** Each API pod requests a small, fixed amount of CPU and memory and is capped, which both protects the small Pi and gives the autoscaler the CPU baseline it measures against.

---

## Chapter 5 - A safe rollout next to the live backend

The live backend serves the whole team on port 80, so the design treats the rollout itself as a requirement, not an afterthought. The principle is the same one we used in Learning Goal 1, where we tested the cluster on port 8080 before taking port 80.

- **Parallel phase.** K3s runs next to the running Compose stack. K3s's Traefik is disabled at install so it does not fight for port 80, and the API is exposed on NodePort 30080. The team keeps using `:80`; we test on `:30080`. The live backend is never touched.
- **Cutover phase.** Only when autoscaling is proven and the team agrees do we move to port 80. The order is: take a database backup, stop the Compose stack (which frees port 80 and keeps the data volume), restore the backup into the K3s database so no data is lost, verify the row counts match, then enable Traefik and apply the Ingress so the city answers on the raw IP again.
- **Rollback.** The live data volume is never touched during the parallel phase, so if the cutover misbehaves we release port 80 from Traefik and bring the Compose stack back up. The original backend returns with its original data.

This design carries the database migration explicitly, because the K3s database starts on a new volume. Tying the cutover to the Learning Goal 2 backup is what makes the move lossless and reversible.

---

## Chapter 6 - Growing to more nodes

The same design grows from one machine to several with no change to the manifests, which answers the "more nodes" goal.

- **Adding a node** is one command on a new Debian or Pi machine that joins it to the K3s server. The API pods then spread across all nodes automatically, giving real capacity and machine-level failover: losing one node no longer takes the API down, because pods run on the others.
- **The database stays pinned.** With the default node-local storage the database stays on the node that holds its data, which keeps the single-writer guarantee from Chapter 3. The trade-off is that losing that one node takes the database down until the node returns, and backups are the recovery path. If the city later needs the database to survive a node failure, the design point to change is the storage class (for example Longhorn for replicated storage), not the database topology, so we never introduce a second writer.
- **The image must reach every node.** On one node we import the image into containerd by hand. For several nodes the design uses the GitLab Container Registry so every node pulls the same image.

So multi-node adds availability for the stateless tier while the stateful tier keeps its single-writer safety. The split-brain guard holds at any node count.

---

## Chapter 7 - Capacity and tuning choices

We tune the autoscaler to the real hardware and justify the numbers, rather than copy the reference guide's values.

- **maxReplicas = 5.** The reference guide used ten. Our target is a small ARM Raspberry Pi, and each API pod uses about 80 MB. Five API pods plus the database plus the system is a safe ceiling for this Pi. The number is meant to be raised on bigger nodes, not kept at a value the Pi cannot run.
- **minReplicas = 2.** This keeps the Learning Goal 1 guarantee: at least two pods are always up, so one failing pod never takes the city down.
- **CPU target 50%.** The autoscaler adds pods when average CPU passes half of the requested CPU. For a live demo on the Pi we note a lower value (about 25 to 30) makes the scale-up visible under light load.
- **Scale up fast, scale down slow.** Scaling up reacts immediately so a traffic spike gets capacity quickly. Scaling down waits two minutes of calm before removing pods, so a short dip does not cause flapping (pods added and removed over and over), which is the sustainable behaviour Mats asked for.

---

## Conclusion

This answers the design question. We design the backend as two tiers: a stateless FastAPI tier that the HorizontalPodAutoscaler scales automatically on CPU between two and five pods behind one Service, and a single PostgreSQL tier that is never scaled and owns one ReadWriteOnce volume, so the shared data keeps one source of truth and cannot split brain. Self-healing comes from liveness and readiness probes on `/health`, which also close the hung-process gap the Learning Goal 1 Analysis found. The rollout is designed to run next to the live backend on a NodePort and cut over to port 80 only after a proven test, with a backup-based data migration and a one-command rollback. The same manifests grow to more nodes, where the stateless tier spreads for real failover while the database stays pinned and single. The design meets Gerald's scale-up-and-down requirement and Mats's sustainability and do-no-harm requirements at the same time.

---

## Recommendation

For the move from this design to the build (Realise) and on to maintenance:

1. Build and test the stack in the parallel phase first, on the NodePort, so the live backend keeps serving. Prove the autoscaling (replicas climb under load and fall back) before any cutover.
2. Treat the cutover as a change with a backup and a rollback, as designed in Chapter 5. Never cut over without a fresh Learning Goal 2 backup.
3. Keep database high-availability (replicated storage or streaming replication) out of scope for now and note it as future work. It is the one area that, done carelessly, reintroduces split brain, so it deserves its own analysis rather than being bolted on.
4. Hand the runbook (`backend/k8s/README.md`) to maintenance (beheer) as a pilot, so the team can run the cutover as a controlled, repeatable step.

---

## References

- Gerald, S. (2026). *Sprint 3 mayor delivery feedback* [Verbal feedback, offline]. Hogeschool van Amsterdam.
- Kubernetes. (2024a). *Horizontal Pod Autoscaling* [Online]. Retrieved June 2026, from https://kubernetes.io/docs/tasks/run-application/horizontal-pod-autoscale/
- Kubernetes. (2024b). *StatefulSets* [Online]. Retrieved June 2026, from https://kubernetes.io/docs/concepts/workloads/controllers/statefulset/
- K3s. (2024). *K3s: Lightweight Kubernetes* [Online]. Retrieved June 2026, from https://docs.k3s.io/
- Khajehfard, M. (2026). *Realise: Backend clustering implementation* [Realise deliverable, Learning Goal 1]. The Embedded Alliance.
- Nygard, M. T. (2018). *Release It! Design and deploy production-ready software* (2nd ed.) [Print]. Pragmatic Bookshelf.
- Otten, M. (2026). *Sprint 3 and Sprint 4 feedback on Smart City deliverables* [Verbal feedback, offline]. Hogeschool van Amsterdam.

---

## Appendix

### Appendix A - Manifest overview

| File | Purpose |
|------|---------|
| `backend/k8s/00-namespace.yaml` | the `citysim` namespace |
| `backend/k8s/01-config.yaml` | non-secret database settings |
| `backend/k8s/02-secret.example.yaml` | template for the database password |
| `backend/k8s/03-postgres.yaml` | single database: StatefulSet, headless Service, PVC |
| `backend/k8s/04-api.yaml` | API Deployment, NodePort Service, wait-for-db init |
| `backend/k8s/05-ingress.yaml` | Traefik Ingress for the port-80 cutover |
| `backend/k8s/06-hpa.yaml` | the HorizontalPodAutoscaler |
| `backend/tests/autoscaling/load.yaml` | in-cluster load generator for the autoscaling test |

### Appendix B - Why only the API scales

| Tier | Object | Scales? | Why |
|------|--------|---------|-----|
| API (stateless) | Deployment + HPA | Yes, 2 to 5 | holds no local state, any pod serves any request |
| Database (stateful) | StatefulSet, replicas 1 | No, ever | one writer of record; scaling it splits the shared data |
