# Design: Recognizing when a car is waiting at the traffic light and sending that information to the backend

## Summary

In this design, I extend my existing four-way traffic-light crossing with a first smart feature: detecting whether a car is waiting at the traffic light and sending that interpreted result to the backend. I do not redesign the full crossing, because that hardware structure was already completed in Sprint 2 with the ESP32-S3, MCP23017, two ULN2803 chips, and external LED power. Instead, I build on that existing structure by adding one KY-021 prototype sensor as a simple stand-in for vehicle presence.

The main design choice is that the sensor does not directly control the traffic lights. The existing `millis()` based traffic-light controller stays responsible for the safe phase sequence, while the new sensor input is read separately and interpreted in the context of the current traffic-light phase. This means the design remains modular and safe. The result is a first step from a fixed-cycle crossing toward a traffic light that can react meaningfully to road input and prepare backend-ready traffic state messages.

## Table of Contents

- [Introduction](#introduction)
- [Design Goal](#design-goal)
- [What Is Already Defined in the Analysis](#what-is-already-defined-in-the-analysis)
- [Starting Point From the Current System](#starting-point-from-the-current-system)
- [Selected Design Direction](#selected-design-direction)
- [Why I Chose the KY-021](#why-i-chose-the-ky-021)
- [Design of the Physical Sensor Concept](#design-of-the-physical-sensor-concept)
- [Electrical Design](#electrical-design)
  - [Controller-side connections](#controller-side-connections)
  - [Sensor wiring](#sensor-wiring)
- [Design of the Software Link](#design-of-the-software-link)
- [Design of the Traffic-Light Phase Link](#design-of-the-traffic-light-phase-link)
- [Interpretation Logic Design](#interpretation-logic-design)
  - [Proposed interpretation structure](#proposed-interpretation-structure)
  - [Proposed timing parameters](#proposed-timing-parameters)
- [Backend Message Design](#backend-message-design)
- [Software Structure Design](#software-structure-design)
- [Safety in the Design](#safety-in-the-design)
- [Relationship to Future Sprints](#relationship-to-future-sprints)
- [Correct Design Result for This Step](#correct-design-result-for-this-step)
- [Limits of This Design](#limits-of-this-design)
- [Conclusion](#conclusion)
- [References](#references)

## Introduction

At this stage of the project, I am no longer designing the traffic light itself from the beginning. Before this learning goal, I had already completed the first physical traffic-light prototype and then expanded it in Sprint 2 into a coordinated four-way crossing with multiple traffic lights that work together safely and clearly. The first version already proved the basic fixed traffic-light sequence, and Sprint 2 already proved the coordinated crossing structure with an ESP32-S3, an MCP23017, two ULN2803 chips, and external LED power (Wesley, 2026c; Wesley, 2026g).  

This design is therefore not about redesigning the whole crossing. It is about extending the current crossing with one new function: recognizing whether a car is waiting at the traffic light and sending that interpreted result to the backend. In the wider project structure, this is the stage where the system starts moving from a fixed-cycle crossing toward a smart traffic light that reacts to real-time input (Wesley, 2026e). The practical reason, the problem definition, the interpreted states, the interpretation rules, the risks, and the acceptance criteria are already explained in the analysis and are therefore not repeated here in full (Wesley, 2026a). 

## Design goal

The goal of this design is to extend the existing four-way traffic-light setup with a simple, testable, and safe detection function that can recognize whether a car is waiting at the stop line and send that interpreted result to the backend.

For this step, the design must:

* keep the existing traffic-light hardware structure
* add one prototype sensor input without disturbing the current crossing logic
* use the current traffic-light phase as part of the interpretation
* produce a backend-ready message instead of only a raw sensor value
* stay non-blocking in software
* remain understandable enough to realise and test in the next step

The broader reasoning behind these design choices is already described in the analysis document (Wesley, 2026a). 

## What is already defined in the analysis

The following parts are already defined in the analysis and are therefore used here as design input instead of being repeated as full explanation:

* why this sprint is about interpretation and not only detection
* the practical question “is there a car waiting at the traffic light, yes or no”
* the interpreted states
* the interpretation rules for red, green, yellow, and unclear input
* the reason why a simple raw trigger is not enough
* the risks of misclassifying passing traffic as waiting traffic
* the acceptance criteria for this step

This design document builds directly on those choices from the analysis (Wesley, 2026a). 

## Starting point from the current system

The design starts from the current coordinated four-way crossing, not from the first single traffic light. At this stage, the controller logic, output expansion, driver stage, and externally powered lamp channels are already in place. The realised setup already proved safe startup, correct phase order, all-red transitions, and the absence of conflicting green states (Wesley, 2026d; Wesley, 2026f).  

The current traffic-light controller already uses a `millis()` based state machine instead of `delay()`. That is important for this design, because sensor handling and backend communication should not be added on top of blocking timing. My earlier advice was also to keep the current hardware structure, keep the simplified crossroads model, and add only one smart feature first (Wesley, 2026b). 

So this design keeps the existing crossing architecture and only adds a prototype detection layer and a backend-message layer.

## Selected design direction

The selected direction for this sprint is:

```text
existing crossing controller + one KY-021 sensor input + phase-aware interpretation logic + backend message output
```

That means the existing structure remains:

```text
ESP32-S3 -> MCP23017 -> 2x ULN2803 -> external power supply -> traffic-light LEDs
```

and this sprint adds:

```text
KY-021 -> ESP32-S3 input -> interpretation logic -> backend message
```

I keep this direction because Sprint 2 already solved the scaling and output-switching problem, so I do not need to redesign those parts again. Instead, I can build on the current system and add one controlled smart feature first, which is also consistent with my earlier advice for the next step (Wesley, 2026b; Wesley, 2026f).  

## Why I chose the KY-021

For this learning goal, I chose the **KY-021 Mini Magnetic Reed Switch Module** as the prototype sensor.

I chose this because my project is built on a **1:64 scale city tile** that is **300 mm by 300 mm and 3.6 mm thick**. In this project, I need a detection method that fits the physical size of the tile and stays manageable as a prototype. A more realistic road-detection construction would make this step unnecessarily complex for the space I have available. Because of that, I use the KY-021 as a simplified replacement sensor that lets me test the logic for detecting and interpreting whether a car is waiting at the traffic light.

This choice matches the analysis, where the KY-021 was already defined as a prototype stand-in for vehicle presence so I could focus first on the interpretation logic instead of making the hardware too complex too early (Wesley, 2026a). The analysis also already explains why this project step is deliberately limited in scope and why one simple sensor is enough for this sprint (Wesley, 2026a). 

## Design of the physical sensor concept

The KY-021 will be used as a hidden prototype detector for one road direction. The sensor is placed at the stop-line area of the chosen lane on the city tile. A small magnet is used on or in the model vehicle so that the reed switch changes state when the vehicle is positioned above the detection point.

This design is intentionally simple. It does not try to copy a full real road-sensor installation. Instead, it creates a testable model in which:

* a vehicle that reaches the stop line can trigger the sensor
* the controller can observe whether that trigger remains stable
* the current traffic-light phase can then be used to interpret the meaning of the trigger

For this sprint, I only add detection for one direction first. That keeps the step controlled and matches the project choice to add one smart feature first instead of making the system too complex too quickly (Wesley, 2026b; Wesley, 2026a). 

## Electrical design

The existing crossing hardware remains unchanged on the output side. The MCP23017, the ULN2803 chips, the external lamp power, and the traffic-light mappings all stay as they already were in Sprint 2 (Wesley, 2026c; Wesley, 2026d). 

The only electrical addition in this design is the sensor input.

### Controller-side connections

| ESP32-S3 pin | Connection                   | Purpose            |
| ------------ | ---------------------------- | ------------------ |
| GPIO 4       | MCP23017 SCL                 | Existing I2C clock |
| GPIO 5       | MCP23017 SDA                 | Existing I2C data  |
| GPIO 6       | KY-021 signal                | New sensor input   |
| 3V3          | KY-021 VCC                   | Sensor power       |
| GND          | KY-021 GND and shared ground | Common reference   |

I use **GPIO 6** for the KY-021 signal because the current crossing already uses GPIO 4 and GPIO 5 for I2C, and the earlier Sprint 2 design deliberately kept GPIO 6 available for later expansion (Wesley, 2026c). 

### Sensor wiring

The KY-021 is connected as a digital input module:

```text
ESP32-S3 3V3   -> KY-021 VCC
ESP32-S3 GND   -> KY-021 GND
ESP32-S3 GPIO6 -> KY-021 signal
```

The existing common-ground principle remains important in this design, because the sensor input and the traffic-light controller must still use the same logic reference as the rest of the system (Wesley, 2026c; Wesley, 2026d). 

## Design of the software link

The sensor should not directly control the light outputs. The existing traffic-light state machine remains the main controller. The sensor only provides input to the decision layer.

That means the design keeps these responsibilities separate:

* **traffic-light phase control**
* **sensor reading**
* **sensor interpretation**
* **backend message creation**

This separation is important because the current traffic-light system already works safely, and the new detection feature should add meaning without weakening the core safety behaviour. The analysis already explains why the signal must be interpreted in the context of the traffic-light phase instead of being used as a raw trigger (Wesley, 2026a). 

## Design of the traffic-light phase link

The existing phase model remains:

1. all red
2. North/South green
3. North/South yellow
4. all red
5. East/West green
6. East/West yellow

The new sensor logic does not replace this model. It only reads the current phase and uses that phase as input for the interpretation step.

The exact interpreted states and interpretation rules are already defined in the analysis. In this design, the important point is only that the software must have access to the current phase while evaluating the sensor signal (Wesley, 2026a). 

## Interpretation logic design

For the first prototype version, I use a simple non-blocking interpretation approach based on `millis()`.

The detailed reasoning for the interpreted states, the red/green difference, and the meaning of unclear input is already described in the analysis (Wesley, 2026a). In the design, that becomes a concrete software structure with timing thresholds.

### Proposed interpretation structure

| Condition                                                 | Design interpretation                          |
| --------------------------------------------------------- | ---------------------------------------------- |
| sensor inactive                                           | no vehicle                                     |
| sensor active during red and stable longer than threshold | waiting vehicle                                |
| sensor active during green and shorter than threshold     | passing vehicle                                |
| sensor active during yellow or transition                 | unclear input unless later context confirms it |
| unstable or noisy switching                               | unclear input                                  |

### Proposed timing parameters

| Parameter                | Initial design value | Purpose                                                |
| ------------------------ | -------------------: | ------------------------------------------------------ |
| debounce time            |                50 ms | ignore switch bounce or noise                          |
| stable waiting threshold |               500 ms | distinguish presence from a very short passing trigger |
| clear timeout            |               200 ms | avoid immediate drop on a brief release                |

These values are design choices for the first prototype. They are not presented as final real-world values. They are meant to make the prototype behaviour testable and understandable.

## Backend message design

The backend should not receive only a raw sensor value. It should receive a meaningful message that already includes the result of the interpretation step. That need is already defined in the analysis, so this design only translates it into a concrete message structure (Wesley, 2026a). 

For this design, the message should contain:

* sensor identifier
* road direction
* current traffic-light phase
* interpreted traffic state
* timestamp
* validity flag

A suitable first payload structure is:

```json
{
  "sensorId": "north_1",
  "direction": "north",
  "phase": "NS_RED",
  "interpretedState": "waiting_vehicle",
  "timestampMs": 123456,
  "valid": true
}
```

For this sprint, the transport layer can remain abstract in the design. The important point is that the software should already produce the message in a backend-ready structure. That makes later implementation with Wi-Fi, HTTP, MQTT, or another backend method easier without redesigning the meaning of the data.

## Software structure design

Because Sprint 2 already showed that blocking timing becomes a problem for later integration, this design keeps the `millis()` based structure and extends it with separate software responsibilities (Wesley, 2026b; Wesley, 2026d). 

For this step, the software should be separated into:

* **phase control**
* **sensor reading**
* **sensor interpretation**
* **backend message creation**
* **output sending**

A simplified structure is:

```text
loop()
 ├── updateTrafficPhase()
 ├── readSensor()
 ├── interpretSensorWithCurrentPhase()
 ├── checkForStateChange()
 └── sendBackendMessageIfNeeded()
```

This keeps the design modular and makes it easier to expand later with more than one sensor or with pedestrian logic. The broader requirement for modular software is already part of the wider project direction (Wesley, 2026e). 

## Safety in the design

The sensor feature must not weaken the safety behaviour that already exists in the current crossing. So this design keeps the following rules central:

* conflicting directions may never become green at the same time
* yellow remains only a transition phase
* all-red remains between incompatible directions
* the system must start safely
* invalid sensor input must not create unsafe phase changes
* if the sensor result is unclear, the traffic light falls back to the safe default logic

The full explanation of why these safety conditions matter, and how unclear input must be handled, is already written in the analysis. In the design, the important point is that the sensor feature is added around the existing safe controller, not instead of it (Wesley, 2026a; Wesley, 2026c).  

## Relationship to future sprints

This design is deliberately limited to one sensor and one first smart feature. That is important because the current setup should be treated as a strong intermediate version, not as the final smart-traffic system (Wesley, 2026b). 

The design leaves room for later steps:

* add one sensor per direction
* add pedestrian request input
* extend backend communication
* move from simple interpretation rules to more adaptive decision-making

Because the current architecture already separates controller logic, output expansion, and power switching, these later additions can be built on top of the same hardware base (Wesley, 2026f). 

## Correct design result for this step

For this step, I consider the design correct when:

* the existing coordinated crossing remains unchanged on the safe output side
* one prototype sensor is added clearly and simply
* the sensor is linked to the controller through a defined ESP32 input pin
* the interpretation uses the current traffic-light phase
* the backend message structure is defined clearly
* the software stays non-blocking and modular
* the design remains understandable enough to realise and test in the next step

The broader correctness conditions for the sensor behaviour itself are already defined in the analysis and do not need to be repeated here in full (Wesley, 2026a). 

## Limits of this design

This design is still limited in scope:

* it only adds one prototype sensor first
* it uses a simplified stand-in instead of a full realistic road-detection method
* it does not yet include multi-sensor traffic comparison
* it does not yet include pedestrian behaviour
* it does not yet include adaptive timing changes across the full crossing
* it still needs realisation and testing to confirm the chosen thresholds and signal behaviour in practice

The broader limits of this sprint are already described in the analysis. In this design, the important point is that the current step stays intentionally controlled and does not try to solve the full smart-traffic problem at once (Wesley, 2026a; Wesley, 2026b). 

## Conclusion

This design extends the existing coordinated four-way crossing with one controlled smart feature: recognizing whether a car is waiting at the traffic light and sending that interpreted result to the backend.

The current crossing hardware remains the same. The ESP32-S3 still controls the logic, the MCP23017 still expands the outputs, and the ULN2803 chips still switch the lamp channels. The new part of the design is the KY-021 sensor input on GPIO 6, the phase-aware interpretation link, and the backend-ready message structure.

So the design for this sprint is:

```text
existing four-way crossing + KY-021 sensor input + phase-aware interpretation + backend message output
```

The detailed reasoning for why this step matters belongs to the analysis. The purpose of this design is to show how that analysis is translated into a concrete sensor connection, software structure, and backend message design (Wesley, 2026a). 

## References

Wesley. (2026a). *Analysis - Recognizing when a car is waiting at the traffic light and sending that information to the backend* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/d8bf5f628df14829a640adcf2e5e1bee2b7e9700/docs/Wesley/learning%20outcomes/sprint%203/Analysis%20-%20Recognizing%20when%20a%20car%20is%20waiting%20at%20the%20traffic%20light%20and%20sending%20that%20information%20to%20the%20backend.md

Wesley. (2026b). *Advice — Learning Goal 1 - Expanding the traffic-light system to multiple traffic lights* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/learning%20goals/sprint%202/Sprint%202,%20Learning%20Goal%201%20-%20Expanding%20the%20traffic-light%20system%20to%20multiple%20traffic%20lights.md

Wesley. (2026c). *Design - Four-Way Traffic-Light Crossing with Low Pin Usage and External LED Power v2* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/d8bf5f628df14829a640adcf2e5e1bee2b7e9700/docs/Wesley/learning%20outcomes/sprint%202/Design%20-%20Four-Way%20Traffic-Light%20Crossing%20with%20Low%20Pin%20Usage%20and%20External%20LED%20Power%20v2.md 

Wesley. (2026d). *Realisation - Breadboard Realisation of the Four-Way Traffic-Light Setup* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/learning%20outcomes/sprint%202/Realisation%20-%20Breadboard%20Realisation%20of%20the%20Four-Way%20Traffic-Light%20Setup.md

Wesley. (2026e). *Smart Traffic Light - When Does a Traffic Light Become Smart?* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/research/Smart%20Traffic%20Light%20-%20When%20Does%20a%20Traffic%20Light%20Become%20Smart.md

Wesley. (2026f). *Sprint 2, Learning Goal 1 - Expanding the traffic-light system to multiple traffic lights* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/learning%20goals/sprint%202/Sprint%202,%20Learning%20Goal%201%20-%20Expanding%20the%20traffic-light%20system%20to%20multiple%20traffic%20lights.md

Wesley. (2026g). *Traffic Light - first iteration - implementation* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/research/Traffic%20Light%20-%20first%20iteration%20-%20implementation.md

## Appendix A — Proof of Design

This appendix contains the design proof for this deliverable. It links the written design choices to the visual design evidence created in Fritzing. Together, these figures show the logical wiring, the practical breadboard layout, and the bill of materials for the prototype design in which the existing four-way traffic-light crossing is extended with the KY-021 sensor input.

### A.1 Fritzing schematic view

**Figure A1. Fritzing schematic view of the design**

Figure A1 shows the logical wiring of the design. In this schematic, the relationship between the main controller, the output-expansion layer, the switching layer, the sensor input, and the traffic-light outputs is made clear.

The figure shows that:

* the **ESP32-S3** remains the main controller
* **GPIO 4** and **GPIO 5** are connected to the **MCP23017** for the I2C connection
* **GPIO 6** is used for the **KY-021** sensor signal
* the **MCP23017** is connected to the **ULN2803** chips for output switching
* the **ULN2803** chips switch the externally powered traffic-light LED channels
* the logic side and the sensor side share the same ground reference

This schematic is important because it proves that the new sensor input is added without changing the already established crossing structure from Sprint 2. It also shows clearly that the KY-021 is an input to the controller and not a direct replacement for the traffic-light control logic.

### A.2 Fritzing breadboard view

**Figure A2. Fritzing breadboard view of the design**

Figure A2 shows the practical breadboard layout of the design. This figure translates the schematic into a physical arrangement that can be used during realisation.

The breadboard view shows:

* the placement of the **ESP32-S3**
* the placement of the **MCP23017**
* the placement of the **two ULN2803 chips**
* the placement of the **KY-021 sensor**
* the routing of the I2C lines
* the connection of the sensor signal to **GPIO 6**
* the continued use of the existing traffic-light output structure
* the shared ground and power connections needed for the system

This figure is useful because it makes the design easier to build in practice. It also shows that the new sensor feature is added as an extra layer on top of the current crossing instead of forcing a redesign of the whole breadboard structure.

### A.3 Bill of Materials — BoM

**Figure A3. Fritzing Bill of Materials**

Figure A3 shows the bill of materials generated from Fritzing. This overview supports the design by listing the main parts required to build the prototype as designed.

The bill of materials confirms the use of:

* **1 ESP32-S3 development board**
* **1 MCP23017**
* **2 ULN2803 chips**
* **1 KY-021 Mini Magnetic Reed Switch Module**
* **traffic-light LEDs**
* **resistors for the LED channels**
* **wires and breadboard connections**
* other small required connection materials

This overview is important because it connects the written design to the actual components needed for realisation. It also helps show that this design remains manageable as a learning project, because the added smart feature only introduces one extra sensor module on top of the already existing traffic-light hardware.

### Appendix conclusion

Together, Figure A1, Figure A2, and Figure A3 provide the design proof for this deliverable. The schematic shows the logical design, the breadboard view shows the physical layout, and the bill of materials shows the required parts. This gives a complete design proof that the added KY-021 sensor input fits correctly into the existing four-way traffic-light structure and supports the next realisation step.
