## 1. Introduction

## 1.1 Design context

At the start of this design phase, I already have a working four-way traffic-light circuit on a breadboard. The working version uses an ESP32-S3 as the controller, an MCP23017 as I/O expander, two ULN2803 chips as driver stage, and external LED power for the traffic-light LEDs. The software already uses a `millis()` based structure, so the controller can keep running without blocking the whole program.

The analysis for this learning goal showed that the main problem is no longer the traffic-light logic itself. The main problem is the physical implementation. A breadboard is useful during prototyping, but it is not the right final form for a circuit that must be mounted in or under a city tile, moved, demonstrated, inspected, and connected to 3D printed traffic-light models.

This design deliverable therefore focuses on the physical and electrical design of the permanent perfboard version. I keep the working traffic-light architecture and design how it should be documented, soldered, labelled, connected, routed, protected, and mounted.