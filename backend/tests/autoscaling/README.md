# Autoscaling test

Proves the HorizontalPodAutoscaler scales the City Sim api up under load and back
down when the load stops. Run this on the Pi (the K3s node), after the stack from
`backend/k8s/` is deployed. Full deploy steps are in `backend/k8s/README.md`.

## What you are testing

- Scale up: under sustained load the api goes from 2 pods to up to 5.
- Scale down: when the load stops, the api returns to 2 pods.
- Recovery during scaling: deleting one api pod does not drop traffic; Kubernetes
  recreates it and the Service keeps routing to the others.

## Steps

1. Confirm the autoscaler can read CPU. On K3s the metrics-server is built in:

   ```bash
   kubectl top pods -n citysim
   kubectl get hpa -n citysim
   ```

   The HPA should show a target like `cpu: 5%/50%` and `REPLICAS 2`. If TARGETS
   shows `<unknown>`, wait a minute for metrics-server, or see the troubleshooting
   note in `backend/k8s/README.md`.

2. In one terminal, watch the autoscaler:

   ```bash
   kubectl get hpa -n citysim -w
   ```

3. In another terminal, start the load:

   ```bash
   kubectl apply -f tests/autoscaling/load.yaml
   ```

4. Watch the `REPLICAS` column climb from 2 toward 5 as CPU passes the target.
   Also watch the pods appear:

   ```bash
   kubectl get pods -n citysim -l app=citysim-api -w
   ```

5. The load Job stops itself after ~3 minutes (or stop it early with
   `kubectl delete job citysim-loadtest -n citysim`). After it stops, the replica
   count falls back to 2 within about 2 minutes (the scale-down stabilisation
   window in `06-hpa.yaml`).

6. Recovery check, while load is running, delete one pod and confirm no downtime:

   ```bash
   kubectl delete pod -n citysim -l app=citysim-api --field-selector status.phase=Running | head -1
   # from a client, the city keeps answering on http://<pi-ip>:30080/health
   ```

## What to record (for the Realise document)

- Start replica count, peak replica count, time to first scale-up.
- Time to scale back down after load stopped.
- Error count during the run (should stay at or near zero).
- Whether deleting a pod caused any failed request.

Paste these into
`docs/Matin/Sprint 4/Realise - Kubernetes autoscaling implementation.md`, which has
placeholder tables waiting for them.

## If you cannot trigger a scale-up

The Pi may not push CPU past 50% with five wget loops. Two safe options:

- Lower the threshold: edit `averageUtilization` in `backend/k8s/06-hpa.yaml` to
  `25`, re-apply, and run the load again. This makes the scale-up visible under
  light load, which is fine for a demo.
- Raise the load: increase `parallelism` and `completions` in `load.yaml`.
