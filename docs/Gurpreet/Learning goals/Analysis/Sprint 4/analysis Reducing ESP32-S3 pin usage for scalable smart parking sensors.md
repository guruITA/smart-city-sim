# Analysis: Reducing ESP32-S3 pin usage for scalable smart parking sensors

**Author:** Gurpreet Singh  
**Date:** 14-05-2026  
**Version:** 1.0  
**Classification:** External  
**Client:** Mayor Mats Otten  
**Company:** The Embedded Alliance  

---

## Table of Contents

- [1. Introduction](#1-introduction)
- [2. Main research question and subquestions](#2-main-research-question-and-subquestions)
- [3. Methodology](#3-methodology)
- [4. Chapter 1: Current pin usage problem](#4-chapter-1-current-pin-usage-problem)
  - [4.1 Introduction](#41-introduction)
  - [4.2 Current smart parking setup](#42-current-smart-parking-setup)
  - [4.3 Why ESP32-S3 pin usage should be reduced](#43-why-esp32-s3-pin-usage-should-be-reduced)
  - [4.4 Subconclusion](#44-subconclusion)
- [5. Chapter 2: Possible solutions for reducing ESP32-S3 pin usage](#5-chapter-2-possible-solutions-for-reducing-esp32-s3-pin-usage)
  - [5.1 Introduction](#51-introduction)
  - [5.2 I/O expander](#52-io-expander)
  - [5.3 Multiplexer](#53-multiplexer)
  - [5.4 Shift register](#54-shift-register)
  - [5.5 Shared trigger setup](#55-shared-trigger-setup)
  - [5.6 Subconclusion](#56-subconclusion)
- [6. Chapter 3: Comparison and technical limitations](#6-chapter-3-comparison-and-technical-limitations)
  - [6.1 Introduction](#61-introduction)
  - [6.2 Comparison of possible solutions](#62-comparison-of-possible-solutions)
  - [6.3 Ultrasonic echo timing limitation](#63-ultrasonic-echo-timing-limitation)
  - [6.4 Subconclusion](#64-subconclusion)
- [7. Final conclusion](#7-final-conclusion)
- [8. Recommendations](#8-recommendations)
- [9. References](#9-references)

---

## 1. Introduction

This document is written for The Embedded Alliance and Mayor Mats Otten. It is also relevant for stakeholders in the Smart City project, such as municipal decision makers, parking operators and people involved in traffic or parking management. The document is written for readers who may not have detailed knowledge of embedded systems or robotics. Therefore, technical concepts such as GPIO pins, ultrasonic sensors and I/O expansion are explained in simple terms where needed.

Smart parking is relevant because parking availability can influence traffic flow, driver behaviour and the use of urban space. A review of smart parking systems explains that parking allocation has become a major problem in modern cities and that smart parking systems can use sensors, networking technologies, user interfaces and computational methods to improve parking management [(Fahim et al., 2021)](https://www.sciencedirect.com/science/article/pii/S2405844021011531).

The context of this document is the smart parking prototype within the Smart City project. In the current prototype, four ultrasonic sensors are used to detect whether four parking spaces are occupied or free. An ultrasonic sensor measures distance by sending out a sound pulse and waiting for the signal to return. The HC-SR04 datasheet explains that the module starts a measurement with a trigger pulse of at least 10 microseconds, sends eight 40 kHz pulses and uses the returned echo signal for the measurement [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

The current prototype uses one ESP32-S3 as the main controller. A controller is the part that reads the sensors and runs the logic of the system. The ESP32-S3 is a low-power microcontroller-based system-on-chip with integrated 2.4 GHz Wi-Fi and Bluetooth Low Energy. This makes it suitable for an IoT-based prototype where sensor data may later be used by other parts of the Smart City system [(Espressif Systems, z.d.)](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf).

In this project, GPIO pins are important because they are the connection points on the ESP32-S3. Sensors, displays and other hardware need these pins to communicate with the controller. The ESP32-S3 provides programmable GPIO pins and several peripheral interfaces. In a shared prototype setup, these pins must be used carefully because multiple Smart City components need hardware connections [(Espressif Systems, z.d.)](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf).

The purpose of this document is to analyse which components or solutions can reduce the ESP32-S3 pin usage of the ultrasonic sensors while keeping the current smart parking prototype logic working. This is important because the system should remain usable for the current four parking spaces, but also be easier to expand when more sensors or other Smart City components are added.

---

## 2. Main research question and subquestions

The main research question of this document is:

**Which components or solutions can reduce the ESP32-S3 pin usage of the ultrasonic sensors while keeping the current smart parking prototype logic working?**

To answer this research question, the following subquestions are used:

1. What is the current pin usage problem in the smart parking prototype?
2. Which types of components or solutions can reduce ESP32-S3 pin usage?
3. How do these solutions compare in terms of scalability, complexity and suitability?
4. What limitations must be considered when ultrasonic sensors are used?
5. Which solution types are most suitable for the current prototype conditions?

These subquestions keep the analysis focused on the pin usage problem and the technical suitability of possible solutions.

---

## 3. Methodology

This analysis is based on technical research into possible components and solutions for reducing ESP32-S3 pin usage. The analysis uses datasheets and technical documentation because these sources describe how the components work, which communication method they use and which limitations should be considered.

The possible solutions are compared based on practical criteria:

- compatibility with the ESP32-S3;
- number of ESP32-S3 pins that can be saved;
- communication method;
- complexity;
- scalability;
- reliability;
- suitability for ultrasonic sensors;
- impact on the current parking prototype logic.

These criteria are used because the solution must reduce pin usage, but should not make the current parking detection unreliable. The ultrasonic sensor uses an echo signal to calculate distance. The HC-SR04 datasheet explains that the echo pulse width is related to the measured distance, which means the timing of the echo signal is important for the distance calculation [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

---

## 4. Chapter 1: Current pin usage problem

### 4.1 Introduction

This chapter answers the following subquestion:

**What is the current pin usage problem in the smart parking prototype?**

The goal of this chapter is to explain why reducing ESP32-S3 pin usage is useful for the current smart parking prototype and for future expansion.

---

### 4.2 Current smart parking setup

The current smart parking prototype uses four ultrasonic sensors. Each sensor is used for one parking space. The sensor measures distance and the software uses that distance to decide whether the parking space is occupied or free.

An ultrasonic sensor normally works with two important signals: a trigger signal and an echo signal. The trigger signal starts the measurement. The echo signal is the returned signal that is used to calculate the distance. The HC-SR04 datasheet explains that the module uses a trigger input and echo output, and that the measured range is based on the echo signal [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

In the current parking prototype, the ESP32-S3 is used as the main controller. It reads the sensor values, processes the parking logic and shows the parking status. The current parking logic already works. Therefore, the technical problem is not the parking logic itself, but the number of ESP32-S3 pins used by the ultrasonic sensor setup.

---

### 4.3 Why ESP32-S3 pin usage should be reduced

The current parking prototype works with four parking spaces. However, the parking system is part of a larger Smart City project. The Embedded Alliance is working with multiple prototypes on one shared ESP32-S3 setup, such as the smart streetlight, speed camera and other city functions.

When many ESP32-S3 pins are used by the ultrasonic sensors, fewer pins remain available for other components. This becomes more important when the project grows, because future versions may need more sensors, indicators, displays or communication modules.

Reducing ESP32-S3 pin usage can help in several ways:

- more GPIO pins remain available for other Smart City components;
- the parking setup becomes easier to expand;
- the wiring can become more organised;
- the prototype becomes easier to maintain;
- the system becomes more suitable for future growth.

This is relevant because smart parking systems are not only judged by whether one sensor works. Literature about smart parking systems shows that these systems can include sensors, networking, user interfaces and services. This means that scalability and system structure are also important parts of a smart parking solution [(Fahim et al., 2021)](https://www.sciencedirect.com/science/article/pii/S2405844021011531).

However, reducing pin usage should not break the current parking logic. The system must still detect occupied and free parking spaces reliably.

---

### 4.4 Subconclusion

Based on this chapter, the current smart parking prototype works, but the ultrasonic sensors use several ESP32-S3 pins. Since the ESP32-S3 is shared with other Smart City components, reducing the number of pins used by the parking sensors is technically relevant for scalability and future expansion.

---

## 5. Chapter 2: Possible solutions for reducing ESP32-S3 pin usage

### 5.1 Introduction

This chapter answers the following subquestion:

**Which types of components or solutions can reduce ESP32-S3 pin usage?**

The goal is to analyse possible solution types that can reduce the number of ESP32-S3 pins used by the ultrasonic sensor setup.

---

### 5.2 I/O expander

An I/O expander is a component that adds extra input and output pins to a microcontroller. This means the ESP32-S3 can control or read more signals without every signal needing its own direct ESP32-S3 pin. For example, the MCP23017 provides 16-bit general purpose I/O expansion through an I2C interface. The MCP23017 has two 8-bit ports, PORTA and PORTB, which together provide 16 I/O pins [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf).

I2C is a communication method where components communicate with the ESP32-S3 using two main lines: SDA for data and SCL for clock. In simple words, this means several components can communicate through the same two communication lines instead of each needing many separate ESP32-S3 pins. This is described in the I2C-bus specification from NXP [(NXP Semiconductors, 2021)](https://www.nxp.com/docs/en/user-guide/UM10204.pdf).

Another example of an I2C I/O expander is the PCF8574. Texas Instruments describes the PCF8574 as an 8-bit input/output expander for a two-line bidirectional I2C bus [(Texas Instruments Incorporated, 2024a)](https://www.ti.com/lit/ds/symlink/pcf8574.pdf).

For the smart parking prototype, an I/O expander can reduce the number of direct ESP32-S3 pins because some sensor-related signals can be handled through the expander instead of being connected directly to the ESP32-S3.

Possible advantages:

- reduces direct ESP32-S3 pin usage;
- adds extra digital I/O pins;
- supports a cleaner hardware structure;
- supports future expansion through a bus-based setup.

Possible disadvantages:

- adds an extra component;
- needs extra code and configuration;
- I2C communication adds an extra communication step compared with direct GPIO use;
- ultrasonic echo timing must be handled carefully.

An I/O expander is technically suitable for adding digital I/O pins. The main point of attention is whether the ultrasonic echo signal can still be measured reliably when an I/O expander is used. This matters because the echo signal is used for the distance calculation.

---

### 5.3 Multiplexer

A multiplexer is a component that can select one signal from multiple signals. In simple words, it works like a selector. It can connect one selected sensor signal to the ESP32-S3, while the other signals are not selected at that moment. Texas Instruments describes the CD74HC4067 as a 16-channel analog multiplexer and demultiplexer. It works as a bidirectional switch, where one selected channel can be connected to a common line [(Texas Instruments Incorporated, 2024b)](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf?ts=1767919871075&ref_url=https%253A%252F%252Fwww.mouser.com%252F).

This type of component is relevant for the smart parking prototype because the ultrasonic sensors are measured one by one. If only one sensor signal needs to be active at a time, a multiplexer can be used to select which signal is connected to the ESP32-S3.

For example, instead of connecting every echo signal to a separate ESP32-S3 pin, a multiplexer can select one echo line and connect it to one ESP32-S3 input pin. This can reduce the number of direct input pins.

Possible advantages:

- can reduce the number of direct input pins;
- fits well with sequential sensor reading;
- can allow the ESP32-S3 to read one selected signal directly, which may be useful for timing-sensitive measurements;
- can support multiple sensors with fewer direct ESP32-S3 inputs.

Possible disadvantages:

- needs select pins to choose the active channel;
- adds extra wiring;
- adds extra logic in the software;
- the selected signal must be stable and safe for the ESP32-S3.

A multiplexer is suitable to consider because the current parking logic already measures the sensors sequentially. The disadvantage is that extra select logic is needed.

---

### 5.4 Shift register

A shift register is a component that can control multiple outputs with only a few microcontroller pins. In simple words, it can be used when the ESP32-S3 needs to send signals to multiple output parts, such as LEDs or enable lines. Nexperia describes the 74HC595 as an 8-bit serial-in, serial or parallel-out shift register with a storage register and 3-state outputs [(Nexperia, 2024)](https://assets.nexperia.com/documents/data-sheet/74HC_HCT595.pdf).

For the smart parking prototype, a shift register can be useful for output signals. For example, it could control LEDs, enable lines or other output components. This can reduce the number of ESP32-S3 pins needed for outputs.

However, a shift register is not a full solution for ultrasonic echo measurement. The ultrasonic echo signal is an input signal, and its timing is used for calculating distance. The HC-SR04 datasheet shows that distance depends on the echo pulse duration, so this signal must be measured accurately [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

Possible advantages:

- useful for reducing output pin usage;
- simple for controlling multiple outputs;
- useful for LEDs or control signals.

Possible disadvantages:

- not suitable as the main solution for reading ultrasonic echo signals;
- mainly useful for output expansion;
- does not solve the full parking sensor pin usage problem by itself.

A shift register can reduce output pin usage, but it is not the strongest option for the main ultrasonic sensor pin problem.

---

### 5.5 Shared trigger setup

Another possible solution is to share the trigger signal between multiple ultrasonic sensors. In this setup, multiple sensors use the same trigger line, while each sensor still has its own echo signal.

This approach is based on the working principle of the HC-SR04. The datasheet explains that the sensor starts measuring after receiving a trigger pulse and then uses the echo output for the returned signal [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

A shared trigger setup can reduce the number of trigger pins needed. However, the echo pins still need to be measured separately if each sensor must provide its own timing signal.

Possible advantages:

- simple to understand;
- no extra component needed;
- reduces the number of trigger pins;
- keeps echo signals directly connected to the ESP32-S3.

Possible disadvantages:

- echo pins still use several ESP32-S3 pins;
- less scalable when many sensors are added;
- does not reduce pin usage enough for larger setups;
- wiring can still become complex when the system grows.

A shared trigger setup is useful for a small prototype, but it does not solve the full scalability problem when more sensors are added.

---

### 5.6 Subconclusion

Based on this chapter, several solution types can reduce ESP32-S3 pin usage.

An I/O expander can add extra digital pins and reduce direct ESP32-S3 pin usage. A multiplexer can select one sensor signal at a time and fits well with sequential sensor reading. A shift register is useful for output expansion, but not as a full solution for ultrasonic echo inputs. A shared trigger setup is simple, but it does not reduce enough pins when the system needs to scale further.

---

## 6. Chapter 3: Comparison and technical limitations

### 6.1 Introduction

This chapter answers the following subquestion:

**How do these solutions compare in terms of scalability, complexity and suitability?**

The goal is to compare the possible solutions and explain the main technical limitation when ultrasonic sensors are used.

---

### 6.2 Comparison of possible solutions

| Solution | Source support | ESP32-S3 pins that can be saved | Communication method | Complexity | Scalability | Reliability | Suitability for ultrasonic sensors | Impact on current parking logic |
|---|---|---|---|---|---|---|---|---|
| I/O expander | MCP23017 and PCF8574 datasheets describe I/O expansion through I2C [(Microchip Technology Inc., 2005](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf); [(Texas Instruments Incorporated, 2024a))](https://www.ti.com/lit/ds/symlink/pcf8574.pdf). | High, because several digital signals can be moved away from direct ESP32-S3 pins. | I2C, using SDA and SCL lines [(NXP Semiconductors, 2021)](https://www.nxp.com/docs/en/user-guide/UM10204.pdf). | Medium, because an extra component and code configuration are needed. | High, because extra digital I/O pins can be added through a bus-based setup. | Medium, because normal digital signals are suitable, but echo timing needs attention. | Suitable for digital expansion, but ultrasonic echo timing must be tested carefully. | The current logic can mostly stay the same, but pin reading and writing must go through the expander. |
| Multiplexer | The CD74HC4067 datasheet describes a 16-channel multiplexer/demultiplexer with bidirectional switches [((Texas Instruments Incorporated, 2024b))](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf?ts=1767919871075&ref_url=https%253A%252F%252Fwww.mouser.com%252F). | Medium to high, because multiple sensor signals can share one ESP32-S3 input. | Select lines and one common signal line. | Medium, because channel selection must be added in software. | Medium to high, because more signals can be selected through channels. | Medium to high, because the ESP32-S3 can read one selected signal directly. | Suitable because the sensors are measured one by one. | Fits the current sequential measurement logic, but needs extra channel selection. |
| Shift register | The 74HC595 datasheet describes an 8-bit serial-in, serial or parallel-out shift register [(Nexperia, 2024)](https://assets.nexperia.com/documents/data-sheet/74HC_HCT595.pdf) | Low to medium, mainly for output signals. | Serial data, clock and latch lines. | Medium, because output data must be shifted in software. | Medium, because more outputs can be controlled with fewer pins. | High for outputs, but not suitable for ultrasonic echo inputs. | Not suitable as the main solution for ultrasonic echo signals. | Limited impact, because it mainly helps with outputs. |
| Shared trigger setup | The HC-SR04 datasheet shows that the trigger starts the measurement and the echo output is used for the returned signal [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf). | Low to medium, because only trigger pins are reduced. | Direct GPIO. | Low, because no extra component is needed. | Low to medium, because echo pins still increase with more sensors. | High, because echo signals stay directly connected to the ESP32-S3. | Suitable for small setups, but limited for larger systems. | Current logic changes only slightly, but pin reduction is limited. |

This comparison shows that each solution has a different role. There is no solution without trade-offs. The best solution depends on how many pins need to be saved and whether the ultrasonic timing remains reliable.

The I/O expander and multiplexer are the most relevant solution types for reducing ESP32-S3 pin usage. The I/O expander is strong for general digital expansion, while the multiplexer is strong for selecting one sensor signal at a time. The shift register is less suitable as the main solution because it is mainly useful for output signals. The shared trigger setup is simple, but it only reduces trigger pin usage and does not solve the echo pin usage problem.

---

### 6.3 Ultrasonic echo timing limitation

The most important technical limitation is the echo signal of the ultrasonic sensor.

The echo signal is timing-sensitive. This means the system must measure how long the echo signal stays active. That measured time is used to calculate the distance. The HC-SR04 datasheet explains that the distance is calculated using the time interval between sending the trigger signal and receiving the echo signal [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

This is important when external components are used. Some components are suitable for normal digital input or output signals, but the echo signal needs accurate timing. An I/O expander uses bus communication, such as I2C. I2C uses SDA and SCL lines for communication between components [(NXP Semiconductors, 2021)](https://www.nxp.com/docs/en/user-guide/UM10204.pdf). Because of this extra communication step, echo measurement through an I/O expander must be tested carefully.

This does not mean that I/O expansion is unsuitable. It means that the system must be designed carefully. Some signals may be suitable for expansion, while timing-sensitive echo signals need extra attention.

For this prototype, the current parking logic should remain working. Therefore, the chosen solution should reduce ESP32-S3 pin usage without making the parking detection unreliable.

---

### 6.4 Subconclusion

Based on this chapter, reducing ESP32-S3 pin usage is possible, but the ultrasonic echo signal must be handled carefully.

An I/O expander can reduce direct ESP32-S3 pin usage, but echo timing is a technical risk. A multiplexer may fit the current sequential reading logic. A shift register is mainly useful for outputs. A shared trigger setup is simple, but does not reduce enough pins for larger expansion.

---

## 7. Final conclusion

This analysis examined which components or solutions can reduce the ESP32-S3 pin usage of the ultrasonic sensors while keeping the current smart parking prototype logic working.

The current smart parking prototype works, but the ultrasonic sensors use several ESP32-S3 pins. Since The Embedded Alliance works with multiple prototypes on one shared ESP32-S3 setup, reducing pin usage is important for scalability and future expansion.

The analysis compared four possible directions:

- I/O expander;
- multiplexer;
- shift register;
- shared trigger setup.

The I/O expander is strong for adding extra digital I/O pins and reducing direct ESP32-S3 pin usage. The multiplexer is strong for selecting one sensor signal at a time and fits the existing sequential measurement logic. The shift register is useful for output expansion, but does not solve the ultrasonic echo input problem. The shared trigger setup is simple, but only reduces trigger pin usage and leaves the echo pin problem mostly unchanged.

The main technical limitation is the ultrasonic echo signal. Because the distance calculation depends on echo timing, the selected solution must not make the measurements unreliable.

Based on this analysis, the most relevant solution types for this prototype are the I/O expander and the multiplexer. Both can reduce ESP32-S3 pin usage, but they do this in different ways. The I/O expander is stronger for general digital expansion, while the multiplexer is stronger for selecting sensor signals one at a time.

---

## 8. Recommendations

Based on this analysis, the following recommendations are made:

1. Focus on solutions that reduce ESP32-S3 pin usage without changing the main parking logic.
2. Give priority to solutions that can support future expansion.
3. Treat the ultrasonic echo signal as timing-sensitive.
4. Do not move timing-sensitive signals to another component without testing measurement stability.
5. Use the I/O expander mainly where digital expansion is needed.
6. Use a multiplexer mainly where one signal must be selected from multiple sensor lines.
7. Do not use a shift register as the main solution for ultrasonic echo inputs.
8. A shared trigger setup can reduce some pins, but is not enough as the main scalability solution.

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
