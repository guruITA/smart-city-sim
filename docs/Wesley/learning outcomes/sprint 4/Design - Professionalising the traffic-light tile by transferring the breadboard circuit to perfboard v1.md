## 1. Introduction

## 1.1 Design context

At the start of this design phase, I already have a working four-way traffic-light circuit on a breadboard. The working version uses an ESP32-S3 as the controller, an MCP23017 as I/O expander, two ULN2803 chips as driver stage, and external LED power for the traffic-light LEDs. The software already uses a `millis()` based structure, so the controller can keep running without blocking the whole program.

The analysis for this learning goal showed that the main problem is no longer the traffic-light logic itself. The main problem is the physical implementation. A breadboard is useful during prototyping, but it is not the right final form for a circuit that must be mounted in or under a city tile, moved, demonstrated, inspected, and connected to 3D printed traffic-light models.

This design deliverable therefore focuses on the physical and electrical design of the permanent perfboard version. I keep the working traffic-light architecture and design how it should be documented, soldered, labelled, connected, routed, protected, and mounted.

## 1.2 Main design question and sub-questions

The main design question for this deliverable is:

**How can I design a permanent perfboard version of my working four-way traffic-light breadboard circuit so that the circuit remains safe, clear, testable, and suitable for mounting in or under the city tile?**

To keep the scope clear, I answer this main question with four sub-questions. These sub-questions are included here in the introduction instead of being placed in a separate chapter:

1. **How should the perfboard transfer be documented in a Fritzing physical design so the component placement, wiring, connectors, and labels remain clear and buildable?**
2. **How should the electrical circuit be defined in a professional KiCad schematic so the power, ground, I2C, driver, and LED connections are unambiguous?**
3. **How should the finished perfboard circuit be mounted and routed in or under the city tile so the wiring is protected and the circuit remains testable?**
4. **Which components, labels, part numbers, quantities, and prices are needed in the Bill of Materials?**

Each sub-question is answered in its own chapter. Each chapter starts with a short introduction and ends with a sub-conclusion.

## 1.3 Scope

This design is limited to the transfer from the working breadboard circuit to a permanent perfboard circuit. It does not redesign the traffic-light sequence, does not replace the MCP23017 and ULN2803 architecture, and does not turn the project into a custom PCB yet.

The design includes:

* Fritzing physical build design;
* KiCad schematic design;
* component labels;
* net labels;
* power and ground routing;
* I2C routing;
* ULN2803 low-side switching;
* traffic-light connectors;
* wire grouping;
* tile mounting;
* wire routing under the tile;
* strain relief;
* test access;
* Bill of Materials.

The design does not include:

* a new traffic algorithm;
* a custom manufactured PCB;
* pedestrian phases;
* emergency vehicle priority;
* backend control changes;
* a full enclosure design.

---