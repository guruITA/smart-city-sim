# Realise - Surprise feature implementation

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | May 2026 |
| **Version** | 0.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Realise (fourth of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - What we built
3. Chapter 2 - Code conventions we followed
4. Chapter 3 - How we tested it
5. Chapter 4 - Test results
6. Chapter 5 - User test
7. Conclusion
8. Recommendation
9. References
10. Appendix

---

## Introduction

### Context

City Sim is a miniature smart city built by our team, The Embedded Alliance, for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Five students each build one physical tile, and every tile sends its sensor data to one shared backend that I maintain. The backend is a FastAPI application with a PostgreSQL 16 database, in Docker, on one Raspberry Pi on the HvA network (145.92.8.137, port 80).

This is the **Realise** outcome for Learning Goal 3, the surprise feature. The Design chose the surprise (a backend override, shown as an emergency vehicle corridor) and laid out the table and endpoints. This document builds them, integrated into the live backend the way the Design intended, and tests the full scenario end to end.

### What this document delivers

It describes the code we added, the conventions we held to, how we tested the override flow, the results, a user test, and the handover to the team.

### Requirements we test against

- Mats (Sprint 3 and 4): one surprise in the backend; decide it, motivate it, make the mayor see the point.
- HvA back-end brief: it must be possible to override the decisions of individual hubs from the backend (set all traffic lights on a road to red for an emergency vehicle).

### Main question

How do we build and verify a backend override that forces every traffic light to red on one call and can be cleared again?

### Method

Unlike the cluster and backup work, the surprise is a feature, so we built it directly into the backend following the existing router pattern. The change is purely additive (one new table, one new router), so nothing that already works can break. We then ran an end to end flow test and a user test.

---

## Chapter 1 - What we built

We added one table, its schemas, one router, and one stdlib test. We registered the router the same way as the other seven.

| File | What it does |
|------|--------------|
| `backend/app/models.py` | Added the `Override` model (the `overrides` table) |
| `backend/app/schemas.py` | Added `OverrideCreate` and `OverrideResponse` |
| `backend/app/routers/override.py` | New router with the five override endpoints |
| `backend/app/main.py` | Registered the router at `/api/v1/override` |
| `backend/tests/override/override_test.py` | End to end flow test, standard library only |

### The endpoints

| Method | Path | Purpose |
|--------|------|---------|
| POST | `/api/v1/override` | Set a generic override on a target |
| GET | `/api/v1/override/active` | Tiles poll this for the forced command |
| POST | `/api/v1/override/{id}/clear` | Clear one override |
| GET | `/api/v1/override` | Override history for the dashboard and audit |
| POST | `/api/v1/override/emergency` | One call: force all traffic lights to red |

### The emergency corridor in code

The `emergency` endpoint is the surprise as a single button. It first clears any earlier active traffic override, so only one is ever in force, then sets one city-wide `all_red` override with the reason `emergency_vehicle`:

```python
@router.post("/emergency", response_model=OverrideResponse)
def emergency_corridor(db: Session = Depends(get_db)):
    earlier = db.query(Override).filter(
        Override.active.is_(True), Override.target == "traffic"
    ).all()
    for record in earlier:
        record.active = False
        record.cleared_at = datetime.now(timezone.utc)

    emergency = Override(
        target="traffic", command="all_red",
        reason="emergency_vehicle", active=True,
    )
    db.add(emergency)
    db.commit()
    db.refresh(emergency)
    return emergency
```

### How a tile obeys it

A traffic light tile polls `GET /api/v1/override/active?target=traffic`. If an `all_red` override comes back, the tile drives its lights red instead of running its own sensor logic; if nothing comes back, it behaves as before. The backend side is additive, so a tile that does not poll yet is simply unaffected. This is the handover point to the embedded side, noted in the recommendation.

### How to trigger it

```bash
# Force every traffic light to red for an emergency vehicle
curl -X POST http://145.92.8.137/api/v1/override/emergency

# A tile polls what it must obey
curl "http://145.92.8.137/api/v1/override/active?target=traffic"
```

---

## Chapter 2 - Code conventions we followed

- **Python style:** PEP 8 with type hints and docstrings, the same as the rest of the backend (Van Rossum et al., 2001). The new router mirrors the structure of `routers/traffic.py` so a reviewer reads a familiar shape.
- **API conventions:** the router lives under the `/api/v1` prefix and uses Pydantic schemas in `schemas.py` for its request and response, exactly as the project rules require for any new endpoint.
- **Data with meta-data:** every override is stored with `created_at`, `cleared_at`, `reason`, and `target`, so the table is an auditable history, which fits the HvA requirement to keep city data with meta-data.
- **Additive only:** no existing model, endpoint, or table was changed, so the feature cannot break current behaviour.
- **Commits:** Conventional Commits (`feat:`, `fix:`, `docs:`, `test:`).

---

## Chapter 3 - How we tested it

We test the one thing the feature promises: that the backend can force every traffic light to red on one call, a tile can read that command, and it can be cleared again.

The flow test (`override_test.py`) runs four steps against a running API:

1. POST `/emergency` and check the response is an active `all_red` override.
2. GET `/active?target=traffic` and check a tile would see the `all_red` command.
3. POST `/{id}/clear` and check the override becomes inactive.
4. GET `/active?target=traffic` again and check no `all_red` override is active.

Each step prints PASS or FAIL, and the script exits non-zero if any step fails, so it can run unattended. We run it against the backend on the Pi.

---

## Chapter 4 - Test results

We ran the flow test against the backend on the Raspberry Pi, because the Pi is where the city actually runs. We ran it on 2026-06-03 against the clustered backend (with the new override router) on the Pi, so the test exercised the real load-balanced setup.

**Override flow test result:**

| Step | Expected | Result |
|------|----------|--------|
| 1. emergency set | active `all_red` returned | PASS - status 200, command `all_red`, active true |
| 2. tile polls active | `all_red` visible to tile | PASS - status 200, 1 active override for target `traffic` |
| 3. clear override | override becomes inactive | PASS - status 200, active false |
| 4. no active override | no `all_red` active | PASS - status 200, 0 active overrides |
| Overall | ALL PASS | ALL PASS |

---

## Chapter 5 - User test

Numbers are not the whole story. The surprise has to land with the team and read as a real city feature. So we end with a user test.

The setup: during a normal demo, one team member triggers `/emergency` from the dashboard or a curl call while the others watch the traffic light tile. The question we ask them: did the city behave the way an emergency corridor should, and did you expect the backend could do that?

> User test outcome: Wesley's traffic light tile was not working during the session, so we could not test the light going red on the hardware. To make the surprise demoable without depending on one tile, we added an Emergency Override panel to the dashboard: an emergency button that forces all traffic lights to red, a clear-all button, and a generic force-command form. On the live backend the panel triggers the override, shows it as active, and clears it again. We also confirmed the override is broader than traffic lights by forcing a `force_down` command on the `barrier` target. The remaining step is the tile-side handover: Wesley's firmware has to poll `/active` and obey `all_red` once his tile works again.

---

## Conclusion

This answers the main question. We built a backend override as one new `overrides` table, two schemas, and a five-endpoint router registered at `/api/v1/override`, all additive so nothing that works today breaks. The `emergency` endpoint is the surprise as one call: it forces every traffic light to red with the reason `emergency_vehicle`, a tile reads the forced command by polling `/active`, and the override can be cleared so the city returns to normal. We verify the whole flow with a standard-library test that sets, reads, clears, and re-checks the override. Once that test passes on the Pi, the learning goal is met: the backend can overrule a hub, which is both a stated requirement we had not built and the unexpected reversal of control that Mats asked us to surprise him with.

---

## Recommendation

For the handover to the team we recommend:

1. Add the override poll to the traffic light tile firmware, so it obeys `all_red` during an emergency. The backend side is ready and additive; the embedded side is the remaining half. This is a task for Wesley's tile and must be agreed with him, not changed by me.
2. Add an emergency button to the dashboard that calls `/emergency`, so the demo is one click.
3. Extend the same override pattern to other targets later (the barrier, all tiles), since the table and endpoints already support any `target` and `command`.
4. Write the flow test result and the user test outcome into this document after the Pi run, as a baseline.

---

## References

- Hogeschool van Amsterdam. (2026). *City Sim project brief: back-end requirements* [Print]. Studio Smart Cities, HvA.
- Otten, M. (2026). *Sprint 3 and Sprint 4 mayor delivery feedback* [Verbal feedback, offline]. Hogeschool van Amsterdam.
- tiangolo. (2024). *FastAPI documentation: bigger applications, multiple files* [Online]. Retrieved May 2026, from https://fastapi.tiangolo.com/tutorial/bigger-applications/
- Van Rossum, G., Warsaw, B., & Coghlan, N. (2001). *PEP 8: Style guide for Python code* [Online]. Retrieved May 2026, from https://peps.python.org/pep-0008/

---

## Appendix

### Appendix A - File overview

| File | Purpose |
|------|---------|
| `backend/app/models.py` | `Override` model (`overrides` table) |
| `backend/app/schemas.py` | `OverrideCreate`, `OverrideResponse` |
| `backend/app/routers/override.py` | Five override endpoints |
| `backend/app/main.py` | Router registered at `/api/v1/override` |
| `backend/tests/override/override_test.py` | End to end flow test |

### Appendix B - The overrides table

| Field | Type | Meaning |
|-------|------|---------|
| id | int | primary key |
| target | string | hub group to override (`traffic`, `barrier`, `all`) |
| command | string | forced command (`all_red`, `green_corridor`, `closed`) |
| reason | string | why it was set (`emergency_vehicle`, `roadworks`) |
| active | bool | whether it is currently in force |
| created_at | datetime | when it was set |
| cleared_at | datetime | when it was cleared (null while active) |

### Appendix C - Run the flow test

```bash
cd backend
python tests/override/override_test.py --url http://145.92.8.137
```
