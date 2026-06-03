#!/usr/bin/env python3
"""End-to-end flow test for the backend override feature (Learning Goal 3).

Runs the full surprise scenario against a running City Sim API:

  1. trigger the emergency: one call forces the WHOLE city (every hub) at once
  2. poll the active overrides the way the tiles would
  3. clear every emergency override
  4. confirm the city is back to normal

Uses only the Python standard library, so it runs anywhere.

Usage:
  python override_test.py --url http://145.92.8.137
  python override_test.py --url http://localhost:80
"""
import argparse
import json
import sys
import urllib.error
import urllib.request


# The hubs the emergency is expected to force at once.
EXPECTED_TARGETS = {"traffic", "barrier", "streetlight", "parking", "eink"}


def call(method, url, data=None):
    """Send one request. Return (status, parsed_json)."""
    body = json.dumps(data).encode() if data is not None else None
    req = urllib.request.Request(url, data=body, method=method)
    req.add_header("Content-Type", "application/json")
    try:
        with urllib.request.urlopen(req, timeout=5) as resp:
            return resp.status, json.loads(resp.read() or "null")
    except urllib.error.HTTPError as exc:
        return exc.code, None


def main():
    parser = argparse.ArgumentParser(description="City Sim override flow test")
    parser.add_argument("--url", required=True, help="base API url, e.g. http://145.92.8.137")
    args = parser.parse_args()
    base = args.url.rstrip("/") + "/api/v1/override"

    failures = 0

    # 1. Trigger the city-wide emergency. Returns one override per hub.
    status, body = call("POST", base + "/emergency")
    rows = body if isinstance(body, list) else []
    targets = {o["target"] for o in rows}
    ok = status == 200 and EXPECTED_TARGETS.issubset(targets) and all(o["active"] for o in rows)
    print(f"1. emergency: whole city -> status {status}, {len(rows)} hubs {sorted(targets)} [{'PASS' if ok else 'FAIL'}]")
    failures += 0 if ok else 1
    ids = [o["id"] for o in rows]

    # 2. Poll active overrides as the tiles would. The whole city is overruled.
    status, body = call("GET", base + "/active")
    active_targets = {o["target"] for o in (body or [])}
    ok = status == 200 and EXPECTED_TARGETS.issubset(active_targets)
    print(f"2. tiles poll active     -> status {status}, {len(body or [])} active {sorted(active_targets)} [{'PASS' if ok else 'FAIL'}]")
    failures += 0 if ok else 1

    # 3. Clear every emergency override.
    cleared = sum(
        1 for oid in ids
        if call("POST", f"{base}/{oid}/clear")[0] == 200
    )
    ok = bool(ids) and cleared == len(ids)
    print(f"3. clear all overrides   -> {cleared}/{len(ids)} cleared [{'PASS' if ok else 'FAIL'}]")
    failures += 0 if ok else 1

    # 4. Confirm the city is back to normal (no emergency override still active).
    status, body = call("GET", base + "/active")
    still_emergency = [o for o in (body or []) if o.get("reason") == "emergency_vehicle"]
    ok = status == 200 and not still_emergency
    print(f"4. city back to normal   -> status {status}, {len(still_emergency)} emergency active [{'PASS' if ok else 'FAIL'}]")
    failures += 0 if ok else 1

    print(f"\nresult: {'ALL PASS' if failures == 0 else f'{failures} FAILED'}")
    sys.exit(1 if failures else 0)


if __name__ == "__main__":
    main()
