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

## 2. Methodology

## 2.1 Design method

I use a step-by-step design method. First, I freeze the current working breadboard architecture, because the perfboard version must behave the same as the working version. After that, I translate the circuit into four design outputs:

| Design output                       | Purpose                                                                                                 |
| ----------------------------------- | ------------------------------------------------------------------------------------------------------- |
| Fritzing physical design            | Shows the physical build view: component placement, wires, connectors, and labels.                      |
| KiCad schematic                     | Defines the electrical circuit professionally with symbols, net labels, power symbols, and title block. |
| Tile mounting and wire-routing plan | Defines where the finished board sits in or under the tile and how the outgoing wires are protected.    |
| Bill of Materials                   | Lists all parts with labels, quantities, prices, manufacturer information, and part numbers.            |

This method keeps the design structured. The Fritzing view helps with physical construction. The KiCad schematic helps with electrical correctness. The tile-integration plan helps with mounting and cable protection. The BoM helps with traceability and ordering.

## 2.2 Design assumptions

The design uses the following assumptions:

| Assumption                                                       | Design consequence                                                                      |
| ---------------------------------------------------------------- | --------------------------------------------------------------------------------------- |
| The current breadboard traffic-light system already works.       | The design preserves the same architecture and output mapping.                          |
| The ESP32-S3 remains accessible.                                 | The USB connection must stay reachable for programming and serial monitor checks.       |
| The ESP32-S3 should not directly power all traffic-light LEDs.   | LED power is supplied externally and switched through the ULN2803 chips.                |
| The MCP23017 and ULN2803 chips should be replaceable if damaged. | DIP sockets are used for the ICs where possible.                                        |
| The traffic-light models are placed away from the perfboard.     | The outgoing LED wires are grouped into labelled connectors.                            |
| The LEDs use external LED power.                                 | The perfboard has a separate LED power rail and shared ground.                          |
| The ULN2803 chips are sink drivers.                              | The LED branches are wired so the ULN2803 pulls the LED return side to ground.          |
| The design must remain testable after soldering.                 | Test points are added for 3.3V, LED power, GND, SDA, SCL, and selected output channels. |
| The tile may be moved during demonstration.                      | Outgoing wires need strain relief and should not pull directly on solder joints.        |

## 2.3 Design file structure

The design files are organised as follows:

```text
docs/Wesley/learning outcomes/sprint 4/
├── Design - Professionalising the traffic-light tile by transferring the breadboard circuit to perfboard.md
├── assets/
│   └── sprint-4-perfboard-design/
│       ├── fritzing-perfboard-physical-view.png
│       ├── kicad-schematic-export.pdf
│       ├── kicad-schematic-export.png
│       ├── tile-mounting-and-wire-routing-plan.png
│       ├── bill-of-materials.csv
│       └── bill-of-materials.png
└── design-files/
    ├── traffic-light-perfboard.fzz
    ├── traffic-light-perfboard.kicad_pro
    ├── traffic-light-perfboard.kicad_sch
    └── traffic-light-perfboard.kicad_pcb
```

The Fritzing file is used only for the physical wiring and build view. The professional schematic is made in KiCad, not in Fritzing. The tile mounting image is a separate drawing and should not repeat the same top-down perfboard component layout.

---


## 3. Fritzing physical design

## 3.1 Chapter introduction

This chapter answers the first sub-question:

**How should the perfboard transfer be documented in a Fritzing physical design so the component placement, wiring, connectors, and labels remain clear and buildable?**

The Fritzing design is used as a physical construction view. It is not used as the professional schematic. The purpose is to show how the circuit is physically arranged on the perfboard, where the components are placed, how the wires are routed on the board, and how the labels match the BoM and schematic.

## 3.2 Purpose of the Fritzing physical design

The Fritzing design is useful because it communicates the physical build in a visual way. For this project, that matters because the circuit contains multiple ICs, two power areas, many LED channels, and outgoing wires to four traffic-light models.

The Fritzing design must make clear:

* where the ESP32-S3 connection is placed;
* where the MCP23017 is placed;
* where both ULN2803 chips are placed;
* where the 3.3V logic route is located;
* where the external LED power route is located;
* where the shared ground route is located;
* where the traffic-light connectors leave the board;
* which connector belongs to which traffic light;
* which wires are SDA and SCL;
* which wires are LED power and switched LED returns.

## 3.3 Fritzing software version and file location

For this design deliverable, the Fritzing physical layout is documented with:

| Item                           | Value                                                  |
| ------------------------------ | ------------------------------------------------------ |
| Software                       | Fritzing                                               |
| Version used for documentation | Fritzing 1.0.7                                         |
| File name                      | `traffic-light-perfboard.fzz`                          |
| Exported image                 | `fritzing-perfboard-physical-view.png`                 |
| Storage location               | `docs/Wesley/learning outcomes/sprint 4/design-files/` |

Fritzing 1.0.7 is used as the documented software version because Fritzing lists version 1.0.7 as a release from 15 April 2026 (Fritzing, 2026). ([Fritzing][1])

## 3.4 Fritzing design rules

The Fritzing design follows these rules:

| Rule                                                              | Reason                                                              |
| ----------------------------------------------------------------- | ------------------------------------------------------------------- |
| Wires are drawn straight where possible.                          | Straight wiring makes the design easier to follow.                  |
| Diagonal wires are avoided.                                       | Diagonal wires make it harder to see exact connection points.       |
| Wires may cross, but connection dots must stay visible.           | A crossing wire must not be confused with an electrical connection. |
| Components do not overlap.                                        | Every component must remain identifiable.                           |
| Labels do not cover components.                                   | The design must still be readable when printed or exported.         |
| Every component has a short code label.                           | The same label is reused in KiCad and the BoM.                      |
| Power specifications are placed near the power connector.         | The external LED supply must not be confused with 3.3V logic.       |
| LED connectors are labelled by traffic-light position and colour. | The wiring must be traceable during building and testing.           |
| Exact board holes or connector pins are visible.                  | The drawing must be useful during soldering and inspection.         |

## 3.5 Fritzing component labels

The following labels are used in the Fritzing design:

| Label   | Component                         | Meaning                                                  |
| ------- | --------------------------------- | -------------------------------------------------------- |
| U0      | ESP32-S3 development board        | Main controller                                          |
| U1      | MCP23017-E/SP                     | I2C I/O expander                                         |
| U2      | ULN2803A / ULN2803C #1            | Driver for traffic-light channels 1–8                    |
| U3      | ULN2803A / ULN2803C #2            | Driver for traffic-light channels 9–12 and spare outputs |
| J1      | Traffic Light 1 connector         | Outgoing wires to traffic light 1                        |
| J2      | Traffic Light 2 connector         | Outgoing wires to traffic light 2                        |
| J3      | Traffic Light 3 connector         | Outgoing wires to traffic light 3                        |
| J4      | Traffic Light 4 connector         | Outgoing wires to traffic light 4                        |
| J5      | External LED power input          | Regulated external LED power input                       |
| J6      | ESP32-S3 connection header        | 3.3V, GND, SDA, SCL                                      |
| J7      | Optional sensor/service connector | Reserved for later tile input wiring                     |
| R1–R12  | LED current-limiting resistors    | One resistor per lamp channel                            |
| R13–R14 | I2C pull-up resistors             | SDA and SCL pull-ups                                     |
| R15     | MCP23017 RESET pull-up            | Keeps MCP23017 out of reset                              |
| C1      | 100 nF capacitor                  | MCP23017 decoupling                                      |
| C2      | 100 µF capacitor                  | LED power input buffer                                   |
| TP1–TP7 | Test points                       | 3.3V, LED power, GND, SDA, SCL, and output test points   |

## 3.6 Fritzing physical build view

Before showing the Fritzing image, it is important to define what the diagram represents. The diagram is not a schematic and not a tile-mounting drawing. It is the physical build view for the perfboard transfer. It shows how the components and board-level wires are arranged so the breadboard circuit can be transferred in a controlled way.

![Figure 1. Fritzing physical perfboard design with labelled components and outgoing traffic-light connectors](assets/sprint-4-perfboard-design/fritzing-perfboard-physical-view.png)

*Figure 1. Fritzing physical view of the perfboard transfer design. The diagram shows the ESP32-S3 connection, MCP23017, two ULN2803 chips, external LED power input, shared ground, test points, and labelled outgoing traffic-light connectors. This image is exported from `traffic-light-perfboard.fzz`.*

The Fritzing design shows the circuit in the same functional order as the working breadboard version:

```text
ESP32-S3 connection side -> MCP23017 -> ULN2803 driver stage -> traffic-light connectors
```

The ESP32-S3 connection is placed on the left side of the perfboard. This keeps the USB side accessible and makes it clear that the ESP32-S3 is the controller, not the LED power source.

The MCP23017 is placed near the ESP32-S3 connection because it needs the I2C lines, 3.3V logic power, and ground. The SDA and SCL wires are kept short and parallel where practical. The I2C pull-up resistors are placed close to the MCP23017 side of the bus.

The two ULN2803 chips are placed to the right of the MCP23017. This keeps the signal direction readable: the MCP23017 outputs go to the ULN2803 inputs, and the ULN2803 outputs go to the LED connector area.

The traffic-light connectors are placed on the board edge. This prevents outgoing wires from crossing the whole board and makes strain relief easier. Each traffic-light connector is labelled with its traffic-light number and pin function.

The external LED power connector is placed near the LED output side, not near the ESP32-S3 3.3V logic header. This physical separation reduces the chance of confusing 3.3V logic power with external LED power.

## 3.7 Design choices shown in the Fritzing view

The Fritzing design makes the following design choices visible:

| Design choice                                                 | Reason                                                                          |
| ------------------------------------------------------------- | ------------------------------------------------------------------------------- |
| The ESP32-S3 is not used as the direct LED power source.      | The ESP32-S3 should control logic, while LED current comes from external power. |
| The MCP23017 is central in the layout.                        | It is the bridge between the controller and the output drivers.                 |
| ULN2803 chips are placed between MCP23017 and LED connectors. | This matches the signal path and makes debugging easier.                        |
| Traffic-light wires leave from one board edge.                | This keeps the tile wiring grouped and easier to secure.                        |
| Power input is labelled near the connector.                   | This prevents wrong voltage connection during testing.                          |
| One resistor is used per LED channel.                         | Each lamp branch remains independently current-limited.                         |
| Test points are placed on important nets.                     | The circuit can be checked after soldering and after mounting.                  |

## 3.8 Sub-conclusion

The Fritzing physical design is used as a construction document. It does not replace the electrical schematic and it does not describe the final tile mounting. Its value is that it shows how the circuit is physically built on perfboard with clear labels, straight wiring, visible connectors, and the same component codes used in the KiCad schematic and BoM.

---

## 4. KiCad schematic design

## 4.1 Chapter introduction

This chapter answers the second sub-question:

**How should the electrical circuit be defined in a professional KiCad schematic so the power, ground, I2C, driver, and LED connections are unambiguous?**

The schematic is made in KiCad because Fritzing is used only for the physical build view, not as the professional schematic tool. The KiCad schematic defines the actual electrical connections using symbols, power symbols, net labels, and a title block.

## 4.2 Purpose of the KiCad schematic

The KiCad schematic is the main electrical design document. It must be possible to understand the circuit even without seeing the physical Fritzing view.

The schematic must show:

* ESP32-S3 connection points;
* MCP23017 power, ground, SDA, SCL, RESET, address pins, and output pins;
* ULN2803 inputs and outputs;
* external LED power input;
* common ground connection;
* traffic-light LED connectors;
* current-limiting resistors;
* test points;
* unused pins and spare outputs;
* net labels instead of unclear long wires.

KiCad’s Schematic Editor is used for schematic drawing, symbol management, footprint assignment, and data transfer to PCB design tools (KiCad, 2026). Even though this sprint uses perfboard instead of a manufactured PCB, KiCad is still useful because it makes the electrical circuit clear and checkable. ([KiCad Documentation][2])

## 4.3 KiCad software version and file location

| Item                           | Value                                                  |
| ------------------------------ | ------------------------------------------------------ |
| Software                       | KiCad                                                  |
| Version used for documentation | KiCad 9.0                                              |
| Project file                   | `traffic-light-perfboard.kicad_pro`                    |
| Schematic file                 | `traffic-light-perfboard.kicad_sch`                    |
| Exported schematic PDF         | `kicad-schematic-export.pdf`                           |
| Exported schematic image       | `kicad-schematic-export.png`                           |
| Storage location               | `docs/Wesley/learning outcomes/sprint 4/design-files/` |

## 4.4 Schematic page setup

The KiCad schematic uses the following page setup:

| Field     | Value                                            |
| --------- | ------------------------------------------------ |
| Page size | A4 landscape                                     |
| Margin    | Approximately 2 cm free space around the drawing |
| Title     | Traffic-light perfboard transfer schematic       |
| Author    | Wesley                                           |
| Version   | 1.0                                              |
| File name | `traffic-light-perfboard.kicad_sch`              |
| Date      | 22 May 2026                                      |

The title block is placed in the bottom-right corner. It includes the author, version number, and file name so the schematic can be identified when exported or printed.

![Figure 2. KiCad schematic export of the traffic-light perfboard circuit](assets/sprint-4-perfboard-design/kicad-schematic-export.png)

*Figure 2. KiCad schematic of the perfboard circuit. The schematic uses power symbols, ground symbols, net labels, component labels, and a bottom-right title block with the author, version number, and file name. This image is exported from `traffic-light-perfboard.kicad_sch`.*

## 4.5 Power and ground symbols

The schematic uses proper KiCad power symbols instead of only drawing coloured wires. This is important because the schematic software must understand which nets are connected.

The design uses these power symbols and power nets:

| Net label    | Meaning                             |
| ------------ | ----------------------------------- |
| `+3V3_LOGIC` | 3.3V logic supply from the ESP32-S3 |
| `+5V_LED`    | External regulated LED power supply |
| `GND`        | Shared ground reference             |
| `SDA`        | I2C data line                       |
| `SCL`        | I2C clock line                      |

The design has one shared ground reference. This ground connects the ESP32-S3, MCP23017, both ULN2803 chips, and the negative side of the external LED power supply.

The schematic keeps `+3V3_LOGIC` and `+5V_LED` separate. They only share `GND`.

## 4.6 Net labels

The schematic uses net labels instead of long unclear wires. This makes the schematic easier to read and prevents the page from becoming crowded.

The most important communication and power net labels are:

| Net label    | Function            |
| ------------ | ------------------- |
| `+3V3_LOGIC` | Logic power         |
| `+5V_LED`    | External LED power  |
| `GND`        | Shared ground       |
| `SDA`        | I2C data            |
| `SCL`        | I2C clock           |
| `MCP_RESET`  | MCP23017 reset line |

The MCP23017-to-ULN2803 control nets are:

| Net label      | Function                                           |
| -------------- | -------------------------------------------------- |
| `TL1_RED_CTRL` | MCP output to ULN input for traffic light 1 red    |
| `TL1_YEL_CTRL` | MCP output to ULN input for traffic light 1 yellow |
| `TL1_GRN_CTRL` | MCP output to ULN input for traffic light 1 green  |
| `TL2_RED_CTRL` | MCP output to ULN input for traffic light 2 red    |
| `TL2_YEL_CTRL` | MCP output to ULN input for traffic light 2 yellow |
| `TL2_GRN_CTRL` | MCP output to ULN input for traffic light 2 green  |
| `TL3_RED_CTRL` | MCP output to ULN input for traffic light 3 red    |
| `TL3_YEL_CTRL` | MCP output to ULN input for traffic light 3 yellow |
| `TL3_GRN_CTRL` | MCP output to ULN input for traffic light 3 green  |
| `TL4_RED_CTRL` | MCP output to ULN input for traffic light 4 red    |
| `TL4_YEL_CTRL` | MCP output to ULN input for traffic light 4 yellow |
| `TL4_GRN_CTRL` | MCP output to ULN input for traffic light 4 green  |

The LED-side switched return nets are labelled separately from the logic-side control nets:

| Net label   | Function                                   |
| ----------- | ------------------------------------------ |
| `TL1_RED_K` | Switched return for traffic light 1 red    |
| `TL1_YEL_K` | Switched return for traffic light 1 yellow |
| `TL1_GRN_K` | Switched return for traffic light 1 green  |
| `TL2_RED_K` | Switched return for traffic light 2 red    |
| `TL2_YEL_K` | Switched return for traffic light 2 yellow |
| `TL2_GRN_K` | Switched return for traffic light 2 green  |
| `TL3_RED_K` | Switched return for traffic light 3 red    |
| `TL3_YEL_K` | Switched return for traffic light 3 yellow |
| `TL3_GRN_K` | Switched return for traffic light 3 green  |
| `TL4_RED_K` | Switched return for traffic light 4 red    |
| `TL4_YEL_K` | Switched return for traffic light 4 yellow |
| `TL4_GRN_K` | Switched return for traffic light 4 green  |

## 4.7 ESP32-S3 to MCP23017 schematic design

The ESP32-S3 is shown as connector `J6`, because the full development board is not redesigned as a custom circuit. The schematic only shows the pins that are used by this perfboard circuit.

| ESP32-S3 function | Connector label | Net          |
| ----------------- | --------------- | ------------ |
| 3.3V              | J6 pin 1        | `+3V3_LOGIC` |
| GND               | J6 pin 2        | `GND`        |
| GPIO5             | J6 pin 3        | `SDA`        |
| GPIO4             | J6 pin 4        | `SCL`        |

The realised breadboard setup uses GPIO5 for SDA and GPIO4 for SCL. This design keeps that mapping so the perfboard version follows the tested version instead of creating a new wiring standard.

The I2C lines use pull-up resistors:

| Label |  Value | Connection            |
| ----- | -----: | --------------------- |
| R13   | 5.1 kΩ | `SDA` to `+3V3_LOGIC` |
| R14   | 5.1 kΩ | `SCL` to `+3V3_LOGIC` |

The pull-ups are connected to 3.3V because the ESP32-S3 uses 3.3V logic.

## 4.8 MCP23017 address, reset, and unused pins

The MCP23017 is labelled as `U1`.

The address pins are wired as follows:

| MCP23017 pin | Connection | Reason            |
| ------------ | ---------- | ----------------- |
| A0           | GND        | Address selection |
| A1           | GND        | Address selection |
| A2           | GND        | Address selection |

With A0, A1, and A2 connected to ground, the expander uses address `0x20`. The Microchip datasheet states that the MCP23017 address pins must be externally biased, so they must not be left floating (Microchip Technology Inc., 2022). ([Microchip][3])

The RESET pin is wired as follows:

| MCP23017 pin | Connection                          | Reason                                             |
| ------------ | ----------------------------------- | -------------------------------------------------- |
| RESET        | Pull-up to `+3V3_LOGIC` through R15 | Keeps the MCP23017 enabled during normal operation |

The interrupt pins are not used in this design:

| MCP23017 pin | Schematic label    | Design decision           |
| ------------ | ------------------ | ------------------------- |
| INTA         | NC / not connected | Interrupt output not used |
| INTB         | NC / not connected | Interrupt output not used |

The I2C pins are not left open:

| MCP23017 pin | Connection             |
| ------------ | ---------------------- |
| SDA          | `SDA` net with pull-up |
| SCL          | `SCL` net with pull-up |

This is important because SDA and SCL are the communication lines between the ESP32-S3 and the MCP23017.

## 4.9 MCP23017 to ULN2803 schematic design

The schematic keeps the final realised output mapping from the working breadboard version. That means the design follows the version that has already been tested.

| Traffic light | Colour | MCP23017 output | Control net    | ULN chip | ULN input |
| ------------- | ------ | --------------- | -------------- | -------- | --------- |
| TL1           | Red    | GPB0            | `TL1_RED_CTRL` | U2       | IN1       |
| TL1           | Yellow | GPB1            | `TL1_YEL_CTRL` | U2       | IN2       |
| TL1           | Green  | GPB2            | `TL1_GRN_CTRL` | U2       | IN3       |
| TL2           | Red    | GPB3            | `TL2_RED_CTRL` | U2       | IN4       |
| TL2           | Yellow | GPB4            | `TL2_YEL_CTRL` | U2       | IN5       |
| TL2           | Green  | GPB5            | `TL2_GRN_CTRL` | U2       | IN6       |
| TL3           | Red    | GPA5            | `TL3_RED_CTRL` | U2       | IN7       |
| TL3           | Yellow | GPA6            | `TL3_YEL_CTRL` | U2       | IN8       |
| TL3           | Green  | GPA4            | `TL3_GRN_CTRL` | U3       | IN1       |
| TL4           | Red    | GPA2            | `TL4_RED_CTRL` | U3       | IN2       |
| TL4           | Yellow | GPA3            | `TL4_YEL_CTRL` | U3       | IN3       |
| TL4           | Green  | GPA1            | `TL4_GRN_CTRL` | U3       | IN4       |

The remaining MCP23017 pins are marked as spare:

| MCP23017 output | Use          |
| --------------- | ------------ |
| GPA0            | Spare output |
| GPA7            | Spare output |
| GPB6            | Spare output |
| GPB7            | Spare output |

These spare outputs are not used in the first perfboard version, but they are labelled so they can be used later if needed.