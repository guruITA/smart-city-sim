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

## Selected direction

The selected direction is:

```text
working breadboard circuit -> planned perfboard layout -> soldered permanent circuit -> mounted tile-ready electronics
```

I choose this direction because it solves the real problem of the current setup. The breadboard has already proven the circuit. Now the circuit needs a more permanent physical form.

The selected perfboard version must keep the same functional architecture:

```text
ESP32-S3 -> MCP23017 -> 2x ULN2803 -> external LED power -> traffic-light LEDs
```

The KY-021 sensor input and future backend-related wiring must also remain possible, but this analysis focuses mainly on professionalising the current electronics base. That means the first priority is to transfer the known working circuit safely and clearly.

This choice is realistic for my project. It improves the physical quality without making the step too large. It gives me a chance to learn soldering, layout planning, grouped wiring, wire protection, and staged verification, which are all relevant to embedded and robotics work.

---

## Important technical points for the perfboard transfer

### Keeping the existing circuit architecture

The perfboard transfer should not redesign the whole system. The current architecture already works, so the purpose of the perfboard is to make that architecture more permanent.

The MCP23017 is suitable for this structure because Microchip describes the MCP23017 as a 16-bit I2C I/O expander with serial interface (Microchip Technology Inc., 2022). The Microchip datasheet also describes the 16-bit I/O port as consisting functionally of two 8-bit ports, PORTA and PORTB (Microchip Technology Inc., 2022). This matches the way I already use the MCP23017 as an output-expansion layer.

The ULN2803A is also suitable for its role because the Texas Instruments datasheet describes it as a high-voltage, high-current Darlington transistor array made from eight NPN Darlington pairs (Texas Instruments, 2004). The same datasheet lists relay-driver, lamp-driver, display-driver, line-driver, and logic-buffer applications (Texas Instruments, 2004). In my project, I use the ULN2803 chips as the switching stage between the MCP23017 logic outputs and the externally powered LED channels.

This means the important analysis decision is not whether I should replace these parts. The important decision is how to place and connect them more permanently.

The perfboard should therefore preserve these layers:

1. ESP32-S3 controller side;
2. MCP23017 I2C output expansion;
3. ULN2803 output switching;
4. external LED power;
5. traffic-light LED channels;
6. sensor and future input expansion.

Keeping this structure reduces the risk of introducing unnecessary changes during the transfer.

---

### Power and ground routing

Power and ground routing are critical in the perfboard design. In the breadboard version, the power rails and jumper wires made it easy to connect everything temporarily. On perfboard, I need to define those routes myself.

The circuit has two important electrical areas:

* logic side: ESP32-S3, MCP23017, I2C pull-ups, sensor input;
* LED power side: external LED supply, ULN2803 switched paths, LED channels.

Even though these areas have different roles, the system still needs a shared ground. Without a shared ground, the logic signals and the switched LED side may not have the same reference. In the current breadboard setup, the shared ground was already necessary so that the ESP32-S3, MCP23017, ULN2803 chips, and external LED power supply used the same reference (Wesley, 2026a).

Espressif’s ESP32-S3 hardware design guidelines include hardware-design recommendations for ESP32-S3-based circuits, including power-supply and schematic design considerations (Espressif Systems, 2025). My project does not require a full custom ESP32-S3 PCB design, but the source supports the general point that power and ground connections around an ESP32-S3 project should be planned carefully.

For the perfboard version, I should therefore analyse and later design:

* one clear logic power route;
* one clear shared ground route;
* one clear external LED power route;
* a clear point where the external LED ground and controller ground are connected;
* separate routing for I2C lines and LED power paths where practical;
* clear labels or documentation for all power connections.

---

### Soldering quality

Moving to perfboard means that soldering quality becomes part of the project quality. A soldered circuit can give a more fixed physical connection than a breadboard, but only if the soldering is done carefully.

SparkFun explains through-hole soldering as a practical technique and describes the tools, basic soldering technique, and rework for through-hole electronics (SparkFun Electronics, n.d.-b). For my project, this is a suitable source because I am not building an industrial product. I am transferring a student prototype from breadboard to perfboard, so a practical electronics-learning source fits the project level better.

For this project, good soldering means:

* no accidental solder bridges between nearby pads;
* no loose solder joints;
* no visually suspicious joints;
* no exposed wires touching each other;
* no overheated component leads;
* no unclear solder routes that are impossible to inspect;
* enough spacing around IC pins to check for mistakes.

The perfboard transfer should therefore include visual inspection before power is applied. I should also use continuity testing before connecting the ESP32-S3 and external LED power. This reduces the risk of damaging components because of a soldering or wiring error.

---

### Connectors and outgoing wires

The traffic-light models will not all sit directly on the perfboard. They need outgoing wires from the controller board to the physical traffic-light positions on the tile. That means connectors or grouped wire connections become important.

If every LED wire is soldered directly from the traffic-light model to the perfboard without planning, the project may become difficult to repair. If one traffic-light model breaks, I would need to desolder wires instead of disconnecting or separating a clear wire group. A connector-based or grouped-wiring approach makes the setup easier to maintain.

For this project, the outgoing wiring should be grouped logically. A practical grouping is one connector or wire group per traffic light:

| Wire group                              | Function                                                 |
| --------------------------------------- | -------------------------------------------------------- |
| Traffic Light 1 connector or wire group | red, yellow, green, and return or related channel wiring |
| Traffic Light 2 connector or wire group | red, yellow, green, and return or related channel wiring |
| Traffic Light 3 connector or wire group | red, yellow, green, and return or related channel wiring |
| Traffic Light 4 connector or wire group | red, yellow, green, and return or related channel wiring |
| Sensor connector or wire group          | VCC, GND, signal                                         |
| Power connector or wire group           | external LED power and ground                            |

The exact connector choice belongs more in the design deliverable, but the analysis already shows that unplanned loose wiring is not suitable for the tile version.

---

### Wire protection and strain relief

Wire protection is important because the tile contains wires that leave the perfboard and go toward traffic-light models or sensors. If those wires are pulled, the force should not go directly into a weak soldered connection.

SparkFun’s wire tutorial explains practical wire-working steps such as stripping wire, preparing wire ends, soldering a splice, and using heat shrink to cover exposed wire after soldering (SparkFun Electronics, n.d.-c). That source supports the practical point that wires should be prepared and insulated properly. It does not prove that heat shrink alone solves mechanical strain. For my project, I still need a physical way to prevent outgoing wires from being pulled directly at the solder joint.

For this project, wire protection and strain relief can be done with simple methods such as:

* heat-shrink tubing around soldered wire joints for insulation;
* tie-wraps to hold wire bundles to the board or tile;
* cable clips under the tile;
* mounting holes near wire exits;
* small wire loops before the soldered point;
* connector placement near the board edge;
* routing wires through fixed holes in the tile.

The important point is that the solder joint should not be the only thing holding the wire mechanically. The solder joint should make the electrical connection; the board, connector, or tile should handle movement and pulling forces as much as possible.

---

### Testability after soldering

A breadboard circuit is easy to change. A perfboard circuit is harder to change after soldering. Because of that, testability must be part of the analysis before the design is made.

The perfboard version must be testable in stages. This is also why I include a testing plan in this analysis. Atlassian describes a test plan as a structured way to define testing scope and objective, outline a test strategy, list resources and roles, develop test scenarios and cases, establish test criteria, and plan for risks (Atlassian, n.d.). I use that idea here in a smaller and simpler way that fits my embedded project.

For this learning goal, the testing plan does not replace the realisation. It defines what I will test later. In the realisation document, I will carry out the tests, record the observed results, add photos or serial monitor proof, and conclude whether each test passed.

---

### Mounting in or under the city tile

The perfboard circuit must be suitable for the physical city tile. That does not mean every component must be hidden inside the tile material itself. The tile is small and thin, so the realistic approach is to mount the perfboard under the tile or in a protected area connected to the tile.

The mounting method should support:

* stable placement of the perfboard;
* access to USB or programming connection if needed;
* access to connectors or test points;
* clean wire routing to the traffic-light models;
* separation between electronics and moving parts;
* prevention of short circuits against screws, metal parts, or exposed wire.

The tile integration is therefore part of the Definition of Done for the physical project, but the learning goal itself remains focused on the electronics transfer from breadboard to perfboard.

---

## Testing plan for the realisation

### Purpose of the testing plan

The purpose of this testing plan is to make the later realisation more focused. The realisation should not become the place where I still have to invent the test approach. The analysis already defines what needs to be verified, so the realisation can focus on building, testing, recording proof, and reflecting on the result.

The main question for the later realisation will be:

**Does the perfboard version still behave like the working breadboard version, while being physically more stable, clearer, and suitable for mounting in the city tile?**

The tests are therefore focused on two areas:

1. **Electrical correctness**: power, ground, I2C, MCP23017 outputs, ULN2803 switching, LED channels, and traffic-light sequence.
2. **Physical professionalisation**: soldering quality, wire grouping, wire protection, strain relief, mounting, and traceability.

---

### Test scope

The testing scope includes:

* the perfboard soldering and visual inspection;
* power and ground routing;
* continuity checks before power is applied;
* ESP32-S3 to MCP23017 I2C communication;
* MCP23017 output behaviour;
* ULN2803 switching behaviour;
* traffic-light channel mapping;
* full traffic-light phase sequence;
* `millis()` based timing behaviour;
* outgoing wire grouping to the traffic-light models;
* physical mounting in or under the city tile;
* stability after moving or handling the tile.

The testing scope does not include:

* redesigning the complete traffic-light logic;
* adding new adaptive traffic algorithms;
* validating a custom PCB;
* testing every possible future sensor expansion;
* full backend integration.

---

### Test environment

The realisation tests should be carried out with:

| Item                        | Purpose                                      |
| --------------------------- | -------------------------------------------- |
| ESP32-S3                    | Main controller                              |
| MCP23017                    | I2C output expander                          |
| 2x ULN2803                  | LED channel switching                        |
| Perfboard circuit           | New permanent version of the circuit         |
| External LED power supply   | Power for the LED channels                   |
| Multimeter                  | Continuity and voltage checks                |
| USB cable and laptop        | Uploading code and serial monitor            |
| Arduino CLI / VS Code setup | Firmware upload and serial observation       |
| Traffic-light LED models    | Final visible output                         |
| City tile                   | Physical integration test                    |
| Existing working code       | Baseline behaviour from the breadboard setup |

The Arduino documentation states that `millis()` returns the number of milliseconds passed since the board began running the current program (Arduino, 2025). In my project, this function is used as part of the already realised non-blocking timing structure. The realisation tests must check that this timing behaviour still works after the circuit is transferred to perfboard.

---

### Test evidence to collect

During the realisation, I should collect proof for each test. The proof can include:

* photos of the perfboard before power is applied;
* photos of the solder side;
* photos of power and ground wiring;
* multimeter continuity results;
* measured voltage values;
* serial monitor output;
* photos or video of the traffic-light sequence;
* mapping table from output channel to physical traffic light;
* photos of wire grouping and mounting under the tile;
* notes about failed tests and fixes.

This evidence matters because the goal is not only to say that the perfboard works. I need to show that the transfer was controlled and that the final circuit is understandable.

---

### Planned test cases

| Test ID | Test name                               | Purpose                                                                               | Method                                                                                                  | Expected result                                                                        | Evidence to collect                      |
| ------- | --------------------------------------- | ------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------- | ---------------------------------------- |
| T1      | Visual inspection before power          | Check whether the perfboard has obvious physical mistakes before powering it          | Inspect the component side and solder side under good light                                             | No visible solder bridges, loose wires, reversed components, or exposed wires touching | Photos of component side and solder side |
| T2      | Continuity check - ground               | Check that all required ground points are connected                                   | Use multimeter continuity mode between ESP32-S3 GND, MCP23017 GND, ULN2803 GND, and external LED ground | All required ground points are connected                                               | Multimeter notes or photo                |
| T3      | Continuity check - no power short       | Check that power and ground are not shorted                                           | Use multimeter continuity mode between 3.3V and GND, and between external LED power and GND             | No short circuit is detected                                                           | Multimeter notes                         |
| T4      | Logic voltage check                     | Check that the logic side receives the correct voltage                                | Power the logic side and measure voltage at MCP23017 VDD and pull-up points                             | Logic voltage is around 3.3V                                                           | Voltage measurement photo or note        |
| T5      | External LED power check                | Check that LED power is present only on the intended power route                      | Measure external LED supply at the LED power input and related points                                   | External LED power is present where expected and not mixed with 3.3V logic             | Voltage measurement photo or note        |
| T6      | ESP32-S3 serial test                    | Check that the controller still runs and can print debug output                       | Upload a small serial test or use existing debug output                                                 | Serial monitor shows expected output                                                   | Serial monitor screenshot                |
| T7      | I2C scanner test                        | Check that the ESP32-S3 can still detect the MCP23017 after the transfer              | Run I2C scanner code                                                                                    | MCP23017 is found at address `0x20`                                                    | Serial monitor screenshot                |
| T8      | MCP23017 single-output test             | Check that at least one MCP23017 output can be controlled correctly                   | Switch one output on and off with test code                                                             | The selected output changes as expected                                                | LED/photo or multimeter proof            |
| T9      | ULN2803 single-channel test             | Check that one MCP23017 output can drive one ULN2803 input and switch one LED channel | Activate one mapped channel through the MCP23017 and ULN2803                                            | The correct LED channel turns on and off                                               | Photo/video proof                        |
| T10     | Full channel mapping test               | Check that every traffic-light colour is connected to the correct output              | Activate each output channel one by one                                                                 | Each physical LED matches the mapping table                                            | Completed mapping checklist and photos   |
| T11     | Safe startup test                       | Check that the transferred circuit starts in the safe state                           | Power or reset the system                                                                               | The system starts in all-red or the defined safe startup state                         | Photo/video proof                        |
| T12     | Traffic-light sequence test             | Check that the full sequence still works after the perfboard transfer                 | Let the system run through multiple cycles                                                              | The traffic lights follow the intended phase order                                     | Video or observation table               |
| T13     | No conflicting green test               | Check that the transfer did not create wrong channel combinations                     | Observe the system during repeated cycles                                                               | Conflicting directions are never green at the same time                                | Observation table                        |
| T14     | `millis()` timing test                  | Check that the non-blocking timing behaviour still works                              | Observe phase durations or use serial timestamps                                                        | Phase timing follows the configured values closely enough for the prototype            | Timing notes or serial output            |
| T15     | Sensor input check, if connected        | Check that the KY-021 input still works after the electronics are mounted             | Trigger the sensor with the magnet-equipped vehicle                                                     | Sensor state changes correctly in the serial monitor                                   | Serial monitor screenshot                |
| T16     | Wire grouping check                     | Check that outgoing wires are organised by traffic light or function                  | Inspect the wire groups between perfboard and traffic-light models                                      | Wires are grouped and traceable                                                        | Photos and mapping notes                 |
| T17     | Wire protection and strain relief check | Check that movement does not pull directly on solder joints                           | Lightly move wire bundles and inspect soldered points                                                   | Wires stay connected and solder joints are not directly stressed                       | Photos and observation notes             |
| T18     | Mounted tile test                       | Check that the circuit still works after mounting in or under the tile                | Mount the perfboard and run the traffic-light sequence again                                            | The system works the same after mounting                                               | Photos/video of mounted setup            |
| T19     | Movement/handling test                  | Check basic robustness when the tile is handled carefully                             | Carefully move or reposition the tile and run the sequence again                                        | No loose connection or unexpected behaviour appears                                    | Observation notes                        |
| T20     | Final full-system test                  | Check the complete professionalised setup as one result                               | Run the full setup with traffic lights, wiring, tile mounting, and optional sensor                      | The tile works as a stable integrated traffic-light setup                              | Final video, photos, and pass/fail table |

---

### Exit criteria

The perfboard realisation can be considered successful when:

* no short circuit is found before powering the circuit;
* the logic side receives the correct voltage;
* the MCP23017 is detected over I2C;
* the ULN2803 channels switch the intended LED outputs;
* all traffic-light outputs match the final mapping table;
* the system starts safely;
* the traffic-light sequence works correctly;
* no conflicting green states occur;
* the `millis()` based timing still behaves correctly;
* outgoing wires are grouped and protected;
* the circuit works after mounting in or under the tile;
* the realisation document contains proof for the tests.

If one of these points fails, the realisation is not automatically a complete failure. It means the issue must be recorded, corrected if possible, and tested again. The realisation document should include failed attempts because they show what had to be fixed during the transfer.

---

## Risks

| Risk                                       | Cause                                                            | Effect                                              | Mitigation                                                               |
| ------------------------------------------ | ---------------------------------------------------------------- | --------------------------------------------------- | ------------------------------------------------------------------------ |
| Wrong connection during transfer           | The breadboard wiring is copied incorrectly                      | A channel may not work or may show the wrong colour | Create a mapping table before soldering and test each channel separately |
| Solder bridge                              | Pads or IC pins are connected accidentally                       | Short circuit or damaged component                  | Inspect visually and use continuity testing before power                 |
| Weak solder joint                          | Poor soldering technique or movement during cooling              | Intermittent faults                                 | Inspect and rework suspicious joints                                     |
| Ground mistake                             | Logic ground and external LED ground are not connected correctly | ULN2803 switching may fail or behave unpredictably  | Define one clear shared ground route                                     |
| Power mix-up                               | 3.3V logic and external LED power are confused                   | Component damage                                    | Label power lines and test voltage before connecting components          |
| Wires pull loose                           | Outgoing traffic-light wires move during tile assembly           | Broken connection                                   | Use grouped wires, clips, tie-wraps, or connectors                       |
| Mapping drift                              | Code, documentation, and physical wiring do not match            | Debugging becomes difficult                         | Freeze a final channel mapping and update it after realisation           |
| Circuit becomes hard to repair             | Everything is soldered permanently without grouped access        | Small faults require major rework                   | Use connectors or grouped removable wiring where useful                  |
| Sensor/backend expansion becomes difficult | No space is left for later inputs or communication               | Future sprint becomes harder                        | Reserve board space and document expansion points                        |
| Tile mounting causes shorts                | Perfboard touches conductive material or exposed wire            | Unstable or unsafe behaviour                        | Use insulation, standoffs, and controlled mounting points                |

The largest risks are not new traffic-light logic risks. The largest risks are physical implementation risks: wrong wiring, weak soldering, unclear mapping, and mechanical stress on wires.

---

## Acceptance criteria

| ID  | Acceptance criterion                                                                                                                     |
| --- | ---------------------------------------------------------------------------------------------------------------------------------------- |
| AC1 | The breadboard circuit is analysed and translated into a clear perfboard transfer direction.                                             |
| AC2 | The existing architecture with ESP32-S3, MCP23017, two ULN2803 chips, external LED power, and shared ground remains the basis.           |
| AC3 | The analysis explains why keeping the circuit on a breadboard is not suitable as the final physical setup.                               |
| AC4 | The analysis defines the requirements for power routing, ground routing, connectors or grouped wiring, wire protection, and testability. |
| AC5 | The selected direction is justified against at least two alternatives.                                                                   |
| AC6 | The main risks of transferring the circuit to perfboard are identified.                                                                  |
| AC7 | The analysis contains a testing plan that can be used directly in the realisation step.                                                  |
| AC8 | The analysis keeps tile integration as a practical requirement, but not as the only learning goal.                                       |
| AC9 | The analysis includes APA 7 in-text citations and a reference list.                                                                      |

---

## What counts as a correct result in this step

For this analysis, I consider the result correct when it clearly defines what must be solved before I can build the perfboard version.

A correct result means:

* I know why the breadboard is not suitable as the final physical form;
* I know what professionalising means for this project;
* I know why perfboard is the best next step compared with hiding the breadboard or designing a custom PCB;
* I know which parts of the existing circuit must remain unchanged;
* I know which physical risks must be controlled;
* I know that power, ground, grouped wiring, wire protection, soldering quality, and testability must be part of the design;
* I have a testing plan that can be used later in the realisation deliverable;
* I have acceptance criteria that can be used later in the design and realisation deliverables.

The correct outcome of this learning goal is not only a nicer-looking tile. The correct outcome is a more permanent embedded hardware setup that still works safely and can be tested, repaired, and extended.

---

## Conclusion

The current breadboard traffic-light setup is a successful prototype, but it is not yet a professional physical implementation. It already proves the traffic-light behaviour, the MCP23017 output expansion, the ULN2803 switching, the external LED power structure, the shared ground, and the `millis()` based timing. However, it still depends on temporary breadboard connections and loose jumper wires.

The best next step is to transfer the working circuit to perfboard. This is stronger than hiding the breadboard because it improves the physical structure of the electronics instead of only hiding the temporary prototype. It is also more realistic than designing a custom PCB at this stage because it keeps the project buildable and testable within the sprint.

The perfboard version must keep the existing architecture, but improve the physical implementation. That means the design must focus on component placement, soldering quality, power and ground routing, grouped wiring or connectors, wire protection, channel mapping, and staged testing.

This analysis also defines the testing plan for the later realisation. That makes the next step clearer: in the realisation, I do not need to invent the testing method again. I can build the perfboard circuit, follow the prepared tests, record the observed results, and prove whether the transfer from breadboard to perfboard was successful.

This analysis answers the learning question by showing that professionalising the traffic-light tile means moving from a temporary prototype to a more permanent, traceable, and testable embedded circuit. The city tile integration and hidden wiring remain important, but they are part of the final implementation quality rather than the whole learning goal.

---

## References

Adafruit. (2016, September 6). *Perma protos*. Adafruit Learning System. [https://learn.adafruit.com/breadboards-for-beginners/perma-protos](https://learn.adafruit.com/breadboards-for-beginners/perma-protos)

Arduino. (2025, June 5). *millis()*. Arduino Documentation. [https://www.arduino.cc/en/Reference/Millis](https://www.arduino.cc/en/Reference/Millis)

Atlassian. (n.d.). *Test plan template*. Retrieved May 20, 2026, from [https://www.atlassian.com/software/confluence/resources/guides/how-to/test-plan](https://www.atlassian.com/software/confluence/resources/guides/how-to/test-plan)

Espressif Systems. (2025). *ESP32-S3 hardware design guidelines*. [https://docs.espressif.com/projects/esp-hardware-design-guidelines/en/latest/esp32s3/esp-hardware-design-guidelines-en-master-esp32s3.pdf](https://docs.espressif.com/projects/esp-hardware-design-guidelines/en/latest/esp32s3/esp-hardware-design-guidelines-en-master-esp32s3.pdf)

Microchip Technology Inc. (2022). *MCP23017/MCP23S17: 16-bit I/O expander with serial interface* [Data sheet]. [https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP23017-Data-Sheet-DS20001952.pdf](https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP23017-Data-Sheet-DS20001952.pdf)

SparkFun Electronics. (n.d.-a). *How to use a breadboard*. SparkFun Learn. Retrieved May 20, 2026, from [https://learn.sparkfun.com/tutorials/how-to-use-a-breadboard/all](https://learn.sparkfun.com/tutorials/how-to-use-a-breadboard/all)

SparkFun Electronics. (n.d.-b). *How to solder: Through-hole soldering*. SparkFun Learn. Retrieved May 20, 2026, from [https://learn.sparkfun.com/tutorials/how-to-solder-through-hole-soldering/all](https://learn.sparkfun.com/tutorials/how-to-solder-through-hole-soldering/all)

SparkFun Electronics. (n.d.-c). *Working with wire*. SparkFun Learn. Retrieved May 20, 2026, from [https://learn.sparkfun.com/tutorials/working-with-wire/all](https://learn.sparkfun.com/tutorials/working-with-wire/all)

Texas Instruments. (2004). *ULN2803A Darlington transistor arrays* (Rev. C) [Data sheet]. [https://cdn.sparkfun.com/assets/f/0/6/6/5/uln2803a.pdf](https://cdn.sparkfun.com/assets/f/0/6/6/5/uln2803a.pdf)

Wesley. (2026a). *Realisation - Breadboard realisation of the four-way traffic-light setup* [Realisation deliverable].

Wesley. (2026b). *Advice — Learning goal 1 - Expanding the traffic-light system to multiple traffic lights* [Advice deliverable].

Wesley. (2026c). *Realise - Recognizing when a car is waiting at the traffic light and sending that information to the backend* [Realisation deliverable].
