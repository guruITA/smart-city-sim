# Design - Professionalising the traffic-light tile by transferring the breadboard circuit to perfboard

## Summary

In this design deliverable, I show how I will transfer my working traffic-light circuit from a breadboard setup to a more permanent perfboard setup. The goal is not to redesign the traffic-light logic itself. The goal is to make the physical electronics clearer, more stable, easier to connect, and easier to test during the realisation phase.

The design is based on the Fritzing layout I made. I use two Fritzing proof images. The first image shows the front/component side of the perfboard. This is the side where the main components are placed and labelled. The second image shows the back/solder side of the perfboard. This is the side where the wires will later be placed and soldered.

The design keeps the existing architecture:

```text
ESP32-S3 -> MCP23017 -> 2x ULN2803 -> LED resistors -> traffic-light connectors
```

The design also includes the KY-021 sensor connection, I2C pull-up resistors, a reset pull-up resistor, external LED power, shared ground, and four 4-pin traffic-light connectors.

Because the new perfboard design separates the traffic-light connector groups more clearly and uses cleaner cable routing, the output mapping in the code must be checked and adjusted during the realisation phase. The new physical wire order from the MCP23017 to the traffic-light channels must match the software mapping, otherwise the wrong traffic-light colour or direction could switch on.

---

## Table of Contents

- [Summary](#summary)
- [Table of Contents](#table-of-contents)
- [1. Introduction](#1-introduction)
  - [1.1 Design context](#11-design-context)
  - [1.2 Main design question and sub-questions](#12-main-design-question-and-sub-questions)
  - [1.3 Scope](#13-scope)
- [2. Methodology](#2-methodology)
  - [2.1 Design method](#21-design-method)
  - [2.2 Design assumptions](#22-design-assumptions)
  - [2.3 Design file structure](#23-design-file-structure)
- [3. Fritzing design proof](#3-fritzing-design-proof)
  - [3.1 Purpose of the Fritzing design](#31-purpose-of-the-fritzing-design)
  - [3.2 Software version and file location](#32-software-version-and-file-location)
  - [3.3 Figure 1: front/component-side design](#33-figure-1-frontcomponent-side-design)
  - [3.4 Figure 2: back/solder-side design](#34-figure-2-backsolder-side-design)
  - [3.5 Sub-conclusion](#35-sub-conclusion)
- [4. Design choices](#4-design-choices)
  - [4.1 Component placement](#41-component-placement)
  - [4.2 Power and ground design](#42-power-and-ground-design)
  - [4.3 I2C and MCP23017 design](#43-i2c-and-mcp23017-design)
  - [4.4 ULN2803 driver design](#44-uln2803-driver-design)
  - [4.5 Traffic-light connector design](#45-traffic-light-connector-design)
  - [4.6 LED resistor design](#46-led-resistor-design)
  - [4.7 Sensor connection design](#47-sensor-connection-design)
  - [4.8 Software mapping consequence](#48-software-mapping-consequence)
  - [4.9 Sub-conclusion](#49-sub-conclusion)
- [5. Connection mapping](#5-connection-mapping)
  - [5.1 ESP32-S3 to MCP23017](#51-esp32-s3-to-mcp23017)
  - [5.2 Pull-up resistor mapping](#52-pull-up-resistor-mapping)
  - [5.3 New MCP23017 cable mapping](#53-new-mcp23017-cable-mapping)
  - [5.4 Traffic-light connector pinout](#54-traffic-light-connector-pinout)
  - [5.5 Sensor connection](#55-sensor-connection)
  - [5.6 Sub-conclusion](#56-sub-conclusion)
- [6. Bill of Materials](#6-bill-of-materials)
  - [6.1 BoM assumptions](#61-bom-assumptions)
  - [6.2 Bill of Materials table](#62-bill-of-materials-table)
  - [6.3 Sub-conclusion](#63-sub-conclusion)
- [7. Design validation checklist](#7-design-validation-checklist)
  - [7.1 Checklist](#71-checklist)
  - [7.2 Sub-conclusion](#72-sub-conclusion)
- [8. Conclusion](#8-conclusion)
- [9. References](#9-references)
- [Appendix A - Responsible use of ChatGPT](#appendix-a---responsible-use-of-chatgpt)



---

## 1. Introduction

### 1.1 Design context

At the start of this design phase, I already have a working traffic-light setup on a breadboard. The system uses an ESP32-S3 as the controller, an MCP23017 as I/O expander, two ULN2803 driver chips, external LED power, and shared ground.

The breadboard version works, but it is not a good final physical form. A breadboard is useful for testing, but it still uses loose temporary connections. For the city tile, I need a more permanent circuit that can be connected to traffic-light models and tested repeatedly without wires coming loose all the time.

This design deliverable focuses on the Fritzing perfboard design I made. The Fritzing design is used as a practical build plan before the circuit is soldered.

### 1.2 Main design question and sub-questions

The main design question for this deliverable is:

**How can I design a perfboard version of my working traffic-light circuit in Fritzing so that the circuit is clear enough to build, test, and connect to the traffic-light models?**

To answer this question, I use these sub-questions:

1. **How should the main components be placed on the front side of the perfboard?**
2. **How should the wiring be planned on the back side of the perfboard?**
3. **How should the traffic-light connectors and LED resistors be included in the design?**
4. **How can the Fritzing design support the realisation phase?**

### 1.3 Scope

This design focuses only on the Fritzing perfboard design.

This design includes:

* front/component-side component placement
* back/solder-side wiring plan
* ESP32-S3 connection
* MCP23017 placement and wiring
* two ULN2803 driver chips
* external LED power
* shared ground
* I2C wiring
* I2C pull-up resistors
* MCP23017 reset pull-up resistor
* KY-021 sensor connection
* four 4-pin traffic-light connectors
* twelve LED resistors
* connection mapping
* the consequence for software mapping in the realisation phase
* Bill of Materials
* design validation checklist

This design does not include:

* a KiCad schematic
* a custom PCB
* a separate tile mounting plan
* a new traffic-light algorithm
* a backend design
* a full enclosure design

The professional schematic feedback is not applied in this deliverable because this design proof is limited to the Fritzing perfboard layout that I actually made. If a later version of the design requires a more professional schematic, that should be made separately in KiCad or another schematic tool.

---

## 2. Methodology

This chapter explains how I made the design and what assumptions I used. I split the Fritzing design into a front side and a back side because a perfboard has two important physical sides. The front side is used for component placement. The back side is used for most of the soldered wiring.

### 2.1 Design method

I used the working breadboard circuit as the starting point. The goal of this design is not to create a new circuit. The goal is to translate the current working setup into a clearer perfboard layout.

I made the design in two parts:

| Part                 | Purpose                                                                        |
| -------------------- | ------------------------------------------------------------------------------ |
| Front/component side | Shows where the main components, connectors, resistors, and labels are placed. |
| Back/solder side     | Shows how the wires will be routed on the solder side of the perfboard.        |

This is important because the front side and back side do not show the same information. The front side is mainly useful for placing the parts. The back side is mainly useful for planning how the connections will be made with solder and wires.

### 2.2 Design assumptions

The design uses the following assumptions:

| Assumption                                                      | Design consequence                                                                                        |
| --------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| The current breadboard system already works.                    | The existing architecture is kept.                                                                        |
| The ESP32-S3 remains the main controller.                       | It is placed on the front side of the perfboard design.                                                   |
| The MCP23017 remains the output expander.                       | It is placed between the ESP32-S3 and the driver section.                                                 |
| The ULN2803 chips are used as low-side sink drivers.            | The LED return paths are routed through the ULN2803 chips.                                                |
| Each traffic light needs four connections.                      | One 4-pin connector is used per traffic light.                                                            |
| Each LED needs current limiting.                                | Twelve LED resistors are included.                                                                        |
| The KY-021 sensor remains separate from the LED output section. | The KY-021 is placed on the controller/input side.                                                        |
| The wiring will mainly be made on the back side.                | A separate back/solder-side Fritzing proof is included.                                                   |
| The cleaner cabling changes the physical output order.          | The code must be checked and adjusted during realisation so the software matches the new MCP23017 wiring. |

### 2.3 Design file structure

The design files are stored as:

```text
docs/Wesley/learning outcomes/sprint 4/
├── Design - Professionalising the traffic-light tile by transferring the breadboard circuit to perfboard.md
├── assets/
│   └── sprint-4-perfboard-design/
│       ├── fritzing-front-component-side.png
│       └── fritzing-back-solder-side.png
└── design-files/
    └── traffic-light-perfboard.fzz
```

The file `traffic-light-perfboard.fzz` is the editable Fritzing file. The two PNG files are the exported proof images used in this document.

---

## 3. Fritzing design proof

This chapter shows the two Fritzing proof images that I made for the perfboard design. The first image shows the front/component side. The second image shows the back/solder side. The two images must be used together. The front-side image shows the component placement and labels. The back-side image shows the planned wiring routes that will later be made on the solder side.

### 3.1 Purpose of the Fritzing design

The Fritzing design is used as a practical build reference for the realisation phase. It helps me see where the components will be placed and how the wiring can be routed before I start soldering.

The Fritzing design is not a custom PCB design. It is a planning document for a hand-soldered perfboard circuit.

### 3.2 Software version and file location

| Item             | Value                                                                      |
| ---------------- | -------------------------------------------------------------------------- |
| Software         | Fritzing                                                                   |
| Version          | Fritzing 1.0.6                  |
| Editable file    | `traffic-light-perfboard.fzz`                                              |
| Front-side image | `fritzing-front-component-side.png`                                        |
| Back-side image  | `fritzing-back-solder-side.png`                                            |
| File location    | `docs/Wesley/learning outcomes/sprint 4/design-files/`                     |
| Image location   | `docs/Wesley/learning outcomes/sprint 4/assets/sprint-4-perfboard-design/` |

### 3.3 Figure 1: front/component-side design

![Front/component-side design](../sprint%204/assets/sprint-4-perfboard-design/fritzing-front-component-side.png)

*Figure 1. Fritzing front/component-side design of the perfboard circuit.*

The front-side image shows the main component placement and the labels I added to make the design easier to understand.

Visible parts in the front-side image include:

| Visible item              | Label in the design     | Function                              |
| ------------------------- | ----------------------- | ------------------------------------- |
| ESP32-S3                  | `U0 ESP32-S3`           | Main controller                       |
| KY-021 sensor             | `S1 KY-021`             | Vehicle sensor input                  |
| External LED power input  | `J5 external LED power` | LED power input                       |
| MCP23017                  | `U1 MCP23017`           | I2C output expander                   |
| ULN2803 #1                | `U2 ULN2803`            | LED driver chip                       |
| ULN2803 #2                | `U3 ULN2803`            | LED driver chip                       |
| Traffic-light connector 1 | `J1 TL1: +5V R Y G`     | Connector for traffic light 1         |
| Traffic-light connector 2 | `J2 TL2: +5V R Y G`     | Connector for traffic light 2         |
| Traffic-light connector 3 | `J3 TL3: +5V R Y G`     | Connector for traffic light 3         |
| Traffic-light connector 4 | `J4 TL4: +5V R Y G`     | Connector for traffic light 4         |
| LED resistors             | `R1` to `R12`           | Current limiting for the LED channels |
| SDA pull-up               | `R13 5.1kΩ`             | Pull-up resistor for SDA              |
| SCL pull-up               | `R14 5.1kΩ`             | Pull-up resistor for SCL              |
| RESET pull-up             | `R15 10kΩ`              | Pull-up resistor for MCP23017 RESET   |

The front-side image does not show every final solder connection. That is intentional, because most of the wiring will be placed on the back side of the perfboard. The front image mainly proves the component placement, connector position, resistor placement, and labels.

The connector parts shown in Fritzing are used as a visual representation of 4-pin traffic-light connectors. The important design choice is that each traffic light has one 4-pin connector with the same pin order.

The LEDs themselves are not placed on the perfboard in this design. They are placed in the traffic-light models. The perfboard contains the connectors and resistors for those LEDs.

### 3.4 Figure 2: back/solder-side design

![Back/solder-side design](../sprint%204/assets/sprint-4-perfboard-design/fritzing-back-solder-side.png)

*Figure 2. Fritzing back/solder-side wiring plan of the perfboard circuit.*

The back-side image shows the planned wiring routes. This is the side where the actual soldered connections and wire bridges will be made later.

Visible design elements in the back-side image include:

| Visible item                        | Function                                                                                                |
| ----------------------------------- | ------------------------------------------------------------------------------------------------------- |
| Red wiring                          | Power routes, including LED power and logic power routes                                                |
| Black wiring                        | Shared ground routes                                                                                    |
| Blue and purple wiring              | I2C or signal routes from the controller side                                                           |
| Green, yellow, and orange wiring    | Output routes between the MCP23017, ULN2803 driver section, resistors, and traffic-light connector area |
| Repeated groups near the top        | Wiring routes for the traffic-light connector and resistor area                                         |
| Long horizontal and vertical routes | Planned solder-side wire paths                                                                          |

The back-side image does not contain the same text labels as the front-side image. Because of that, the back-side image must be read together with the front-side image and the connection mapping tables in this document.

The back-side plan is important because a perfboard does not automatically connect the holes. The connections must be made manually with solder bridges, jumper wires, or solid core wire on the solder side. This figure helps prevent random wiring during realisation.

### 3.5 Sub-conclusion

The Fritzing design is split into a front-side proof and a back-side proof. Figure 1 shows where the components are placed and how they are labelled. Figure 2 shows how the wiring is planned on the solder side. Together, the two images give a clearer build reference than only showing one side of the perfboard.

---

## 4. Design choices

This chapter explains the most important design choices in the Fritzing layout. The choices focus on component placement, power and ground routing, I2C wiring, the ULN2803 driver stage, the traffic-light connectors, the LED resistors, the KY-021 sensor connection, and the software consequence of changing the MCP23017 cable order.

### 4.1 Component placement

The front-side design groups the circuit into functional areas.

The ESP32-S3 is placed on the lower-left side of the perfboard. This keeps the controller side separate from the output driver section. It also makes the board easier to understand, because the controller is clearly the starting point of the circuit.

The MCP23017 is placed on the right side of the perfboard. This component receives I2C signals from the ESP32-S3 and controls the output channels that go toward the ULN2803 chips.

The two ULN2803 chips are placed above the MCP23017 and close to the traffic-light connectors. This makes sense because the ULN2803 chips switch the LED channels that leave the board through the connectors.

The four traffic-light connectors are placed at the top of the board. This makes the outgoing traffic-light wiring easy to group. Instead of loose wires going in many directions, every traffic light gets one connector group.

### 4.2 Power and ground design

The design uses separate ideas for logic power and LED power.

| Power line   | Purpose                                                                          |
| ------------ | -------------------------------------------------------------------------------- |
| `3V3 logic`  | Used for the ESP32-S3 logic side, MCP23017, pull-up resistors, and KY-021 sensor |
| `+5V_LED`    | Used as external power for the traffic-light LEDs                                |
| `GND shared` | Common ground reference for the whole circuit                                    |

The label near the external power input shows:

```text
J5 external LED power
+5V_LED
GND shared
```

This is important because the external LED power supply must share ground with the ESP32-S3 circuit. Without shared ground, the logic signals and driver stage may not behave correctly.

### 4.3 I2C and MCP23017 design

The ESP32-S3 communicates with the MCP23017 through I2C.

The front-side image labels the ESP32-S3 connections as:

```text
GPIO5 SDA
GPIO4 SCL
3V3 logic
GND shared
```

The design also includes pull-up resistors for the I2C lines:

| Label |  Value | Purpose             |
| ----- | -----: | ------------------- |
| R13   | 5.1 kΩ | SDA pull-up to 3.3V |
| R14   | 5.1 kΩ | SCL pull-up to 3.3V |

The labels in the image show that R13 connects SDA to 3.3V and R14 connects SCL to 3.3V.

The design also includes R15 as a reset pull-up resistor for the MCP23017. This keeps the RESET pin pulled high during normal operation.

### 4.4 ULN2803 driver design

The design uses two ULN2803 chips as low-side sink drivers.

This means the ULN2803 chips do not supply positive voltage to the LEDs. Instead, they switch the return side of each LED channel to ground.

The LED current path is designed as:

```text
+5V_LED -> traffic-light LED -> LED return wire -> resistor -> ULN2803 output -> GND
```

The exact physical order of LED and resistor can differ as long as the resistor is in series with the LED. In this design, the resistors are placed near the connector and driver area so they are easy to inspect.

### 4.5 Traffic-light connector design

The design uses four 4-pin connectors, one connector for each traffic light.

The labels in the front-side image show:

```text
J1 TL1: +5V R Y G
J2 TL2: +5V R Y G
J3 TL3: +5V R Y G
J4 TL4: +5V R Y G
```

This means every traffic light uses the same connector structure:

| Pin meaning | Function                                          |
| ----------- | ------------------------------------------------- |
| `+5V`       | Shared positive LED supply for that traffic light |
| `R`         | Red LED return                                    |
| `Y`         | Yellow LED return                                 |
| `G`         | Green LED return                                  |

This is better than loose jumper wires because each traffic light becomes one clear cable group.

### 4.6 LED resistor design

The design includes twelve LED resistors, one for each traffic-light LED channel.

The calculation for the number of LED resistors is:

```text
4 traffic lights × 3 colours = 12 LED channels
```

So the design needs:

```text
R1 to R12
```

The front-side image shows R1 as `220Ω`. The other LED resistors are grouped with labels from R2 to R12. The design intention is that every LED channel has its own current-limiting resistor. The exact resistor value should be checked against the actual LED colour, LED supply voltage, and desired brightness before final soldering.

The traffic-light LEDs are not placed directly on the perfboard. They are inside the traffic-light models. For the Bill of Materials and the realisation, the LED colours and size should still be recorded as red, yellow, and green traffic-light LEDs, for example 5 mm LEDs if that is the size used in the models.

### 4.7 Sensor connection design

The design includes the KY-021 sensor module on the left side. It is labelled as:

```text
S1 KY-021
S: GPIO sensor input
+: 3V3
-: GND
```

This shows that the sensor is connected to the ESP32-S3 as an input. It is separate from the LED output section. That keeps the sensor input wiring easier to understand and prevents it from being mixed with the traffic-light output wiring.

### 4.8 Software mapping consequence

Because the perfboard design has cleaner separation between traffic-light connectors and better cable grouping, the physical cable order from the MCP23017 has changed compared with the earlier breadboard setup. This means the software mapping must be checked and adjusted during the realisation phase.

The code must match the new MCP23017 output wiring. If the code still uses the old mapping, the system may switch the wrong traffic light or the wrong colour. For example, a software command for traffic light 1 green could physically activate another colour or another traffic light if the output mapping is not updated.

For the realisation phase, I must therefore update the output mapping in the code after the final wiring is confirmed. The correct order should be tested one channel at a time before running the full traffic-light sequence.

### 4.9 Sub-conclusion

The Fritzing design groups the circuit into clear functional sections. The controller and sensor are on the left side, the output-expander and driver section are on the right side, and the traffic-light connectors are placed at the top. The front image shows the component placement and labels. The back image shows the planned solder-side wiring.

The design also has a software consequence. Because the new cabling is cleaner and separated differently, the code must be updated in the realisation phase to match the new MCP23017-to-traffic-light wiring.

---

## 5. Connection mapping

This chapter gives the connection mapping that belongs to the Fritzing design. The front-side image makes the component groups visible, but the tables are still needed because the image does not fully describe every exact signal in text.

### 5.1 ESP32-S3 to MCP23017

| ESP32-S3 connection | MCP23017 connection | Function      |
| ------------------- | ------------------- | ------------- |
| 3.3V                | VDD                 | Logic power   |
| GND                 | VSS                 | Shared ground |
| GPIO5               | SDA                 | I2C data      |
| GPIO4               | SCL                 | I2C clock     |

### 5.2 Pull-up resistor mapping

| Label |  Value | Connection    | Purpose                     |
| ----- | -----: | ------------- | --------------------------- |
| R13   | 5.1 kΩ | SDA to 3.3V   | I2C SDA pull-up             |
| R14   | 5.1 kΩ | SCL to 3.3V   | I2C SCL pull-up             |
| R15   |  10 kΩ | RESET to 3.3V | Keeps MCP23017 out of reset |

### 5.3 New MCP23017 cable mapping

The new cable mapping below reflects the newer separated cabling direction from the MCP23017. This mapping must be checked during realisation and then reflected in the code.

| MCP23017 output | Cable colour | Traffic light |
| --------------- | ------------ | ------------- |
| GPB0            | Green        | TL2           |
| GPB1            | Yellow       | TL2           |
| GPB2            | Orange       | TL2           |
| GPB3            | Green        | TL1           |
| GPB4            | Yellow       | TL1           |
| GPB5            | Orange       | TL1           |
| GPA7            | Orange       | TL3           |
| GPA6            | Yellow       | TL3           |
| GPA5            | Green        | TL3           |
| GPA4            | Orange       | TL4           |
| GPA3            | Yellow       | TL4           |
| GPA2            | Green        | TL4           |

The colour names in this table describe the cable colours used in the design. They do not automatically mean the LED colour unless the code and connector mapping also define them that way. During realisation, I must test each output one by one and then update the software constants so the code matches the actual wiring.

The safest test order is:

| Step | Action             | Expected result          |
| ---- | ------------------ | ------------------------ |
| 1    | Activate GPB0 only | One TL2 channel switches |
| 2    | Activate GPB1 only | One TL2 channel switches |
| 3    | Activate GPB2 only | One TL2 channel switches |
| 4    | Activate GPB3 only | One TL1 channel switches |
| 5    | Activate GPB4 only | One TL1 channel switches |
| 6    | Activate GPB5 only | One TL1 channel switches |
| 7    | Activate GPA7 only | One TL3 channel switches |
| 8    | Activate GPA6 only | One TL3 channel switches |
| 9    | Activate GPA5 only | One TL3 channel switches |
| 10   | Activate GPA4 only | One TL4 channel switches |
| 11   | Activate GPA3 only | One TL4 channel switches |
| 12   | Activate GPA2 only | One TL4 channel switches |

After these tests, I can write the final software mapping with confidence.

### 5.4 Traffic-light connector pinout

Each traffic light uses one 4-pin connector.

| Connector | Pin 1     | Pin 2      | Pin 3         | Pin 4        |
| --------- | --------- | ---------- | ------------- | ------------ |
| J1 TL1    | `+5V_LED` | Red return | Yellow return | Green return |
| J2 TL2    | `+5V_LED` | Red return | Yellow return | Green return |
| J3 TL3    | `+5V_LED` | Red return | Yellow return | Green return |
| J4 TL4    | `+5V_LED` | Red return | Yellow return | Green return |

All four connectors use the same order. This reduces mistakes during assembly and testing.

### 5.5 Sensor connection

| Sensor module | Pin | ESP32-S3 connection |
| ------------- | --- | ------------------- |
| KY-021        | `S` | GPIO sensor input   |
| KY-021        | `+` | 3.3V                |
| KY-021        | `-` | GND                 |

### 5.6 Sub-conclusion

The connection mapping defines how the Fritzing design should be interpreted during realisation. The most important mapping change is the new MCP23017 cable order. Because the physical wiring has changed, the code must also be checked and updated so that each traffic-light phase still controls the correct physical LEDs.

---

## 6. Bill of Materials

This chapter lists the parts that are needed for the Fritzing perfboard design. The improved Bill of Materials includes labels, quantities, manufacturer or type, part number or SKU, description, source or datasheet, estimated unit price, and subtotal. The prices are estimates and must be checked with the final supplier before ordering.

### 6.1 BoM assumptions

The BoM uses the following assumptions:

| Assumption                                        | Explanation                                                                                                                       |
| ------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------- |
| The prices are estimates.                         | Final prices depend on supplier, stock, VAT, and shipping.                                                                        |
| Some parts may already be available.              | If parts are already in stock, the real cost may be lower.                                                                        |
| Generic parts may be replaced by equivalents.     | Resistors, headers, wires, and perfboard can use equivalent parts if the values and pitch are correct.                            |
| The LED size must match the traffic-light models. | If the 3D printed traffic lights use 5 mm LEDs, the BoM should list 5 mm LEDs. If they use another size, the BoM must be updated. |
| The connector labels match the Fritzing design.   | J1 to J4 are the traffic-light connectors and J5 is external LED power.                                                           |

### 6.2 Bill of Materials table

| Label  | Used quantity | Manufacturer / type                               | Official part number / SKU                              | Description                                                          | Datasheet / source                                | Estimated unit price | Subtotal |
| ------ | ------------: | ------------------------------------------------- | ------------------------------------------------------- | -------------------------------------------------------------------- | ------------------------------------------------- | -------------------: | -------: |
| U0     |             1 | Espressif / development board                     | ESP32-S3 development board, exact board used in project | ESP32-S3 DevKitC-1 N16          | [Espressif ESP32-S3 documentation](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf) |               €21.00 |   €21.00 |
| U1     |             1 | Microchip Technology                              | MCP23017/SP                                           | 16-bit I2C I/O expander in through-hole DIP package                  | [Microchip MCP23017 datasheet](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf)                      |                €4.00 |    €4.00 |
| U2     |             1 | Texas Instruments / STMicroelectronics equivalent | ULN2803A or ULN2803C                                    | 8-channel Darlington transistor array used as LED sink driver        | [Texas Instruments ULN2803 datasheet](https://www.ti.com/product/ULN2803C)                |                €0.60 |    €0.60 |
| U3     |             1 | Texas Instruments / STMicroelectronics equivalent | ULN2803A or ULN2803C                                    | Second 8-channel Darlington transistor array used as LED sink driver | [Texas Instruments ULN2803 datasheet](https://www.ti.com/product/ULN2803C)               |                €0.60 |    €0.60 |
| S1     |             1 | Generic sensor module                             | KY-021                                                  | Magnetic reed switch sensor module used as vehicle-detection input   | [Joy-IT ](https://sensorkit.joy-it.net/en/sensors/ky-021)     |                €3.79 |    €3.79 |
| J1     |             1 | JST-XH compatible or KF2510 compatible            | 4-pin 2.54 mm or compatible connector set               | Connector for traffic light 1 with `+5V R Y G` order                 | [Supplier connector page](https://www.tinytronics.nl/en/cables-and-connectors/cables-and-adapters/jst-compatible/jst-xh-connector-with-cable-and-pcb-connector-4-pins)                           |                €0.80 |    €0.80 |
| J2     |             1 | JST-XH compatible or KF2510 compatible            | 4-pin 2.54 mm or compatible connector set               | Connector for traffic light 2 with `+5V R Y G` order                 | [Supplier connector page](https://www.tinytronics.nl/en/cables-and-connectors/cables-and-adapters/jst-compatible/jst-xh-connector-with-cable-and-pcb-connector-4-pins)                           |                €0.80 |    €0.80 |
| J3     |             1 | JST-XH compatible or KF2510 compatible            | 4-pin 2.54 mm or compatible connector set               | Connector for traffic light 3 with `+5V R Y G` order                 | [Supplier connector page](https://www.tinytronics.nl/en/cables-and-connectors/cables-and-adapters/jst-compatible/jst-xh-connector-with-cable-and-pcb-connector-4-pins)                           |                €0.80 |    €0.80 |
| J4     |             1 | JST-XH compatible or KF2510 compatible            | 4-pin 2.54 mm or compatible connector set               | Connector for traffic light 4 with `+5V R Y G` order                 | [Supplier connector page](https://www.tinytronics.nl/en/cables-and-connectors/cables-and-adapters/jst-compatible/jst-xh-connector-with-cable-and-pcb-connector-4-pins)                           |                €0.80 |    €0.80 |
| J5     |             1 | Breadboard power supply 5V en 3.3V         | 2-pin terminal block                                    | External LED power input for `+5V_LED` and shared `GND`              | [Supplier connector page](https://www.tinytronics.nl/en/power/voltage-converters/voltage-regulators/breadboard-power-supply-5v-en-3.3v)                           |                €2.50 |    €2.50 |
| R1–R12 |            12 | Generic resistor                                  | 220 Ω through-hole resistor                             | LED current-limiting resistors, one per LED channel                  | [Supplier resistor page](https://www.tinytronics.nl/en/components/resistors/resistors/220%CF%89-resistor(led-series-resistor))                            |                €0.05 |    €0.60 |
| R13    |             1 | Generic resistor                                  | 5.1 kΩ through-hole resistor                            | SDA pull-up resistor to 3.3V                                         | [Supplier resistor page](https://www.tinytronics.nl/en/components/resistors/resistors/5.1k%CF%89-resistor)                            |                €0.05 |    €0.05 |
| R14    |             1 | Generic resistor                                  | 5.1 kΩ through-hole resistor                            | SCL pull-up resistor to 3.3V                                         | [Supplier resistor page](https://www.tinytronics.nl/en/components/resistors/resistors/5.1k%CF%89-resistor)                            |                €0.05 |    €0.05 |
| R15    |             1 | Generic resistor                                  | 10 kΩ through-hole resistor                             | MCP23017 RESET pull-up resistor to 3.3V                              | [Supplier resistor](https://www.tinytronics.nl/en/components/resistors/resistors/10k%CF%89-resistor-(standard-pull-up-or-pull-down-resistor)) page                            |                €0.05 |    €0.05 |
| D1–D4  |             4 | Generic LED                                       | 5 mm red LED or exact LED size used                     | Red traffic-light LEDs placed in the traffic-light models            | [Supplier LED page](https://www.tinytronics.nl/en/components/leds/leds/teng-jie-red-led-5mm-diffused)                                 |                €0.10 |    €0.40 |
| D5–D8  |             4 | Generic LED                                       | 5 mm yellow LED or exact LED size used                  | Yellow traffic-light LEDs placed in the traffic-light models         | [Supplier LED page](https://www.tinytronics.nl/en/components/leds/leds/yellow-led-5mm-diffuse)                                 |                €0.10 |    €0.40 |
| D9–D12 |             4 | Generic LED                                       | 5 mm green LED or exact LED size used                   | Green traffic-light LEDs placed in the traffic-light models          | [Supplier LED page](https://www.tinytronics.nl/en/components/leds/leds/teng-jie-green-led-5mm-diffused)                                 |                €0.10 |    €0.40 |

**total:** €37.64, excluding shipping and depending on supplier availability.

The labels in this BoM match the labels used in the Fritzing design. This makes it easier to trace a part from the document to the design image and later to the physical build.

### 6.3 Sub-conclusion

The improved Bill of Materials gives more detail than a simple parts list. It includes the part labels, used quantities, manufacturer or type, part number or SKU, description, source or datasheet, unit price, and subtotal. This makes the BoM more useful for ordering, checking, and explaining the design.

---

## 7. Design validation checklist

This chapter gives the checklist I will use before and during the realisation phase. The checklist is meant to make sure the Fritzing design is complete enough before I start soldering.

### 7.1 Checklist

| ID   | Design check                                                                         | Status                       |
| ---- | ------------------------------------------------------------------------------------ | ---------------------------- |
| DC1  | The front-side image shows the main component placement.                             | Designed                     |
| DC2  | The back-side image shows the planned solder-side wiring routes.                     | Designed                     |
| DC3  | The ESP32-S3 is labelled with SDA, SCL, 3.3V, and GND.                               | Designed                     |
| DC4  | The MCP23017 is included and labelled.                                               | Designed                     |
| DC5  | The two ULN2803 chips are included and labelled.                                     | Designed                     |
| DC6  | The four traffic-light connectors are included and labelled.                         | Designed                     |
| DC7  | The connector labels show `+5V R Y G`.                                               | Designed                     |
| DC8  | Twelve LED resistors are included.                                                   | Designed                     |
| DC9  | R13 and R14 are labelled as 5.1 kΩ I2C pull-up resistors.                            | Designed                     |
| DC10 | R15 is labelled as a 10 kΩ RESET pull-up resistor.                                   | Designed                     |
| DC11 | The KY-021 sensor connection is included and labelled.                               | Designed                     |
| DC12 | External LED power is labelled as `+5V_LED` and shared ground.                       | Designed                     |
| DC13 | The back-side design is available as a separate wiring plan.                         | Designed                     |
| DC14 | The final MCP23017-to-LED mapping is checked against the physical wiring.            | To verify before realisation |
| DC15 | The code is adjusted to match the new MCP23017 cable mapping.                        | To do during realisation     |
| DC16 | Each MCP23017 output is tested one by one before the full sequence is tested.        | To do during realisation     |
| DC17 | The resistor value for the LEDs is checked before soldering.                         | To verify before realisation |
| DC18 | The back-side wiring is checked for possible short circuits before applying power.   | To verify during realisation |
| DC19 | The BoM is checked against the actual parts before soldering.                        | To verify before realisation |
| DC20 | The 4-pin connector pin order is checked before connecting the traffic-light models. | To verify during realisation |

### 7.2 Sub-conclusion

The checklist makes the design usable for the next phase. It does not only check whether the drawing exists, but also whether the design can be safely built, tested, and matched with the software.

---

## 8. Conclusion

This design answers the main design question:

**How can I design a perfboard version of my working traffic-light circuit in Fritzing so that the circuit is clear enough to build, test, and connect to the traffic-light models?**

The answer is to use a two-sided Fritzing design. The front/component-side design shows where the components, connectors, labels, and resistors are placed. The back/solder-side design shows how the wiring will be routed on the underside of the perfboard.

The design keeps the existing architecture with the ESP32-S3, MCP23017, two ULN2803 chips, external LED power, shared ground, KY-021 sensor input, and four traffic-light connector groups. The design also adds the LED resistors and the pull-up resistors needed for the MCP23017 and I2C bus.

The design is suitable for the realisation phase because it gives a clear build reference. During realisation, I can use the front-side image to place the parts and the back-side image to plan the wiring. After soldering, I can test power, ground, I2C communication, individual LED channels, the connector groups, and finally the full traffic-light sequence.

The realisation phase must also include a software update. Because the new perfboard layout separates the cabling more clearly, the code must be adjusted so the MCP23017 output mapping matches the physical wire mapping. This prevents the wrong traffic-light colour or direction from switching on.

---

## 9. References

Espressif Systems. (n.d.). *ESP32-S3 documentation*. Retrieved May 2026, from [https://documentation.espressif.com/esp32-s3_datasheet_en.pdf](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf)

Fritzing. (n.d.). *Fritzing*. Retrieved May 2026, from [https://fritzing.org/](https://fritzing.org/)

Microchip Technology Inc. (2022). *MCP23017/MCP23S17: 16-bit I/O expander with serial interface* [Data sheet]. Retrieved May 2026, from [https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf)

Texas Instruments. (2025). *ULN2803C Darlington transistor array* [Data sheet]. Retrieved May 2026, from [https://www.ti.com/product/ULN2803C](https://www.ti.com/product/ULN2803C)

Wesley. (2026a). *Realisation - Breadboard realisation of the four-way traffic-light setup* [Realisation deliverable](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/1517d508a00f814741af422f5b2da3bdd0271d52/docs/Wesley/learning%20outcomes/sprint%202/Realisation%20-%20Breadboard%20Realisation%20of%20the%20Four-Way%20Traffic-Light%20Setup.md).

Wesley. (2026b). *Analysis - Professionalising the traffic-light tile by transferring the breadboard circuit to perfboard* [Analysis deliverable](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/c718540b8fae7ddd4f0a44cd362841b4bc214b23/docs/Wesley/learning%20outcomes/sprint%204/Analysis%20-%20Professionalising%20the%20traffic-light%20tile%20by%20transferring%20the%20breadboard%20circuit%20to%20perfboard%20v2.md).

---

## Appendix A - Responsible use of ChatGPT

For this deliverable, I used ChatGPT as a support tool for grammar, spelling, and suggestions for formatting. I did not use it to replace my own project work or to hide the source of the work.

The technical content, project context, circuit choices, learning goal, and final responsibility remain mine. ChatGPT was used to help improve readability, sentence structure, spelling, and the organisation of the document. I checked the final text myself so that it still matches my project and my own understanding.

I include this appendix to be transparent about the use of AI support. This makes the process honest and helps prevent the work from being presented as hidden AI-generated work or as plagiarism.
