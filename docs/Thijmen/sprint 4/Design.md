# Professionalization Design E-ink display

## Table of Contents

1. [Introduction](#1-introduction)
2. [Project Overview](#2-project-overview)
3. [Breadboard Prototype](#3-breadboard-prototype)
4. [Transition to Perfboard](#4-transition-to-perfboard)
5. [Circuit Design](#5-circuit-design)
6. [Perfboard Design](#6-perfboard-design)
7. [Bill of Materials (BOM)](#7-bill-of-materials-bom)
8. [Challenges During Transition](#8-challenges-during-transition)
9. [Future PCB Design](#9-future-pcb-design)
10. [Conclusion](#10-conclusion)

---

## 1. Introduction

This document describes the transition of an embedded hardware project from a breadboard prototype to a more permanent perfboard implementation, as well as the planned future development of a custom PCB.

The project uses an ESP32-C3 SuperMini, an e-ink display HAT, a TP4056 charging module, and a rechargeable battery system. The goal of the redesign was to improve reliability, reduce loose wiring, and create a cleaner and more compact hardware solution.

---

## 2. Project Overview

The system consists of the following main components:

* ESP32-C3 SuperMini
* E-ink display HAT
* TP4056 charging module
* Rechargeable battery connection

Initially, the system was assembled on a breadboard for rapid prototyping and testing. After successful testing, the design will be transferred to a perfboard to create a more stable and reusable version.

A future PCB design is planned to further improve compactness and reliability.

### Development Evolution

At the beginning of the project, all City Sim functionalities were executed on a single ESP32-S3 development board. This included the logic for communication, processing, and controlling the e-ink display.

As the project evolved, the e-ink display functionality was separated into its own dedicated module using an ESP32-C3 SuperMini. This redesign improved modularity and made the display system more compact and easier to integrate into the overall City Sim environment.

Using a dedicated ESP32-C3 for the e-ink display also provided several advantages:

* Reduced hardware size
* Lower power consumption
* Better separation of responsibilities
* Easier hardware integration
* Improved scalability for future expansion

The transition to the ESP32-C3 SuperMini formed the basis for the perfboard and future PCB designs described in this document.

## 3. Breadboard Prototype

The first implementation of the system was built on a breadboard.

### Purpose of the Breadboard

The breadboard version was used to:

* Test component compatibility
* Verify SPI communication with the e-ink display
* Validate power delivery
* Test firmware functionality
* Debug hardware connections

### Advantages

* Easy to modify
* Fast prototyping
* No soldering required
* Components can be replaced quickly

### Limitations

Although the breadboard was useful for prototyping, several limitations became apparent:

* Loose jumper wire connections
* Large physical size
* Limited durability
* Difficult cable management
* Reduced reliability during movement

These limitations motivated the transition to a perfboard solution.

---

## 4. Transition to Perfboard

After validating the breadboard prototype, the circuit was transferred to a perfboard.

The main goal of this transition was to create a more permanent and stable hardware implementation while keeping the flexibility of manual assembly.

### Goals of the Transition

* Improve connection reliability
* Reduce loose wiring
* Create a compact layout
* Improve portability
* Prepare the design for a future PCB

### Approach

The same schematic used on the breadboard was recreated on the perfboard using soldered connections.

Special attention was given to:

* Component placement
* Shorter wire routing
* Stable power connections
* Accessible GPIO pins

---

## 5. Circuit Design

The circuit design remained identical between the breadboard and perfboard versions.

### Communication

The e-ink display communicates with the ESP32-C3 using SPI.

### Power System

The TP4056 charging module connects to the rechargeable battery and supplies power to the system.

### Schematic

The schematic was used as the foundation for both the perfboard and future PCB designs.

![Schematic of the ESP32-C3 e-ink display system](assets/city-sim-e-ink-display-schematic.png)

### Design Consistency

Maintaining the same schematic between prototype stages ensured:

* Firmware compatibility
* Easier debugging
* Simpler transition to future PCB development

---

## 6. Perfboard Design

The perfboard implementation contains the same components as the breadboard prototype but uses soldered connections instead of jumper wires.

### Main Components

#### ESP32-C3 SuperMini

The ESP32-C3 acts as the main controller and handles:

* Wireless communication
* SPI communication
* Display control

#### E-ink Display HAT

The e-ink display is connected through header pins and communicates using SPI.

#### TP4056 Charging Module

The TP4056 module manages charging of the lithium battery and powers the system.

---

### Perfboard Layout

The components were manually positioned on the perfboard to minimize wiring complexity and improve stability.

 ![Perfboard layout design in KiCad PCB Editor](assets/city-sim-e-ink-display-perfboard-editor.png)

---

### Perfboard 3D Visualization

The 3D render was used to verify component placement and spacing before assembly.

#### Front View

![3D front view of the perfboard design](assets/city-sim-e-ink-display-perfboard-3d-front.png)

#### Back View

 ![3D back view of the perfboard design](assets/city-sim-e-ink-display-perfboard-3d-back.png)

---

### Advantages of the Perfboard Design

Compared to the breadboard, the perfboard version offers:

* More reliable electrical connections
* Reduced risk of disconnected wires
* Improved durability
* Cleaner appearance
* Better portability

### Limitations of the Perfboard Design

Although the perfboard improved the prototype significantly, some limitations still remain:

* Manual soldering is time-consuming
* Wiring can still become complex
* Layout optimization is limited
* Reproducing identical boards is difficult

These limitations led to the consideration of a custom PCB design.

---

## 7. Bill of Materials (BOM)

| ID | Designator | Footprint                             | Quantity | Designation            | Reference |
| -- | ---------- | ------------------------------------- | -------- | ---------------------- |-------------------|
| 1  | BT1        | JST_EH_S2B-EH_1x02_P2.50mm_Horizontal | 1        | Battery_Cell           | [Digikey](https://www.digikey.com/en/products/detail/jst-sales-america-inc/S2B-EH/926533) |
| 2  | E1         | PinSocket_1x08_P2.54mm_Vertical       | 1        | E-ink display HAT      | [Mouser](https://nl.mouser.com/ProductDetail/Harwin/M50-3030842?qs=%252BdQmOuGyFcEVh5gBUNIiFA%3D%3D) |
| 3  | U1         | MODULE_ESP32-C3_SUPERMINI             | 1        | ESP32-C3_SUPERMINI_SMD | [Tinytronics](https://www.tinytronics.nl/en/development-boards/microcontroller-boards/with-wi-fi/esp32-c3-supermini-plus-development-board) |
| 4  | U2         | TP4056-Module                         | 1        | TP4056_Module          | [Mouser](https://nl.mouser.com/ProductDetail/Soldered/333014?qs=sGAEpiMZZMs5TKDXZEoCqOJ%252BWtJg0exjNv7rV%2FQFNRhneIj2%2FqDp%252BA%3D%3D) |

---

## 8. Challenges During Transition

Several challenges were encountered while moving from breadboard to perfboard.

### Soldering Accuracy

Because the perfboard does not contain predefined traces, connections had to be manually soldered carefully to avoid shorts and incorrect wiring.

### Component Placement

The placement of modules had to be optimized to:

* Reduce wire length
* Prevent overlapping connections
* Improve accessibility

### Power Routing

Stable power routing became more important because permanent soldered connections are harder to modify than breadboard wiring.

---

## 9. Future PCB Design

A custom PCB is planned as the next step in the hardware development process.

### Goals of the PCB Design

The future PCB aims to provide:

* Compact form factor
* Cleaner routing
* Improved reliability
* Easier assembly
* Reproducible manufacturing

---

### PCB Layout Design

The PCB layout was designed in KiCad to organize components and optimize routing.

![PCB layout designed in KiCad PCB Editor](assets/city-sim-e-ink-display-pcb-editor.png)

---

### PCB 3D Visualization

3D renders were created to validate the mechanical design and overall component placement.

#### Front View

![3D front view of the PCB design.](assets/city-sim-e-ink-display-pcb-3d-front.png)

#### Back View

![3D back view of the PCB design](assets/city-sim-e-ink-display-pcb-3d-back.png)

---

### Planned Improvements

Possible improvements include:

* Integrated traces instead of manual wiring
* Optimized component placement
* Dedicated mounting holes
* Better power distribution
* Smaller overall size

### Benefits Compared to Perfboard

A PCB would improve:

* Manufacturing consistency
* Long-term durability
* Professional appearance
* Electrical reliability

The PCB design will use the same schematic developed during the breadboard and perfboard stages.

---

## 10. Conclusion

The transition from breadboard to perfboard significantly improved the reliability and usability of the embedded hardware system.

The breadboard prototype was valuable for testing and debugging, while the perfboard implementation provided a more stable and compact solution suitable for extended use.

The future PCB design builds upon the experience gained during the prototyping stages and aims to create a more compact, reliable, and reproducible hardware platform.
