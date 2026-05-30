#!/usr/bin/env python3
"""Resilience tests for the clustered City Sim backend (Learning Goal 1).

Three test types, each maps to a failure mode from the Analysis:

  load     - sustained concurrent requests, measures requests/sec and errors.
             Verifies the cluster keeps serving under normal multi-tile load.
  soak     - the same load for a long duration, while you watch memory with
             `docker stats`. Verifies there is no memory leak (failure 4).
  recovery - kills one api replica, then times how long until the city serves
             a healthy response again. Verifies the 5 second target (failure 2).

The script uses only the Python standard library, so it runs anywhere.

Usage:
  python resilience_test.py load     --url http://145.92.8.137/health --seconds 30 --concurrency 20
  python resilience_test.py soak     --url http://145.92.8.137/health --seconds 1800 --concurrency 10
  python resilience_test.py recovery --url http://145.92.8.137/health --container <api_container_id>
"""
import argparse
import statistics
import subprocess
import sys
import threading
import time
import urllib.request


def one_request(url, timeout=5):
    """Send one GET. Return (ok, latency_seconds)."""
    start = time.perf_counter()
    try:
        with urllib.request.urlopen(url, timeout=timeout) as resp:
            resp.read()
            return resp.status == 200, time.perf_counter() - start
    except Exception:
        return False, time.perf_counter() - start


def run_load(url, seconds, concurrency):
    """Hit the url with N worker threads for the given duration."""
    stop_at = time.time() + seconds
    results = []
    lock = threading.Lock()

    def worker():
        while time.time() < stop_at:
            ok, latency = one_request(url)
            with lock:
                results.append((ok, latency))

    threads = [threading.Thread(target=worker) for _ in range(concurrency)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()

    total = len(results)
    errors = sum(1 for ok, _ in results if not ok)
    latencies = [lat for _, lat in results]
    print(f"requests:        {total}")
    print(f"errors:          {errors} ({(errors / total * 100) if total else 0:.2f}%)")
    print(f"throughput:      {total / seconds:.1f} req/s")
    if latencies:
        print(f"latency avg:     {statistics.mean(latencies) * 1000:.1f} ms")
        print(f"latency p95:     {sorted(latencies)[int(len(latencies) * 0.95)] * 1000:.1f} ms")
        print(f"latency max:     {max(latencies) * 1000:.1f} ms")


def run_recovery(url, container):
    """Kill one replica, then measure how long until the city serves 200 again."""
    print(f"baseline: {'healthy' if one_request(url)[0] else 'UNHEALTHY'}")
    print(f"killing replica: {container}")
    subprocess.run(["docker", "kill", container], check=True)

    killed_at = time.perf_counter()
    recovered_at = None
    # Poll fast until the cluster serves a healthy response again.
    while time.perf_counter() - killed_at < 60:
        ok, _ = one_request(url, timeout=2)
        if ok:
            recovered_at = time.perf_counter()
            break
        time.sleep(0.1)

    if recovered_at is None:
        print("did NOT recover within 60s")
        sys.exit(1)
    downtime = recovered_at - killed_at
    print(f"recovered after: {downtime:.2f} s")
    print(f"target (<=5s):   {'PASS' if downtime <= 5 else 'FAIL'}")


def main():
    parser = argparse.ArgumentParser(description="City Sim backend resilience tests")
    parser.add_argument("mode", choices=["load", "soak", "recovery"])
    parser.add_argument("--url", required=True, help="endpoint to probe, e.g. http://145.92.8.137/health")
    parser.add_argument("--seconds", type=int, default=30, help="duration for load/soak")
    parser.add_argument("--concurrency", type=int, default=20, help="worker threads for load/soak")
    parser.add_argument("--container", help="api container id to kill (recovery mode)")
    args = parser.parse_args()

    if args.mode in ("load", "soak"):
        label = "SOAK" if args.mode == "soak" else "LOAD"
        print(f"=== {label} test: {args.url} for {args.seconds}s, concurrency {args.concurrency} ===")
        run_load(args.url, args.seconds, args.concurrency)
    else:
        if not args.container:
            parser.error("recovery mode needs --container <api_container_id>")
        print(f"=== RECOVERY test: {args.url} ===")
        run_recovery(args.url, args.container)


if __name__ == "__main__":
    main()
