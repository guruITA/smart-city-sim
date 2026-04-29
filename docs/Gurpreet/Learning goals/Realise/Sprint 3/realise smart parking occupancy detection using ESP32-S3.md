# Realise: Smart parking occupancy detection using ESP32-S3 (4 Parking Spaces)

- **Author:** Gurpreet Singh  
- **Date:** 29-04-2026  
- **Version:** 1.0  
- **Classification:** Internal  
- **Client:** Mayor Mats Otten  
- **Company:** The Embedded Alliance  

## Table of Contents

- [Table of Contents](#table-of-contents)
- [1. Introduction](#1-introduction)
- [2. Main question and subquestions](#2-main-question-and-subquestions)
- [3. Methodology](#3-methodology)
- [4. Chapter 1: Working implementation](#4-chapter-1-working-implementation)
    - [4.1 Introduction](#41-introduction)
    - [4.2 GitLab source code](#42-gitlab-source-code)
    - [4.3 Integration in the shared project](#43-integration-in-the-shared-project)
    - [4.4 Subconclusion](#44-subconclusion)
- [5. Chapter 2: Important technical choices](#5-chapter-2-important-technical-choices)
    - [5.1 Introduction](#51-introduction)
    - [5.2 Avoiding blocking `pulseIn()` logic](#52-avoiding-blocking-pulsein-logic)
    - [5.3 My non-blocking measurement structure](#53-my-non-blocking-measurement-structure)
    - [5.4 Interrupt-based echo handling](#54-interrupt-based-echo-handling)
    - [5.5 Static instance and custom interrupt function](#55-static-instance-and-custom-interrupt-function)
    - [5.6 IRAM_ATTR and short interrupt logic](#56-iram_attr-and-short-interrupt-logic)
    - [5.7 Sequential sensor reading](#57-sequential-sensor-reading)
    - [5.8 Subconclusion](#58-subconclusion)
- [6. Chapter 3: Parking status logic and output](#6-chapter-3-parking-status-logic-and-output)
    - [6.1 Introduction](#61-introduction)
    - [6.2 Occupied and free detection](#62-occupied-and-free-detection)
    - [6.3 OLED display output](#63-oled-display-output)
    - [6.4 Subconclusion](#64-subconclusion)
- [7. Final conclusion](#7-final-conclusion)
- [8. Recommendations](#8-recommendations)
- [9. References](#9-references)
- [10. Appendix](#10-appendix)

---

## 1. Introduction

This document is written for The Embedded Alliance and Mayor Mats Otten.  
The context of this document is the realisation phase of the smart parking prototype for four parking spaces.

The purpose of this document is to show that the parking system has been built and works in practice using one ESP32-S3, four ultrasonic sensors, and an OLED display.

This document is written for the internal project team and project client. It assumes that the reader has basic prior knowledge of embedded systems and prototype development.

---

## 2. Main question and subquestions

The main question of this document is:

**How was the smart parking prototype successfully realised into a working system?**

To answer this question, the following subquestions were formulated:

1. How was the system implemented in code?
2. Which technical choices were important during development?
3. How does the system detect occupied and free parking spaces?
4. How is the result shown to the user?
5. Why is this implementation suitable for future expansion?

---

## 3. Methodology

This document was created using the following methods:

- building and testing the smart parking prototype in hardware
- programming the ESP32-S3 in VS Code using Arduino CLI
- implementing the parking system inside the existing library-like project structure
- creating a separate `Parking.h` and `Parking.cpp` component so the code fits the shared project setup
- replacing blocking ultrasonic measurement logic with non-blocking logic
- using interrupts to read the echo signal without stopping the rest of the program
- validating the parking status on the OLED display during testing.

These methods were chosen because the parking system had to work inside the existing shared Smart City project. The code could not be written as one loose `.ino` file, because it had to fit together with the streetlight, speed camera, and train signal components. The ultrasonic measurement also had to be non-blocking, because blocking logic such as `pulseIn()` would stop the main loop while waiting for an echo result.

---

## 4. Chapter 1: Working implementation

### 4.1 Introduction

This chapter answers the following subquestion:

**How was the system implemented in code?**

To answer this, the chapter shows where the working code is stored and how the parking component is integrated into the shared Smart City project.

### 4.2 GitLab source code

The complete working implementation is stored in GitLab using fixed commit permalinks. This is important because the linked code version will not change when the branch is updated later.

The relevant files are:

- [Parking.h](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3bd744e54a93ced2f31e8c30764443104ea6423e/embedded/city-sim/lib/Parking/Parking.h)  
- [Parking.cpp](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3bd744e54a93ced2f31e8c30764443104ea6423e/embedded/city-sim/lib/Parking/Parking.cpp)  
- [Config.h](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3bd744e54a93ced2f31e8c30764443104ea6423e/embedded/city-sim/lib/Config/Config.h)  
- [city-sim.ino](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3bd744e54a93ced2f31e8c30764443104ea6423e/embedded/city-sim/city-sim.ino)  

`Parking.h` contains the class definition, method declarations, sensor state structure, and private variables.  
`Parking.cpp` contains the working logic for measuring distance, handling interrupts, updating occupied status, and drawing the OLED screen.  
`Config.h` contains the pin numbers, threshold values, timeout settings, and OLED settings for the parking system.  
`city-sim.ino` creates the `Parking` object, calls `parking.begin()` in `setup()`, and calls `parking.update()` in `loop()` together with the other Smart City components.

### 4.3 Integration in the shared project

The parking system is not written as one loose `.ino` file. It is implemented as a separate component inside the existing library-like project structure.

This means the parking functionality can run next to the other components in the same project, such as:

- streetlight.
- speed camera.
- train signal.

The main file only starts and updates the component. The detailed parking logic stays inside `Parking.h` and `Parking.cpp`. This keeps the shared project cleaner and makes the parking system easier to maintain or extend later.

### 4.4 Subconclusion

Based on this chapter, it can be concluded that the parking system was implemented as a working component inside the shared Smart City project structure. The code is stored with fixed GitLab permalinks and can be checked directly in the linked files.

---

## 5. Chapter 2: Important technical choices

### 5.1 Introduction

This chapter answers the following subquestion:

**Which technical choices were important during development?**

To answer this, this chapter explains the most important implementation choices in the parking system. The focus is on why `pulseIn()` was not used, how the echo signal is handled with interrupts, how the static instance structure was implemented, and how the four sensors are measured one by one.

### 5.2 Avoiding blocking `pulseIn()` logic

The first important technical choice was not using `pulseIn()` for the ultrasonic measurements.

`pulseIn()` can block the program while it waits for a pulse to start or end. The Robotics Back-End explains that with a timeout, `pulseIn()` can block the full program for a certain amount of time, depending on the signal and timeout value. It also explains that this can be avoided by using interrupts to store the start and end time of a pulse instead of waiting inside `pulseIn()` [(Ed, 2021)](https://roboticsbackend.com/arduino-pulsein-with-interrupts/#The_interrupt_function_%E2%80%93_detect_the_pulse).

This was important for my implementation because the parking system is not the only component in the project. The same `city-sim.ino` file also runs other components, such as the streetlight, speed camera, and train signal. If the parking code blocks while waiting for one ultrasonic echo, the other components can also be delayed. That would not fit the shared Smart City project.

Therefore, I used a non-blocking approach. The parking system does not wait inside one blocking function until the ultrasonic measurement is done. Instead, the measurement is checked step by step inside `parking.update()`.

### 5.3 My non-blocking measurement structure

The non-blocking structure in my code is built around the function:

`updateDistanceMeasurement(ParkingSpot& spot)`

This function does not finish the full measurement in one blocking call. Instead, it checks which step the current parking sensor is in and returns `false` when the measurement is still running. It returns `true` when the measurement is finished or when a timeout happens.

For this, I created my own measurement states:

- `IDLE`
- `WAITING_FOR_ECHO_START`
- `WAITING_FOR_ECHO_END`

These state names are my own implementation choice. They are not copied from the source. I used them to make the measurement flow clear in my own `Parking` component.

The flow works like this:

1. In `IDLE`, the code selects the active echo pin, resets the interrupt flags, attaches the interrupt, and sends the 10 microsecond trigger pulse.
2. In `WAITING_FOR_ECHO_START`, the code waits until the interrupt detects that the echo signal has gone HIGH.
3. In `WAITING_FOR_ECHO_END`, the code waits until the interrupt detects that the echo signal has gone LOW.
4. When the measurement is finished, the distance is calculated and the sensor state is reset to `IDLE`.

This approach lets the rest of the program continue running between measurement steps. That is the important part. The source gave me the idea of measuring an ultrasonic sensor without blocking the main loop, but the state structure itself was made for my own parking code [(Instructables, 2017)](https://www.instructables.com/Non-blocking-Ultrasonic-Sensor-for-Arduino/).

### 5.4 Interrupt-based echo handling

The echo signal is measured with interrupts.

[(AttachInterrupt() | Arduino Documentation, n.d.)](https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/) and [(DigitalPinToInterrupt() | Arduino Documentation, n.d.)](https://docs.arduino.cc/language-reference/en/functions/external-interrupts/digitalPinToInterrupt/) explains that `attachInterrupt()` can attach an interrupt service routine to a pin and that the recommended syntax is to use `digitalPinToInterrupt(pin)` as the first argument. It also explains that `CHANGE` can be used when the interrupt should react to both HIGH and LOW changes [(AttachInterrupt() | Arduino Documentation, n.d.)](https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/). The separate Arduino page for `digitalPinToInterrupt()` explains that this function checks whether a pin can be used as an interrupt pin and returns the interrupt value for that pin [(DigitalPinToInterrupt() | Arduino Documentation, n.d.)](https://docs.arduino.cc/language-reference/en/functions/external-interrupts/digitalPinToInterrupt/).

In my code, this is used here:

`attachInterrupt(digitalPinToInterrupt(_activeEchoPin), handleEchoChangeISR, CHANGE);`

This means the active echo pin is watched for both changes:

- LOW to HIGH = echo started
- HIGH to LOW = echo ended.

Inside `handleEchoChange()`, the code reads the active echo pin. When the pin becomes HIGH, it stores the start time with `micros()`. When the pin becomes LOW, it stores the end time and marks the measurement as finished.

This matches the interrupt idea from [(Ed, 2021)](https://roboticsbackend.com/arduino-pulsein-with-interrupts/#The_interrupt_function_%E2%80%93_detect_the_pulse), where the pulse start and pulse end are stored using an interrupt and the pulse duration is calculated later.

After the measurement is finished or a timeout happens, my code calls:

`detachInterrupt(digitalPinToInterrupt(_activeEchoPin));`

The Arduino documentation explains that `detachInterrupt()` turns off a previously attached interrupt [(DetachInterrupt() | Arduino Documentation, n.d.)](https://docs.arduino.cc/language-reference/en/functions/external-interrupts/detachInterrupt/). I use this so that only the active sensor is listened to during its measurement.

### 5.5 Static instance and custom interrupt function

A specific challenge was that the interrupt service routine cannot directly call a normal class method in the same way as regular code.

The [(Instructables, 2017)](https://www.instructables.com/Non-blocking-Ultrasonic-Sensor-for-Arduino/) example uses a static instance pointer in the `HC_SR04` class. In that example, the class stores a static `_instance`, and the interrupt function retrieves that instance to access the object data.

I used the same idea, but adapted it to my own `Parking` class.

In `Parking.h`, I declared:

`static Parking* _instance;`

In `Parking.cpp`, I start with no active object:

`Parking* Parking::_instance = NULL;`

In `begin()`, I connect the current object to the static pointer:

`_instance = this;`

Then the static interrupt function forwards the interrupt to the active object:

`_instance->handleEchoChange();`

This is important because the detailed logic still stays inside the `Parking` class. Without this static instance structure, the interrupt logic would have to be placed outside the class, which would make the code less clean and less reusable. Since the project already uses a library-like structure, keeping the parking logic inside `Parking.h` and `Parking.cpp` was the better choice.

### 5.6 IRAM_ATTR and short interrupt logic

Because this runs on an ESP32-S3, the interrupt functions use `IRAM_ATTR`.

The Last Minute Engineers source explains that ESP32 interrupt service routines should be short and fast, should avoid slow operations such as `delay()` and `Serial.print()`, and should mainly set flags or store values for the main program to process later. It also explains that `IRAM_ATTR` is used so the interrupt routine is stored in internal RAM instead of flash memory [(Prabhu, 2026)](https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/).

That is why my interrupt logic does not calculate the distance directly inside the interrupt. It only stores:

- `_echoStartUsInterrupt`
- `_echoEndUsInterrupt`
- `_echoRiseDetected`
- `_echoMeasurementDone`

The actual distance calculation happens later in `updateDistanceMeasurement()`. This keeps the interrupt short and makes the code safer for the ESP32-S3.

### 5.7 Sequential sensor reading

The parking system uses four ultrasonic sensors, but they are not measured at the same time.

In my code, `_currentSensorIndex` decides which parking sensor is currently active. The system measures one sensor, updates the occupied state when the measurement is finished, and then moves to the next sensor.

The flow is:

1. measure current sensor
2. wait until the measurement is finished or timed out
3. update occupied/free status
4. increase `_currentSensorIndex`
5. return to sensor 1 after sensor 4.

This fits the design choice from the previous document. It keeps the measurement flow controlled and reduces the chance that the echo signal from one ultrasonic sensor is mixed with another sensor.

### 5.8 Subconclusion

Based on this chapter, it can be concluded that the most important technical choices were avoiding `pulseIn()`, using interrupts for the echo signal, adapting the static instance structure from the non-blocking ultrasonic sensor example, using `IRAM_ATTR` for ESP32 interrupt safety, and measuring the sensors one by one. These choices made the parking system fit better inside the existing shared Smart City project structure.

---

## 6. Chapter 3: Parking status logic and output

### 6.1 Introduction

This chapter answers the following subquestion:

**How does the system detect occupied and free parking spaces?**

To answer this, the chapter discusses the threshold logic and the OLED output.

### 6.2 Occupied and free detection

The system uses distance thresholds to decide whether a parking space is occupied or free.

In the current configuration, the parking logic uses these values:

- `PARKED_THRESHOLD_ON_CM = 5.0`
- `PARKED_THRESHOLD_OFF_CM = 10.0`
- `INVALID_DISTANCE_CM = -1.0`

A parking space becomes occupied when the measured distance is below 5 cm. A parking space becomes free again when the measured distance is above 10 cm.

Two separate thresholds are used to prevent unstable switching. This means the status does not constantly switch between `FREE` and `OCCUPIED` when the measured value is close to one limit.

Invalid measurements are stored as `-1.0`. When a measurement is invalid, the occupied state is not changed. This prevents one failed sensor reading from immediately changing the parking status.

### 6.3 OLED display output

The OLED display shows:

- parking spot 1 to 4
- `OCCUPIED` or `FREE`
- `NO DATA` when measurement fails
- total number of free spaces.

This output is shown during the live demonstration.

### 6.4 Subconclusion

Based on this chapter, it can be concluded that the system correctly translates sensor measurements into clear parking status output.

---

## 7. Final conclusion

This document examined how the smart parking prototype was realised into a working system.

First, the parking system was successfully implemented inside the shared project structure.  
Second, the blocking `pulseIn()` approach was avoided because it would not fit well inside a shared project with multiple active components.  
Third, interrupt-based echo handling made it possible to store the echo start and end time without stopping the rest of the program.  
Fourth, the static instance structure made it possible to keep interrupt handling inside the `Parking` class structure.  
Fifth, parking status is clearly shown on the OLED display.

Based on the full implementation, it can be concluded that **the smart parking prototype was successfully realised and is ready for demonstration and future expansion.**

---

## 8. Recommendations

Based on this implementation, the following recommendations are made:

1. Test with real model vehicles for final calibration.
2. Fine-tune threshold values if needed.
3. Keep the non-blocking structure when adding new features.
5. Reuse the class structure for other Smart City systems.

---

## 9. References

1. Ed. (2021, September 28). Arduino PulseIn() with interrupts. The Robotics Back-End. [https://roboticsbackend.com/arduino-pulsein-with-interrupts/](https://roboticsbackend.com/arduino-pulsein-with-interrupts/) viewed on 21 April 2026.

2. Instructables. (2017a, oktober 10). Non-blocking Ultrasonic Sensor for Arduino. Instructables. [https://www.instructables.com/Non-blocking-Ultrasonic-Sensor-for-Arduino/](https://www.instructables.com/Non-blocking-Ultrasonic-Sensor-for-Arduino/) viewed on 17 April 2026.

3. AttachInterrupt() | Arduino Documentation. (n.d.). [https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/](https://docs.arduino.cc/language-reference/en/functions/external-interrupts/attachInterrupt/) viewed on 23 April 2026.

4. DigitalPinToInterrupt() | Arduino Documentation. (n.d.). [https://docs.arduino.cc/language-reference/en/functions/external-interrupts/digitalPinToInterrupt/](https://docs.arduino.cc/language-reference/en/functions/external-interrupts/digitalPinToInterrupt/) viewed on 23 April 2026.

5. DetachInterrupt() | Arduino Documentation. (n.d.). [https://docs.arduino.cc/language-reference/en/functions/external-interrupts/detachInterrupt/](https://docs.arduino.cc/language-reference/en/functions/external-interrupts/detachInterrupt/) viewed on 23 April 2026.

6. Prabhu, A. (2026, 20 januari). Configuring & handling ESP32 GPIO interrupts in Arduino IDE. Last Minute Engineers. [https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/](https://lastminuteengineers.com/handling-esp32-gpio-interrupts-tutorial/) viewed on 23 April 2026.