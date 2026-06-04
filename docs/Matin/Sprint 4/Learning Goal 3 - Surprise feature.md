# Sprint 4 - Learning Goal 3: Surprise feature

## Learning Question

How do I add an unexpected feature to the backend that goes beyond the basic requirements and shows creativity?

## S - Situation

This goal builds on the Sprint 3 mayor delivery feedback. At that delivery Mats Otten, the client's representative, said: "1 surprise in the backend." In the later Sprint 4 round he added that the surprise should tie back to feedback from an earlier sprint, so we keep this learning goal anchored to that Sprint 3 request (Otten, 2026). The backend currently does what it is supposed to do. It receives sensor data, stores it, and shows it on a dashboard. But there is nothing unexpected. No feature that makes someone say "I did not expect that."

The backend already has 20+ endpoints, 7 routers, 6 tables, and a dashboard with 5 tile panels. The foundation is solid. Now it needs something extra. Backend reliability and control is the challenge that runs across this whole sprint: Learning Goal 1 keeps the backend online, Learning Goal 2 keeps its data safe, and this goal lets the backend take command of the city, so we reuse the same "make the backend the dependable brain" thread in more than one outcome.

## T - Task

First decide what the surprise feature will be and write it down with a clear motivation. Then design and build it. It should be something the mayor does not expect but immediately sees the point of. It must integrate with the existing architecture without breaking anything.

Mats Otten's feedback on this goal was that we still needed to decide what the surprise is, write it down, and explain why we chose it (Otten, 2026).

**Decided surprise: a backend override.** The backend can overrule an individual tile, shown as an emergency vehicle corridor where one call forces every traffic light to red. We chose it because it closes a stated back-end requirement we had not built (override the decisions of individual hubs from the backend), because it reverses the city's data flow from tiles-push-up to backend-commands-down, which is the unexpected part, and because an emergency corridor is a real public-safety feature for the client. The motivation is worked out in the Design deliverable (Khajehfard, 2026a).

### Deliverables

**Design document** - [Design - Surprise feature concept](Design%20-%20Surprise%20feature%20concept.md)

What the feature is, why we chose it, why it adds value to the city, and how it fits the existing architecture.

**Realise document** - [Realise - Surprise feature implementation](Realise%20-%20Surprise%20feature%20implementation.md)

The built feature, testing, and integration with the dashboard.

## A - Action

The first step was deciding the surprise and writing down why, which is what Mats Otten asked for (Otten, 2026). We chose a **backend override**, shown as an emergency vehicle corridor: one call forces every traffic light to red so an ambulance can pass. We motivated it on three grounds in the **Design** (Khajehfard, 2026a): it closes a stated back-end requirement we had not built ("override the decisions of individual hubs from the back-end"), it reverses the city's data flow from tiles-push-up to backend-commands-down which is the unexpected part, and an emergency corridor is a real public-safety feature for the client.

In the **Realise** we built it directly into the live backend, following the existing router pattern, and kept it purely additive so nothing that works today can break:

- `models.py`: a new `Override` model (the `overrides` table)
- `schemas.py`: `OverrideCreate` and `OverrideResponse`
- `routers/override.py`: five endpoints, including `POST /emergency` (the surprise as one button)
- `main.py`: the router registered at `/api/v1/override`, the same way as the other seven
- `backend/tests/override/override_test.py`: a standard-library flow test

The `emergency` endpoint first clears any earlier active traffic override, then sets one city-wide `all_red` with the reason `emergency_vehicle`, so only one is ever in force. A traffic light tile obeys it by polling `GET /api/v1/override/active?target=traffic`.

## R - Result

The override feature is built and integrated as the eighth router and seventh table, with every change checked by `py_compile`. The backend can now overrule a hub, which is both the stated requirement we had not built and the unexpected reversal of control Mats Otten asked us to surprise him with (Otten, 2026). The Design deliverable is finished and submitted in Portflow (Khajehfard, 2026a).

The end to end flow test result is now in. On 2026-06-03 we ran `override_test.py` against the backend on the Pi and all four steps passed: triggering `/emergency` set an active `all_red` override, a polling tile saw it, clearing it made it inactive, and the re-check showed no active override left. The result is in the Realise document (Khajehfard, 2026b). The remaining half, making the traffic light tile actually poll and obey `all_red`, is a handover to Wesley's tile firmware and has to be agreed with him, not changed by us. The Reflection and Transfer below are written after the sprint review.

## R - Reflection

This is the goal I was most excited about, and also the one that let me down the most. The override works. I tested the emergency corridor on the backend: one call sets every traffic light to red, a polling tile sees it, and clearing it brings the city back. All four steps passed. I was proud of it.

But I never got to show it the way I wanted. The plan was an ambulance moment, hit the emergency button and watch the whole city go red at once. For that to happen, the traffic tile has to actually poll the backend and obey. On the day, the embedded tiles were not working, so there was nothing physical to react. The dashboard was nothing but some visuals. I had built a feature that can take command of the city, and then I had no city to command. That was a real bummer, and I was honestly sad I could not put the surprise in front of the mayor.

The hard part to accept is that it was not really a backend problem. My side was ready and tested. But a surprise you cannot show is only half a surprise. I leaned on the tiles being ready at the end, they were not, and I had no backup way to demo the feature without them.

## T - Transfer

For the next sprint I take this with me: agree the integration point with the people I depend on early, and build a fallback way to show my feature without their hardware. If I had put a small simulated traffic light on the dashboard that obeys the override, I could have shown the emergency corridor even with the physical tiles down. Next time I build that fallback first, so my work does not sit invisible because someone else's part is not finished.

## References

Khajehfard, M. (2026a). *Design: Surprise feature concept* [Design deliverable]. Hogeschool van Amsterdam. [Online]. [Design - Surprise feature concept](Design%20-%20Surprise%20feature%20concept.md)

Khajehfard, M. (2026b). *Realise: Surprise feature implementation* [Realise deliverable]. Hogeschool van Amsterdam. [Online]. [Realise - Surprise feature implementation](Realise%20-%20Surprise%20feature%20implementation.md)

Otten, M. (2026). *Sprint 3 mayor delivery feedback (Mats)*. Hogeschool van Amsterdam. [Verbal, offline].
