#!/bin/bash
# City Sim database backup (Learning Goal 2).
#
# Takes one consistent pg_dump of the whole citysim database from the running
# db container, writes a timestamped compressed file to the local backup dir
# (tier 1), copies it off the Pi (tier 2), then prunes old local dumps.
#
# Designed to be run by cron on the Pi. The database keeps serving while the
# dump runs; pg_dump takes a consistent snapshot without locking writes.
#
# Usage:
#   ./backup.sh
#
# Configuration comes from environment variables with safe defaults:
#   DB_CONTAINER   name of the postgres container        (default citysim-db)
#   DB_USER        postgres user                         (default citysim)
#   DB_NAME        database name                         (default citysim)
#   BACKUP_DIR     local dir for dumps (tier 1)          (default ./backups)
#   OFFSITE_DIR    off-Pi copy destination (tier 2)      (default empty = skip)
#   RETENTION_DAYS days to keep local dumps              (default 7)
set -euo pipefail

DB_CONTAINER="${DB_CONTAINER:-citysim-db}"
DB_USER="${DB_USER:-citysim}"
DB_NAME="${DB_NAME:-citysim}"
BACKUP_DIR="${BACKUP_DIR:-./backups}"
OFFSITE_DIR="${OFFSITE_DIR:-}"
RETENTION_DAYS="${RETENTION_DAYS:-7}"

timestamp="$(date +%Y-%m-%d_%H%M)"
outfile="${BACKUP_DIR}/citysim_${timestamp}.dump"

mkdir -p "${BACKUP_DIR}"

# 1. Dump the whole database in custom, compressed format (-Fc).
#    Streamed out of the container so no file is left inside it.
echo "[backup] dumping ${DB_NAME} from ${DB_CONTAINER} -> ${outfile}"
docker exec "${DB_CONTAINER}" pg_dump -U "${DB_USER}" -Fc "${DB_NAME}" > "${outfile}"

# Fail loudly if the dump is empty, so cron does not record a false success.
if [ ! -s "${outfile}" ]; then
    echo "[backup] ERROR: dump file is empty, removing it" >&2
    rm -f "${outfile}"
    exit 1
fi
echo "[backup] wrote $(du -h "${outfile}" | cut -f1) to ${outfile}"

# 2. Copy off the Pi (tier 2) so an SD card failure cannot take the backups.
if [ -n "${OFFSITE_DIR}" ]; then
    mkdir -p "${OFFSITE_DIR}"
    cp "${outfile}" "${OFFSITE_DIR}/"
    echo "[backup] copied dump off the Pi to ${OFFSITE_DIR}"
else
    echo "[backup] WARNING: OFFSITE_DIR not set, dump exists only on the Pi" >&2
fi

# 3. Prune local dumps older than the retention window (tier 1).
find "${BACKUP_DIR}" -name 'citysim_*.dump' -type f -mtime "+${RETENTION_DAYS}" -delete
echo "[backup] pruned local dumps older than ${RETENTION_DAYS} days"

echo "[backup] done"
