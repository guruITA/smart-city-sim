#!/bin/bash
# City Sim backup freshness check (Learning Goal 2).
#
# Catches a silently failed backup job. Looks at the newest dump in the backup
# dir and fails if it is missing, too old, or suspiciously small. Meant to be
# run by cron after the backup, or by hand, so a broken schedule is noticed
# before the day we actually need a restore.
#
# Usage:
#   ./check_backup.sh
#
# Configuration:
#   BACKUP_DIR     local dir with dumps             (default ./backups)
#   MAX_AGE_HOURS  fail if newest dump older than   (default 26)
#   MIN_SIZE_KB    fail if newest dump smaller than (default 1)
set -euo pipefail

BACKUP_DIR="${BACKUP_DIR:-./backups}"
MAX_AGE_HOURS="${MAX_AGE_HOURS:-26}"
MIN_SIZE_KB="${MIN_SIZE_KB:-1}"

# Find the newest dump file.
newest="$(ls -t "${BACKUP_DIR}"/citysim_*.dump 2>/dev/null | head -n1 || true)"

if [ -z "${newest}" ]; then
    echo "[check] FAIL: no backup found in ${BACKUP_DIR}" >&2
    exit 1
fi

# Age in hours.
now="$(date +%s)"
mtime="$(date -r "${newest}" +%s)"
age_hours=$(( (now - mtime) / 3600 ))

# Size in KB.
size_kb=$(( $(wc -c < "${newest}") / 1024 ))

echo "[check] newest backup: ${newest}"
echo "[check] age: ${age_hours}h (max ${MAX_AGE_HOURS}h), size: ${size_kb}KB (min ${MIN_SIZE_KB}KB)"

status=0
if [ "${age_hours}" -gt "${MAX_AGE_HOURS}" ]; then
    echo "[check] FAIL: newest backup is too old, the schedule may have stopped" >&2
    status=1
fi
if [ "${size_kb}" -lt "${MIN_SIZE_KB}" ]; then
    echo "[check] FAIL: newest backup is too small, it may be empty or broken" >&2
    status=1
fi

if [ "${status}" -eq 0 ]; then
    echo "[check] OK: backup is fresh and non-empty"
fi
exit "${status}"
