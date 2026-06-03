#!/bin/bash
# City Sim database restore (Learning Goal 2).
#
# Loads a pg_dump file back into a PostgreSQL database with pg_restore, then
# verifies the row counts of the key tables so we know the backup is usable.
#
# Two uses:
#   - Real recovery: restore into the live citysim database after data loss.
#   - Test restore:  restore into a throwaway database to prove a backup works
#                    without touching the live data (pass a different DB_NAME).
#
# Usage:
#   ./restore.sh <dump-file> [target-db]
# Examples:
#   ./restore.sh ./backups/citysim_2026-05-30_0300.dump citysim_test   # test
#   ./restore.sh ./backups/citysim_2026-05-30_0300.dump citysim        # real
#
# Configuration:
#   DB_CONTAINER   name of the postgres container   (default citysim-db)
#   DB_USER        postgres user                    (default citysim)
set -euo pipefail

DB_CONTAINER="${DB_CONTAINER:-citysim-db}"
DB_USER="${DB_USER:-citysim}"

dump_file="${1:-}"
target_db="${2:-citysim_test}"

if [ -z "${dump_file}" ] || [ ! -s "${dump_file}" ]; then
    echo "usage: ./restore.sh <dump-file> [target-db]" >&2
    echo "the dump file must exist and not be empty" >&2
    exit 1
fi

echo "[restore] target database: ${target_db}"
if [ "${target_db}" = "citysim" ]; then
    echo "[restore] WARNING: restoring into the LIVE database."
    echo "[restore] press Ctrl+C within 5 seconds to abort."
    sleep 5
fi

# 1. Create the target database (ignore error if it already exists).
echo "[restore] creating database ${target_db} (ok if it already exists)"
docker exec "${DB_CONTAINER}" createdb -U "${DB_USER}" "${target_db}" 2>/dev/null || true

# 2. Restore the dump. --clean drops objects first so a re-run is repeatable.
echo "[restore] loading ${dump_file} into ${target_db}"
docker exec -i "${DB_CONTAINER}" pg_restore -U "${DB_USER}" -d "${target_db}" --clean --if-exists < "${dump_file}"

# 3. Verify: print the row counts of the key tables.
echo "[restore] verifying row counts:"
docker exec "${DB_CONTAINER}" psql -U "${DB_USER}" -d "${target_db}" -c \
    "SELECT 'sensor_readings' AS table, count(*) FROM sensor_readings
     UNION ALL
     SELECT 'parking_spots', count(*) FROM parking_spots;"

echo "[restore] done. Check the counts above against the source database."
