#!/usr/bin/env python3
"""End-to-end flow test for the backend override feature (Learning Goal 3).

Runs the full surprise scenario against a running City Sim API:

  1. trigger the emergency corridor (force all traffic lights to red)
  2. poll the active overrides the way a traffic light tile would
  3. clear the override
  4. confirm no traffic override is active anymore

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

    # 1. Trigger the emergency corridor.
    status, body = call("POST", base + "/emergency")
    ok = status == 200 and body and body["command"] == "all_red" and body["active"] is True
    print(f"1. emergency set        -> status {status}, command {body and body.get('command')} [{'PASS' if ok else 'FAIL'}]")
    failures += 0 if ok else 1
    override_id = body["id"] if body else None

    # 2. Poll active overrides as a traffic tile would.
    status, body = call("GET", base + "/active?target=traffic")
    ok = status == 200 and any(o["command"] == "all_red" for o in (body or []))
    print(f"2. tile polls active    -> status {status}, {len(body or [])} active [{'PASS' if ok else 'FAIL'}]")
    failures += 0 if ok else 1

    # 3. Clear the override.
    if override_id is not None:
        status, body = call("POST", f"{base}/{override_id}/clear")
        ok = status == 200 and body and body["active"] is False
        print(f"3. clear override       -> status {status}, active {body and body.get('active')} [{'PASS' if ok else 'FAIL'}]")
        failures += 0 if ok else 1

    # 4. Confirm no traffic override is active.
    status, body = call("GET", base + "/active?target=traffic")
    ok = status == 200 and not any(o["command"] == "all_red" for o in (body or []))
    print(f"4. no active override    -> status {status}, {len(body or [])} active [{'PASS' if ok else 'FAIL'}]")
    failures += 0 if ok else 1

    print(f"\nresult: {'ALL PASS' if failures == 0 else f'{failures} FAILED'}")
    sys.exit(1 if failures else 0)


if __name__ == "__main__":
    main()
