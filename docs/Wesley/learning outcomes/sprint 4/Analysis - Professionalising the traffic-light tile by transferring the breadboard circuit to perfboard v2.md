# Analysis - Professionalising the traffic-light tile by transferring the breadboard circuit to perfboard

## Summary

This analysis investigates how I can professionalise my current four-way traffic-light tile by transferring the working breadboard circuit to a more permanent perfboard circuit. The goal is to improve the physical structure, wiring clarity, testability, maintainability, and mounting quality of the electronics without changing the traffic-light logic.

The current system already works on a breadboard. It uses an ESP32-S3, an MCP23017 I2C output expander, two ULN2803 driver chips, external LED power, shared ground, and a `millis()` based traffic-light state machine. The breadboard version starts safely, follows the intended traffic-light sequence, prevents conflicting green states, and cycles stably (Wesley, 2026a).

The main limitation is the physical implementation. A solderless breadboard is useful for temporary circuits and prototyping because it allows circuits to be built and changed without soldering (SparkFun Electronics, n.d.-a). That fits the earlier prototype phase, but the project now needs a more permanent physical form for a tile that will be mounted, moved, demonstrated, connected to traffic-light models, and extended later.

The analysis compares three options: keeping the breadboard, transferring the circuit to perfboard, and designing a custom PCB. Perfboard is selected as the best next step because it keeps the project buildable and testable while reducing the dependency on temporary breadboard contacts. A custom PCB could be a future improvement, but it adds a separate PCB design workflow, including schematic work, footprint selection, board layout, and checks before production (KiCad, 2025). That is too large for this learning goal.

A correct result is a working perfboard version of the current traffic-light electronics that can be mounted in or under the city tile, has clear wiring and channel mapping, has inspected soldered connections, has protected outgoing wires, remains testable after mounting, and still behaves like the working breadboard version.

---

## Table of Contents


- [1. Introduction](#1-introduction)
  - [1.1 Project context](#11-project-context)
  - [1.2 Problem statement](#12-problem-statement)
  - [1.3 Main research question](#13-main-research-question)
  - [1.4 Sub-questions](#14-sub-questions)
  - [1.5 Methodology](#15-methodology)
  - [1.6 Scope and limitations](#16-scope-and-limitations)
- [2. Current situation](#2-current-situation)
- [3. Sub-question 1: Why is the current breadboard setup not suitable as the final physical form?](#3-sub-question-1-why-is-the-current-breadboard-setup-not-suitable-as-the-final-physical-form)
  - [3.1 Breadboard as prototype](#31-breadboard-as-prototype)
  - [3.2 Risks in the tile context](#32-risks-in-the-tile-context)
  - [3.3 Sub-conclusion](#33-sub-conclusion)
- [4. Sub-question 2: What does professionalising mean for this traffic-light project?](#4-sub-question-2-what-does-professionalising-mean-for-this-traffic-light-project)
  - [4.1 Physical professionalisation](#41-physical-professionalisation)
  - [4.2 Electrical professionalisation](#42-electrical-professionalisation)
  - [4.3 Maintainability and testability](#43-maintainability-and-testability)
  - [4.4 Sub-conclusion](#44-sub-conclusion)
- [5. Sub-question 3: Which implementation approach is most suitable for this stage?](#5-sub-question-3-which-implementation-approach-is-most-suitable-for-this-stage)
  - [5.1 Option 1 - keep the breadboard and hide it in the tile](#51-option-1---keep-the-breadboard-and-hide-it-in-the-tile)
  - [5.2 Option 2 - transfer the circuit to perfboard](#52-option-2---transfer-the-circuit-to-perfboard)
  - [5.3 Option 3 - design a custom PCB](#53-option-3---design-a-custom-pcb)
  - [5.4 Comparison table](#54-comparison-table)
  - [5.5 Sub-conclusion](#55-sub-conclusion)
- [6. Sub-question 4: What technical points must be controlled during the perfboard transfer?](#6-sub-question-4-what-technical-points-must-be-controlled-during-the-perfboard-transfer)
  - [6.1 Existing circuit architecture](#61-existing-circuit-architecture)
  - [6.2 Power and ground routing](#62-power-and-ground-routing)
  - [6.3 MCP23017 and I2C connections](#63-mcp23017-and-i2c-connections)
  - [6.4 ULN2803 low-side switching](#64-uln2803-low-side-switching)
  - [6.5 Soldering quality](#65-soldering-quality)
  - [6.6 Connectors, outgoing wires, and strain relief](#66-connectors-outgoing-wires-and-strain-relief)
  - [6.7 Mounting in or under the city tile](#67-mounting-in-or-under-the-city-tile)
  - [6.8 Sub-conclusion](#68-sub-conclusion)
- [7. Sub-question 5: How can the perfboard version be verified during realisation?](#7-sub-question-5-how-can-the-perfboard-version-be-verified-during-realisation)
  - [7.1 Verification strategy](#71-verification-strategy)
  - [7.2 Planned test cases](#72-planned-test-cases)
  - [7.3 Exit criteria](#73-exit-criteria)
  - [7.4 Sub-conclusion](#74-sub-conclusion)
- [8. Requirements](#8-requirements)
- [9. Risks](#9-risks)
- [10. Acceptance criteria for the realisation](#10-acceptance-criteria-for-the-realisation)
- [11. Conclusion](#11-conclusion)
  - [11.1 Answer to the main research question](#111-answer-to-the-main-research-question)
  - [11.2 Result for the next phase](#112-result-for-the-next-phase)
- [References](#references)
- [Appendix A - Responsible use of ChatGPT](#appendix-a---responsible-use-of-chatgpt)


---

## 1. Introduction

### 1.1 Project context

At the start of this learning goal, I already have a working four-way traffic-light setup. In earlier sprints, I first built a basic single traffic light and then expanded it into a coordinated crossing. The current crossing uses an ESP32-S3 as the controller, an MCP23017 as I/O expander, two ULN2803 driver chips, and external LED power. This structure was chosen earlier because it uses fewer ESP32-S3 pins than a direct one-pin-per-lamp approach, separates controller logic from LED switching, and leaves room for later expansion (Wesley, 2026a).

The current four-way version has already been realised on a breadboard. It starts safely, follows the intended traffic-light sequence, avoids conflicting green states, uses `millis()` based timing, and cycles stably (Wesley, 2026a). The Arduino documentation states that `millis()` returns the number of milliseconds since the board began running the current program, which matches the timing basis used in the current controller (Arduino, 2025).

The project has also been extended with a first smart-input feature. A KY-021 sensor was added to one direction so the system can recognise whether a car is waiting and create backend-ready traffic-state information. That extension kept the existing safe traffic-light behaviour stable, but it also made the physical setup more complex because the system now includes both output wiring and sensor wiring (Wesley, 2026c).

This learning goal focuses on professionalising the physical electronics. The goal is not to change the traffic-light logic. The goal is to transfer the working circuit from the breadboard to a permanent perfboard setup that can be mounted in or under the city tile and connected to the traffic-light models in a clearer and more controlled way.

### 1.2 Problem statement

The current breadboard setup works, but it is not suitable as the final physical form of the traffic-light tile. SparkFun describes solderless breadboards as useful for temporary circuits and prototyping because they do not require soldering (SparkFun Electronics, n.d.-a). In my project, that temporary character was useful while testing the circuit, but it is not the physical form I want for the mounted tile.

The city tile must be moved, demonstrated, connected to traffic-light models, and extended later. Those are project-specific conditions from my own build context, not general claims from an external source. Under those conditions, loose jumper wires and temporary breadboard contacts are weak points because the circuit depends on connections that can be disturbed during handling.

The problem is therefore not only cosmetic. Hiding the breadboard would make the tile look cleaner, but the electronics would still depend on a temporary prototyping method. The circuit needs a more permanent form without losing the behaviour that already works.

### 1.3 Main research question

How can I professionalise my working four-way traffic-light breadboard circuit by transferring it to a perfboard setup, while keeping the existing traffic-light behaviour safe, traceable, testable, and suitable for mounting in or under the city tile?

### 1.4 Sub-questions

1. Why is the current breadboard setup not suitable as the final physical form for the traffic-light tile?
2. What does professionalising mean for this traffic-light project?
3. Which implementation approach is most suitable for this stage: keeping the breadboard, transferring the circuit to perfboard, or designing a custom PCB?
4. What technical points must be controlled when transferring the current circuit to perfboard?
5. How can the perfboard version be verified during the realisation phase?

### 1.5 Methodology

This analysis uses a structured comparison and verification method. First, the current working breadboard situation is described, because the perfboard version must preserve the behaviour that already works. After that, the analysis is divided into five sub-questions. Each sub-question answers one part of the main research question and ends with a sub-conclusion.

The analysis uses three source types:

| Source type | Use in this analysis |
|---|---|
| Earlier project deliverables | Used for the current situation, existing behaviour, realised wiring, previous advice, and project-specific constraints. |
| Manufacturer documentation and datasheets | Used for technical claims about the MCP23017, ULN2803, ESP32-S3, and Arduino timing function. |
| Practical electronics documentation | Used for general prototyping, soldering, wiring, and test-planning practices. |

Not every sentence in this analysis needs an external source. Some statements are project-specific design reasoning, such as the fact that my tile must be moved and demonstrated. External sources are used where the document makes technical or general claims, such as what a breadboard is normally used for, what the MCP23017 provides, what the ULN2803 is designed to do, and what `millis()` returns.

### 1.6 Scope and limitations

This analysis focuses on the transfer from the current working breadboard circuit to a more permanent perfboard circuit. It does not redesign the traffic-light logic, replace the MCP23017 and ULN2803 architecture, or create a custom PCB. The existing traffic-light behaviour, output-expansion structure, external LED power approach, shared ground principle, and `millis()` based state machine are treated as the current working baseline (Wesley, 2026a).

This analysis does not prove the final soldered result yet. The actual construction, test evidence, problems found, fixes, and final result belong to the realisation phase.

---

## 2. Current situation

The current traffic-light system is built around a modular hardware structure. The ESP32-S3 controls the logic, the MCP23017 expands the number of output channels over I2C, and the ULN2803 chips switch the LED channels. The LEDs are powered from an external supply, while the ESP32-S3 and MCP23017 use the logic supply. A shared ground is used so that the controller, expander, drivers, and LED power supply have the same reference (Wesley, 2026a).

The realised breadboard setup uses GPIO 5 for SDA and GPIO 4 for SCL. The MCP23017 uses I2C address `0x20`, because A0, A1, and A2 are connected to ground. The breadboard version has already been tested for safe startup, correct sequence, no conflicting green states, correct `millis()` timing, and stable repeated cycling (Wesley, 2026a).

The MCP23017 is a suitable part for the current output-expansion role because Microchip describes it as a 16-bit I/O expander with a serial interface and identifies the MCP23017 as the I2C version of the device family (Microchip Technology Inc., 2022). The ULN2803A is suitable for the current switching role because the Texas Instruments datasheet describes it as a Darlington transistor array and lists lamp-driver and display-driver applications (Texas Instruments, 2004).

The main limitation is physical. The circuit is still built on a breadboard with many jumper wires and temporary contacts. That form was suitable for building and testing, but the city tile now needs a more stable and maintainable physical implementation.

---

## 3. Sub-question 1: Why is the current breadboard setup not suitable as the final physical form?

### 3.1 Breadboard as prototype

A solderless breadboard is useful during prototyping because components and wires can be placed without soldering. SparkFun explains that solderless breadboards are used for temporary circuits and prototyping because they allow circuits to be built without soldering (SparkFun Electronics, n.d.-a). This matches how the breadboard was used in the previous sprint: it made it possible to test the ESP32-S3, MCP23017, ULN2803 chips, LED channels, and traffic-light timing before making the build permanent (Wesley, 2026a).

The same temporary character becomes a disadvantage in the final tile. A breadboard depends on removable contacts. That is useful for experiments, but it does not match my goal of making the electronics more fixed, traceable, and suitable for repeated handling.

### 3.2 Risks in the tile context

The city tile creates different requirements from a desk prototype. The circuit must stay connected while the tile is moved, while wires are routed to traffic-light models, and while the system is demonstrated. These are project-specific requirements from my own build context. With the current breadboard setup, a jumper wire can be pulled loose and the wiring can become difficult to inspect.

The project has also grown beyond a simple LED test. It now contains logic wiring, I2C wiring, driver wiring, external LED power, shared ground, outgoing LED wires, and sensor input (Wesley, 2026a; Wesley, 2026c). Keeping all of that on a breadboard makes the physical implementation harder to maintain because many temporary connections must remain correct at the same time.

### 3.3 Sub-conclusion

The current breadboard setup is not suitable as the final physical form because it depends on temporary contacts and loose jumper wires. It proved that the circuit works, but the city tile needs electronics that are more permanent, easier to inspect, easier to mount, and less vulnerable to movement. This conclusion follows from the temporary purpose of a breadboard and from the practical conditions of my own traffic-light tile.

---

## 4. Sub-question 2: What does professionalising mean for this traffic-light project?

### 4.1 Physical professionalisation

In this learning goal, professionalising means improving the project from a temporary prototype into a more permanent embedded build. It does not mean making a commercial product.

For this project, physical professionalisation means:

- transferring the circuit from breadboard to perfboard;
- soldering the important electrical connections;
- reducing loose jumper-wire dependency;
- grouping outgoing wires to the traffic-light models;
- protecting wire exits with strain relief;
- mounting the electronics in or under the tile in a stable way;
- keeping access to connectors, test points, and USB/programming where needed.

Adafruit describes Perma-Proto boards as non-solderless versions of breadboards, using a printed circuit board with a layout that is nearly identical to a breadboard (Adafruit, 2016). I do not need that exact product, but the concept supports the direction from temporary breadboard wiring to a soldered prototype board.

### 4.2 Electrical professionalisation

Electrical professionalisation means that the circuit remains understandable and controlled after the transfer. The perfboard version should have a planned structure for logic power, external LED power, shared ground, I2C lines, driver inputs, driver outputs, and LED connections.

For this project, that means:

- one clear logic power route;
- one clear shared ground route;
- one clear external LED power route;
- a defined connection point between controller ground and external LED power ground;
- clear SDA and SCL routing between the ESP32-S3 and MCP23017;
- correct MCP23017 address and RESET connections;
- clear mapping from MCP23017 outputs to ULN2803 inputs;
- correct LED power routing through the ULN2803 switching stage;
- separation between 3.3V logic and external LED power where practical.

Espressif’s ESP32-S3 hardware design guidelines are intended for integrating ESP32-S3 into a product and include schematic and power-supply design guidance (Espressif Systems, 2025). My project is not a custom ESP32-S3 PCB, so I only use this source to support the general point that power and ground planning matter when building an ESP32-S3-based circuit.

### 4.3 Maintainability and testability

Professionalising also means that the circuit remains understandable after soldering. A circuit can be more permanent but still poor if it becomes difficult to test, repair, or explain. This is a project-quality requirement, based on the fact that the traffic-light system is still being developed and extended.

For this project, maintainability and testability mean:

- the channel mapping is traceable from code to MCP23017 pin to ULN2803 channel to physical traffic light;
- outgoing wires are grouped by traffic light or function;
- power and ground are labelled or documented;
- the circuit can be inspected from both component side and solder side;
- the circuit can be tested in stages before final mounting;
- the layout leaves space for later sensor or backend-related wiring.

This matches my earlier project advice, where I concluded that I should freeze one final mapping and continue using staged testing before adding more complexity (Wesley, 2026b).

### 4.4 Sub-conclusion

Professionalising this traffic-light project means making the electronics more permanent, traceable, testable, and suitable for tile integration. For this stage, that means transferring the proven breadboard circuit to a planned perfboard layout with soldered connections, clear power and ground routing, grouped outgoing wires, strain relief, and staged verification.

---

## 5. Sub-question 3: Which implementation approach is most suitable for this stage?

### 5.1 Option 1 - keep the breadboard and hide it in the tile

The simplest option is to keep the current breadboard and hide it inside or under the city tile. This is fast because the current circuit already works. It also avoids the immediate risk of soldering mistakes.

The disadvantage is that the electronics remain temporary. Breadboards are suitable for temporary circuits and prototyping because they do not require soldering (SparkFun Electronics, n.d.-a). Hiding the breadboard improves appearance, but it does not remove the dependency on temporary breadboard contacts and jumper wires.

**Assessment:** not suitable for this learning goal, because it hides the prototype instead of professionalising it.

### 5.2 Option 2 - transfer the circuit to perfboard

The second option is to transfer the circuit to perfboard. With perfboard, the components and wires can be soldered into a fixed layout. This can make the circuit more permanent than a solderless breadboard because the connections no longer depend on removable breadboard contact points. However, this is only an improvement if the soldering, routing, strain relief, and testing are done carefully.

The advantage is that the existing architecture can remain the same. The MCP23017, ULN2803 chips, resistors, power routes, ground routes, and outgoing wire groups can be placed in a planned layout. The existing breadboard circuit can be used as a reference and transferred step by step.

The disadvantage is that soldering introduces new risks. SparkFun’s through-hole soldering tutorial discusses soldering technique and rework, which supports the point that soldering is a skill that must be controlled rather than assumed to be automatically correct (SparkFun Electronics, n.d.-b). A wrong solder bridge, weak solder joint, reversed connection, or badly routed wire can create faults that are harder to fix than on a breadboard.

**Assessment:** best option for this learning goal, because it directly improves the physical implementation while keeping the scope realistic.

### 5.3 Option 3 - design a custom PCB

The third option is to design a custom PCB. A PCB could have fixed traces, connectors, mounting holes, labels, and a compact layout. That could make the final electronics cleaner and easier to reproduce.

The disadvantage is that a PCB adds a separate design workflow. KiCad documentation describes a project workflow that includes schematic creation, footprint selection, electrical rules checking, transfer to the board editor, and PCB layout (KiCad, 2025). For this learning goal, that extra workflow would shift the focus away from transferring and validating the already working circuit. It would also introduce manufacturing and possible revision work, which is outside the intended scope of this step.

**Assessment:** useful as a future improvement, but not the best first professionalisation step.

### 5.4 Comparison table

| Option | Strengths | Weaknesses | Suitability |
|---|---|---|---|
| Keep the breadboard and hide it | Fast, low effort, current circuit already works | Still depends on temporary contacts and jumper wires | Not suitable |
| Transfer to perfboard | Soldered connections, still flexible, realistic for this stage | Requires careful soldering, mapping, inspection, and rework if mistakes happen | Best option |
| Design a custom PCB | Cleanest and most reproducible final result | Adds schematic-to-PCB workflow, production, and revision risk | Future improvement |

### 5.5 Sub-conclusion

Perfboard is the most suitable approach for this learning goal. Keeping the breadboard would only hide the temporary prototype. A custom PCB could be better later, but it adds too much extra design and production work for this stage. Perfboard gives the best balance between permanence, flexibility, testability, and realistic sprint scope.

---

## 6. Sub-question 4: What technical points must be controlled during the perfboard transfer?

### 6.1 Existing circuit architecture

The perfboard transfer should keep the existing architecture:

```text
ESP32-S3 -> MCP23017 -> 2x ULN2803 -> external LED power -> traffic-light LEDs
```

The MCP23017 is suitable for this structure because Microchip describes it as a 16-bit I/O expander with a serial interface. The datasheet identifies the MCP23017 as the I2C version and describes the device as providing 16-bit general-purpose parallel I/O expansion through serial communication (Microchip Technology Inc., 2022).

The ULN2803A is suitable as a driver stage because Texas Instruments describes it as a Darlington transistor array with eight NPN Darlington pairs. The datasheet lists applications such as relay drivers, lamp drivers, display drivers, line drivers, and logic buffers (Texas Instruments, 2004). In this project, I use the ULN2803 chips to switch externally powered LED channels instead of letting the ESP32-S3 power all traffic-light outputs directly (Wesley, 2026a).

### 6.2 Power and ground routing

Power and ground routing are critical in the perfboard version because the circuit contains both a logic side and an LED power side:

- logic side: ESP32-S3, MCP23017, I2C pull-ups, sensor input;
- LED power side: external LED supply, ULN2803 switched paths, LED channels.

These areas have different roles, but they still need a shared reference. The current breadboard setup already uses a shared ground between the ESP32-S3, MCP23017, ULN2803 chips, and external LED power supply (Wesley, 2026a). The perfboard layout must therefore define a clear logic power route, shared ground route, external LED power route, and connection point between controller ground and external LED ground.

### 6.3 MCP23017 and I2C connections

The MCP23017 connection must be transferred carefully because it is the communication link between the ESP32-S3 and the output channels. The current realised setup uses GPIO 5 for SDA and GPIO 4 for SCL. It also uses I2C address `0x20`, because A0, A1, and A2 are connected to ground (Wesley, 2026a). Microchip states that the MCP23X17 address pins A2, A1, and A0 must be externally biased, which supports the need to wire the address pins deliberately instead of leaving them floating (Microchip Technology Inc., 2022).

The transfer must preserve:

- SDA and SCL wiring;
- I2C pull-up resistors;
- VDD and VSS wiring;
- A0, A1, and A2 address wiring;
- RESET wiring;
- mapping from MCP23017 output pins to ULN2803 inputs.

The I2C scanner test is the first communication check after soldering, because it verifies whether the controller can still detect the expander at the expected address.

### 6.4 ULN2803 low-side switching

In this project, the ULN2803 works as a low-side switching stage. The LED current comes from the external LED supply, passes through the LED branch, and is switched to ground through the ULN2803 when the matching input is active. This interpretation follows from the ULN2803A being an NPN Darlington transistor array with high-voltage outputs for switching loads (Texas Instruments, 2004).

This matters because the ULN2803 does not provide positive LED power. It sinks current. The perfboard layout must therefore keep the positive LED supply and switched ground paths clear.

The transfer must control:

- correct orientation of both ULN2803 chips;
- correct input connections from MCP23017 outputs;
- correct output connections to LED branches;
- correct ground connection;
- correct resistor placement for each LED channel;
- separation between logic signals and LED current paths.

### 6.5 Soldering quality

Soldering quality becomes part of the project quality when moving to perfboard. SparkFun explains through-hole soldering as a practical technique and includes soldering tools, basic soldering technique, and rework (SparkFun Electronics, n.d.-b).

For this project, good soldering means no accidental solder bridges, no loose joints, no exposed wires touching each other, no overheated component leads, and enough spacing around IC pins to inspect the work. These are practical quality checks for the realisation phase. Visual inspection and continuity testing are needed before applying power.

### 6.6 Connectors, outgoing wires, and strain relief

The traffic-light models are not all placed directly on the perfboard. They need outgoing wires from the controller board to the physical traffic-light positions on the tile. These wires should be grouped logically, preferably by traffic light or function.

| Wire group | Function |
|---|---|
| Traffic light 1 | Red, yellow, green, and return or related channel wiring |
| Traffic light 2 | Red, yellow, green, and return or related channel wiring |
| Traffic light 3 | Red, yellow, green, and return or related channel wiring |
| Traffic light 4 | Red, yellow, green, and return or related channel wiring |
| Sensor | VCC, GND, signal |
| Power | External LED power and ground |

Wire protection is important because movement should not pull directly on solder joints. SparkFun’s wire tutorial explains practical wire-working steps such as stripping wire, preparing wire ends, soldering splices, and using heat shrink after soldering (SparkFun Electronics, n.d.-c). Heat shrink supports insulation, but mechanical strain relief still requires physical support such as tie-wraps, cable clips, mounting holes, or connector placement near the board edge. That strain-relief choice is project-specific and must be finalised in the design and realisation phases.

### 6.7 Mounting in or under the city tile

The perfboard circuit must be suitable for the physical city tile. Because the tile is small and thin, the realistic approach is to mount the perfboard under the tile or in a protected area connected to the tile. This is a project-specific constraint from my model, not an external electronics rule.

The mounting method must support stable placement, access to USB or programming when needed, access to connectors or test points, clean wire routing, and prevention of short circuits against screws, metal parts, or exposed wire. These requirements follow from the goal that the circuit must remain testable and maintainable after it is mounted.

### 6.8 Sub-conclusion

The perfboard transfer must control the existing architecture, power routing, shared ground, MCP23017 I2C setup, ULN2803 low-side switching, soldering quality, outgoing wire grouping, strain relief, and safe mounting. These points determine whether the perfboard version is actually a professionalised version of the breadboard prototype.

---

## 7. Sub-question 5: How can the perfboard version be verified during realisation?

### 7.1 Verification strategy

The perfboard version must be verified in stages. A single final test is not enough for this project because a soldering or wiring fault can be difficult to locate once the whole circuit is mounted. Atlassian describes a test plan as a structured way to define scope, strategy, scenarios, criteria, and risks (Atlassian, n.d.). I use that idea here in a smaller embedded-hardware context.

The main verification question is:

**Does the perfboard version still behave like the working breadboard version, while being physically more stable, clearer, and suitable for mounting in the city tile?**

The verification focuses on two areas:

1. **Electrical correctness**: power, ground, I2C, MCP23017 outputs, ULN2803 switching, LED channels, and traffic-light sequence.
2. **Physical professionalisation**: soldering quality, wire grouping, wire protection, strain relief, mounting, and traceability.

### 7.2 Planned test cases

| Test ID | Test name | Expected result | Evidence |
|---|---|---|---|
| T1 | Visual inspection before power | No visible solder bridges, loose wires, reversed ICs, or exposed wires touching | Photos of component side and solder side |
| T2 | Ground continuity check | ESP32-S3, MCP23017, ULN2803 chips, and external LED ground share the required reference | Multimeter notes or photo |
| T3 | Power short check | No short between 3.3V and GND or between external LED power and GND | Multimeter notes |
| T4 | Logic voltage check | MCP23017 and I2C pull-ups receive the correct logic voltage | Voltage measurement |
| T5 | External LED power check | LED power is present only on the intended route and not mixed with 3.3V logic | Voltage measurement |
| T6 | ESP32-S3 serial test | Controller runs and serial output works | Serial monitor screenshot |
| T7 | I2C scanner test | MCP23017 is detected at address `0x20` | Serial monitor screenshot |
| T8 | MCP23017 output test | Selected MCP23017 output changes as expected | LED, multimeter, or serial proof |
| T9 | ULN2803 single-channel test | One MCP23017 output switches one LED channel through the ULN2803 | Photo or video proof |
| T10 | Full channel mapping test | Every physical LED matches the final mapping table | Mapping checklist and photos |
| T11 | Safe startup test | System starts in all-red or the defined safe startup state | Photo or video proof |
| T12 | Full sequence test | Traffic lights follow the intended phase order without conflicting green states | Video or observation table |
| T13 | `millis()` timing test | Phase timing follows the configured values closely enough for the prototype | Timing notes or serial output |
| T14 | Wire grouping and strain relief check | Wires are grouped and movement does not stress solder joints directly | Photos and observation notes |
| T15 | Mounted tile test | System still works after mounting in or under the tile | Final photos and video |

The `millis()` timing test is included because the current traffic-light controller already depends on non-blocking timing. The Arduino documentation states that `millis()` returns the number of milliseconds since the board began running the current program (Arduino, 2025). The perfboard transfer should not change the software timing behaviour that was already tested on the breadboard (Wesley, 2026a).

### 7.3 Exit criteria

The perfboard realisation is successful when:

- no short circuit is found before powering the circuit;
- the logic side receives the correct voltage;
- the MCP23017 is detected over I2C;
- the ULN2803 channels switch the intended LED outputs;
- all traffic-light outputs match the final mapping table;
- the system starts safely;
- the traffic-light sequence works correctly;
- no conflicting green states occur;
- the `millis()` based timing still behaves correctly;
- outgoing wires are grouped and protected;
- the circuit works after mounting in or under the tile.

### 7.4 Sub-conclusion

The perfboard version must be verified with staged tests. The realisation first proves electrical safety, then communication with the MCP23017, then ULN2803 switching, then channel mapping, and finally the complete mounted tile behaviour. This keeps the transfer controlled and evidence-based.

---

## 8. Requirements

| ID | Requirement | Priority | Basis |
|---|---|---|---|
| R1 | The perfboard circuit must keep the existing traffic-light behaviour. | Must | The breadboard version already works and is the baseline for this transfer (Wesley, 2026a). |
| R2 | The ESP32-S3, MCP23017, ULN2803, external LED power, and shared ground structure must remain the basis. | Must | This architecture is already realised and tested in the current setup (Wesley, 2026a). |
| R3 | The circuit must have clear power and ground routing. | Must | Power and ground planning are important in ESP32-S3-based circuits, and the current setup uses both logic and external LED power (Espressif Systems, 2025; Wesley, 2026a). |
| R4 | The traffic-light output channels must be traceable from code to MCP23017 pin to driver input to LED wire. | Must | The project already depends on channel mapping between software and hardware (Wesley, 2026a). |
| R5 | The outgoing wires to the traffic-light models must be grouped and secured. | Must | The physical tile has separate traffic-light models and outgoing wiring, so movement may stress connections. |
| R6 | The circuit must be testable in stages after soldering. | Must | A soldered circuit is harder to change than a breadboard, so staged verification reduces debugging risk (Atlassian, n.d.; SparkFun Electronics, n.d.-b). |
| R7 | The perfboard must be mountable in or under the city tile. | Should | The learning goal is tile integration, not only a desk prototype. |
| R8 | The layout should leave space for later sensor or backend-related wiring. | Should | The project has already been extended with a smart-input feature and may continue to expand (Wesley, 2026c). |
| R9 | The soldering and wiring should be inspectable. | Should | Inspection and rework are part of practical soldering quality control (SparkFun Electronics, n.d.-b). |
| R10 | The setup should remain understandable as a learning project. | Should | The goal is a controlled student prototype, not the smallest possible circuit. |

---

## 9. Risks

| Risk | Cause | Effect | Mitigation |
|---|---|---|---|
| Wrong connection during transfer | Breadboard wiring is copied incorrectly | A channel may not work or may show the wrong colour | Create a mapping table and test each channel separately |
| Solder bridge | Pads or IC pins are connected accidentally | Short circuit or damaged component | Inspect visually and use continuity testing before power |
| Weak solder joint | Poor soldering technique or movement during cooling | Intermittent faults | Inspect and rework suspicious joints |
| Ground mistake | Logic ground and external LED ground are not connected correctly | ULN2803 switching may fail or behave unpredictably | Define one clear shared ground route |
| Power mix-up | 3.3V logic and external LED power are confused | Component damage | Label power lines and test voltage before connecting components |
| I2C wiring error | SDA/SCL swapped, missing pull-ups, or wrong RESET/address wiring | MCP23017 is not detected or does not respond | Run I2C scanner test before full output testing |
| IC orientation error | MCP23017 or ULN2803 is placed in the wrong direction | Outputs fail or component may be damaged | Mark pin 1 and check orientation before soldering |
| Wrong ULN2803 switching assumption | LED branches are wired as if the ULN2803 provides positive power | LED channels do not switch correctly | Treat ULN2803 as low-side sink driver and test one channel first |
| Wires pull loose | Outgoing traffic-light wires move during tile assembly | Broken connection | Use grouped wires, clips, tie-wraps, or connectors |
| Mapping drift | Code, documentation, and physical wiring do not match | Debugging becomes difficult | Freeze and update one final channel mapping |
| Tile mounting causes shorts | Perfboard touches conductive material or exposed wire | Unstable or unsafe behaviour | Use insulation, standoffs, and controlled mounting points |

---

## 10. Acceptance criteria for the realisation

The acceptance criteria below define when the later perfboard realisation can be considered successful. They are not used to judge this analysis document itself. They are derived from the current working breadboard behaviour, the selected component architecture, the technical risks identified in this analysis, and the verification strategy for the realisation phase.

| ID | Acceptance criterion | Basis / source |
|---|---|---|
| AC1 | The working breadboard circuit is transferred to a soldered perfboard circuit without changing the intended traffic-light behaviour. | Current working baseline from the breadboard realisation (Wesley, 2026a). |
| AC2 | The perfboard version keeps the existing ESP32-S3, MCP23017, two ULN2803 chips, external LED power, and shared ground architecture. | Existing project architecture from the breadboard realisation and project advice (Wesley, 2026a; Wesley, 2026b). |
| AC3 | The ESP32-S3 can communicate with the MCP23017 over I2C after the transfer, and the MCP23017 is detected at address `0x20`. | Existing realised I2C setup and MCP23017 I2C function described in the datasheet (Wesley, 2026a; Microchip Technology Inc., 2022). |
| AC4 | The MCP23017 outputs control the correct ULN2803 input channels according to the final channel mapping. | Existing channel-mapping requirement from the realised breadboard setup and MCP23017 output-expander role (Wesley, 2026a; Microchip Technology Inc., 2022). |
| AC5 | The ULN2803 chips switch the intended LED channels correctly as low-side drivers. | ULN2803 driver role based on the component datasheet and existing project use (Texas Instruments, 2004; Wesley, 2026a). |
| AC6 | The 3.3V logic power, external LED power, and shared ground routes are clearly separated and tested before full operation. | Current project power structure and ESP32-S3 hardware-design attention to power and ground planning (Wesley, 2026a; Espressif Systems, 2025). |
| AC7 | No short circuit is detected between 3.3V and GND or between external LED power and GND before powering the full circuit. | Derived from the soldering and power-routing risks identified in this analysis; supported by the need for inspection before powering a soldered circuit (SparkFun Electronics, n.d.-b). |
| AC8 | The system starts in the defined safe startup state after power-up or reset. | Existing breadboard behaviour that must be preserved (Wesley, 2026a). |
| AC9 | The traffic lights follow the intended phase sequence after the circuit is transferred to perfboard. | Existing breadboard behaviour that must be preserved (Wesley, 2026a). |
| AC10 | No conflicting green states occur during repeated operation. | Existing safety behaviour from the breadboard realisation and project safety requirement (Wesley, 2026a). |
| AC11 | The `millis()` based timing still works correctly after the transfer. | Existing non-blocking timing structure and Arduino `millis()` documentation (Wesley, 2026a; Arduino, 2025). |
| AC12 | Every physical traffic-light LED matches the documented final channel mapping. | Derived from the traceability requirement in this analysis and the existing mapping dependency in the breadboard setup (Wesley, 2026a). |
| AC13 | Outgoing wires to the traffic-light models are grouped by traffic light or function. | Derived from the maintainability and traceability requirements in this analysis. |
| AC14 | Outgoing wires are protected so normal tile handling does not pull directly on solder joints. | Derived from the wire-protection risk in this analysis; supported by practical wire-preparation and insulation guidance (SparkFun Electronics, n.d.-c). |
| AC15 | The solder side and component side are visually inspected and show no visible solder bridges, loose wires, or unsafe exposed connections. | Derived from the soldering-quality risk in this analysis; supported by through-hole soldering and rework guidance (SparkFun Electronics, n.d.-b). |
| AC16 | The perfboard circuit remains accessible enough for testing, repair, or inspection after mounting. | Derived from the testability and maintainability requirements in this analysis. |
| AC17 | The perfboard version works after being mounted in or under the city tile. | Derived from the main project context and the tile-integration goal defined in this analysis. |
| AC18 | The realisation document includes test evidence, such as photos, multimeter results, serial monitor output, mapping checks, and video or observation proof of the working traffic-light sequence. | Derived from the verification strategy in this analysis and the idea that a test plan defines scope, test cases, evidence, and criteria (Atlassian, n.d.). |

---

## 11. Conclusion

### 11.1 Answer to the main research question

The main research question was:

**How can I professionalise my working four-way traffic-light breadboard circuit by transferring it to a perfboard setup, while keeping the existing traffic-light behaviour safe, traceable, testable, and suitable for mounting in or under the city tile?**

The answer is that the current working breadboard circuit should be transferred to a planned perfboard layout while keeping the existing ESP32-S3, MCP23017, two ULN2803 chips, external LED power, shared ground, and `millis()` based traffic-light state machine unchanged. The goal is not to redesign the traffic-light system, but to make the physical electronics more permanent, clearer, and better suited for the city tile.

The breadboard should be treated as the proven prototype, not as the final physical implementation. Breadboards are useful for temporary circuits and prototyping because they do not require soldering (SparkFun Electronics, n.d.-a). That matched the earlier testing phase, but it does not match the next project need: a tile-mounted setup that can be handled, demonstrated, connected to traffic-light models, and extended.

Perfboard is the best next step because it allows the existing circuit to be soldered into a fixed layout while keeping the project understandable and adjustable. A custom PCB could be a future improvement, but it adds a separate PCB workflow with schematic capture, footprint selection, checking, and board layout (KiCad, 2025). Keeping the breadboard would be faster, but it would not solve the physical implementation problem.

The perfboard transfer must control power and ground routing, correct I2C wiring to the MCP23017, correct ULN2803 low-side switching, traceable channel mapping, separation between 3.3V logic and external LED power, soldering quality, grouped outgoing wires, strain relief, and access for testing after mounting.

### 11.2 Result for the next phase

This analysis leads to a clear design direction: a perfboard layout with fixed component placement, documented channel mapping, grouped outgoing wires, planned power and ground routes, and testable connections. The realisation phase can then build that layout and verify it through staged tests.

A correct final result is not only a cleaner-looking tile. A correct result is a more permanent embedded hardware setup that keeps the current safe traffic-light behaviour, makes the wiring easier to understand, protects outgoing wires, remains testable after soldering, and can be mounted in or under the city tile.

---

## References

Adafruit. (2016, September 6). *Perma protos*. Adafruit Learning System. Retrieved May 20, 2026, from https://learn.adafruit.com/breadboards-for-beginners/perma-protos

Arduino. (2025, June 5). *millis()*. Arduino Documentation. Retrieved May 20, 2026, from https://www.arduino.cc/en/Reference/Millis

Atlassian. (n.d.). *Test plan template*. Retrieved May 20, 2026, from https://www.atlassian.com/software/confluence/resources/guides/how-to/test-plan

Espressif Systems. (2025). ESP32-S3 hardware design guidelines. https://docs.espressif.com/projects/esp-hardware-design-guidelines/en/latest/esp32s3/esp-hardware-design-guidelines-en-master-esp32s3.pdf

KiCad. (2025). *Getting started in KiCad*. KiCad Documentation. https://docs.kicad.org/9.0/en/getting_started_in_kicad/getting_started_in_kicad.html

Microchip Technology Inc. (2022). MCP23017/MCP23S17 - 16-Bit I/O Expander with Serial Interface. https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP23017-Data-Sheet-DS20001952.pdf

SparkFun Electronics. (n.d.-a). *How to use a breadboard*. SparkFun Learn. Retrieved May 20, 2026, from https://learn.sparkfun.com/tutorials/how-to-use-a-breadboard/all

SparkFun Electronics. (n.d.-b). *How to solder: Through-hole soldering*. SparkFun Learn. Retrieved May 20, 2026, from https://learn.sparkfun.com/tutorials/how-to-solder-through-hole-soldering/all

SparkFun Electronics. (n.d.-c). *Working with wire*. SparkFun Learn. Retrieved May 20, 2026, from https://learn.sparkfun.com/tutorials/working-with-wire/all

Texas Instruments. (2004). *ULN2803A Darlington transistor arrays* (Rev. C) [Data sheet]. https://cdn.sparkfun.com/assets/f/0/6/6/5/uln2803a.pdf

Wesley. (2026a). *Realisation - Breadboard realisation of the four-way traffic-light setup* [Realisation deliverable]. https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/b5f926b9d9d3c148d7a5338833cf6bd165b40239/docs/Wesley/learning%20outcomes/sprint%202/Realisation%20-%20Breadboard%20Realisation%20of%20the%20Four-Way%20Traffic-Light%20Setup.md

Wesley. (2026b). *Advice — Learning goal 1 - Expanding the traffic-light system to multiple traffic lights* [Advice deliverable]. https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/b5f926b9d9d3c148d7a5338833cf6bd165b40239/docs/Wesley/learning%20outcomes/sprint%202/Advice%20%E2%80%94%20Learning%20Goal%201%20-%20Expanding%20the%20traffic-light%20system%20to%20multiple%20traffic%20lights.md

Wesley. (2026c). *Realise - Recognizing when a car is waiting at the traffic light and sending that information to the backend* [Realisation deliverable]. https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/b5f926b9d9d3c148d7a5338833cf6bd165b40239/docs/Wesley/learning%20outcomes/sprint%203/Realise%20-%20Recognizing%20when%20a%20car%20is%20waiting%20at%20the%20traffic%20light%20and%20sending%20that%20information%20to%20the%20backend.md


## Appendix A - Responsible use of ChatGPT

For this deliverable, I used ChatGPT as a support tool for grammar, spelling, and suggestions for formatting. I did not use it to replace my own project work or to hide the source of the work.

The technical content, project context, circuit choices, learning goal, and final responsibility remain mine. ChatGPT was used to help improve readability, sentence structure, spelling, and the organisation of the document. I checked the final text myself so that it still matches my project and my own understanding.

I include this appendix to be transparent about the use of AI support. This makes the process honest and helps prevent the work from being presented as hidden AI-generated work or as plagiarism.