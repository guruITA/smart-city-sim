# Realise - Surprise feature implementation

| | |
|---|---|
| **Author** | Matin Khajehfard, Backend Developer (junior) |
| **Client** | Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented by the mayor (Mats Otten) |
| **Target audience** | Technical readers: the City Sim development team (embedded and backend engineers) and the technical lead at the client side |
| **Date** | June 2026 |
| **Version** | 1.1 |
| **Classification** | Internal |
| **Company** | The Embedded Alliance |
| **Learning outcome** | Realise (fourth of the four outcomes: Analysis > Advise > Design > Realise) |

---

## Table of contents

1. Introduction
2. Chapter 1 - The override built into the live backend
3. Chapter 2 - The code follows the project conventions
4. Chapter 3 - The test scenario and the flow test verify the override
5. Chapter 4 - The user test makes the surprise demoable
6. Conclusion
7. Recommendation
8. References
9. Appendix

---

## Introduction

### Context

City Sim is a miniature smart city built by our team, The Embedded Alliance, for the Studio Smart Cities semester at the Hogeschool van Amsterdam (HvA). Five students each build one physical tile, and every tile sends its sensor data to one shared backend that we maintain. The backend is a FastAPI application with a PostgreSQL 16 database, in Docker, on one Raspberry Pi on the HvA network (145.92.8.137, port 80). The author of this document is Matin Khajehfard, the backend developer (junior) on the team, so the implementation and the testing of a backend feature are his to own.

This is the **Realise** outcome for Learning Goal 3, the surprise feature, and the fourth step in our Analysis > Advise > Design > Realise order. The Design chose the surprise, a backend override shown as an emergency vehicle corridor, and laid out the table and the endpoints. This document builds them, integrated into the live backend the way the Design intended, and tests the full scenario end to end. It describes the code we added, the conventions we held to, the test scenario and how we ran it, the results, a user test, and the handover to the team.

We write the document for a technical audience: the City Sim development team, meaning the embedded engineers who build the tiles, in particular Wesley Oerlmans, who builds the traffic light tile, and the backend engineers who maintain the API, plus the technical lead on the client side. The form is a detailed, internal engineering document because the audience implements and maintains the override, so the readers need the code, the conventions, and the test evidence, not a summary. The client is Gemeente Amsterdam, afdeling Verkeer & Openbare Ruimte (V&OR), represented at the mayor delivery by Mats Otten, and the client is not the same group as the audience: the client asks for the capability while the audience builds and runs it. We classify the document Internal because it describes our own backend and is meant to circulate inside the team and the client's technical staff. The version is 1.1 because this is a revision after a second round of writing feedback from mister mayor Gerald Stap (Stap, 2026).

### Requirements we test against

Two requirements drive what we test. Mats Otten asked, across Sprint 3 and Sprint 4, for one surprise in the backend that we decide on, motivate, and make the mayor see the point of (Otten, 2026). The HvA back-end brief states that it must be possible to override the decisions of individual hubs from the backend, for example to set all traffic lights on a road to red for an emergency vehicle (Hogeschool van Amsterdam, 2026). We carry both forward into the test so the Realise proves the feature against them.

### Main question

How do we build and verify a backend override that forces every traffic light to red on one call and that can be cleared again?

Unlike the cluster and backup work, the surprise is a feature, so we built it directly into the backend following the existing router pattern. The change is purely additive, one new table and one new router, so nothing that already works can break. We then ran an end to end flow test and a user test, and each chapter below ends with a short sub-conclusion.

---

## Chapter 1 - The override built into the live backend

We built the override directly into the running backend, because the surprise has to live in the real city, not in a side project. The work touched five files, and we registered the new router the same way as the other seven so a reviewer reads a familiar shape (FastAPI, 2024). The table below lists what we added and what each part does.

| File | What it does |
|------|--------------|
| `backend/app/models.py` | Added the `Override` model (the `overrides` table) |
| `backend/app/schemas.py` | Added `OverrideCreate` and `OverrideResponse` |
| `backend/app/routers/override.py` | New router with the five override endpoints |
| `backend/app/main.py` | Registered the router at `/api/v1/override` |
| `backend/tests/override/override_test.py` | End to end flow test, standard library only |

The five files map cleanly onto the Design: the model creates the seventh table, the schemas validate the request and the response, the router holds the endpoints, the registration line wires the router into the app at `/api/v1/override`, and the test exercises the whole flow. The endpoints the router exposes are the five the Design fixed, repeated below so the implementation is self-contained.

| Method | Path | Purpose |
|--------|------|---------|
| POST | `/api/v1/override` | Set a generic override on a target |
| GET | `/api/v1/override/active` | Tiles poll this for the forced command |
| POST | `/api/v1/override/{id}/clear` | Clear one override |
| GET | `/api/v1/override` | Override history for the dashboard and audit |
| POST | `/api/v1/override/emergency` | One call: force all traffic lights to red |

The five routes cover the override lifecycle: set, poll, clear, read the history, and the emergency shortcut. The `emergency` endpoint is the surprise as a single button. It first clears any earlier active traffic override, so only one is ever in force, then sets one city-wide `all_red` override with the reason `emergency_vehicle`:

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

A traffic light tile obeys the override by polling `GET /api/v1/override/active?target=traffic`. If an `all_red` override comes back, the tile drives its lights red instead of running its own sensor logic, and if nothing comes back, the tile behaves as before. The backend side is additive, so a tile that does not poll yet is simply unaffected, which makes the polling step the handover point to the embedded side that we name in the recommendation. From the operator side the whole surprise is one request, with a second request standing in for the tile poll:

```bash
# Force every traffic light to red for an emergency vehicle
curl -X POST http://145.92.8.137/api/v1/override/emergency

# A tile polls what it must obey
curl "http://145.92.8.137/api/v1/override/active?target=traffic"
```

### Sub-conclusion

We built the override as five files on the live backend: a model for the seventh table, two schemas, a five-endpoint router registered at `/api/v1/override`, and a flow test. The `emergency` endpoint turns the requirement into one call, and a tile obeys it by polling the active route, so the feature is complete on the backend side and additive.

---

## Chapter 2 - The code follows the project conventions

We held the new code to the same conventions as the rest of the backend, so the override reads like a native part of the project rather than a bolt-on. The Python style follows PEP 8 with type hints and docstrings, the style guide the project standard points to (Van Rossum et al., 2001), and the new router mirrors the structure of `routers/traffic.py` so a reviewer reads a familiar shape. The API conventions match the project rules: the router lives under the `/api/v1` prefix and uses Pydantic schemas in `schemas.py` for its request and its response, exactly as the project requires for any new endpoint, following the multiple-files pattern FastAPI documents (FastAPI, 2024).

The data conventions matter for more than tidiness. Every override is stored with `created_at`, `cleared_at`, `reason`, and `target`, so the table is an auditable history, which fits the HvA requirement to keep city data with meta-data for later analysis (Hogeschool van Amsterdam, 2026). The change stays additive, because no existing model, endpoint, or table was touched, so the feature cannot break current behaviour. We commit the work under Conventional Commits (`feat:`, `fix:`, `docs:`, `test:`), the same scheme the rest of the repository uses.

### Sub-conclusion

The override follows the project's Python, API, data, and commit conventions, so the new code is consistent with the existing backend and the override history is auditable.

---

## Chapter 3 - The test scenario and the flow test verify the override

We test the one thing the feature promises: that the backend can force every traffic light to red on one call, that a tile can read that command, and that the override can be cleared again. The override is a single end-to-end behaviour rather than a load problem, so the right test is a functional flow test, not a performance test. For completeness we name the three performance tests the wider Sprint 4 work uses and say why they do not apply here: a load test pushes normal concurrent traffic to measure throughput and error rate, a soak test runs for a long time to catch a memory leak, and a stress test pushes past normal load to find the breaking point. Those three belong to the clustering Realise, where concurrency is the risk. The surprise stands or falls on its flow being correct, so we drive it with the flow test below.

The table sets out the scenario before the results: the steps we take, what we expect, what actually happened, and the verdict. We ran the scenario on 2026-06-03 against the backend with the new override router (the load-balanced Sprint 4 setup), and the real run is saved at `docs/Matin/Sprint 4/test-evidence/pi_results_2026-06-03_1030.txt`.

| # | Scenario | Steps | Expected result | Actual result | Pass/Fail |
|---|----------|-------|-----------------|---------------|-----------|
| 1 | Emergency forces all red | POST `/api/v1/override/emergency` | status 200, an active `all_red` override is returned | status 200, command `all_red` | PASS |
| 2 | A tile sees the command | GET `/api/v1/override/active?target=traffic` | status 200, one active override for target `traffic` | status 200, 1 active | PASS |
| 3 | Clearing releases the city | POST `/api/v1/override/{id}/clear` | status 200, the override becomes inactive | status 200, active False | PASS |
| 4 | No override remains | GET `/api/v1/override/active?target=traffic` | status 200, no active override | status 200, 0 active | PASS |

The four rows walk the whole promise of the feature, and each row passed: the emergency call returns an active `all_red`, a tile poll sees exactly one active override for `traffic`, the clear flips the override to inactive, and a final poll shows nothing active. The flow test that drives the scenario, `override_test.py`, prints PASS or FAIL per step and exits non-zero if any step fails, so it can run unattended in a pipeline.

### Sub-conclusion

The functional flow test is the right test for the surprise, and the four scenario steps all passed on the Pi run of 2026-06-03, so the override sets, is visible to a tile, and clears as designed.

---

## Chapter 4 - The user test makes the surprise demoable

The flow test proves the backend is correct, but correctness is not the whole story for a surprise. The override has to land with the team and read as a real city feature, so we end the Realise with a user test. The setup is a normal demo: one team member triggers `/emergency` from the dashboard or a curl call while the others watch the traffic light tile, and we ask them whether the city behaved the way an emergency corridor should and whether they expected the backend could do that.

The session did not go fully to plan, and we report the real outcome. Wesley Oerlmans' traffic light tile was not working during the session, so we could not test the light going red on the hardware. To make the surprise demoable without depending on one tile, we added an Emergency Override panel to the dashboard: an emergency button that forces all traffic lights to red, a clear-all button, and a generic force-command form. On the live backend the panel triggers the override, shows the override as active, and clears the override again. We also confirmed the override is broader than traffic lights by forcing a `force_down` command on the `barrier` target. The remaining step is the tile-side handover, where Wesley's firmware has to poll `/active` and obey `all_red` once his tile works again, which we carry into the recommendation.

### Sub-conclusion

The surprise demoes end to end from the dashboard even with one tile down, and the override works on more than one target. The open item is the traffic tile firmware, which has to poll and obey once the hardware is back.

---

## Conclusion

We set out to answer how we build and verify a backend override that forces every traffic light to red on one call and that can be cleared again. First, we built the override into the live backend as one new `overrides` table, two schemas, and a five-endpoint router registered at `/api/v1/override`, all additive so nothing that works today breaks. Second, the code follows the project's Python, API, data, and commit conventions, so the override is consistent with the existing backend and the history is auditable. Third, the functional flow test ran on the Pi on 2026-06-03 and all four scenario steps passed, so the `emergency` call forces every traffic light to red with the reason `emergency_vehicle`, a tile reads the forced command by polling `/active`, and the override clears so the city returns to normal, while the user test showed the surprise demoes from the dashboard even with one tile down. Together the build, the conventions, and the passing test answer the main question: we build the override as one additive router and one table, and we verify it with a flow test that sets, reads, clears, and re-checks the override, all passing on the Pi. So the backend can overrule a hub, which is both a stated requirement we had not built and the unexpected reversal of control that Mats asked us to surprise him with. The one open half is the traffic tile firmware, handed to the embedded side in the recommendation.

---

## Recommendation

We recommend handing the override to the team and to maintenance (beheer) as a working proof of concept, with the backend side ready and one embedded half to finish. Concretely:

1. Add the override poll to the traffic light tile firmware, so the tile obeys `all_red` during an emergency. The backend side is ready and additive, and the embedded side is the remaining half. The poll is a task for Wesley Oerlmans' tile and must be agreed with him, not changed by us. This handover is the one hard dependency for the feature to work end to end on the hardware: the traffic tile firmware must poll `/active` and obey the forced command.
2. Add an emergency button to the dashboard that calls `/emergency`, so the demo is one click. We already added an Emergency Override panel for the user test, so this step is mostly to keep the panel as the standard control for beheer.
3. Extend the same override pattern to other targets later, such as the barrier or all tiles, since the table and the endpoints already support any `target` and `command`.
4. Keep the flow test in the pipeline as a baseline, so a future change to the backend that breaks the override is caught before a demo.

---

## References

- FastAPI. (2024). *Bigger applications: Multiple files* [Online]. Retrieved June 5, 2026, from https://fastapi.tiangolo.com/tutorial/bigger-applications/
- Hogeschool van Amsterdam. (2026). *City Sim project brief: base requirements (General, Embedded, Back-end)* [Print]. Studio Smart Cities, HvA.
- Otten, M. (2026, May 20). *Sprint 4 mayor delivery feedback (Mats)* [Verbal, offline]. Hogeschool van Amsterdam.
- Stap, G. (2026, May 20). *Sprint 4 feedback on Smart City deliverables (mister mayor Gerald Stap)* [Verbal, offline]. Hogeschool van Amsterdam.
- Van Rossum, G., Warsaw, B., & Coghlan, N. (2001). *PEP 8: Style guide for Python code* [Online]. Retrieved June 5, 2026, from https://peps.python.org/pep-0008/

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

### Appendix C - Run the flow test (step by step)

A colleague can reproduce the test with these steps:

1. Make sure the backend with the override router is running and reachable, for example on the Pi at `http://145.92.8.137`.
2. Open a terminal and change into the backend folder:
   ```bash
   cd backend
   ```
3. Run the flow test against that backend:
   ```bash
   python tests/override/override_test.py --url http://145.92.8.137
   ```
4. Read the output: each of the four steps prints PASS or FAIL, and the script exits non-zero if any step fails, so a non-zero exit code means the override flow is broken.

### Appendix D - Use of AI

We used an AI assistant (Claude) as a writing aid for this document. It helped restructure the text to the agreed feedback standard, check the APA formatting and the in-text citations, and rephrase passages for clarity. It did not produce the engineering work or the measured results: the code, the conventions, and the test numbers are our own, the measured results are the author's real runs on the Pi, and they were reviewed by the author, who is responsible for the content.
