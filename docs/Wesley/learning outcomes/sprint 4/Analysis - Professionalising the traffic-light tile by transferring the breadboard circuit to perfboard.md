# Analysis - Professionalising the traffic-light tile by transferring the breadboard circuit to perfboard

## Introduction

At the start of this learning goal, I already have a working traffic-light project. In earlier sprints, I first built a basic single traffic light and then expanded it into a coordinated four-way crossing. The current crossing is no longer controlled by one direct ESP32-S3 pin per LED. Instead, the system uses an ESP32-S3, an MCP23017 I/O expander, two ULN2803 driver chips, and external LED power. This structure was chosen earlier because it uses fewer ESP32-S3 pins, separates controller logic from LED switching, and leaves room for later expansion (Wesley, 2026a).

The current four-way version has already been realised on a breadboard. It starts safely, follows the intended traffic-light sequence, avoids conflicting green states, uses `millis()` based timing, and cycles stably. The realised setup also showed that the ESP32-S3 only needs two I2C lines to control the MCP23017, while the MCP23017 and ULN2803 chips handle the output channels for the traffic lights (Wesley, 2026a).

In my advice after the previous sprint, I already concluded that the current system is a good intermediate version, but that it is still a breadboard prototype. The project works, but the physical form is still more vulnerable to wiring mistakes and layout issues than a cleaner final build. I also concluded that I should keep the current hardware structure, keep the `millis()` based state machine, freeze one final mapping, and continue using staged testing (Wesley, 2026b).

This learning goal therefore focuses on professionalising the physical electronics. The goal is not to change the traffic-light logic itself. The goal is to transfer the working circuit from the breadboard to a permanent perfboard setup that can be mounted in the city tile and connected to the traffic-light models in a cleaner and more reliable way.

## Current situation

The current traffic-light system is built around a modular hardware structure. The ESP32-S3 controls the logic, the MCP23017 expands the number of output channels over I2C, and the ULN2803 chips switch the LED channels. The LEDs are powered from an external supply, while the ESP32-S3 and MCP23017 use the logic supply. A shared ground is used so that the controller, expander, drivers, and LED power supply have the same reference (Wesley, 2026a).

The realised breadboard setup uses GPIO 5 for SDA and GPIO 4 for SCL. The MCP23017 uses I2C address `0x20`, because A0, A1, and A2 are connected to ground. The final realised output mapping connects the four traffic lights through MCP23017 outputs and ULN2803 switching channels. The breadboard version has already been tested for safe startup, correct sequence, no conflicting green states, correct `millis()` timing, and stable repeated cycling (Wesley, 2026a).

The current project has also been extended with a first smart-input feature. A KY-021 sensor was added to one direction so the system can recognise whether a car is waiting and create backend-ready traffic-state information. That extension kept the existing safe traffic-light behaviour stable, but it also showed that the project is becoming more than a small LED test circuit (Wesley, 2026c).

The main limitation of the current physical setup is that it is still built on a breadboard. The breadboard was suitable for building and testing, but the city tile needs a more stable physical form. If the project is moved, mounted, or demonstrated repeatedly, loose jumper wires and temporary breadboard contacts can become a weakness.

---

## Problem definition

The main problem in this learning goal is how to turn the current working breadboard circuit into a more permanent and professional circuit without breaking the system that already works.

The solution must meet these conditions:

- it must keep the existing ESP32-S3, MCP23017, ULN2803, external LED power structure;
- it must keep the safe traffic-light sequence and non-blocking timing;
- it must make the wiring more fixed and traceable than loose breadboard jumper wires;
- it must make the power, ground, I2C, driver, and LED wiring easier to inspect;
- it must support mounting in or under the city tile;
- it must support outgoing wires to the traffic-light models;
- it must remain testable after soldering;
- it must remain understandable enough for a learning project.

The problem is not only cosmetic. Hiding the breadboard would make the tile look cleaner, but it would not solve the real embedded-hardware issue. The real issue is that a temporary prototype must become a more permanent physical implementation.

SparkFun explains that solderless breadboards are useful for temporary circuits and prototyping because they allow circuits to be built without soldering (SparkFun Electronics, n.d.-a). That matches how I used the breadboard in the previous sprint. However, the same temporary character is also why the breadboard should not remain the final physical form for a tile-mounted traffic-light system.


## Why the breadboard setup is not suitable as the final physical form

The breadboard setup is suitable for testing, but not for the final physical form of this project. On a breadboard, components and wires are placed in temporary contact points. This is practical while I still need to experiment, but it also means that the circuit depends on many removable connections.

SparkFun describes solderless breadboards as useful for temporary circuits and prototyping, and the source explains that they allow circuit building without soldering (SparkFun Electronics, n.d.-a). That is exactly why the breadboard was useful earlier: I could test the ESP32-S3, MCP23017, ULN2803 chips, LED channels, and timing logic before making the build permanent.

For the city tile, the situation is different. The project now needs to be mounted, moved, connected to traffic-light models, and demonstrated. In that situation, the temporary nature of the breadboard becomes a disadvantage. A wire that works during one test can be pulled loose during movement or mounting. A breadboard layout with many jumper wires can also become difficult to inspect when something stops working.

The breadboard also makes the wiring harder to manage as the project grows. The current system contains logic wiring, I2C wiring, driver wiring, external LED power, shared ground, and outgoing LED wires. When all of that stays on a breadboard with jumper wires, it becomes easier to lose track of which wire belongs to which traffic-light channel.

For this reason, the breadboard should be treated as the proven prototype, not as the final physical implementation.

---

## What professionalising means in this learning goal

In this learning goal, professionalising does not mean making a commercial product. It means improving the project from a temporary prototype into a more permanent and controlled embedded build.

For this project, professionalising means:

- moving the circuit from breadboard to perfboard;
- soldering the important electrical connections;
- making power and ground routes clear;
- using fixed connections instead of loose jumper wires;
- using connectors or clearly grouped wires for the traffic-light models;
- protecting and organising outgoing wires;
- keeping the channel mapping traceable;
- making the circuit testable after soldering;
- mounting the electronics in or under the tile in a stable way.

This is different from only hiding wires. Hidden wiring is part of the final result, but it is not the main learning. The main learning is how to make the electronics more permanent, traceable, and maintainable.

Adafruit describes Perma-Proto boards as non-solderless versions of breadboards, using a sturdy printed circuit board with a layout that is nearly identical to a breadboard (Adafruit, 2016). I do not have to use that exact product, but the idea supports the chosen direction: move from temporary breadboard wiring to a soldered prototype board while keeping the layout understandable.


## Requirements for the professionalised circuit

The professionalised perfboard version must satisfy both functional and physical requirements.

| ID | Requirement | Priority | Reason |
|---|---|---|---|
| R1 | The perfboard circuit must keep the existing traffic-light behaviour. | Must | The project already works on the breadboard, so the transfer may not break the safe sequence. |
| R2 | The ESP32-S3, MCP23017, ULN2803, external LED power, and shared ground structure must remain the basis. | Must | The current architecture already solves the output and power-switching problem. |
| R3 | The circuit must have clear power and ground routing. | Must | Power and ground mistakes can make the full system unstable or difficult to debug. |
| R4 | The traffic-light output channels must be traceable from code to MCP23017 pin to driver input to LED wire. | Must | Debugging becomes difficult if the physical and software mappings no longer match. |
| R5 | The outgoing wires to the traffic-light models must be grouped and secured. | Must | The tile will contain separate traffic-light models, so the outgoing wiring must not pull loose easily. |
| R6 | The circuit must be testable in stages after soldering. | Must | A soldered circuit is harder to change, so testing must be possible before final mounting. |
| R7 | The perfboard must be mountable in or under the city tile. | Should | The circuit should support the physical tile integration. |
| R8 | The layout should leave space for later sensor or backend-related wiring. | Should | The project is still being extended with smart features. |
| R9 | The soldering and wiring should be inspectable. | Should | Visual inspection helps find possible solder bridges, weak-looking joints, and wrong connections. |
| R10 | The setup should remain understandable as a learning project. | Should | A clean but understandable layout is better than a compact layout that becomes impossible to debug. |

These requirements make the learning goal stronger because they focus on embedded-system quality instead of only appearance.

---

## Possible approaches

### Option 1 - keep the breadboard and hide it in the tile

The simplest option would be to keep the current breadboard and hide it inside or under the city tile. This would make the visible tile cleaner, but it would not solve the main technical problem.

The advantage of this option is that it is fast. The current circuit already works, so hiding the existing board would require less rebuilding. It would also reduce the risk of making soldering mistakes during transfer.

The disadvantage is that the electronics would still be temporary. Breadboards are suitable for temporary circuits and prototyping because they do not require soldering (SparkFun Electronics, n.d.-a). Hiding the breadboard would make the project look more finished, but the electrical connections would still depend on removable jumper wires and breadboard contacts.

**Assessment:** This option is not strong enough for this learning goal. It improves appearance, but it does not professionalise the electronics enough.

---

### Option 2 - transfer the circuit to perfboard

The second option is to transfer the circuit to perfboard. With perfboard, I can solder the components and wires into a fixed layout. This makes the circuit more permanent than a solderless breadboard while still being practical for a learning project.

The advantage of perfboard is that I can keep the same circuit architecture while improving the physical build. I can place the MCP23017, ULN2803 chips, resistors, power routes, ground routes, and outgoing wire groups in a planned layout. I can also use the existing breadboard circuit as a reference and transfer it step by step.

This option fits the current project stage. A perfboard version is more permanent than the breadboard, but it does not require the full process of designing, ordering, and validating a custom PCB. Adafruit’s Perma-Proto concept is a useful example of this type of transition, because it keeps a breadboard-like layout while making the connections solderable and more permanent (Adafruit, 2016).

The disadvantage is that soldering introduces new risks. A wrong solder bridge, weak solder joint, reversed connection, or badly routed wire can create faults that are harder to fix than on a breadboard. That means the transfer must be designed and tested carefully.

**Assessment:** This is the best option for this learning goal. It directly addresses the feedback because the learning becomes about professionalising the electronics instead of only hiding the prototype.

---

### Option 3 - design a custom PCB

The third option is to design a custom PCB for the traffic-light controller. This could create a cleaner final electronics solution. A PCB could have fixed traces, connectors, mounting holes, labels, and a compact layout.

The advantage is that a PCB would be the most polished option. It could make the project easier to reproduce and could reduce wiring mistakes after the design is validated.

The disadvantage is that this is probably too large for this learning goal. A custom PCB adds extra work: schematic capture, PCB layout, design-rule checking, manufacturing, delivery time, and possible revision if mistakes are found. It would also make debugging harder if the first PCB version contains an error.

For this project stage, I still need a build that I can realise and test quickly. Perfboard gives me a better balance between permanence and flexibility.

**Assessment:** A custom PCB is a good future improvement, but not the best first professionalisation step.

---