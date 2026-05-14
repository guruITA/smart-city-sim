# Advise - Team integration and deployment strategy

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | April 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group - HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document assesses each team member's readiness for Pi integration and recommends a deployment strategy. I read everyone's code and documentation to build this picture. Before deploying, I needed to know who can connect and what blocks them.

### Main question

What deployment strategy and integration order should the team follow?

### Sub-questions

1. Which tiles are ready for Pi integration?
2. What hardware conflicts need resolving first?

---

## Chapter 1 - Tile readiness per team member

### Method

I read every teammate's Arduino sketches, documentation, and backend code. For each person I checked: working prototype, WiFi connectivity, API integration, and hardware conflicts.

### Findings

**Thijmen (Railroad Crossing)** - Fully ready. His Sprint 2 work refactors the standalone sketch into a modular library with 4 components: TrainDetector, SignalController, CommunicationHandler, and CrossingController. Dedicated API endpoints are already integrated into the backend. His ESP32 can point at the Pi's IP and start sending data immediately.

**Gurpreet (Streetlight / Speed Camera)** - Working prototype. LDR sensor and relay functional. Sprint 2 adds speed camera with 2 IR sensors. Sends data via the generic readings endpoint. Has a GPIO conflict with Wesley (see Chapter 2). Once the conflict is resolved, he can integrate.

**Wesley (Traffic Light)** - Significant hardware progress. Uses an MCP23017 I/O expander and ULN2803 driver arrays for a four-way crossing. Has detailed documentation. But no backend integration yet. His ESP32 does not send HTTP requests to the API. Needs a new endpoint or adaptation of the generic endpoint for traffic light states.

**Betul (Pedestrian Crossing)** - Analysis and design documents exist. No hardware prototype. No code that connects to the backend. Not ready for integration in Sprint 2.

### Status summary

| Member | Prototype | WiFi | API ready | Blockers |
|--------|-----------|------|-----------|----------|
| Thijmen | Yes | Yes | Yes | None |
| Gurpreet | Yes | Partial | Generic only | GPIO conflict |
| Wesley | Yes | No | No | Needs backend integration |
| Betul | No | No | No | No hardware |

### Sub-conclusion

Thijmen is the only fully ready tile. Gurpreet is close but blocked by a hardware conflict. Wesley and Betul need more work before Pi integration.

---

## Chapter 2 - GPIO conflict and hardware risks

### Context

While reading Gurpreet's and Wesley's code, I noticed they both use GPIO 4 and GPIO 5.

### Findings

Gurpreet uses GPIO 4 and 5 for analog input (LDR sensor readings). Wesley uses GPIO 4 and 5 for I2C communication with the MCP23017 I/O expander. If both tiles run on the same physical setup or share a breadboard, these pins conflict.

This was not caught earlier because each person works on their own tile in isolation. Nobody mapped out pin usage across the team.

### Recommendation

Gurpreet should move his analog pins to GPIO 32-39 (ADC1 channels on ESP32). These pins do not conflict with I2C. Wesley keeps GPIO 4/5 for I2C since the MCP23017 requires specific I2C pins. This should be resolved before any physical integration on the shared city model.

### Sub-conclusion

GPIO 4/5 conflict between Gurpreet and Wesley. Gurpreet moves his analog pins. Wesley keeps I2C.

---

## Chapter 3 - Phased deployment plan

### Recommended order

**Phase 1** - Deploy backend on Pi with parking + railroad crossing endpoints. These are fully tested. Thijmen's ESP32 connects immediately. My parking sensor connects immediately. This proves the Pi works.

**Phase 2** - Team members update their ESP32 sketches to use the Pi's IP address instead of localhost. Gurpreet resolves the GPIO conflict and tests his speed camera against the backend.

**Phase 3** - Add tile-specific endpoints as each team member achieves WiFi connectivity and HTTP communication. Wesley gets a traffic light endpoint. Betul gets a pedestrian crossing endpoint if hardware is ready.

### Why phased

Deploying everything at once is risky. If something breaks, I cannot tell if it is the Pi, the network, or a specific tile. Phased deployment isolates problems.

### Sub-conclusion

3 phases. Start with proven tiles. Add others as they become ready.

---

## Conclusion

Thijmen is fully ready. Gurpreet is close but needs to move his analog pins. Wesley needs backend integration work. Betul is not ready. Deploy in 3 phases starting with parking + railroad crossing. Resolve the GPIO conflict before physical integration.

---

## References

- ESP32-S3 datasheet: GPIO pin reference. https://docs.espressif.com/
- MCP23017 I2C I/O expander datasheet. Microchip Technology.
