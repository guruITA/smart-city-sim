# Sprint 1 - Learning Goal 2: Generic database schema

## Learning Question

How do I design a database schema that accepts data from 5 different sensor types without needing a separate table for each?

## S - Situation

Our smart city has 5 tiles, each with different sensors producing different data. Gurpreet sends LDR light values. Thijmen sends IR trigger timestamps. Wesley sends traffic light states. I send ultrasonic distances. All of this goes to one shared backend. If each tile gets its own table and endpoints, the backend becomes unmaintainable for one person.

## T - Task

Design a database schema that stores sensor readings from all tiles in a single generic table, plus a separate table for parking spot state that supports fast real-time queries. The schema must handle at least 4 tile types without code changes.

### Deliverables

**Analysis document** - [Analysis - Parking search traffic problem identification](Analysis%20-%20Parking%20search%20traffic%20problem%20identification.md)

6W problem analysis (Verhoeven, 2010), research questions, methodology, and limitations.

**Design document** - [Design - Dual-table database pattern](Design%20-%20Dual-table%20database%20pattern.md)

The dual-table pattern: generic history table + dedicated state table.

## A - Action

I designed two tables. `sensor_readings` has columns for `tile`, `sensor_type`, `value`, `unit`, and `created_at`. This stores full history from every tile in one place. For parking, I added a `parking_spots` table that only stores current state: `spot_number`, `is_occupied`, `updated_at`.

Every time a new sonar reading comes in, the parking endpoint updates the spot's status based on a distance threshold (< 10 cm = occupied). This dual-table approach separates history from state. I documented the tradeoffs in the [Design](Design%20-%20Dual-table%20database%20pattern.md).

The problem analysis behind why parking data matters is in the [Analysis](Analysis%20-%20Parking%20search%20traffic%20problem%20identification.md). That document uses the 6W method to frame parking search traffic as a city problem.

## R - Result

The [Analysis](Analysis%20-%20Parking%20search%20traffic%20problem%20identification.md) identified that up to 30% of inner-city traffic in busy areas consists of cars searching for parking (Shoup, 2017). Real-time occupancy data could reduce this.

The [Design](Design%20-%20Dual-table%20database%20pattern.md) produced a dual-table pattern. The generic `sensor_readings` table handled all 4 tile types during testing. Parking status queries are instant because they only read `parking_spots` (3 rows) instead of scanning full history. The schema required zero changes when team members started sending different sensor types.

## R - Reflection

The dual-table pattern turned out to be reusable. In Sprint 2, Thijmen's railroad crossing needed the same thing: generic readings for history, plus dedicated `train` and `barrier` tables for state. I should have documented this pattern explicitly as a design decision. Instead I only documented the tables themselves.

## T - Transfer

For any future IoT backend: start with a generic table for raw data, add dedicated tables only when a specific use case needs structured state. This avoids over-engineering upfront while keeping the path to specialization open. I applied this pattern again in Sprint 3 for the speed camera.

## References

Matin. (2026). Analysis: Parking search traffic problem identification [Analysis deliverable]. [Analysis - Parking search traffic problem identification](Analysis%20-%20Parking%20search%20traffic%20problem%20identification.md)

Matin. (2026). Design: Dual-table database pattern [Design deliverable]. [Design - Dual-table database pattern](Design%20-%20Dual-table%20database%20pattern.md)
