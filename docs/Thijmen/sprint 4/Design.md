# Professionalization of a Breadboard Prototype into a Perfboard and PCB Solution for a City Sim E-Ink Display

* **Author:** Thijmen Walter (Embedded & Robotics Engineer Student)
* **Date:** 31-05-2026
* **Version:** 2.0
* **Classification:** Internal
* **Client:** Mayor Mats Otten
* **Company:** The Embedded Alliance

## Table of Contents

* [1. Introduction](#1-introduction)
* [2. Main Question and Subquestions](#2-main-question-and-subquestions)
* [3. Methodology](#3-methodology)
* [4. Chapter 1: Hardware Architecture](#4-chapter-1-hardware-architecture)
    * [4.1 Introduction](#41-introduction)
    * [4.2 Original Architecture](#42-original-architecture)
    * [4.3 Modular Architecture](#43-modular-architecture)
    * [4.4 Subconclusion](#44-subconclusion)
* [5. Chapter 2: Hardware Design and Validation](#5-chapter-2-hardware-design-and-validation)
    * [5.1 Introduction](#51-introduction)
    * [5.2 Required Components](#52-required-components)
    * [5.3 Schematic Design](#53-schematic-design)
    * [5.4 KiCad Design Workflow](#54-kicad-design-workflow)
    * [5.4 Subconclusion](#55-subconclusion)
* [6. Chapter 3: Prototype Professionalization](#6-chapter-3-prototype-professionalization)
    * [6.1 Introduction](#61-introduction)
    * [6.2 Breadboard Prototype](#62-breadboard-prototype)
    * [6.3 Perfboard Implementation](#63-perfboard-implementation)
    * [6.4 Future PCB Design](#64-future-pcb-design)
    * [6.5 Subconclusion](#65-subconclusion)
* [7. Final Conclusion](#7-final-conclusion)
* [8. Recommendations](#8-recommendations)
* [9. References](#9-references)
* [10. Appendix](#10-appendix)

---

# 1. Introduction

This document was written for The Embedded Alliance and the Smart Cities learning environment.

The context of this document is the development of a reusable e-ink display module for the City Sim project. During development, an initial breadboard prototype was created to validate communication between the ESP32-C3 and the e-ink display. After successful testing, opportunities were identified to improve the reliability, maintainability, portability, and scalability of the hardware.

The purpose of this document is to investigate how the prototype can be professionalized into a more permanent and reusable hardware solution. This is relevant because City Sim requires reliable display modules that can be integrated into multiple city tiles while remaining easy to reproduce and maintain.

The design process focused on improving reliability, reducing wiring complexity, increasing portability, and preparing the hardware for future manufacturing. The design decisions presented in this document build upon the requirements and findings identified during the analysis phase (Walter, 2026).

This document is written for project stakeholders, students, and technical supervisors. It assumes that the reader has a basic understanding of embedded systems and electronic prototyping.

---

# 2. Main Question and Subquestions

The main design question of this document is:

**How can the City Sim e-ink display prototype be professionalized into a reliable, reusable, and scalable hardware solution?**

To answer this question, the following subquestions were formulated:

1. How should the hardware architecture of the e-ink display module be designed?
2. How can the breadboard prototype be converted into a more reliable perfboard implementation?
3. Which components are required for a portable and reusable e-ink display module?
4. How can KiCad be used to support schematic design, layout planning, and validation?
5. How can the design be prepared for future PCB manufacturing and large-scale deployment?

---

# 3. Methodology

This document was created using the following methods:

* Literature research
* Documentation analysis
* Hardware prototyping
* Schematic design in KiCad
* Perfboard layout planning
* PCB design development
* Prototype evaluation and testing

These methods were chosen because they provide both theoretical and practical insight into improving the reliability and manufacturability of embedded hardware systems.

---

# 4. Chapter 1: Hardware Architecture

## 4.1 Introduction

This chapter answers the following subquestion:

**How should the hardware architecture of the e-ink display module be designed?**

To answer this question, both the original architecture and the redesigned modular architecture are discussed.

## 4.2 Original Architecture

At the beginning of the project, all City Sim functionality was executed on a single ESP32-S3 development board.

The board was responsible for:

* Backend communication
* Data processing
* Display control
* General system logic

While functional, this solution occupied more space than necessary and combined multiple responsibilities within a single hardware module.

## 4.3 Modular Architecture

Following the analysis phase, the display functionality was separated into a dedicated module using an ESP32-C3 SuperMini (Walter, 2026).

This redesign provided several advantages:

* Reduced hardware size
* Lower power consumption
* Better separation of responsibilities
* Easier integration into City Sim tiles
* Improved scalability

The ESP32-C3 was selected because it provides Wi-Fi connectivity, sufficient processing power, and a compact form factor suitable for embedded IoT applications (Espressif Systems, 2025).

The ESP32-C3 SuperMini became the foundation for all subsequent prototype designs.

## 4.4 Subconclusion

The modular architecture provides a more suitable foundation for the City Sim display module because it reduces complexity, improves scalability, and separates display functionality from the remainder of the system.

---

# 5. Chapter 2: Hardware Design and Validation

## 5.1 Introduction

This chapter answers the following subquestions:

* Which components are required for a portable and reusable e-ink display module?
* How can KiCad be used to support schematic design, layout planning, and validation?

To answer these questions, the hardware components, schematic design, and KiCad workflow are discussed.

## 5.2 Required Components

The final design consists of four primary functional blocks.

### ESP32-C3 SuperMini

The ESP32-C3 acts as the primary controller and handles:

* Wi-Fi communication
* Backend communication
* SPI communication
* Display control

### E-Ink Display HAT

The e-ink display is connected using SPI communication, as identified during the analysis phase (Walter, 2026).

The display receives:

* MOSI
* SCK
* CS
* DC
* RST
* BUSY

signals from the ESP32-C3.

Compatibility with Waveshare and Seengreat displays was investigated because documentation for the MH-ET Live display was limited (Seengreat, n.d.; Waveshare, n.d.; Kravec, 2025).

### TP4056 Charging Module

The TP4056 charging module manages battery charging and protection.

### Battery System

A rechargeable lithium-ion battery provides portable power for the display module.

## 5.3 Schematic Design

The schematic was created in KiCad and served as the foundation for all hardware implementations (KiCad, 2025).

The design consists of four main functional blocks:

* ESP32-C3 SuperMini
* E-Ink Display HAT
* TP4056 Charging Module
* Battery System

### Schematic Diagram

![City Sim E-Ink Display Schematic](assets/city-sim-e-ink-display-schematic.png)

The schematic remained unchanged throughout all prototype stages to maintain firmware compatibility and simplify debugging.

## 5.4 KiCad Design Workflow

KiCad was used throughout the entire design process for schematic design, perfboard planning, PCB layout development, and 3D visualization (KiCad, 2025).

### Schematic Design

KiCad was used to create and maintain the electrical schematic.

### Perfboard Layout Design

The KiCad PCB Editor was used to digitally recreate the perfboard layout before physical assembly.

This helped:

* Reduce wiring errors
* Verify placement
* Optimize routing

### PCB Layout Design

A future PCB design was created using the same schematic.

### 3D Visualization

KiCad's 3D viewer was used to validate:

* Component placement
* Connector accessibility
* Mechanical fit
* Overall appearance

The corresponding KiCad project files are included in the project repository.

## 5.5 Subconclusion

The selected hardware components provide a suitable platform for the display module, while KiCad enabled efficient schematic development, design validation, and preparation for future PCB manufacturing.

---

# 6. Chapter 3: Prototype Professionalization

## 6.1 Introduction

This chapter answers the following subquestions:

* How can the breadboard prototype be converted into a more reliable perfboard implementation?
* How can the design be prepared for future PCB manufacturing and large-scale deployment?

To answer these questions, the breadboard prototype, perfboard implementation, and future PCB design are discussed.

## 6.2 Breadboard Prototype

The first implementation was built on a breadboard.

The breadboard prototype was used to:

* Verify SPI communication
* Validate power delivery
* Test firmware functionality
* Test component compatibility
* Debug hardware connections

The prototype was created to validate assumptions identified during the analysis phase regarding display communication and hardware compatibility (Walter, 2026).

### Advantages

Breadboards are commonly used during early-stage hardware development because they allow rapid prototyping, easy modifications, and reusable components without soldering (Adafruit, 2024).

Advantages included:

* Rapid prototyping
* Easy modifications
* Fast troubleshooting
* No soldering required
* Reusable components

### Limitations

Several limitations became apparent during testing:

* Loose jumper wire connections
* Large physical footprint
* Difficult cable management
* Limited durability
* Reduced reliability during movement

These limitations motivated the transition to a perfboard design.

## 6.3 Perfboard Implementation

After successful testing, the design was transferred to a perfboard.

The goals were:

* Improve reliability
* Reduce loose wiring
* Improve portability
* Create a cleaner design
* Prepare for future PCB development

Perfboards provide a more permanent prototyping solution than traditional breadboards while maintaining flexibility during development (MKTPCB, 2023).

The same schematic was reused to ensure consistency throughout the design process.

### Perfboard Layout

Design considerations included:

* Short wire lengths
* Accessible GPIO pins
* Stable power connections
* Compact component placement

![Perfboard Layout](assets/city-sim-e-ink-display-perfboard-editor.png)

### 3D Validation

![Perfboard Front View](assets/city-sim-e-ink-display-perfboard-3d-front.png)

![Perfboard Back View](assets/city-sim-e-ink-display-perfboard-3d-back.png)

### Advantages

Compared to the breadboard:

* More reliable electrical connections
* Improved durability
* Better portability
* Cleaner appearance
* Reduced wiring complexity

### Limitations

Some limitations remain:

* Manual soldering is required
* Reproducing boards is difficult
* Layout flexibility is limited
* Wiring complexity still exists

These limitations motivated the development of a custom PCB design.

## 6.4 Future PCB Design

A custom PCB was designed as the next stage of development.

The PCB uses the same validated schematic while replacing manual wiring with dedicated copper traces.

Printed circuit boards provide improved reliability, manufacturability, and electrical consistency compared to manually assembled prototypes (SparkFun Electronics, 2025; TechTarget, 2024).

### PCB Layout

![PCB Layout](assets/city-sim-e-ink-display-pcb-editor.png)

### PCB 3D Validation

![PCB Front View](assets/city-sim-e-ink-display-pcb-3d-front.png)

![PCB Back View](assets/city-sim-e-ink-display-pcb-3d-back.png)

### Expected Improvements

The PCB design is expected to provide:

* Improved reliability
* Cleaner routing
* Reduced assembly effort
* Easier manufacturing
* Better power distribution
* Dedicated mounting holes
* Smaller overall size

These improvements support the scalability and maintainability goals identified during the analysis phase (Walter, 2026).

## 6.5 Subconclusion

The breadboard prototype successfully validated the design, the perfboard implementation improved reliability and portability, and the future PCB design provides a clear path toward large-scale deployment and manufacturing.

# 7. Final Conclusion

This document investigated how the City Sim e-ink display prototype can be professionalized into a reliable, reusable, and scalable hardware solution.

First, a modular hardware architecture based on the ESP32-C3 SuperMini was developed to improve scalability and reduce complexity.

Second, the required hardware components were identified and validated using KiCad throughout the design process.

Third, the breadboard prototype was successfully converted into a more reliable perfboard implementation that improved portability and durability.

Finally, a custom PCB design was developed to prepare the system for future manufacturing and large-scale deployment.

Based on the full analysis, it can be concluded that **the combination of a modular architecture, perfboard implementation, and future PCB design provides a reliable, maintainable, and scalable hardware solution for the City Sim platform.**

---

# 8. Recommendations

Based on the results of this document, the following recommendations are made:

1. Continue using the ESP32-C3 SuperMini as the standard display controller.
2. Use perfboards during future prototype iterations before PCB production.
3. Manufacture and test the custom PCB design.
4. Perform long-term reliability testing using battery-powered operation.
5. Standardize the PCB design for all City Sim display tiles.

---

# 9. References

1. Adafruit. (2024). *Perma-Proto Guide*. Retrieved May 31, 2026, from https://learn.adafruit.com/breadboards-for-beginners/perma-protos

2. Espressif Systems. (2025). *ESP32-C3 Series Datasheet*. Retrieved May 31, 2026, from https://www.espressif.com/

3. KiCad. (2025). *KiCad Documentation*. Retrieved May 31, 2026, from https://docs.kicad.org/

4. Kravec, M. (2025, February 23). *Control 4-color MH-ET Live Epaper using Arduino*. Retrieved April 22, 2026, from https://kravemir.org/how-to/control-4-color-mh-et-live-epaper-using-arduino/

5. MKTPCB. (2023). *Perfboard | A Quick Guide | Types, Uses, Techniques, and More*. Retrieved May 31, 2026, from https://www.mktpcb.com/perfboard/

6. Seengreat. (n.d.). *2.9inch SPI e-INK Display Expansion Module HAT 296x128 Wiki*. Retrieved April 22, 2026, from https://seengreat.com/wiki/132/29inch-e-ink-display

7. SparkFun Electronics. (2025). *PCB Basics*. Retrieved May 31, 2026, from https://learn.sparkfun.com/tutorials/pcb-basics/all

8. TechTarget. (2024). *What is a Printed Circuit Board (PCB)?*. Retrieved May 31, 2026, from https://www.techtarget.com/whatis/definition/printed-circuit-board-PCB

9. Walter, T. (2026). *Professionalization of a Breadboard Prototype into a Perfboard or PCB Solution*. The Embedded Alliance.

10. Waveshare. (n.d.). *2.9inch e-Paper Module Manual*. Retrieved April 22, 2026, from https://www.waveshare.com/wiki/2.9inch_e-Paper_Module_Manual

---

# 10. Appendix

## Appendix A – Schematic Diagram

ESP32-C3 e-ink display schematic.

## Appendix B – Perfboard Layout

KiCad perfboard layout and design files.

## Appendix C – Perfboard 3D Renders

Front and back validation renders.

## Appendix D – PCB Design

PCB layout and routing design.

## Appendix E – PCB 3D Renders

Front and back PCB visualizations.

## Appendix F – Bill of Materials

Detailed supplier and pricing information.

## Appendix G – KiCad Project Files

Schematic, PCB, footprints, and project source files.
