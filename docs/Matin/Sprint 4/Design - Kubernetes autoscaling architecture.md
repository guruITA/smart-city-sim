# Design - Kubernetes autoscaling architecture

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | June 2026 |
| **Version** | 1.1 |
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
    - Appendix A - Manifest overview
    - Appendix B - Why only the API scales
    - Appendix C - Use of AI

---

## Introduction

### Context

City Sim is a miniature smart city our team, The Embedded Alliance, builds for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Five students each build a physical tile, and every tile sends its sensor data to one shared backend that we maintain: a FastAPI application with a PostgreSQL database, in Docker, on one Raspberry Pi on the HvA network (145.92.8.137, port 80).

In Learning Goal 1 we made that backend auto-recover. We built a Docker Compose cluster with NGINX in front of two API replicas and tested it live: it fails over with zero failed requests and restarts a crashed replica in about a second, both inside the 5 second target the client asked for. That setup scales by hand: an operator types `--scale api=3`. The Realise document for Learning Goal 1 closed with recommendation #5: for **automatic** scaling on load, move to Kubernetes (K3s) with a HorizontalPodAutoscaler. This document is the design of that recommendation.

### Who this document is for, and why this form

We write this document for a technical audience: the City Sim development team, meaning the embedded and backend engineers who build the tiles and the shared backend, and the technical lead on the client side. We chose a detailed, internal engineering document on purpose, because that audience implements and maintains the backend, so it needs the manifests, the probes, and the safety rules in full. A short public summary or a teacher-facing report would hide exactly the detail that the people running the cutover depend on, so it would be the wrong form for this reader.

The document properties at the top support that choice. The author is Matin Khajehfard, the backend developer who owns the shared backend, so the design of how that backend scales is his to write. The client is Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented at the mayor delivery by Mats Otten; the client is not the same as the audience, because the client commissions the city while the engineers above build and run it. We keep the classification Internal, because the design names internal hosts and the live Pi's address and is meant for the team, not the public. The version is 1.1 because this is a revision after the second round of written feedback from mister mayor Gerald Stap, the teacher who reviews our Smart City deliverables.

### Where this fits in the four outcomes

This is the **Design** outcome for the autoscaling follow-up to Learning Goal 1, and it follows the order Analysis, Advise, Design, Realise. The Analysis (failure modes) and Advise (technology choices) from Learning Goal 1 still hold and are reused here: the same failure modes, the same preference for native tooling. What changes is the platform, from Docker Compose to K3s, so this design focuses on the new architecture and, above all, on doing it without putting the shared backend at risk.

### Requirements we design against

We carry the same requirements through so the design stays traceable.

- Mister mayor Gerald Stap (Sprint 3): load balancing, failover, scale up and scale down. The "scale up and down on its own" part is what is still open after Learning Goal 1.
- Mats Otten, the client representative (Sprint 3 and 4): keep it sustainable, design a durable system, do not break what works.
- HvA base requirement: the backend is shared by all five tiles, so its data must stay correct and available. A scaling change must not corrupt or split that shared state.
- Project constraint: the ESP32 tiles are hard-coded to the Pi's IP address, not a domain name, so the public entry must answer on the raw IP.

### Design question

This is a design question, and the choice is deliberately not baked into the question: **How do we design a backend deployment that scales the API automatically under load, and can grow to more machines, without putting the one shared database at risk of corruption or split brain?**

### Method

We start from the Learning Goal 1 architecture and the K3s autoscaling guide the team adopted as a reference. We separate the backend into a stateless tier and a stateful tier, design autoscaling for the stateless tier only, and design explicit guards for the stateful tier. We then design the rollout so the live backend keeps serving until a proven cutover. We justify each choice against the requirements above.

---

## Chapter 1 - Requirements translated to design

This chapter is the hinge between the brief and the architecture. The sub-question it answers is: how does each requirement become a concrete design decision, so the design is a direct answer to the brief and not a copy of the reference K3s guide we used as a starting point? We work through the requirements one by one and pin each to a single Kubernetes mechanism, so a reader can trace any decision back to its origin.

The table below reads the requirements down the left and our design answer on the right. Each row is a promise the architecture has to keep.

| Requirement | Design decision |
|-------------|-----------------|
| Scale up and down on load, on its own (mister mayor Gerald Stap) | A HorizontalPodAutoscaler on the API Deployment, scaling on CPU between a minimum and a maximum (Kubernetes, 2024) |
| Load balancing (mister mayor Gerald Stap) | A Kubernetes Service in front of the API pods spreads requests across them |
| Failover (mister mayor Gerald Stap, LG1) | The Service routes only to ready pods; a failed pod is dropped and rescheduled |
| Sustainable, durable (Mats Otten) | Liveness and readiness probes on `/health`, self-healing reschedules, fixed resource requests and limits |
| Shared data stays correct (HvA base) | The database is one pod, never scaled, with its own ReadWriteOnce volume (Kubernetes, 2024) |
| Tiles use the IP, not a domain (project) | A hostless Ingress (and a NodePort for testing) so the entry answers on the raw IP |
| Do not break the live backend (Mats Otten) | K3s runs next to Compose; test on a NodePort; cut over to port 80 only when proven |

Reading the table, two halves stand out. The top half is about doing more (scale, load balance, fail over) and maps cleanly onto stock Kubernetes objects, so those requirements are nearly free once we are on the platform. The bottom half is about doing no harm (keep the shared data correct, do not break the live backend), and those rows drive the harder parts of the design, the single database and the parallel rollout, which the later chapters spend most of their words on.

### Sub-conclusion

Every requirement lands on exactly one design mechanism, and the split between "do more" and "do no harm" is what shapes the rest of the architecture. The scaling requirements are answered by the HorizontalPodAutoscaler and the Service; the safety requirements are answered by the single-pod database and the parallel cutover, which the next chapters design in full.

---

## Chapter 2 - The architecture

The sub-question here is how the backend is structured on K3s so that the scaling parts and the fixed parts are cleanly separated. We answer it by splitting the backend into two tiers, then naming the Kubernetes object behind each piece, and finally showing the whole shape as one diagram.

The first move is the split itself. The backend divides into two tiers with opposite rules:

- **Stateless tier (scales):** the FastAPI application. Every pod is identical and keeps no local state. All request state lives in the database, so any pod can serve any request, and we can add or remove pods at will.
- **Stateful tier (fixed):** the PostgreSQL database. It holds the one copy of the city's data. It is a single pod with its own persistent volume, which Kubernetes manages as a StatefulSet so the storage and the network identity stay stable across restarts (Kubernetes, 2024).

The second move is to map each piece onto a concrete Kubernetes object. The table below lists the components, the object we use for each, and the role it plays, so the diagram that follows has a key.

| Component | Kubernetes object | Role |
|-----------|-------------------|------|
| Database | StatefulSet (replicas 1) + headless Service + PersistentVolumeClaim | the single source of truth, with stable storage and DNS |
| API | Deployment + Service | the stateless backend, load balanced behind one internal address |
| Autoscaler | HorizontalPodAutoscaler | adds and removes API pods on CPU load |
| Test entry | NodePort (30080) | reach the API in parallel with the live backend, off port 80 |
| Live entry | Ingress (Traefik, hostless) | serve on port 80 at cutover, on the raw IP |
| Config and secret | ConfigMap + Secret | DB settings and password, kept out of the image |

The table makes the design's spine clear: the only object that ever changes in number is the API Deployment, driven by the HorizontalPodAutoscaler, while every other object is there to keep one stable database and one stable entry point. K3s ships these objects in its lightweight Kubernetes distribution, so we get them without running a full cluster (K3s, 2024).

We model the same architecture as a UML deployment diagram, the standard notation for showing software artifacts placed on nodes and the connections between them (OMG, 2017). We pick a deployment diagram on purpose, because the question this chapter answers is where each piece runs and what talks to what, which is exactly what a deployment diagram is for.

![UML deployment diagram of the K3s autoscaling topology](k3sAutoscalingTopology.png)

*Figure 1. UML deployment diagram (OMG, 2017) of the K3s autoscaling topology. Clients and tiles on the left reach the backend through the entry (the NodePort during testing, the hostless Ingress at cutover). The API Service load balances across the two to five API pods, the HorizontalPodAutoscaler watches those pods' CPU and changes their count, and one pinned database pod with its own volume sits outside the scaling loop while every API pod reads and writes it.*

The diagram shows the data flow that ties the tiers together. A tile or the dashboard calls the entry, the Service load balances across the ready API pods, and each pod reads or writes the one database through the `postgres` Service name. The autoscaler watches the API pods' CPU and changes their count. The database sits outside that loop on purpose, which is the point the next chapter defends.

### Sub-conclusion

The architecture is two tiers with opposite rules: a stateless API tier built from a Deployment and a Service that the HorizontalPodAutoscaler is allowed to resize, and a stateful database tier built from a single-replica StatefulSet that nothing resizes. The deployment diagram shows both tiers, the entry, and the one-way scaling of the API, which sets up the safety argument that follows.

---

## Chapter 3 - Keeping the shared database safe (no split brain)

This chapter is the core of the design, because the sub-question behind it (how do we let the API scale while the one shared database can never diverge) is where a careless scaling change does the most damage. We start with what the danger is, then show how a naive design walks straight into it, then set out the guard we build instead.

Split brain is the danger we are guarding against. Split brain is when two parts of a system both believe they are in charge of the same data and write to it independently, so the data diverges and there is no longer one correct version (Kleppmann, 2017). For a shared backend that five tiles depend on, a divergence like that is the worst outcome, because the parking tile and the dashboard could then read different truths from what is supposed to be one city.

A naive scaling design walks right into split brain. The tempting mistake is to scale the database the same way as the API, for example setting the database to two or more pods so it is "more available". Two PostgreSQL pods writing to their own storage are two primaries, and they diverge. Two pods sharing one read-write-many volume corrupt each other's files. Either path ends in split brain or corruption, which is exactly the single-writer trap Kleppmann (2017) warns about, so we design the opposite.

Our guard is to never scale the stateful tier, and we enforce that in four concrete ways. The database is a StatefulSet with `replicas: 1`, and that number is fixed (Kubernetes, 2024). No HorizontalPodAutoscaler points at the database, because only the API Deployment is ever a scaling target. The database uses a ReadWriteOnce PersistentVolumeClaim, so only one node can mount the data at a time; even if the pod moves to another node, the volume detaches first and attaches once, so there is never a second writer. Availability of the database is handled by backups (Learning Goal 2), not by cloning the database, because recovering from a backup keeps a single source of truth while running a second live database would break it.

### Sub-conclusion

The system scales where scaling is safe (the stateless API) and stays single where scaling is dangerous (the database). Many API pods all talk to the one database, which is exactly one writer of record, so the shared state cannot split brain. That separation is the design answer to the shared-backend risk in the design question.

---

## Chapter 4 - Self-healing and sustainability

The sub-question here is how the design stays durable on its own, because Mats Otten asked for a sustainable system, not a one-off fix. We answer it by reusing the Learning Goal 1 Analysis, where the biggest gap was failure mode 2: a process that is up but hung, which a simple restart policy cannot catch because the process never exits (Nygard, 2018). K3s closes that gap and a few others at the orchestrator level, and the rest of this chapter walks the mechanisms that make the deployment heal itself.

The two probes do the watching. A liveness probe on `/health` lets Kubernetes kill and recreate a pod that stops answering, which is the orchestrator version of the Docker healthcheck from Learning Goal 1 and catches the hung-process case that `restart: always` missed (Kubernetes, 2024). A readiness probe on the same endpoint makes a pod receive traffic only once it is ready, so a starting or unhealthy pod is kept out of the load balancer instead of serving errors. Together the probes give the orchestrator a true picture of which pods are healthy, which is the basis for both healing and load balancing.

On top of the probes, three more mechanisms keep the deployment steady over time. Self-healing means that if a pod or a whole node is lost, the Deployment recreates the missing API pods, on another node when there is one, so the desired replica count is restored automatically. An ordered start, done with an init container that waits for the database to accept connections before the API starts, stops pods from crash-looping on first boot while PostgreSQL is still initialising. Resource requests and limits give each API pod a small, fixed amount of CPU and memory with a cap, which protects the small Pi and at the same time gives the autoscaler the CPU baseline it measures against.

### Sub-conclusion

The design is durable without a person watching it: the probes detect a hung or unready pod and the Deployment replaces lost pods on its own, the ordered start avoids boot crash-loops, and the fixed resource budget keeps the Pi safe while feeding the autoscaler its baseline. That self-healing, run by the orchestrator rather than a human, is the sustainability Mats Otten asked for.

---

## Chapter 5 - A safe rollout next to the live backend

The live backend serves the whole team on port 80, so the sub-question here is how we roll out K3s without ever taking that backend down, which means the rollout itself is a requirement and not an afterthought. The principle is the same one we used in Learning Goal 1, where we tested the cluster on port 8080 before taking port 80, and the design splits the rollout into a parallel phase, a cutover, and a rollback.

In the parallel phase, K3s runs next to the running Compose stack. K3s's bundled Traefik is disabled at install so it does not fight for port 80, and the API is exposed on NodePort 30080 instead (K3s, 2024). The team keeps using `:80` while we test on `:30080`, so the live backend is never touched during the build and the autoscaling test.

The cutover phase happens only when autoscaling is proven and the team agrees. The order is deliberate: take a database backup, stop the Compose stack (which frees port 80 and keeps the data volume), restore the backup into the K3s database so no data is lost, verify the row counts match, then enable Traefik and apply the Ingress so the city answers on the raw IP again. The design carries the database migration explicitly, because the K3s database starts on a new volume, so tying the cutover to the Learning Goal 2 backup is what makes the move lossless.

The rollback closes the loop. The live data volume is never touched during the parallel phase, so if the cutover misbehaves we release port 80 from Traefik and bring the Compose stack back up, and the original backend returns with its original data.

### Sub-conclusion

The rollout never risks the live backend: a parallel phase tests K3s on a NodePort while port 80 keeps serving, a backup-backed cutover migrates the data with matching row counts, and a rollback brings the untouched Compose stack back if anything goes wrong. That staged, reversible move is how the design honours the do-no-harm requirement.

---

## Chapter 6 - Growing to more nodes

The sub-question here is how the same design grows from one machine to several without losing the safety from Chapter 3, because the brief asks the system to scale beyond a single Pi. The good news is that the manifests do not change at all; what changes is the cluster underneath them, and three points explain how that stays safe.

Adding a node is one command on a new Debian or Pi machine that joins it to the K3s server (K3s, 2024). The API pods then spread across all nodes automatically, which gives real capacity and machine-level failover, so losing one node no longer takes the API down because pods keep running on the others.

The database stays pinned while the API spreads. With the default node-local storage the database stays on the node that holds its data, which keeps the single-writer guarantee from Chapter 3. The trade-off is that losing that one node takes the database down until the node returns, and backups are the recovery path. If the city later needs the database to survive a node failure, the design point to change is the storage class (for example Longhorn for replicated storage), not the database topology, so we never introduce a second writer.

The image has to reach every node for any of this to run. On one node we import the image into containerd by hand, and for several nodes the design uses the GitLab Container Registry so every node pulls the same image.

### Sub-conclusion

Multi-node adds availability for the stateless tier while the stateful tier keeps its single-writer safety, and the split-brain guard from Chapter 3 holds at any node count. Growth is a cluster change, not a manifest change, so the design scales out without reopening the database risk.

---

## Chapter 7 - Capacity and tuning choices

The last sub-question is how we set the autoscaler's numbers for the real hardware, because copying the reference guide's values onto a small Pi would be wrong. We tune four settings and justify each against the Pi and the requirements (Kubernetes, 2024).

The replica bounds come first. We set maxReplicas to 5 where the reference guide used ten, because our target is a small ARM Raspberry Pi and each API pod uses about 80 MB, so five API pods plus the database plus the system is a safe ceiling for this Pi; the number is meant to be raised on bigger nodes, not kept at a value the Pi cannot run. We set minReplicas to 2, which keeps the Learning Goal 1 guarantee that at least two pods are always up, so one failing pod never takes the city down.

The trigger and the timing come next. The CPU target is 50%, so the autoscaler adds pods when average CPU passes half of the requested CPU, and for a live demo on the Pi we note that a lower value (about 25 to 30) makes the scale-up visible under light load. The timing is deliberately asymmetric: scaling up reacts immediately so a traffic spike gets capacity quickly, while scaling down waits two minutes of calm before removing pods, so a short dip does not cause flapping (pods added and removed over and over), which is the sustainable behaviour Mats Otten asked for.

### Sub-conclusion

The autoscaler is tuned to this Pi, not to the reference guide: it stays between two and five pods, triggers at 50% CPU (lower for a demo), and scales up fast but down slow to avoid flapping. The numbers honour both the Learning Goal 1 floor of two pods and Mats Otten's call for sustainable behaviour.

---

## Conclusion

We set out to answer the design question: how do we design a backend deployment that scales the API automatically under load, and can grow to more machines, without putting the one shared database at risk of corruption or split brain?

We build the answer from the chapter sub-conclusions. First, every requirement maps to one design mechanism, split into a "do more" half answered by the HorizontalPodAutoscaler and the Service and a "do no harm" half answered by the single database and the parallel cutover. Second, the architecture is two tiers with opposite rules, shown in the deployment diagram: a stateless API tier the autoscaler may resize, and a single-replica database tier nothing resizes. Third, the shared database is kept safe by never scaling it, by giving it a ReadWriteOnce volume so there is only ever one writer, which is the guard against split brain. Fourth, the deployment heals itself through the `/health` probes and the Deployment's automatic replacement of lost pods, so it is sustainable without a person watching. Fifth, the rollout runs next to the live backend on a NodePort and only cuts over to port 80 after a proven test, with a backup-based migration and a rollback. Sixth, the same manifests grow to more nodes where the stateless tier spreads for real failover while the database stays pinned. Seventh, the autoscaler is tuned to this Pi, between two and five pods at a 50% CPU target, scaling up fast and down slow.

Together these answer the design question: the design scales the stateless API automatically and grows to more machines, while a single, never-scaled database with one writer keeps the shared data safe from corruption and split brain. Every requirement in the Chapter 1 table maps to a design element, so the design meets the scale-up-and-down requirement from mister mayor Gerald Stap and the sustainability and do-no-harm requirements from Mats Otten at the same time.

---

## Recommendation

For the move from this design to the build (Realise) and on to maintenance:

1. Build and test the stack in the parallel phase first, on the NodePort, so the live backend keeps serving. Prove the autoscaling (replicas climb under load and fall back) before any cutover.
2. Treat the cutover as a change with a backup and a rollback, as designed in Chapter 5. Never cut over without a fresh Learning Goal 2 backup.
3. Keep database high-availability (replicated storage or streaming replication) out of scope for now and note it as future work. It is the one area that, done carelessly, reintroduces split brain, so it deserves its own analysis rather than being bolted on.
4. Hand the runbook (`backend/k8s/README.md`) to maintenance (beheer) as a pilot, so the team can run the cutover as a controlled, repeatable step.

---

## References

- Hogeschool van Amsterdam. (2026). *City Sim project brief: base requirements (General, Embedded, Back-end)*. Studio Smart Cities, HvA. [Print].
- K3s. (2024). *K3s: Lightweight Kubernetes*. [Online]. Retrieved June 5, 2026, from https://docs.k3s.io/
- Khajehfard, M. (2026). *Realise: Backend clustering implementation* [Realise deliverable, Learning Goal 1]. The Embedded Alliance. [Print].
- Kleppmann, M. (2017). *Designing data-intensive applications*. O'Reilly Media. [Print].
- Kubernetes. (2024). *Horizontal Pod Autoscaling*. [Online]. Retrieved June 5, 2026, from https://kubernetes.io/docs/tasks/run-application/horizontal-pod-autoscale/
- Kubernetes. (2024). *StatefulSets*. [Online]. Retrieved June 5, 2026, from https://kubernetes.io/docs/concepts/workloads/controllers/statefulset/
- Nygard, M. T. (2018). *Release It! Design and deploy production-ready software* (2nd ed.). Pragmatic Bookshelf. [Print].
- Object Management Group. (2017). *OMG Unified Modeling Language (OMG UML), version 2.5.1*. [Online]. Retrieved June 5, 2026, from https://www.omg.org/spec/UML/2.5.1/
- Otten, M. (2026, May 20). *Sprint 4 mayor delivery feedback (Mats)*. Hogeschool van Amsterdam. [Verbal, offline].
- Stap, G. (2026, May 20). *Sprint 4 feedback on Smart City deliverables (mister mayor Gerald Stap)*. Hogeschool van Amsterdam. [Verbal, offline].

---

## Appendix

### Appendix A - Manifest overview

The manifests that realise this design are listed below, in the order they are applied, so a colleague can see which file owns which Kubernetes object before opening the build.

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

The list mirrors the architecture: one file per Kubernetes object, with the single database (`03-postgres.yaml`) and the scaling API (`04-api.yaml` plus `06-hpa.yaml`) kept apart, which is the two-tier split the design is built on.

### Appendix B - Why only the API scales

The table below states the rule once more in plain terms, so the single most important safety decision in the design is in one place.

| Tier | Object | Scales? | Why |
|------|--------|---------|-----|
| API (stateless) | Deployment + HPA | Yes, 2 to 5 | holds no local state, any pod serves any request |
| Database (stateful) | StatefulSet, replicas 1 | No, ever | one writer of record; scaling it splits the shared data |

The contrast is the whole point: the API scales because it is stateless, and the database never does because it is the one writer, which is the line that keeps the shared data from splitting brain.

### Appendix C - Use of AI

We used an AI assistant (Claude) as a writing aid for this document. It helped restructure the text to the agreed feedback standard, check the APA formatting and the in-text citations, and rephrase passages for clarity. It did not produce the engineering work or the design choices: the architecture, the choices, and the manifests are our own and were reviewed by the author, who is responsible for the content.
