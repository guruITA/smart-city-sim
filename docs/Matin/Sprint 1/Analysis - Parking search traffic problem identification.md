# Analysis - Parking search traffic problem identification

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | March 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group - HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document analyzes the city problem behind my parking tile. The question is not "how does the sensor work" but "why does it matter." I used the 6W problem analysis method (Verhoeven, 2010) to frame the problem and define research questions.

### Main question

How can real-time parking occupancy data reduce unnecessary search traffic?

### Sub-questions

1. How much urban traffic consists of cars searching for parking, and what is the environmental impact?
2. What sensor data is needed to determine whether a parking spot is occupied in real time?
3. How should parking occupancy data be collected, stored, and served to make it accessible to drivers?
4. What infrastructure is needed to run a reliable real-time parking detection system?

---

## Chapter 1 - 6W Problem Analysis (Verhoeven, 2010)

### What

Drivers in Amsterdam spend 10 to 20 minutes searching for free parking spots in busy neighborhoods. Up to 30% of inner-city traffic in peak areas consists of cars circling for parking (Shoup, 2017). This search traffic produces unnecessary CO2 emissions, noise pollution, and fine particles in residential streets.

### Trigger

The City Sim project requires each student to address a human-centered problem in a smart city context. Parking search traffic is a problem that can be addressed with real-time sensor data. If drivers know which spots are free before they start driving, they do not need to circle the block.

### Who

- Residents of dense neighborhoods (De Pijp, Oost, Centrum) who experience noise and pollution from circling cars
- Drivers who waste time and fuel searching for spots
- The municipality of Amsterdam, which spends resources on traffic management that could be avoided with better data availability

### When

The problem is worst during peak hours: workday evenings, weekend afternoons. But search traffic occurs at any time in popular areas because drivers have no way of knowing spot availability before arriving.

### Why

Amsterdam targets emission-free traffic in the city center by 2030. Reducing unnecessary driving from parking search directly contributes to this target. Beyond emissions, search traffic increases congestion and reduces quality of life for residents.

### Where

Amsterdam inner city. Specifically residential neighborhoods with high parking demand and limited supply. For this proof of concept: a miniature city tile with 3 parking spots monitored by ultrasonic sensors.

---

## Chapter 2 - Methodology and findings

### Sub-question 2: What sensor data is needed?

I researched how parking occupancy is detected in existing smart parking systems. Three common approaches:

- **Ultrasonic sensors** - Distance measurement. Detects object above/below threshold.
- **Magnetometer sensors** - Detects metal mass of a vehicle.
- **Camera-based detection** - Image recognition. Requires more compute.

For the proof of concept, I selected ultrasonic (HC-SR04). It is cheap, works with ESP32 GPIO, and gives a clear binary signal: distance < threshold means occupied. The 10 cm threshold was determined through physical testing with a toy car on the miniature tile.

### Sub-question 3: How should data be collected and served?

I analyzed the communication flow: ESP32 reads sensor, connects to WiFi, sends HTTP POST with JSON, backend validates and stores, dashboard reads via HTTP GET. This is the same pattern as a standard web application. The only difference is a microcontroller as the client instead of a browser.

I analyzed each team member's sensor setup to check if a single API design could serve all tiles:

| Team member | Tile | Status in Sprint 1 |
|-------------|------|---------------------|
| Thijmen | Railroad Crossing | 3 working demos with IR sensors, OLED, FSM. No WiFi. |
| Gurpreet | Streetlight | LDR sensor with relay. Functional. No WiFi. |
| Wesley | Traffic Light | No code in repository. |
| Betul | Scrum Master | No individual tile. |

**Finding:** All tiles produce data that fits `{tile, sensor_type, value, unit}`. A generic API endpoint can serve all of them. Tile-specific endpoints should wait until team members have WiFi connectivity.

---

## Chapter 3 - Limitations

- Sub-question 1 relies on secondary sources (Shoup, 2017). I did not collect primary data on Amsterdam parking search traffic.
- The proof of concept uses 3 parking spots on a miniature model. Real-world deployment would require different sensor types and higher volumes.
- The team member analysis is a snapshot from Sprint 1. Their progress changes each sprint.
- No user testing or stakeholder interviews were conducted.

---

## Conclusion

Up to 30% of inner-city traffic in busy areas consists of cars searching for parking (Shoup, 2017). Real-time occupancy data from ultrasonic sensors can reduce this. My parking tile demonstrates the concept at proof-of-concept scale with 3 spots. The generic API design supports all team tiles with one endpoint. The biggest gap is the lack of primary research and stakeholder validation.

---

## References

- Shoup, D. (2017). *The High Cost of Free Parking*. Routledge.
- Verhoeven, N. (2010). *Wat is onderzoek?* Boom Lemma.
- FastAPI documentation. https://fastapi.tiangolo.com/
- Arduino HTTPClient library. https://docs.arduino.cc/libraries/httpclient/
