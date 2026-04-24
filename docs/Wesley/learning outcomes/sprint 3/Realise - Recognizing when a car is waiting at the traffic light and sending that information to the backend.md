## 1. Introduction

In this document, I describe how I realised the first smart-input extension of my traffic-light project based on the earlier analysis and design work. In this phase, I did not rebuild the full crossing from the beginning. Instead, I used the already working four-way traffic-light setup as the base and added one prototype sensor input to test whether the system can recognise when a car is waiting at the traffic light and send that interpreted result to the backend.

The analysis already defined that this sprint is not only about detection, but about interpretation. It also defined that the project should distinguish between no vehicle, waiting vehicle, passing vehicle, and unclear input, and that the traffic-light phase must be part of that interpretation (Wesley, 2026a). The design then translated that into a concrete hardware and software structure using a KY-021 on GPIO 6, a phase-aware interpretation step, and a backend-ready message structure (Wesley, 2026b).

This realisation phase focuses on building and testing that design in practice.
