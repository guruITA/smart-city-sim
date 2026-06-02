# Advise Report — HTTPS/TLS Integration for the City Sim Backend

| Document information |                                                                |
| -------------------- | -------------------------------------------------------------- |
| Title                | Advise Report — HTTPS/TLS Integration for the City Sim Backend |
| Author               | Betül Aydin                                                    |
| Date                 | 1 June 2026                                                    |
| Version              | 1.0                                                            |
| Classification       | Internal                                                       |
| Mayor                | Mats                                                           |
| Company              | Amsterdam University of Applied Sciences                       |
| Learning outcome     | Advise                                                         |
| Sprint               | Sprint 4                                                       |

## Table of Contents

1. Introduction
2. Main- and Sub-Questions
3. Evaluation of the Realisation
4. Recommended Integration Approach
5. Advice to the Backend Team
6. Limitations
7. Conclusion
8. References


## 1. Introduction

This advise report gives a recommendation for how HTTPS/TLS should be integrated into the City Sim backend after the local HTTPS/TLS prototype test.

In the Realisation phase, HTTPS/TLS was tested locally with a self-signed certificate and a separate Docker Compose test setup. The test confirmed that the FastAPI backend can respond through (About HTTPS - FastAPI, z.d.) HTTPS. However, the prototype was not deployed to the shared Raspberry Pi environment, because that backend is actively used by the team, ESP32 devices and dashboard.

This advice is mainly written for the backend team, especially the backend infrastructure side, because future HTTPS/TLS integration should be done in a controlled way without breaking the current deployment.


## 2. Advice Question and Sub-Questions

### Main Question

What is the best next step for integrating HTTPS/TLS into the shared City Sim backend deployment?

### Sub-Questions

1. What did the local HTTPS/TLS prototype prove?
2. Why should the prototype not be deployed directly to the shared Raspberry Pi?
3. How should HTTPS/TLS be integrated in the future?