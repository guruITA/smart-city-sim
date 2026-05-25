# Advice: Reducing ESP32-S3 pin usage for scalable smart parking sensors

**Author:** Gurpreet Singh  
**Date:** 25-05-2026  
**Version:** 2.0  
**Classification:** External  
**Client:** Mayor Mats Otten  
**Company:** The Embedded Alliance  

---

## Table of Contents
- [1. Introduction](#1-introduction)
- [2. Main advice question and subquestions](#2-main-advice-question-and-subquestions)
- [3. Methodology](#3-methodology)
- [4. Chapter 1: Component comparison for reducing ESP32-S3 pin usage](#4-chapter-1-component-comparison-for-reducing-esp32-s3-pin-usage)
    - [4.1 Introduction](#41-introduction)
    - [4.2 Central decision problem](#42-central-decision-problem)
    - [4.3 Component options](#43-component-options)
    - [4.4 Comparison of the component options](#44-comparison-of-the-component-options)
    - [4.5 Selection of the best solution](#45-selection-of-the-best-solution)
    - [4.6 Second choice](#46-second-choice)
    - [4.7 Subconclusion](#47-subconclusion)
- [5. Chapter 2: Advice for a scalable technical setup](#5-chapter-2-advice-for-a-scalable-technical-setup)
    - [5.1 Introduction](#51-introduction)
    - [5.2 Recommended system setup](#52-recommended-system-setup)
    - [5.3 Why the MCP23017 is advised](#53-why-the-mcp23017-is-advised)
    - [5.4 Keeping the current parking logic working](#54-keeping-the-current-parking-logic-working)
    - [5.5 Subconclusion](#55-subconclusion)
- [6. Chapter 3: Advice, risks and implementation direction](#6-chapter-3-advice-risks-and-implementation-direction)
    - [6.1 Introduction](#61-introduction)
    - [6.2 Advice based on requirements](#62-advice-based-on-requirements)
    - [6.3 Risks and recommendations](#63-risks-and-recommendations)
    - [6.4 Subconclusion](#64-subconclusion)
- [7. Final conclusion](#7-final-conclusion)
- [8. Recommendations](#8-recommendations)
- [9. Previous work](#9-previous-work)
- [10. References](#10-references)

---

## 1. Introduction

Smart parking is an important part of smart city development. In modern cities, parking availability can influence traffic flow, space usage and the experience of drivers. A review of smart parking systems explains that smart parking systems can use sensors, networking technologies, user interfaces and computational methods to improve parking management [(Fahim et al., 2021)](https://www.sciencedirect.com/science/article/pii/S2405844021011531).

This advice document is written for Mayor Mats Otten and other stakeholders in the Smart City project, such as parking operators, municipal officials and project members of The Embedded Alliance. The document advises which component is most suitable for reducing ESP32-S3 pin usage in the current smart parking prototype, while keeping the current parking logic working.

The current smart parking prototype uses four ultrasonic sensors to detect whether four parking spaces are occupied or free. An ultrasonic sensor measures distance by sending out a sound pulse and using the returned echo signal to calculate the distance. The HC-SR04 datasheet explains that the module starts a measurement with a trigger signal and uses the echo output signal for the returned measurement [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

The sensors are connected to one ESP32-S3. The ESP32-S3 is used as the main controller in the prototype. Espressif describes the ESP32-S3 as a microcontroller-based system-on-chip with programmable GPIO pins and several peripheral interfaces. This makes GPIO usage an important design point when multiple components need to work together on the same controller [(Espressif Systems, z.d.)](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf).

In the current Smart City project, The Embedded Alliance is working with multiple prototypes on one shared ESP32-S3 setup. The smart parking system is not the only function in the project. Other components, such as the smart streetlight, speed camera and other city functions, also need pins or communication lines. If too many ESP32-S3 pins are used by the ultrasonic sensors, fewer pins remain available for the rest of the system.

This advice builds on the findings from the analysis document: [Analysis: Reducing ESP32-S3 pin usage for scalable smart parking sensors](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/f1468907568a7099e80ed96108f3eeabdb348dc0/docs/Gurpreet/Learning%20goals/Analysis/Sprint%204/analysis%20Reducing%20ESP32-S3%20pin%20usage%20for%20scalable%20smart%20parking%20sensors.md). In that analysis, the current pin usage problem was examined and several possible solution types were compared.

The analysis showed that I/O expanders and multiplexers are the most relevant directions for reducing direct ESP32-S3 pin usage. It also showed that ultrasonic echo timing is an important technical limitation, because the HC-SR04 uses the echo signal to calculate distance [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

Based on the analysis, the most important decision is which component can reduce ESP32-S3 pin usage without making the current parking detection unreliable. The advised solution must therefore fit the existing ultrasonic sensor logic, support future expansion and remain practical for the current prototype.

---

## 2. Main advice question and subquestions

The main advice question of this document is:

**Which component should be advised to reduce the ESP32-S3 pin usage of the ultrasonic sensors while keeping the current smart parking prototype logic working?**

To answer this main advice question, the following subquestions are used:

1. Which component options are most relevant based on the analysis?

2. How do the I/O expander options compare based on practical decision criteria?

3. How do the multiplexer options compare based on practical decision criteria?

4. Which component is the best fit for the current smart parking prototype?

5. Which component is the second-best option if the advised component is not used?

6. What risks must be considered when using the advised component with ultrasonic sensors?

These subquestions help structure the advice and make the reasoning clear for the client and stakeholders.

---

## 3. Methodology

This advice document is based on the findings from the analysis document: [Analysis: Reducing ESP32-S3 pin usage for scalable smart parking sensors](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/f1468907568a7099e80ed96108f3eeabdb348dc0/docs/Gurpreet/Learning%20goals/Analysis/Sprint%204/analysis%20Reducing%20ESP32-S3%20pin%20usage%20for%20scalable%20smart%20parking%20sensors.md). The analysis investigated technical solution types that could reduce ESP32-S3 pin usage.

The component choice is made by comparing the options on practical decision criteria. These criteria are used because the chosen component must not only be technically possible, but also practical for the current prototype.

The following criteria are used:

- number of extra I/O pins or channels gained;
- expected reduction of direct ESP32-S3 pin usage;
- estimated component price;
- availability of documentation and examples;
- community or library support;
- implementation complexity;
- suitability for the current ultrasonic sensor logic;
- scalability for future expansion;
- risk for timing-sensitive echo measurements.

The advice compares two I/O expanders and two multiplexers. The I/O expanders are the MCP23017 and PCF8574. The multiplexers are the CD74HC4067 and CD74HC4051/74HC4051. These four components were selected because they directly match the decision problem: reducing ESP32-S3 pin usage in a way that can still work with the current ultrasonic parking prototype.

The advice uses the analysis document, component datasheets, library documentation and practical supplier information. Datasheets are used to explain the technical function of the components. Library documentation is used to evaluate practical software support. Supplier information is used to compare estimated prices and practical availability.

---

## 4. Chapter 1: Component comparison for reducing ESP32-S3 pin usage

### 4.1 Introduction

This chapter answers the following subquestion:

**How do the relevant component options compare based on practical decision criteria?**

The analysis document showed which solution types are technically possible for reducing ESP32-S3 pin usage. This chapter uses those analysis results as the starting point and turns them into a practical component comparison.

The comparison focuses on the components that are most relevant for the prototype: two I/O expanders and two multiplexers.

---

### 4.2 Central decision problem

The central decision problem follows directly from the analysis document. The analysis concluded that reducing ESP32-S3 pin usage is technically relevant because the smart parking prototype is part of a larger shared ESP32-S3 setup. It also showed that any selected solution must take ultrasonic echo timing into account.

The current smart parking prototype already works, but the ultrasonic sensors use several direct ESP32-S3 pins. This limits the number of pins that remain available for other Smart City components.

For that reason, the selected component must solve the pin usage problem without making the existing parking detection unreliable. The component should reduce direct ESP32-S3 pin usage without forcing a full redesign of the current parking logic. It should also be affordable, documented well enough, available for prototyping and suitable for future expansion.

For this advice, the most important decision criteria are:

- extra pins or channels gained;
- direct ESP32-S3 pins saved;
- estimated component price;
- documentation and examples;
- community or library support;
- implementation complexity;
- fit with the current ultrasonic sensor logic;
- scalability;
- timing risk.

This makes the advice more practical than only comparing the components technically. A component can look strong in a datasheet, but still be the wrong choice if it is difficult to implement, poorly documented or unsuitable for the current prototype.

---

### 4.3 Component options

Based on the analysis document and the decision problem, four component options are relevant for this advice: two I/O expanders and two multiplexers.

The first I/O expander option is the **MCP23017**. This component provides 16 digital I/O pins through I2C. This makes it useful when more input or output pins are needed without connecting every signal directly to the ESP32-S3 [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf). The component is also practical for Arduino-based development because Adafruit provides an Arduino library for MCP23017 and related MCP23xxx I/O expanders [(Adafruit, z.d.)](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library).

The second I/O expander option is the **PCF8574**. This component also uses I2C, but it provides 8 extra I/O pins instead of 16. This makes it a smaller expansion option [(Texas Instruments Incorporated, 2024a)](https://www.ti.com/lit/ds/symlink/pcf8574.pdf). It also has available Arduino/ESP32 library support, for example through the PCF8574 library by Renzo Mischianti [(Mischianti, z.d.)](https://github.com/xreef/PCF8574_library).

The first multiplexer option is the **CD74HC4067**. This component can select one signal from 16 channels. This is relevant because the ultrasonic sensors are measured one by one, so selecting one signal at a time fits the current measurement logic [(Texas Instruments Incorporated, 2024b)](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf). TinyTronics also describes the HC4067 module as a 16-channel analog multiplexer where four address pins are used to select which of the 16 pins is connected to the signal pin [(TinyTronics, z.d.-c)](https://www.tinytronics.nl/en/communication-and-signals/io-converters/hc4067-16-channel-analog-multiplexer).

The second multiplexer option is the **CD74HC4051 / 74HC4051**. Texas Instruments describes the CD74HC4051 as a 5V, 8:1, 1-channel analog multiplexer [(Texas Instruments Incorporated, z.d.)](https://www.ti.com/product/CD74HC4051). SparkFun/OpenCircuit also describes the 74HC4051 breakout as an 8-channel multiplexer/demultiplexer with selectable inputs/outputs [(OpenCircuit, z.d.-a)](https://opencircuit.shop/product/sparkfun-multiplexer-breakout-8-channel). This makes it a logical smaller multiplexer alternative to the CD74HC4067.

---

### 4.4 Comparison of the component options

| Component             | Type         |  Extra pins / channels |                                                                                                                                                                                                                                                                                                                               Estimated price | Documentation and support                                                                                                                                                                                                                          | Complexity    | Suitability for ultrasonic sensor logic                                        | Main limitation                      |
| --------------------- | ------------ | ---------------------: | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------- | ------------------------------------------------------------------------------ | ------------------------------------ |
| MCP23017              | I/O expander |    16 digital I/O pins |                                                                                                                                                  About €4.00 based on TinyTronics [(TinyTronics, z.d.-a)](https://www.tinytronics.nl/en/components/ics-and-microcontroller-chips/ics/mcp23017-i2c-port-expander-16-inputs-outputs-28-pin-dip) | Strong datasheet support and common Arduino library support [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf); [(Adafruit, z.d.)](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library) | Medium        | Suitable for general digital I/O expansion, but echo timing must be tested     | I2C adds an extra communication step |
| PCF8574               | I/O expander |     8 digital I/O pins |                                                                                                                                                                                       About €2.50 based on TinyTronics [(TinyTronics, z.d.-b)](https://www.tinytronics.nl/en/communication-and-signals/io-converters/pcf8574-i2c-io-expander) | Good datasheet support and common examples/libraries [(Texas Instruments Incorporated, 2024a)](https://www.ti.com/lit/ds/symlink/pcf8574.pdf); [(Mischianti, z.d.)](https://github.com/xreef/PCF8574_library)                                      | Low to medium | Possible, but less scalable than MCP23017                                      | Fewer I/O pins than MCP23017         |
| CD74HC4067            | Multiplexer  | 16 selectable channels |                                                                                                                                                                          About €2.50 based on TinyTronics [(TinyTronics, z.d.-c)](https://www.tinytronics.nl/en/communication-and-signals/io-converters/hc4067-16-channel-analog-multiplexer) | Good datasheet support and Arduino library support [(Texas Instruments Incorporated, 2024b)](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf); [(Arduino, 2023)](https://docs.arduino.cc/libraries/cd74hc4067/)                                  | Medium        | Strong for selecting one echo signal at a time                                 | Needs extra select logic             |
| CD74HC4051 / 74HC4051 | Multiplexer  |  8 selectable channels | About €0.98 for DIP chip based on Budgetronics, or higher as breakout module depending on supplier [(Budgetronics, z.d.)](https://www.budgetronics.eu/nl/component-assortimenten/74hc4051-single-8-channel-multi-demultiplexer/a-17943-25); [(OpenCircuit, z.d.-a)](https://opencircuit.shop/product/sparkfun-multiplexer-breakout-8-channel) | Good datasheet and breakout board documentation [(Texas Instruments Incorporated, z.d.)](https://www.ti.com/product/CD74HC4051); [(OpenCircuit, z.d.-a)](https://opencircuit.shop/product/sparkfun-multiplexer-breakout-8-channel)                 | Low to medium | Suitable for selecting one signal at a time, but less scalable than CD74HC4067 | Only 8 selectable channels           |

The estimated prices are based on supplier information from TinyTronics, Budgetronics and OpenCircuit. TinyTronics lists the MCP23017 I2C port expander for about €4.00 and describes it as a chip that gives access to 16 extra inputs or outputs through I2C communication [(TinyTronics, z.d.-a)](https://www.tinytronics.nl/en/components/ics-and-microcontroller-chips/ics/mcp23017-i2c-port-expander-16-inputs-outputs-28-pin-dip). TinyTronics lists the PCF8574 I2C IO Expander as a module with 8 extra pins [(TinyTronics, z.d.-b)](https://www.tinytronics.nl/en/communication-and-signals/io-converters/pcf8574-i2c-io-expander). TinyTronics also lists the HC4067 as a 16-channel analog multiplexer module [(TinyTronics, z.d.-c)](https://www.tinytronics.nl/en/communication-and-signals/io-converters/hc4067-16-channel-analog-multiplexer). Budgetronics lists the 74HC4051 DIP chip for about €0.98, while OpenCircuit lists a SparkFun 74HC4051 breakout module as an 8-channel multiplexer/demultiplexer [(Budgetronics, z.d.)](https://www.budgetronics.eu/nl/component-assortimenten/74hc4051-single-8-channel-multi-demultiplexer/a-17943-25); [(OpenCircuit, z.d.-a)](https://opencircuit.shop/product/sparkfun-multiplexer-breakout-8-channel).

The comparison shows that the MCP23017 gives the best balance for the current prototype. It provides 16 extra digital I/O pins, has clear datasheet support, has practical Arduino library support and supports general digital input/output expansion. The PCF8574 is cheaper and simpler, but it only provides 8 I/O pins, which makes it less suitable for future expansion.

The CD74HC4067 is the strongest multiplexer option because it provides 16 selectable channels. This gives it more room for future sensor expansion than the CD74HC4051, which provides 8 selectable channels. The CD74HC4051 is still a useful smaller multiplexer option, but it is less scalable than the CD74HC4067.

The key trade-off is that multiplexers are technically strong for selecting one ultrasonic echo signal at a time, while I/O expanders are stronger for general digital I/O expansion. Because the central decision problem is broader than only echo signal selection, the MCP23017 remains the best overall component for this advice.

---

### 4.5 Selection of the best solution

The advised component is the **MCP23017 I/O expander**.

The MCP23017 is the best fitting option because it provides 16 digital I/O pins through I2C, which directly supports the goal of reducing ESP32-S3 pin usage [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf). It also gives more expansion capacity than the PCF8574, which only provides 8 I/O pins [(Texas Instruments Incorporated, 2024a)](https://www.ti.com/lit/ds/symlink/pcf8574.pdf).

The estimated price is also acceptable for a prototype. The MCP23017 is more expensive than the PCF8574 and the 74HC4051 chip, but the difference is small compared with the extra expansion capacity and general I/O flexibility. For this project, the extra I/O capacity and scalability are more important than choosing the cheapest component.

TinyTronics lists the MCP23017 as directly available and describes it as a way to create more inputs or outputs through I2C communication [(TinyTronics, z.d.-a)](https://www.tinytronics.nl/en/components/ics-and-microcontroller-chips/ics/mcp23017-i2c-port-expander-16-inputs-outputs-28-pin-dip). This supports the practical choice for the prototype, because the component is not only technically suitable, but also available and understandable for prototype use.

The MCP23017 is also easier to justify as a general expansion solution because it can be used for different digital input and output signals. This fits the Smart City project, where more components may be added later.

The MCP23017 also has practical library support for Arduino-style development. This lowers the implementation risk because the project does not need to build the full communication layer from zero [(Adafruit, z.d.)](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library).

The main condition is that ultrasonic echo timing must be tested carefully. The HC-SR04 uses the echo pulse to calculate distance, so the system must still be validated after the MCP23017 is added [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

---

### 4.6 Second choice

The second-best component is the **CD74HC4067 multiplexer**.

The CD74HC4067 is a strong alternative because it can select one signal from 16 channels. This fits the current smart parking logic because the ultrasonic sensors are already measured one by one [(Texas Instruments Incorporated, 2024b)](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf). TinyTronics also describes the HC4067 module as using four address pins to select which of the 16 pins is connected to the signal pin [(TinyTronics, z.d.-c)](https://www.tinytronics.nl/en/communication-and-signals/io-converters/hc4067-16-channel-analog-multiplexer).

The CD74HC4067 is stronger than the CD74HC4051 as a multiplexer option because it provides 16 selectable channels instead of 8. The CD74HC4051 is still useful for smaller setups, but it gives less expansion room. Since the smart parking system may grow beyond the current four sensors, the CD74HC4067 is the stronger multiplexer choice.

For ultrasonic echo timing, the CD74HC4067 has a technical advantage because it can route one selected sensor signal to the ESP32-S3, allowing the ESP32-S3 to measure that selected signal more directly. This is why it is the second-best option and not a weak alternative.

However, the CD74HC4067 is less suitable as a general I/O expansion solution because it mainly selects signals instead of adding independent input/output pins. It also needs extra software logic to select the correct channel before taking a measurement.

For this advice, the MCP23017 remains the better first choice because the goal is scalable digital I/O expansion while keeping the current prototype logic working. The CD74HC4067 becomes more attractive if the main priority becomes direct echo signal selection.

---

### 4.7 Subconclusion

Based on the comparison, the MCP23017 is the best fitting component for reducing ESP32-S3 pin usage in the current smart parking prototype. It provides 16 extra digital I/O pins, has useful documentation and library support, supports scalable expansion and fits the goal of keeping the current parking logic mostly intact.

The CD74HC4067 is the second-best option because it fits sequential signal selection well and can support more direct echo signal reading. It is stronger than the CD74HC4051 because it provides 16 selectable channels instead of 8. However, it is less general than the MCP23017 and requires extra channel selection logic.

---

## 5. Chapter 2: Advice for a scalable technical setup

### 5.1 Introduction

This chapter answers the following subquestion:

**How should the advised component be used in a scalable smart parking setup?**

The goal is to explain how the MCP23017 fits into the current prototype and how it supports future expansion.

---

### 5.2 Recommended system setup

The advised setup is:

- one ESP32-S3 as the main controller;
- four ultrasonic sensors for the current parking spaces;
- one MCP23017 I/O expander to reduce direct ESP32-S3 pin usage;
- I2C communication between the ESP32-S3 and the MCP23017;
- sequential sensor reading to keep the measurement flow controlled;
- threshold logic with hysteresis to determine free or occupied status;
- clear output through the OLED display or later through a dashboard/backend.

The ESP32-S3 controls the system and processes the parking logic. The ultrasonic sensors detect the distance to the parking spaces. The MCP23017 adds extra digital I/O capacity, which makes it easier to expand the system later.

This setup follows the direction from the analysis document. The analysis showed that an I/O expander is useful for general digital expansion and that the multiplexer is mainly useful for selecting one signal at a time. Because the goal of this advice is broader scalable I/O expansion, the MCP23017 is advised as the main solution.

---

### 5.3 Why the MCP23017 is advised

The MCP23017 is advised because it directly supports the goal of reducing ESP32-S3 pin usage. It provides 16-bit general purpose I/O expansion through I2C [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf). In simple words, this means the ESP32-S3 can use the MCP23017 to access extra digital pins without connecting every signal directly to the ESP32-S3.

This is useful because a smart parking system may grow beyond the current four parking spaces. If every signal is connected directly to the ESP32-S3, the system can eventually run into GPIO limitations. By using the MCP23017, the design becomes easier to expand.

The MCP23017 also fits a scalable structure because I2C uses two main communication lines: SDA and SCL [(NXP Semiconductors, 2021)](https://www.nxp.com/docs/en/user-guide/UM10204.pdf). This means the ESP32-S3 can communicate with the MCP23017 through a bus instead of using many separate direct pins.

The MCP23017 is therefore suitable for the current Smart City setup, where multiple prototypes need to share the same controller and where direct GPIO pins should be used carefully.

---

### 5.4 Keeping the current parking logic working

A key reason for advising the MCP23017 is that the current parking logic can mostly stay the same.

The smart parking prototype already works with sequential measurement logic. This means the sensors are measured one by one. The MCP23017 does not require the full parking concept to be redesigned. The main change is that pin reading and writing are handled through the MCP23017 instead of only through direct ESP32-S3 GPIO pins.

This fits the learning goal because the aim is not to rebuild the full smart parking system. The aim is to reduce ESP32-S3 pin usage while keeping the current parking prototype logic working.

However, the echo signal from an ultrasonic sensor remains an important technical point. The HC-SR04 uses the echo signal to calculate distance, and the echo pulse timing is part of the measurement [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf). Because of this, the MCP23017 solution must be tested carefully to make sure the measured distances remain stable enough for the prototype.

This also connects to the analysis conclusion. The analysis showed that reducing pin usage is possible, but that ultrasonic echo timing must remain reliable. Therefore, the MCP23017 should be implemented in a way that preserves the current sequential parking logic and validates the measured results during testing.

---

### 5.5 Subconclusion

Based on this chapter, the advised technical setup is to use the ESP32-S3 with the current ultrasonic sensors and an MCP23017 I/O expander.

The MCP23017 is suitable because it reduces direct ESP32-S3 pin usage, supports future expansion and allows the current parking logic to remain mostly the same. The CD74HC4067 multiplexer remains the second-best option if direct signal selection becomes more important than general I/O expansion.

---

## 6. Chapter 3: Advice, risks and implementation direction

### 6.1 Introduction

This chapter answers the following subquestion:

**What risks must be considered when using an I/O expander with ultrasonic sensors?**

Every technical choice has risks. The goal is not to avoid all risk. The goal is to choose a realistic solution and manage the risks properly.

---

### 6.2 Advice based on requirements

The advised solution should meet the following functional requirements:

- reduce ESP32-S3 pin usage for the ultrasonic sensor setup;
- keep the current smart parking prototype logic working;
- detect whether each parking space is free or occupied;
- keep the free parking count stable;
- support future expansion to more parking spaces or other Smart City components.

The advised solution should also meet the following non-functional requirements:

- simple enough to build in the current project phase;
- understandable for stakeholders without embedded systems experience;
- stable enough for a demonstration or pilot;
- affordable enough for a prototype;
- supported by documentation and examples;
- scalable for future growth;
- maintainable by the project team.

The advised solution meets these requirements because the MCP23017 provides extra digital I/O pins, reduces the number of direct ESP32-S3 pins needed and supports a cleaner expansion structure.

---

### 6.3 Risks and recommendations

#### Risk 1: Ultrasonic echo timing

The main risk is the ultrasonic echo signal. The HC-SR04 calculates distance based on the time between the trigger signal and the returned echo signal. This means the echo signal is timing-sensitive [(Tech Support, z.d.)](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf).

When the echo signal is read directly by the ESP32-S3, the controller can measure the signal timing more directly. When the echo signal is read through an external component, such as an I/O expander, there is an extra communication step. This does not automatically make the solution unusable, but it does mean that the measurements must be checked carefully.

**Recommendation:**  
Test the ultrasonic echo measurements carefully after adding the MCP23017. The system should only be accepted if the parking status remains stable enough for the prototype.

---

#### Risk 2: I2C communication adds an extra step

The MCP23017 communicates through I2C. I2C uses SDA and SCL lines for communication between components [(NXP Semiconductors, 2021)](https://www.nxp.com/docs/en/user-guide/UM10204.pdf).

This is useful because it reduces the number of ESP32-S3 pins needed. However, reading or writing a pin through the MCP23017 is not the same as reading or writing a direct ESP32-S3 GPIO pin. For normal digital signals, this is usually acceptable. For timing-sensitive signals, such as ultrasonic echo signals, it needs extra attention.

**Recommendation:**  
Keep the I2C setup stable and avoid unnecessary delays during sensor measurements. If echo measurements become unstable, review whether the echo signal should be handled more directly by the ESP32-S3 or through a multiplexer.

---

#### Risk 3: MCP23017 is not ideal for every signal

The MCP23017 is useful for expanding digital I/O through I2C [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf). However, not every signal has the same timing requirement. Ultrasonic echo measurement depends on pulse timing, while other signals may only need normal digital input or output.

**Recommendation:**  
Use the MCP23017 mainly for scalable digital expansion. Do not use it blindly for every signal without checking whether the signal is timing-sensitive.

---

#### Risk 4: Extra hardware adds complexity

Adding the MCP23017 makes the system more scalable, but it also adds extra wiring, I2C addressing and software logic.

**Recommendation:**  
Document the I2C address, connected signals and pin allocation clearly in the design document. This makes the setup easier to understand, test and maintain.

---

#### Risk 5: Alternative solution may be better for direct echo timing

The CD74HC4067 multiplexer can select one signal from multiple channels [(Texas Instruments Incorporated, 2024b)](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf). This can be useful if the project later needs the ESP32-S3 to measure one selected echo signal more directly.

**Recommendation:**  
Use the CD74HC4067 as the second choice if testing shows that the MCP23017 is not stable enough for ultrasonic echo measurement.

---

### 6.4 Subconclusion

The advised solution is technically suitable, but only if the risks are managed correctly. The MCP23017 should reduce ESP32-S3 pin usage and support future expansion, but ultrasonic echo timing must be tested carefully.

The best implementation direction is therefore to use the MCP23017 as the main I/O expansion solution, while keeping the CD74HC4067 multiplexer as the second option if direct signal selection becomes more important.

---

## 7. Final conclusion

This advice document examined which component should be advised to reduce the ESP32-S3 pin usage of the ultrasonic sensors while keeping the current smart parking prototype logic working.

The first conclusion is that the advice focuses on selecting the most suitable component for reducing ESP32-S3 pin usage. The analysis already showed that several solution types are technically possible. The component comparison shows which option fits the current prototype best.

The second conclusion is that the MCP23017 is the best fitting component for the current prototype. It provides 16 digital I/O pins through I2C and supports a scalable hardware structure [(Microchip Technology Inc., 2005)](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf). It is more suitable than the PCF8574 because it provides more I/O pins.

The third conclusion is that the CD74HC4067 multiplexer is the second-best option. It can select one signal from 16 channels, which fits the current sequential measurement logic [(Texas Instruments Incorporated, 2024b)](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf). It is stronger than the CD74HC4051 because it provides more selectable channels and therefore gives more room for future expansion.

The final advice is:

**Use the MCP23017 I/O expander to reduce ESP32-S3 pin usage for the ultrasonic sensors, while keeping the current smart parking prototype logic working.**

This solution is the best fit for the current Smart City project because it balances pin reduction, scalability, documentation support, acceptable prototype cost and the need to preserve the current parking logic. The main condition is that ultrasonic echo timing must be tested carefully after implementation.

---

## 8. Recommendations

Based on this advice document, the following recommendations are made for Mayor Mats Otten and the project stakeholders:

1. Use the MCP23017 as the selected I/O expansion component for the smart parking prototype.

2. Use the MCP23017 to reduce the number of ultrasonic sensor signals connected directly to the ESP32-S3.

3. Keep the current parking logic as much as possible, especially the sequential sensor measurement structure.

4. Test the ultrasonic echo measurements carefully after adding the MCP23017.

5. Validate the occupied/free status for each parking space after implementation.

6. Check whether the free parking count remains stable during testing.

7. Keep the wiring, I2C address and pin allocation clearly documented in the design document.

8. Mention the echo timing limitation clearly, so the solution is technically honest and defendable.

9. Keep the setup scalable so that future parking spaces or other Smart City components can be added more easily.

10. Use the CD74HC4067 multiplexer as the second choice if direct signal selection becomes more important than general I/O expansion.

11. Do not treat the PCF8574 as the second-best main solution, because it is mainly a smaller I/O expander alternative with fewer pins than the MCP23017.

12. Do not treat the CD74HC4051 as stronger than the CD74HC4067, because it has fewer selectable channels and is less scalable for future sensor expansion.

13. Do not use the MCP23017 blindly for every signal without checking whether the signal is timing-sensitive.

---

## 9. Previous work

This advice document is based on the earlier analysis document:

- [Analysis: Reducing ESP32-S3 pin usage for scalable smart parking sensors](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/f1468907568a7099e80ed96108f3eeabdb348dc0/docs/Gurpreet/Learning%20goals/Analysis/Sprint%204/analysis%20Reducing%20ESP32-S3%20pin%20usage%20for%20scalable%20smart%20parking%20sensors.md)

The analysis document forms the technical basis for the component choice in this advice. It explains the current ESP32-S3 pin usage problem, compares possible solution types and identifies the ultrasonic echo timing limitation.

---

## 10. References

1. Adafruit. (z.d.). Adafruit MCP23017 Arduino Library. [https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library) viewed on 22 May 2026

2. Arduino. (2023, 26 April). CD74HC4067. [https://docs.arduino.cc/libraries/cd74hc4067/](https://docs.arduino.cc/libraries/cd74hc4067/) viewed on 22 May 2026

3. Budgetronics. (z.d.). 74HC4051 Single 8-channel Multi/Demultiplexer. [https://www.budgetronics.eu/nl/component-assortimenten/74hc4051-single-8-channel-multi-demultiplexer/a-17943-25](https://www.budgetronics.eu/nl/component-assortimenten/74hc4051-single-8-channel-multi-demultiplexer/a-17943-25) viewed on 22 May 2026

4. Espressif Systems. (z.d.-b). ESP32-S3 Series datasheet. [https://documentation.espressif.com/esp32-s3_datasheet_en.pdf](https://documentation.espressif.com/esp32-s3_datasheet_en.pdf) viewed on 11 May 2026

5. Fahim, A., Hasan, M., & Chowdhury, M. A. (2021). Smart parking systems: comprehensive review based on various aspects. Heliyon, 7(5), e07050. [https://doi.org/10.1016/j.heliyon.2021.e07050](https://doi.org/10.1016/j.heliyon.2021.e07050) viewed on 11 May 2026

6. Microchip Technology Inc. (2005). MCP23017/MCP23S17 (pp. 1–6). [https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf](https://ww1.microchip.com/downloads/en/devicedoc/20001952c.pdf) viewed on 11 May 2026

7. Mischianti, R. (z.d.). PCF8574 library. [https://github.com/xreef/PCF8574_library](https://github.com/xreef/PCF8574_library) viewed on 22 May 2026

8. NXP Semiconductors. (2021). I²C-bus specification and user manual (UM10204 Rev. 7.0). [https://www.nxp.com/docs/en/user-guide/UM10204.pdf](https://www.nxp.com/docs/en/user-guide/UM10204.pdf) viewed on 14 May 2026

9. OpenCircuit. (z.d.-a). SparkFun Multiplexer Breakout - 8 Channel (74HC4051). [https://opencircuit.shop/product/sparkfun-multiplexer-breakout-8-channel](https://opencircuit.shop/product/sparkfun-multiplexer-breakout-8-channel) viewed on 22 May 2026

10. Tech Support. (z.d.). Ultrasonic Ranging Module HC - SR04. [https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) viewed on 17 April 2026

11. Texas Instruments Incorporated. (2024a). PCF8574 Remote 8-Bit I/O Expander for I2C Bus [Technical manual]. [https://www.ti.com/lit/ds/symlink/pcf8574.pdf](https://www.ti.com/lit/ds/symlink/pcf8574.pdf) viewed on 14 May 2026

12. Texas Instruments Incorporated. (2024b). CD74HCx4067 High-Speed CMOS Logic 16-Channel Analog Multiplexer and Demultiplexer [Technical manual]. In CD74HC4067 CD74HCT4067 (SCHS209D editie, pp. 1–16). [https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf](https://www.ti.com/lit/ds/symlink/cd74hct4067.pdf) viewed on 14 May 2026

13. Texas Instruments Incorporated. (z.d.). CD74HC4051 data sheet, product information and support. [https://www.ti.com/product/CD74HC4051](https://www.ti.com/product/CD74HC4051) viewed on 22 May 2026

14. TinyTronics. (z.d.-a). MCP23017 I2C Port Expander 16 inputs-outputs - 28-pin DIP. [https://www.tinytronics.nl/en/components/ics-and-microcontroller-chips/ics/mcp23017-i2c-port-expander-16-inputs-outputs-28-pin-dip](https://www.tinytronics.nl/en/components/ics-and-microcontroller-chips/ics/mcp23017-i2c-port-expander-16-inputs-outputs-28-pin-dip) viewed on 22 May 2026

15. TinyTronics. (z.d.-b). PCF8574 I2C IO Expander. [https://www.tinytronics.nl/en/communication-and-signals/io-converters/pcf8574-i2c-io-expander](https://www.tinytronics.nl/en/communication-and-signals/io-converters/pcf8574-i2c-io-expander) viewed on 22 May 2026

16. TinyTronics. (z.d.-c). HC4067 16-channel Analog Multiplexer. [https://www.tinytronics.nl/en/communication-and-signals/io-converters/hc4067-16-channel-analog-multiplexer](https://www.tinytronics.nl/en/communication-and-signals/io-converters/hc4067-16-channel-analog-multiplexer) viewed on 22 May 2026