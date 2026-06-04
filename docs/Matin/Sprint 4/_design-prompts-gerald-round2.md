# Design diagram prompts (Gerald round-2)

> Working document. Paste each prompt below into Claude (design / artifact) to
> generate the PNG, then save it in this folder (`docs/Matin/Sprint 4/`) under the
> exact filename given. The revision agents already inserted the matching embed
> and the named-notation prose in the Design docs, so the moment the PNG lands
> under that filename it renders in mkdocs.

## House style (every diagram must match the existing architecture diagram)

- Dark background `#1f1e1c`. Sans-serif font. Rounded rectangles (corner radius
  ~8px for boxes, ~16px for group containers). Thin 0.5px strokes.
- Colours by role (fill / stroke / text):
  - Clients, tiles, dashboard: `#633806` / `#ef9f27` / text `#fac775`
  - Entry + infra (NGINX, Ingress, Service, scripts): `#085041` / `#5dcaa5` / text `#9fe1cb`
  - API / application (FastAPI pods): `#0c447c` / `#85b7eb` / text `#b5d4f4`
  - Database (PostgreSQL): `#3c3489` / `#afa9ec` / text `#cecbf6`
  - Control / autoscaler / accent: terracotta `#712b13` / `#f0997b`
  - Arrows: `#BA7517`, arrowhead at the end. Use dashed arrows for control
    signals (the autoscaler changing pod count), solid arrows for request/data.
- Titles in `#faf9f5`, secondary labels in `#c2c0b6`.
- Canvas about 820 wide, landscape. Export as PNG.

---

## 1. REQUIRED - K3s autoscaling topology (UML deployment diagram)

Filename to save: **`k3sAutoscalingTopology.png`**
Goes in: Design - Kubernetes autoscaling architecture.md (replaces the old placeholder)

> Prompt to paste:

Make a clean UML deployment diagram titled "K3s autoscaling topology (UML
deployment diagram)" on a dark background `#1f1e1c`, sans-serif, rounded boxes,
thin strokes. It shows how the City Sim backend runs on Kubernetes (K3s) on one
Raspberry Pi, where only the stateless API scales and the database stays single.

Layout left to right:
1. Left: a client box "Tiles + Dashboard" (amber `#633806`/`#ef9f27`, text
   `#fac775`), subtitle "HTTP to raw IP 145.92.8.137". A solid arrow `#BA7517`
   goes right to the entry.
2. Entry box "Ingress (Traefik, hostless) + NodePort 30080" (green
   `#085041`/`#5dcaa5`), subtitle "answers on the raw IP". Solid arrow to the
   Service.
3. "API Service (ClusterIP, load balances)" (green). Solid arrows fan out to the
   API pods.
4. A dashed group box labelled "Stateless tier - scales" containing 3 identical
   pods "FastAPI pod" (blue `#0c447c`/`#85b7eb`), drawn slightly stacked, with a
   small label "min 2, max 5".
5. A control box "HorizontalPodAutoscaler (CPU 50%, 2-5 pods)" (terracotta
   `#712b13`/`#f0997b`) with a DASHED arrow `#BA7517` pointing at the API pod
   group, labelled "scales pod count".
6. A dashed group box labelled "Stateful tier - fixed (never scaled)" containing
   one box "PostgreSQL pod (StatefulSet replicas:1)" (purple
   `#3c3489`/`#afa9ec`) attached to a database cylinder "PVC ReadWriteOnce
   volume". Label under it: "one writer of record".
7. Solid arrows from every API pod to the PostgreSQL pod, labelled "read/write
   via postgres Service".
8. Wrap everything except the client box in a thin rounded container labelled
   "K3s node (Raspberry Pi)".

Keep labels short. The key visual message: many API pods, one database; the HPA
arrow only touches the API pods, never the database. Export as PNG named
`k3sAutoscalingTopology.png`.

---

## 2. REQUIRED - overrides data model (entity-relationship diagram, crow's foot)

Filename to save: **`overridesErd.png`**
Goes in: Design - Surprise feature concept.md (Chapter 3, the data model)

> Prompt to paste:

Make a clean entity-relationship diagram (ERD) in crow's foot notation titled
"City Sim data model - overrides entity (ERD, crow's foot notation, after Chen,
1976)" on a dark background `#1f1e1c`, sans-serif, rounded boxes, thin strokes.
Each entity is a box with a coloured header (the table name) and attribute rows
under it; mark the primary key with "PK" and underline it.

Entities:
1. "overrides" (purple header `#3c3489`/`#afa9ec`, text `#cecbf6`), the new
   table, with rows:
   - id : integer (PK, underlined)
   - target : string  (traffic / barrier / all)
   - command : string  (all_red / green_corridor / closed)
   - reason : string  (emergency_vehicle / roadworks)
   - active : boolean
   - created_at : datetime
   - cleared_at : datetime (null while active)
2. For context, two existing entities in the same purple style, smaller, showing
   only key columns:
   - "sensor_readings" : id (PK), tile, type, value, unit, ip, recorded_at
   - "parking_spots" : id (PK), spot_id, status, distance_cm, updated_at

Relationship and note: draw the two existing entities on one side and "overrides"
clearly separated. Add a dashed, non-identifying connector from "overrides" to a
small note box (terracotta `#712b13`/`#f0997b`) that reads: "overrides is
independent: it targets a hub group by name (traffic/barrier/all) and is obeyed
by tiles through polling, not through a database foreign key. Purely additive -
it adds one table and changes none of the existing six." Put a small legend in a
corner explaining the crow's foot symbols (one, many). Export as PNG named
`overridesErd.png`.

---

## 3. OPTIONAL - healthcheck and failover loop (UML sequence diagram)

Filename to save: **`failoverSequence.png`**
Goes in: Design - Backend clustering and failover architecture.md (Chapter 2).
If you generate it, add this embed line under the Chapter 2 recovery-loop text:
`![UML sequence diagram of the healthcheck and failover loop](failoverSequence.png)`

> Prompt to paste:

Make a clean UML sequence diagram titled "Healthcheck and failover loop (UML
sequence diagram)" on a dark background `#1f1e1c`, sans-serif. Five vertical
lifelines with labelled heads, time flowing downward, solid arrows for calls,
dashed arrows for returns, and one "restart" self-message.

Lifelines (left to right):
- "Tile / Dashboard" (amber `#633806`/`#ef9f27`)
- "NGINX" (green `#085041`/`#5dcaa5`)
- "API replica 1" (blue `#0c447c`/`#85b7eb`)
- "API replica 2" (blue `#0c447c`/`#85b7eb`)
- "Docker engine" (terracotta `#712b13`/`#f0997b`)

Messages top to bottom:
1. Docker -> API replica 1 and Docker -> API replica 2: "GET /health every 5s"
   with dashed returns "200 healthy".
2. A note over API replica 1: "replica 1 hangs - still up, stops answering".
3. Docker -> API replica 1: "GET /health (3s timeout, 1 retry)" with no reply,
   then Docker self-message "mark unhealthy -> restart replica 1".
4. Tile -> NGINX: "POST reading" (this happens during the restart).
5. NGINX -> API replica 2: "proxy_pass (replica 1 skipped)", dashed return
   "200 OK", then NGINX dashed return to Tile "200 OK".
6. A note over API replica 1: "passes /health again -> rejoins the pool".

Keep arrow labels short. The message of the diagram: the city keeps answering
through replica 2 while replica 1 is detected and restarted, all inside the 5
second target. Export as PNG named `failoverSequence.png`.

---

## 4. Existing diagrams - keep as is

These already exist and the agents kept their embeds, only adding the named
notation in the prose. No need to regenerate unless you want the notation in the
image title too:
- `failoverTopology.png` - now described as a UML deployment diagram.
- `backupFlow.png` - now described as a data flow diagram (DFD), Gane-Sarson.
- `emergencyOverride.png` - now described as a UML sequence diagram.

If you do want to regenerate any of them on the new house style, reuse the
palette at the top of this file and the same titles.
