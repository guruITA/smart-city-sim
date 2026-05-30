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

[To be filled after implementation]

## R - Result

[To be filled after implementation]

## R - Reflection

[To be filled after sprint review]

## T - Transfer

[To be filled after sprint review]

## References

Matin. (2026). Design: Surprise feature concept [Design deliverable]. [Design - Surprise feature concept](Design%20-%20Surprise%20feature%20concept.md)

Matin. (2026). Realise: Surprise feature implementation [Realise deliverable]. [Realise - Surprise feature implementation](Realise%20-%20Surprise%20feature%20implementation.md)
