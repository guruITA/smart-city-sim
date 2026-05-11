# Sprint 2 - Learning Goal 3: Team integration analysis

## Learning Question

How ready is each team member for backend integration, and what deployment strategy fits our constraints?

## S - Situation

The backend was getting ready for the Pi. But I had no idea which tiles could actually connect. Each team member was building their hardware independently. Some had WiFi working. Some did not. One pair had a GPIO conflict neither of them knew about.

Before deploying, I needed to know the state of every tile and advise on the order of integration.

## T - Task

Analyze the full project codebase. Assess each team member's readiness for Pi integration. Identify hardware conflicts. Propose a phased deployment plan.

### Deliverables

**Analysis document** - [Analysis - Backend scalability and multi-tile architecture](Analysis%20-%20Backend%20scalability%20and%20multi-tile%20architecture.md)

How the backend architecture scales as more tiles are added.

**Advise document** - [Advise - Team integration and deployment strategy](Advise%20-%20Team%20integration%20and%20deployment%20strategy.md)

Team status per member, GPIO conflict report, phased deployment plan.

## A - Action

I read every teammate's code and documentation. For each person I checked: do they have a working prototype, do they have WiFi connectivity, do they send data to an API, and are there hardware conflicts.

I found that Thijmen was fully ready. His railroad crossing had dedicated endpoints already integrated. Gurpreet had a working prototype but shared GPIO pins 4 and 5 with Wesley. Neither of them knew. Wesley had hardware working with an MCP23017 I/O expander but no backend integration yet. Betul had analysis documents but no hardware.

I also analyzed the backend architecture. The schema grew from 2 tables to 4 tables in Sprint 2. The dashboard scaled well with independent panels. I documented scalability findings in the [Analysis](Analysis%20-%20Backend%20scalability%20and%20multi-tile%20architecture.md).

Based on all of this, I proposed a 3-phase deployment plan. Documented in the [Advise](Advise%20-%20Team%20integration%20and%20deployment%20strategy.md).

## R - Result

The [Analysis](Analysis%20-%20Backend%20scalability%20and%20multi-tile%20architecture.md) showed the generic-plus-dedicated-table pattern works well. Tiles with simple data use the generic table. Tiles with complex state (like the railroad crossing) get their own tables.

The [Advise](Advise%20-%20Team%20integration%20and%20deployment%20strategy.md) produced a clear deployment order: parking + railroad first, then streetlight and speed camera, then traffic light. The GPIO conflict between Gurpreet and Wesley was flagged for immediate resolution.

## R - Reflection

I should have done this analysis earlier. The GPIO conflict could have been caught in Sprint 1 if someone had mapped out pin usage. As the backend owner, I was in the best position to see the full picture. I waited too long.

The analysis also showed me that not all tiles need dedicated endpoints. The generic table handles simple sensors fine. I had been thinking about giving every tile its own router, but that is unnecessary overhead for straightforward use cases.

## T - Transfer

In Sprint 3, I will check in with each team member about their backend needs early in the sprint. I will also maintain a shared pin allocation document so hardware conflicts are visible before integration.

## References

Matin. (2026). Analysis: Backend scalability and multi-tile architecture [Analysis deliverable]. [Analysis - Backend scalability and multi-tile architecture](Analysis%20-%20Backend%20scalability%20and%20multi-tile%20architecture.md)

Matin. (2026). Advise: Team integration and deployment strategy [Advise deliverable]. [Advise - Team integration and deployment strategy](Advise%20-%20Team%20integration%20and%20deployment%20strategy.md)
