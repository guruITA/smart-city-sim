## 1. Introduction

In this document, I describe how I realised the first smart-input extension of my traffic-light project based on the earlier analysis and design work. In this phase, I did not rebuild the full crossing from the beginning. Instead, I used the already working four-way traffic-light setup as the base and added one prototype sensor input to test whether the system can recognise when a car is waiting at the traffic light and send that interpreted result to the backend.

The analysis already defined that this sprint is not only about detection, but about interpretation. It also defined that the project should distinguish between no vehicle, waiting vehicle, passing vehicle, and unclear input, and that the traffic-light phase must be part of that interpretation (Wesley, 2026a). The design then translated that into a concrete hardware and software structure using a KY-021 on GPIO 6, a phase-aware interpretation step, and a backend-ready message structure (Wesley, 2026b).

This realisation phase focuses on building and testing that design in practice.

## 2. What is already handled in the previous deliverables

The analysis deliverable already explains the project background, the practical problem, the interpreted states, the interpretation rules, the risks, and the acceptance criteria for this sprint (Wesley, 2026a). The design deliverable already explains the chosen hardware addition, the use of the KY-021, the proposed software structure, the phase link, and the backend message format (Wesley, 2026b).

Because those parts are already documented there, I do not repeat their full reasoning here. In this realisation document, I focus on how I added the sensor to the current four-way crossing, how I connected it in practice, how I used the current `millis()` based structure, and how the realised prototype behaved during testing.

This also follows the same way of working that I used in Sprint 2, where I first defined the reasoning in analysis and design and then focused the realisation on building, testing, and observing the actual system behaviour (Wesley, 2026c; Wesley, 2026f).

## 3. Goal of the Realisation

The goal of this realisation was to build the designed waiting-car detection extension on top of the already existing four-way traffic-light crossing and verify that the system works safely and correctly in practice.

This included checking that:

* the KY-021 sensor can be read correctly by the ESP32-S3
* the sensor can be integrated without disturbing the existing crossing behaviour
* the current traffic-light phase can be used during interpretation
* stable detection during red can be interpreted as waiting traffic
* short detection during green is not incorrectly treated as waiting traffic
* unclear or missing input does not create unsafe behaviour
* the system can produce a backend-ready traffic state message
* the crossing still keeps its safe phase order and non-blocking timing

These goals follow directly from the current analysis and design (Wesley, 2026a; Wesley, 2026b).
