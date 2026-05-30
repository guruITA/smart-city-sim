# Sprint 4 - Learning Goal 3: Surprise feature

## Learning Question

How do I add an unexpected feature to the backend that goes beyond the basic requirements and shows creativity?

## S - Situation

Mats said at the Sprint 3 delivery: "1 surprise in the backend." The backend currently does what it is supposed to do. It receives sensor data, stores it, and shows it on a dashboard. But there is nothing unexpected. No feature that makes someone say "I did not expect that."

The backend already has 20+ endpoints, 7 routers, 6 tables, and a dashboard with 5 tile panels. The foundation is solid. Now it needs something extra.

## T - Task

First decide what the surprise feature will be and write it down with a clear motivation. Then design and build it. It should be something the mayor does not expect but immediately sees the point of. It must integrate with the existing architecture without breaking anything.

Mats feedback on this goal: you still need to decide what the surprise is. Write it down and explain why you chose it.

**Decided surprise: a backend override.** The backend can overrule an individual tile, shown as an emergency vehicle corridor where one call forces every traffic light to red. I chose it because it closes a stated back-end requirement we had not built (override the decisions of individual hubs from the backend), because it reverses the city's data flow from tiles-push-up to backend-commands-down, which is the unexpected part, and because an emergency corridor is a real public-safety feature for the client. The motivation is worked out in the Design document.

### Deliverables

**Design document** - [Design - Surprise feature concept](Design%20-%20Surprise%20feature%20concept.md)

What the feature is, why I chose it, why it adds value to the city, and how it fits the existing architecture.

**Realise document** - [Realise - Surprise feature implementation](Realise%20-%20Surprise%20feature%20implementation.md)

The built feature, testing, and integration with the dashboard.

## A - Action

The first step was deciding the surprise and writing down why, which is what Mats asked for. We chose a **backend override**, shown as an emergency vehicle corridor: one call forces every traffic light to red so an ambulance can pass. We motivated it on three grounds in the **Design**: it closes a stated back-end requirement we had not built ("override the decisions of individual hubs from the back-end"), it reverses the city's data flow from tiles-push-up to backend-commands-down which is the unexpected part, and an emergency corridor is a real public-safety feature for the client.

In the **Realise** we built it directly into the live backend, following the existing router pattern, and kept it purely additive so nothing that works today can break:

- `models.py`: a new `Override` model (the `overrides` table)
- `schemas.py`: `OverrideCreate` and `OverrideResponse`
- `routers/override.py`: five endpoints, including `POST /emergency` (the surprise as one button)
- `main.py`: the router registered at `/api/v1/override`, the same way as the other seven
- `backend/tests/override/override_test.py`: a standard-library flow test

The `emergency` endpoint first clears any earlier active traffic override, then sets one city-wide `all_red` with the reason `emergency_vehicle`, so only one is ever in force. A traffic light tile obeys it by polling `GET /api/v1/override/active?target=traffic`.

## R - Result

The override feature is built and integrated as the eighth router and seventh table, with every change checked by `py_compile`. The backend can now overrule a hub, which is both the stated requirement we had not built and the unexpected reversal of control Mats asked us to surprise him with. The Design deliverable is finished and submitted in Portflow.

The end to end flow test result is not in yet, because it has to run against the backend on the Pi. The Realise keeps explicit `[to be filled after Pi test]` placeholders. The plan is to run `python tests/override/override_test.py --url http://145.92.8.137` on the Pi, confirm the emergency-set, tile-poll, clear, and re-check steps all pass, and paste the result into the Realise. The remaining half, making the traffic light tile actually poll and obey `all_red`, is a handover to Wesley's tile firmware and has to be agreed with him, not changed by me. The Reflection and Transfer below are written after the sprint review.

## R - Reflection

[To be filled after sprint review]

## T - Transfer

[To be filled after sprint review]

## References

Matin. (2026). Design: Surprise feature concept [Design deliverable]. [Design - Surprise feature concept](Design%20-%20Surprise%20feature%20concept.md)

Matin. (2026). Realise: Surprise feature implementation [Realise deliverable]. [Realise - Surprise feature implementation](Realise%20-%20Surprise%20feature%20implementation.md)
