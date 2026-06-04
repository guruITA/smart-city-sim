#!/bin/bash
# One-shot Sprint 4 test runner for the Pi (Learning Goals 1, 2 and 3).
#
# Runs every Sprint 4 test in sequence against the running backend and writes a
# single timestamped results file. The output is grouped per Realise document so
# the numbers can be copied straight into the test tables.
#
# Run this ON the Pi (it needs docker for the recovery and backup tests). The
# HTTP tests also work from a laptop if you point --url at the Pi.
#
# Usage:
#   ./run_pi_tests.sh                       # all tests, url http://localhost
#   URL=http://145.92.8.137 ./run_pi_tests.sh
#   LOAD_SECONDS=30 SOAK_SECONDS=1800 ./run_pi_tests.sh
#
# Configuration (environment variables with safe defaults):
#   URL            base API url                         (default http://localhost)
#   API_CONTAINER  api replica to kill in recovery test (default: auto-detect)
#   DB_CONTAINER   postgres container                   (default citysim-db)
#   DB_USER        postgres user                        (default citysim)
#   DB_NAME        live database name                   (default citysim)
#   BACKUP_DIR     dir for dumps                         (default ./backups)
#   LOAD_SECONDS   load test duration                   (default 30)
#   LOAD_CONC      load test concurrency                (default 20)
#   SOAK_SECONDS   soak test duration, 0 to skip        (default 0)
#   SOAK_CONC      soak test concurrency                (default 10)
set -uo pipefail

# Always run from the backend dir so the scripts find ./backups and ./tests.
cd "$(dirname "$0")/.." || exit 1

URL="${URL:-http://localhost}"
DB_CONTAINER="${DB_CONTAINER:-citysim-db}"
DB_USER="${DB_USER:-citysim}"
DB_NAME="${DB_NAME:-citysim}"
BACKUP_DIR="${BACKUP_DIR:-./backups}"
LOAD_SECONDS="${LOAD_SECONDS:-30}"
LOAD_CONC="${LOAD_CONC:-20}"
SOAK_SECONDS="${SOAK_SECONDS:-0}"
SOAK_CONC="${SOAK_CONC:-10}"
VERIFY_DB="citysim_verify"

stamp="$(date +%Y-%m-%d_%H%M)"
results="tests/pi_results_${stamp}.txt"

# Tee everything to the results file and the screen.
exec > >(tee "${results}") 2>&1

hr() { printf '\n========== %s ==========\n' "$1"; }

echo "City Sim Sprint 4 test run"
echo "date:    $(date)"
echo "url:     ${URL}"
echo "results: ${results}"

# ---------------------------------------------------------------------------
# LG3 - Surprise feature: emergency override flow.
# Maps to: Realise - Surprise feature implementation.md (flow test table).
# ---------------------------------------------------------------------------
hr "LG3 override flow (Surprise feature Realise)"
python3 tests/override/override_test.py --url "${URL}" || echo "[lg3] override flow returned non-zero"

# ---------------------------------------------------------------------------
# LG1 - Load test under concurrent traffic.
# Maps to: Realise - Backend clustering implementation.md (load test table).
# ---------------------------------------------------------------------------
hr "LG1 load test (Clustering Realise)"
python3 tests/resilience/resilience_test.py load \
    --url "${URL}/health" --seconds "${LOAD_SECONDS}" --concurrency "${LOAD_CONC}" \
    || echo "[lg1] load test returned non-zero"

# ---------------------------------------------------------------------------
# LG1 - Recovery test: kill one api replica, time the failover.
# Maps to: Realise - Backend clustering implementation.md (recovery table).
# ---------------------------------------------------------------------------
hr "LG1 recovery / failover (Clustering Realise)"
API_CONTAINER="${API_CONTAINER:-$(docker ps --format '{{.Names}}' | grep -i api | head -n1)}"
if [ -z "${API_CONTAINER}" ]; then
    echo "[lg1] SKIP recovery: no running api container found."
    echo "[lg1] start the cluster first: docker compose -f docker-compose.cluster.yml up --build --scale api=2"
else
    echo "[lg1] killing replica: ${API_CONTAINER}"
    python3 tests/resilience/resilience_test.py recovery \
        --url "${URL}/health" --container "${API_CONTAINER}" \
        || echo "[lg1] recovery test returned non-zero"
fi

# ---------------------------------------------------------------------------
# LG1 - Soak test (optional, long). Watch memory in another shell:
#   docker stats   (replica memory at start vs end -> memory leak table).
# ---------------------------------------------------------------------------
if [ "${SOAK_SECONDS}" -gt 0 ]; then
    hr "LG1 soak test (${SOAK_SECONDS}s) - watch 'docker stats' in another shell"
    python3 tests/resilience/resilience_test.py soak \
        --url "${URL}/health" --seconds "${SOAK_SECONDS}" --concurrency "${SOAK_CONC}" \
        || echo "[lg1] soak test returned non-zero"
else
    hr "LG1 soak test SKIPPED (set SOAK_SECONDS=1800 to run the real soak)"
fi

# ---------------------------------------------------------------------------
# LG2 - Backup, then restore-verify into a throwaway db, then freshness check.
# Maps to: Realise - Database backup implementation.md (backup/restore tables).
# ---------------------------------------------------------------------------
hr "LG2 backup (Database backup Realise)"
DB_CONTAINER="${DB_CONTAINER}" DB_USER="${DB_USER}" DB_NAME="${DB_NAME}" \
    BACKUP_DIR="${BACKUP_DIR}" bash scripts/backup.sh \
    || echo "[lg2] backup returned non-zero"

hr "LG2 source row counts (live ${DB_NAME})"
docker exec "${DB_CONTAINER}" psql -U "${DB_USER}" -d "${DB_NAME}" -c \
    "SELECT 'sensor_readings' AS table, count(*) FROM sensor_readings
     UNION ALL
     SELECT 'parking_spots', count(*) FROM parking_spots;" \
    || echo "[lg2] could not read source row counts"

hr "LG2 restore-verify into ${VERIFY_DB} (does NOT touch live data)"
newest_dump="$(ls -t "${BACKUP_DIR}"/citysim_*.dump 2>/dev/null | head -n1 || true)"
if [ -z "${newest_dump}" ]; then
    echo "[lg2] SKIP restore: no dump found in ${BACKUP_DIR}"
else
    echo "[lg2] restoring newest dump: ${newest_dump}"
    DB_CONTAINER="${DB_CONTAINER}" DB_USER="${DB_USER}" \
        bash scripts/restore.sh "${newest_dump}" "${VERIFY_DB}" \
        || echo "[lg2] restore returned non-zero"
    # Drop the throwaway verify database so re-runs stay clean.
    docker exec "${DB_CONTAINER}" dropdb -U "${DB_USER}" "${VERIFY_DB}" 2>/dev/null \
        && echo "[lg2] dropped throwaway ${VERIFY_DB}" || true
fi

hr "LG2 backup freshness check (Database backup Realise)"
BACKUP_DIR="${BACKUP_DIR}" bash scripts/check_backup.sh \
    || echo "[lg2] check_backup returned non-zero"

hr "DONE"
echo "All output saved to ${results}"
echo "Copy the numbers above into the three Realise docs (Sprint 4)."
