# City Sim backend on K3s - autoscaling deployment

This folder runs the City Sim backend on K3s with real autoscaling. It is the
build of recommendation #5 from
`docs/Matin/Sprint 4/Realise - Backend clustering implementation.md`: move from the
Docker Compose cluster (manual `--scale`) to an orchestrator that scales the api on
its own.

It is the follow-up to Learning Goal 1. The Design is in
`docs/Matin/Sprint 4/Design - Kubernetes autoscaling architecture.md`.

## Safety first - read this before you run anything

The live backend the whole team uses runs as Docker Compose on the Pi on port 80.
**Nothing here touches it until you choose to cut over.** The plan is:

1. K3s runs next to Compose. The api is exposed on NodePort **30080**, so you test
   on `http://<pi-ip>:30080` while the team keeps using `http://<pi-ip>:80`.
2. Only when autoscaling is proven and the team agrees do you cut over to port 80,
   and even then with a backup taken first and Compose kept as the rollback.

The one rule that keeps the shared backend safe: **only the stateless api scales.
The database stays a single pod and is never scaled.** One database means one
source of truth, so no two api pods can ever disagree (no split brain).

## What is in this folder

| File | What it is |
|------|-----------|
| `00-namespace.yaml` | the `citysim` namespace |
| `01-config.yaml` | non-secret DB settings (user, db name, host) |
| `02-secret.example.yaml` | template for the DB password (copy to `02-secret.yaml`) |
| `03-postgres.yaml` | the single database: StatefulSet (replicas 1) + headless Service + PVC |
| `04-api.yaml` | the FastAPI backend: Deployment + NodePort Service + wait-for-db |
| `05-ingress.yaml` | Traefik Ingress for the cutover to port 80 (apply later) |
| `06-hpa.yaml` | the HorizontalPodAutoscaler (the autoscaling) |

Load test for autoscaling: `../tests/autoscaling/`.

## Prerequisites

- A Pi (or any Debian/Linux box) you can SSH into. The team Pi is `145.92.8.137`.
- Docker is already on the Pi (the live backend uses it), so `docker build` works.

## Step 1 - install K3s (without Traefik, for the parallel test)

During the parallel test the live backend owns port 80, and K3s's built-in Traefik
also wants port 80. To avoid the clash, install K3s with Traefik disabled. You reach
the api on the NodePort instead. Traefik gets enabled later, at cutover.

```bash
curl -sfL https://get.k3s.io | INSTALL_K3S_EXEC="--disable traefik" sh -

# let kubectl work without sudo
mkdir -p ~/.kube && sudo cp /etc/rancher/k3s/k3s.yaml ~/.kube/config && sudo chown $(id -u):$(id -g) ~/.kube/config

kubectl get nodes      # the Pi should be Ready
```

K3s ships the metrics-server, which the autoscaler needs, so there is nothing extra
to install for HPA.

## Step 2 - build the image and load it into K3s

K3s uses containerd, **not** Docker, so it cannot see images in Docker's store. Build
the image and import it into containerd. (The reference K3s guide skipped this
because it used a public `nginx` image; our image is our own build.)

```bash
# from the repo root, on the Pi
docker build -t citysim-api:0.1 backend/
docker save citysim-api:0.1 | sudo k3s ctr images import -

# confirm it is there
sudo k3s ctr images ls | grep citysim-api
```

The manifests use `image: citysim-api:0.1` with `imagePullPolicy: IfNotPresent`, so
K3s uses this local image and never tries to pull it from the internet.

## Step 3 - set the database password

```bash
cd backend/k8s
cp 02-secret.example.yaml 02-secret.yaml
# edit 02-secret.yaml: replace REPLACE_WITH_DB_PASSWORD in BOTH lines with the real
# password (use the same one as the live .env if you want identical behaviour)
```

`02-secret.yaml` is gitignored, so the real password never lands in git.

## Step 4 - deploy

Apply in order. Do **not** run `kubectl apply -f k8s/` over the whole folder: that
would also apply the example secret and the cutover ingress too early.

```bash
cd backend
kubectl apply -f k8s/00-namespace.yaml
kubectl apply -f k8s/01-config.yaml
kubectl apply -f k8s/02-secret.yaml
kubectl apply -f k8s/03-postgres.yaml
kubectl apply -f k8s/04-api.yaml
kubectl apply -f k8s/06-hpa.yaml

# watch it come up
kubectl get pods -n citysim -w
```

You should see `postgres-0` become Ready, then two `citysim-api` pods become Ready
after the `wait-for-db` init container finishes.

## Step 5 - test it (parallel, no impact on the live backend)

```bash
# health and a real endpoint, through the NodePort
curl http://<pi-ip>:30080/health
curl http://<pi-ip>:30080/api/v1/parking/status

# autoscaling
kubectl get hpa -n citysim
```

Now run the autoscaling test in `../tests/autoscaling/` (apply the load Job, watch
`kubectl get hpa -n citysim -w`, see replicas go 2 -> 5 and back to 2). Record the
numbers for the Realise document.

## Step 6 - cut over to port 80 (only when proven and the team agrees)

This is the one moment that touches the live backend. Take it in this order so a
rollback is always one command away.

```bash
# 1. back up the live database first (Learning Goal 2 script)
cd backend && ./scripts/backup.sh

# 2. stop the live Compose stack. This frees port 80 and KEEPS the data volume.
docker compose down

# 3. move the live data into the K3s database so nothing is lost
LATEST=$(ls -t backend/backups/*.dump | head -1)
kubectl cp "$LATEST" citysim/postgres-0:/tmp/db.dump
kubectl exec -n citysim postgres-0 -- pg_restore -U citysim -d citysim --clean --if-exists /tmp/db.dump

# 4. verify the row counts match what the live DB had
kubectl exec -n citysim postgres-0 -- psql -U citysim -d citysim -c "select count(*) from sensor_readings;"
kubectl exec -n citysim postgres-0 -- psql -U citysim -d citysim -c "select count(*) from parking_spots;"

# 5. enable Traefik (re-run the installer without the disable flag) and route :80
curl -sfL https://get.k3s.io | sh -
sudo systemctl daemon-reload && sudo systemctl restart k3s
kubectl apply -f backend/k8s/05-ingress.yaml

# 6. the city now answers on port 80 from K3s
curl http://<pi-ip>/health
```

The tiles keep posting to the same IP on port 80, so no tile firmware changes.

### Rollback

If anything is wrong, go back to Compose. The live data volume was never touched.

```bash
kubectl delete -f backend/k8s/05-ingress.yaml   # release :80 from Traefik
cd backend && docker compose up -d               # live backend returns on :80
```

## Going multi-node (the "more nodes" goal)

More machines give real capacity and machine-level failover. Adding a node:

```bash
# on the Pi (server), get the join token
sudo cat /var/lib/rancher/k3s/server/node-token

# on each new Debian/Pi node
curl -sfL https://get.k3s.io | K3S_URL=https://145.92.8.137:6443 K3S_TOKEN=<token> sh -

# back on the server
kubectl get nodes      # the new node should be Ready
```

Two things to handle on more nodes:

1. **The image must exist on every node.** Either import it on each node (Step 2),
   or push it once to the GitLab Container Registry and let K3s pull it:

   ```bash
   docker build -t <registry-host>/<group>/<repo>/citysim-api:0.1 backend/
   docker push <registry-host>/<group>/<repo>/citysim-api:0.1
   kubectl create secret docker-registry regcred -n citysim \
     --docker-server=<registry-host> --docker-username=<user> --docker-password=<deploy-token>
   # then set image: to the registry ref and add imagePullSecrets: [{name: regcred}]
   # to the Deployment in 04-api.yaml
   ```

2. **The database stays on its node.** With the default `local-path` storage the
   database's data is node-local, so the postgres pod stays on the node that holds
   it. That keeps a single writer (no split brain), but it means a loss of that one
   node takes the database down until the node returns; backups are the recovery
   path. If you want the database to survive a node failure, install Longhorn and
   set `storageClassName: longhorn` in `03-postgres.yaml`. The api pods, being
   stateless, spread across all nodes automatically.

## Troubleshooting

- **`kubectl get hpa` shows TARGETS `<unknown>`.** Give metrics-server a minute. Check
  `kubectl top pods -n citysim`. If it stays unknown, confirm the api container has a
  CPU `request` (it does, in `04-api.yaml`) - the HPA needs it.
- **api pod stuck `Init:0/1`.** The `wait-for-db` init container is waiting for
  postgres. Check `kubectl get pods -n citysim` and `kubectl logs -n citysim postgres-0`.
- **api pod `ImagePullBackOff`.** The image was not imported into containerd on that
  node. Redo Step 2 on the node, or use the registry path.
- **postgres pod `Pending`.** The PVC could not bind. Check
  `kubectl get pvc -n citysim` and that the `local-path` storage class exists
  (`kubectl get storageclass`).
