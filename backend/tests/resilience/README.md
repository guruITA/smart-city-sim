# Resilience tests (Learning Goal 1)

These tests verify the clustered backend (`docker-compose.cluster.yml`) against
the failure modes from the Analysis. They use only the Python standard library.

## Run the clustered stack first

```bash
cd backend
docker compose -f docker-compose.cluster.yml up --build --scale api=2
```

NGINX listens on port 80, with two API replicas behind it sharing one database.

## The three tests

| Test | Command | Verifies |
|------|---------|----------|
| Load | `python tests/resilience/resilience_test.py load --url http://145.92.8.137/health --seconds 30 --concurrency 20` | Cluster serves normal multi-tile load without errors |
| Soak | `python tests/resilience/resilience_test.py soak --url http://145.92.8.137/health --seconds 1800 --concurrency 10` | No memory leak over a long run (watch `docker stats`) |
| Recovery | `python tests/resilience/resilience_test.py recovery --url http://145.92.8.137/health --container <api_container_id>` | A killed replica recovers within 5 seconds |

For the recovery test, list the replica ids first:

```bash
docker ps --filter "name=api" --format "{{.ID}} {{.Names}}"
```

Then pass one id to `--container`. While that replica restarts, NGINX should
keep serving from the other replica, so the recovery time stays low.

## What to record

- Load: requests/sec, error percentage, p95 latency.
- Soak: memory of each api replica at start and end (from `docker stats`).
- Recovery: the measured downtime in seconds and PASS or FAIL against 5s.

Paste these numbers into the Realise document, chapter on test results.
