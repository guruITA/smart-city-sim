# Advice: Reducing ESP32-S3 pin usage for scalable smart parking sensors

**Author:** Gurpreet Singh  
**Date:** 20-05-2026  
**Version:** 1.0  
**Classification:** External  
**Client:** Mayor Mats Otten  
**Company:** The Embedded Alliance  

---

## Table of Contents
- [1. Introduction](#1-introduction)
- [2. Main advice question and subquestions](#2-main-advice-question-and-subquestions)
- [3. Methodology](#3-methodology)
- [4. Chapter 1: Advice for reducing ESP32-S3 pin usage](#4-chapter-1-advice-for-reducing-esp32-s3-pin-usage)
    - [4.1 Introduction](#41-introduction)
    - [4.2 Summary of the analysed options](#42-summary-of-the-analysed-options)
    - [4.3 Best fitting solution](#43-best-fitting-solution)
    - [4.4 Second choice](#44-second-choice)
    - [4.5 Subconclusion](#45-subconclusion)
- [5. Chapter 2: Advice for a scalable technical setup](#5-chapter-2-advice-for-a-scalable-technical-setup)
    - [5.1 Introduction](#51-introduction)
    - [5.2 Reduction of ESP32-S3 pin usage](#52-reduction-of-esp32-s3-pin-usage)
    - [5.3 Scalability for future expansion](#53-scalability-for-future-expansion)
    - [5.4 Keeping the current parking logic working](#54-keeping-the-current-parking-logic-working)
    - [5.5 Subconclusion](#55-subconclusion)
- [6. Chapter 3: Advice, risks and implementation direction.](#6-chapter-3-advice-risks-and-implementation-direction)
    - [6.1 Introduction](#61-introduction)
    - [6.2 Echo timing risk](#62-echo-timing-risk)
    - [6.3 I2C communication risk](#63-i2c-communication-risk)
    - [6.4 Testing and validation](#64-testing-and-validation)
    - [6.5 Subconclusion](#65-subconclusion)
- [7. Final advice](#7-final-advice)
- [8. Recommendations](#8-recommendations)
- [9. References](#9-references)

---

## 1. Introduction

This document is written for The Embedded Alliance and Mayor Mats Otten. It is also relevant for stakeholders in the Smart City project, such as municipal decision makers, parking operators and people involved in traffic or parking management. The document is written for readers who may not have detailed knowledge of embedded systems or robotics. Therefore, technical concepts are explained in simple words where needed.

Smart parking is relevant because parking availability can influence traffic flow, driver behaviour and the use of urban space. A review of smart parking systems explains that smart parking systems can use sensors, networking technologies, user interfaces and computational methods to improve parking management [(Fahim et al., 2021)](https://www.sciencedirect.com/science/article/pii/S2405844021011531).

The current smart parking prototype uses four ultrasonic sensors to detect whether four parking spaces are occupied or free. An ultrasonic sensor measures distance by sending out a sound pulse and using the returned echo signal to calculate the distance. The HC-SR04 datasheet explains that the module starts a measurement with a trigger signal and uses the echo output signal for the returned measurement [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

The sensors are connected to one ESP32-S3. The ESP32-S3 is used as the main controller in the prototype. Espressif describes the ESP32-S3 as a microcontroller-based system-on-chip with programmable GPIO pins and several peripheral interfaces. This makes GPIO usage an important design point when multiple components need to work together on the same controller [(Espressif Systems, z.d.)](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf).

This is a problem because The Embedded Alliance is working with multiple prototypes on one shared ESP32-S3 setup. The smart parking system is not the only function in the project. Other components, such as the smart streetlight, speed camera and other city functions, also need pins or communication lines. If too many ESP32-S3 pins are used by the ultrasonic sensors, fewer pins remain available for the rest of the Smart City system.

The purpose of this advice document is to recommend the best fitting solution for reducing the ESP32-S3 pin usage of the ultrasonic sensors, while keeping the current smart parking prototype logic working.

The advice is based on the previous analysis, where several possible solutions were compared: an I/O expander, a multiplexer, a shift register and a shared trigger setup. The MCP23017 is relevant in this advice because it provides 16 digital I/O pins through an I2C interface [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf). The I2C bus uses SDA and SCL lines for communication between components, which makes it useful when reducing direct controller pin usage [(NXP Semiconductors, 2021)](https://www.nxp.com/docs/en/user-guide/UM10204.pdf).

---

## 2. Main advice question and subquestions

The main advice question of this document is:

**Which solution should be used to reduce the ESP32-S3 pin usage of the ultrasonic sensors while keeping the current smart parking prototype logic working?**

To answer this advice question, the following subquestions are used:

1. Which solution fits the current smart parking prototype best?
2. Why is this solution better than the other analysed options?
3. Which solution is the second best option if the advised solution is not used?
4. How does the advised solution reduce ESP32-S3 pin usage?
5. How does the advised solution support future expansion?
6. What risks and conditions must be considered during implementation?

These subquestions help structure the advice and make the final recommendation clear for the client and stakeholders.

---

## 3. Methodology

This advice document is based on the findings from the analysis document. In that analysis, possible solutions were compared based on:

- compatibility with the ESP32-S3;
- number of ESP32-S3 pins that can be saved;
- communication method;
- complexity;
- scalability;
- reliability;
- suitability for ultrasonic sensors;
- impact on the current parking prototype logic.

The advice also uses technical datasheets and documentation. The MCP23017 datasheet is used to support the explanation of I/O expansion through I2C. The HC-SR04 datasheet is used to explain why the ultrasonic echo signal must be handled carefully. The PCF8574, CD74HC4067 and 74HC595 datasheets are used to compare the alternatives.

The advice is not only based on which component provides the most pins. It also considers whether the solution fits the current prototype, whether the existing parking logic can remain working, and whether the setup can be expanded later.

---

## 4. Chapter 1: Advice for reducing ESP32-S3 pin usage

### 4.1 Introduction

This chapter answers the following subquestion:

**Which solution fits the current smart parking prototype best?**

The goal is to use the analysis results and turn them into a clear advice.

---

### 4.2 Summary of the analysed options

In the analysis document, four possible solution types were compared.

| Solution             | Main strength                                                     | Main limitation                                                        |
| -------------------- | ----------------------------------------------------------------- | ---------------------------------------------------------------------- |
| I/O expander         | Adds extra digital I/O pins and reduces direct ESP32-S3 pin usage | Echo timing must be handled carefully when ultrasonic sensors are used |
| Multiplexer          | Can select one sensor signal at a time                            | Needs extra select logic and wiring                                    |
| Shift register       | Useful for controlling multiple output signals                    | Not suitable as the main solution for ultrasonic echo inputs           |
| Shared trigger setup | Simple and does not need an extra component                       | Only reduces trigger pin usage and does not solve the echo pin issue   |

Based on this comparison, the most relevant solution types are the **I/O expander** and the **multiplexer**.

The shift register is less suitable because it mainly helps with output signals. The 74HC595, for example, is described as an 8-bit serial-in, serial or parallel-out shift register, which makes it more relevant for output expansion than for timing-sensitive ultrasonic echo input signals [(Nexperia, 2024)](https://assets.nexperia.com/documents/data-sheet/74HC_HCT595.pdf). The shared trigger setup is simple, but it does not reduce enough pins for a scalable setup.

---

### 4.3 Best fitting solution

The advised solution is to use an **MCP23017 I/O expander**.

The MCP23017 is the best fitting option for the current smart parking prototype because it reduces the number of direct ESP32-S3 pins needed for the ultrasonic sensor setup. The MCP23017 provides 16 digital I/O pins through an I2C interface. This means the ESP32-S3 can communicate with the MCP23017 through the I2C bus, while the MCP23017 provides extra input and output pins for the prototype [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf).

This fits the goal of the project because the main problem is not that the parking logic does not work. The problem is that the ultrasonic sensors use several ESP32-S3 pins. By using an I/O expander, the hardware setup becomes more scalable and leaves more ESP32-S3 pins available for other Smart City components.

The MCP23017 is also more suitable than the PCF8574 for this prototype. The MCP23017 provides 16 I/O pins, while the PCF8574 provides 8 I/O pins [(Microchip Technology Inc., 2005](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf); [Texas Instruments Incorporated, 2024a)](https://www.ti.com/lit/ds/symlink/pcf8574.pdf). Since the goal is to reduce pin usage and support future expansion, the larger number of I/O pins makes the MCP23017 the stronger option.

---

### 4.4 Second choice

The second best option is the **CD74HC4067 multiplexer**.

The CD74HC4067 is a 16-channel multiplexer/demultiplexer. A multiplexer can select one signal from multiple signals. This is useful for the smart parking prototype because the ultrasonic sensors are measured one by one. The CD74HC4067 can therefore fit the existing sequential measurement logic [(Texas Instruments Incorporated, 2024b)](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf?ts=1767919871075&ref_url=https%253A%252F%252Fwww.mouser.com%252F).

The multiplexer is a good second choice because it can reduce the number of direct ESP32-S3 input pins. For example, instead of connecting every echo signal to a separate ESP32-S3 pin, a multiplexer can select one echo line and connect that selected signal to one ESP32-S3 input pin.

However, the CD74HC4067 is not advised as the first choice because it adds extra select logic. The software must choose which channel is active before reading the signal. This makes the setup less general than an I/O expander. A multiplexer is strong when selecting one sensor signal at a time, but the MCP23017 is stronger as a general digital I/O expansion solution.

The PCF8574 is not the second best main option. It is an alternative I/O expander, but it provides fewer I/O pins than the MCP23017. This makes it less suitable for a scalable prototype where extra pin capacity is important [(Texas Instruments Incorporated, 2024a)](https://www.ti.com/lit/ds/symlink/pcf8574.pdf).

---

### 4.5 Subconclusion

Based on the analysed options, the MCP23017 I/O expander is the best fitting solution for this prototype. It reduces direct ESP32-S3 pin usage, provides more expansion than an 8-bit I/O expander and fits better as a general scalable solution than a shift register or shared trigger setup.

The CD74HC4067 multiplexer is the second best option because it can select one sensor signal at a time and fits the sequential measurement logic. However, it is less flexible as a general expansion solution than the MCP23017.

---

## 5. Chapter 2: Advice for a scalable technical setup

### 5.1 Introduction

This chapter answers the following subquestion:

**Why is the MCP23017 suitable for the current smart parking prototype?**

The goal is to explain why the MCP23017 fits the current setup and future expansion needs.

---

### 5.2 Reduction of ESP32-S3 pin usage

The main reason to use the MCP23017 is that it reduces direct ESP32-S3 pin usage.

In the current smart parking prototype, the ultrasonic sensors need several pins. If each sensor signal is connected directly to the ESP32-S3, the number of used GPIO pins grows quickly. This becomes a problem because the ESP32-S3 is also used by other Smart City components.

The MCP23017 solves this by adding 16 extra digital I/O pins through I2C. The MCP23017 uses two 8-bit ports, PORTA and PORTB, which together provide 16 I/O pins [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf).

In simple words:

- the ESP32-S3 uses the I2C connection;
- the MCP23017 provides extra digital pins;
- fewer ultrasonic sensor signals need to be connected directly to the ESP32-S3.

This makes the parking setup cleaner and leaves more ESP32-S3 pins available for other components.

---

### 5.3 Scalability for future expansion

The MCP23017 also supports future expansion.

The current prototype focuses on four parking spaces. However, the Smart City project can grow further. More parking spaces, sensors or indicators may be added later. If all signals are connected directly to the ESP32-S3, the setup becomes harder to expand.

Because the MCP23017 uses I2C, it fits better in a scalable hardware structure. I2C uses two main communication lines: SDA and SCL [(NXP Semiconductors, 2021)](https://www.nxp.com/docs/en/user-guide/UM10204.pdf). This means extra hardware can communicate through the same communication bus, instead of every signal needing its own direct ESP32-S3 pin.

This makes the MCP23017 useful for a shared Smart City setup, where multiple prototypes need to work together on the same controller.

---

### 5.4 Keeping the current parking logic working

Another important reason to advise the MCP23017 is that the current parking logic can mostly stay the same.

The smart parking prototype already works with sequential measurement logic. This means the sensors are measured one by one. The MCP23017 does not require the full parking concept to be redesigned. The main change is that pin reading and writing are handled through the MCP23017 instead of direct ESP32-S3 GPIO pins.

This fits the goal of the learning question because the aim is not to rebuild the full smart parking system. The aim is to reduce ESP32-S3 pin usage while keeping the current parking prototype logic working.

However, the echo signal from an ultrasonic sensor remains an important technical point. The HC-SR04 uses the echo signal to calculate distance, and the echo pulse timing is part of the measurement [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf). Because of this, the MCP23017 solution must be tested carefully to make sure the measured distances remain stable enough for the prototype.

---

### 5.5 Subconclusion

The MCP23017 fits the current prototype because it reduces ESP32-S3 pin usage, supports future expansion and allows the current parking logic to remain mostly the same. The main condition is that the ultrasonic echo measurements must stay reliable during testing.

---

## 6. Chapter 3: Advice, risks and implementation direction

### 6.1 Introduction

This chapter answers the following subquestion:

**What risks and conditions must be considered during implementation?**

Every technical choice has limitations. The goal is to use the MCP23017 in a controlled and realistic way.

---

### 6.2 Echo timing risk

The main risk is the ultrasonic echo signal.

The HC-SR04 calculates distance based on the time between the trigger signal and the returned echo signal. This means the echo signal is timing-sensitive [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

When the echo signal is read directly by the ESP32-S3, the controller can measure the signal timing more directly. When the echo signal is read through an external component, such as an I/O expander, there is an extra communication step. This does not automatically make the solution unusable, but it does mean that the measurements must be checked carefully.

For this prototype, the MCP23017 can be used if testing shows that the parking status remains stable and reliable enough. If the system gives unstable readings, the echo timing should be reviewed.

---

### 6.3 I2C communication risk

The MCP23017 communicates through I2C. I2C uses SDA and SCL lines for communication between components [(NXP Semiconductors, 2021)](https://www.nxp.com/docs/en/user-guide/UM10204.pdf).

This is useful because it reduces the number of ESP32-S3 pins needed. However, I2C is still a communication bus. Reading or writing a pin through the MCP23017 is not the same as reading or writing a direct ESP32-S3 GPIO pin.

For normal digital signals, this is not a major problem. For timing-sensitive signals, such as ultrasonic echo signals, it needs extra attention. This means that the I2C setup should be kept stable, and the software should avoid unnecessary delays during sensor measurements.

---

### 6.4 Testing and validation

The MCP23017 should only be accepted as the final solution if the prototype still works reliably after implementation.

The following points should be tested:

- each parking space must still show the correct occupied/free status;
- the free parking count must remain stable;
- the system should not switch rapidly between occupied and free;
- the OLED output should still show the parking status clearly;
- the system should still work together with the other Smart City components;
- the ESP32-S3 pin usage should be reduced compared with the previous setup.

This is important because a scalable setup is only useful if the basic parking detection still works correctly. Reducing pins is not useful if the system becomes unreliable.

---

### 6.5 Subconclusion

The MCP23017 is a suitable solution, but it must be implemented carefully. The main risk is ultrasonic echo timing. If testing shows that the measurements remain stable, the MCP23017 is a strong choice for reducing ESP32-S3 pin usage and supporting future expansion.

---

## 7. Final advice

The final advice is:

**Use the MCP23017 I/O expander to reduce the ESP32-S3 pin usage of the ultrasonic sensors while keeping the current smart parking prototype logic working.**

The MCP23017 is the best fitting solution because it provides 16 digital I/O pins through I2C, reduces direct ESP32-S3 pin usage and supports a more scalable hardware setup [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf).

Compared with the PCF8574, the MCP23017 provides more I/O pins. Compared with a shift register, it is more suitable for general digital input and output expansion. Compared with only sharing the trigger line, it provides a stronger scalability improvement.

The CD74HC4067 multiplexer is the second best option. It is suitable when one sensor signal needs to be selected at a time, and this fits the current sequential measurement logic [(Texas Instruments Incorporated, 2024b)](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf?ts=1767919871075&ref_url=https%253A%252F%252Fwww.mouser.com%252F). However, the MCP23017 is advised as the first choice because it provides a more general and scalable I/O expansion structure.

The main condition is that ultrasonic echo timing must be tested carefully. The HC-SR04 uses echo timing to calculate distance, so the system must still be checked for stable parking detection after the MCP23017 is added [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

This solution fits the current project because it reduces ESP32-S3 pin usage without requiring a full redesign of the parking logic. It also leaves more room for the other Smart City components within The Embedded Alliance project.

---

## 8. Recommendations

Based on this advice document, the following recommendations are made:

1. Use the MCP23017 as the selected I/O expansion component for the smart parking prototype.

2. Use the MCP23017 to reduce the number of ultrasonic sensor signals connected directly to the ESP32-S3.

3. Keep the current parking logic as much as possible, especially the sequential sensor measurement structure.

4. Test the ultrasonic echo measurements carefully after adding the MCP23017.

5. Validate the occupied/free status for each parking space after implementation.

6. Check whether the free parking count remains stable during testing.

7. Keep the wiring and pin allocation clearly documented in the design document.

8. Mention the echo timing limitation clearly, so the solution is technically honest and defendable.

9. Keep the setup scalable so that future parking spaces or other Smart City components can be added more easily.

10. Use the CD74HC4067 multiplexer as the second choice if signal selection becomes more important than general I/O expansion.

11. Do not treat the PCF8574 as the second best main solution, because it is mainly a smaller I/O expander alternative with fewer pins than the MCP23017.

12. Do not use the MCP23017 blindly for every signal without checking whether the signal is timing-sensitive.

---

## 9. References

1. Espressif Systems. (z.d.-b). ESP32-S3 Series datasheet. [https://documentation.espressif.com/esp32-s3_datasheet_en.pdf](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf) viewed on 11 May 2026

2. Fahim, A., Hasan, M., & Chowdhury, M. A. (2021). Smart parking systems: comprehensive review based on various aspects. Heliyon, 7(5), e07050. [https://doi.org/10.1016/j.heliyon.2021.e07050](https://doi.org/10.1016/j.heliyon.2021.e07050) viewed on 11 May 2026

3. Microchip Technology Inc. (2005). MCP23017/MCP23S17 (pp. 1–6). [https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf) viewed on 11 May 2026

4. Nexperia. (2024). 74HC595; 74HCT595 8-bit serial-in, serial or parallel-out shift register with output latches; 3-state. In Nexperia [Product data sheet]. [https://assets.nexperia.com/documents/data-sheet/74HC_HCT595.pdf](https://assets.nexperia.com/documents/data-sheet/74HC_HCT595.pdf) viewed on 14 May 2026

5. NXP Semiconductors. (2021). I²C-bus specification and user manual (UM10204 Rev. 7.0). [https://www.nxp.com/docs/en/user-guide/UM10204.pdf](https://www.nxp.com/docs/en/user-guide/UM10204.pdf) viewed on 14 May 2026

6. Tech Support. (z.d.). Ultrasonic Ranging Module HC - SR04. [https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) viewed on 17 April 2026

7. Texas Instruments Incorporated. (2024a). PCF8574 Remote 8-Bit I/O Expander for I2C Bus [Technical manual]. [https://www.ti.com/lit/ds/symlink/pcf8574.pdf](https://www.ti.com/lit/ds/symlink/pcf8574.pdf) viewed on 14 May 2026

8. Texas Instruments Incorporated. (2024b). CD74HCx4067 High-Speed CMOS Logic 16-Channel Analog Multiplexer and Demultiplexer [Technical manual]. In CD74HC4067 CD74HCT4067 (SCHS209D editie, pp. 1–16). [https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf?ts=1767919871075&ref_url=https%253A%252F%252Fwww.mouser.com%252F](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf?ts=1767919871075&ref_url=https%253A%252F%252Fwww.mouser.com%252F) viewed on 14 May 2026