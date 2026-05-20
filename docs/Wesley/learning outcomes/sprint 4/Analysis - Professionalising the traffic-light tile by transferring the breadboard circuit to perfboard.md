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
