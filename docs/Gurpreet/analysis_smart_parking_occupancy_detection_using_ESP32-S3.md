# Analysis: Smart parking occupancy detection using ESP32-S3 (4 parking spaces)

- **Author:** Gurpreet Singh  
- **Date:** 16-04-2026  
- **Version:** 1.0  
- **Classification:** Internal  
- **Client:** Mayor Mats Otten  
- **Company:** The Embedded Alliance  

## Table of Contents
- [Table of Contents](#table-of-contents)
- [1. Introduction](#1-introduction)
- [2. Main research question and subquestions](#2-main-research-question-and-subquestions)
- [3. Methodology](#3-methodology)
- [4. Chapter 1: Suitable sensor for parking occupancy detection](#4-chapter-1-suitable-sensor-for-parking-occupancy-detection)
    - [4.1 Introduction](#41-introduction)
    - [4.2 Possible sensor options for this prototype](#42-possible-sensor-options-for-this-prototype)
    - [4.3 Why the HC-SR04 is the best fit](#43-why-the-hc-sr04-is-the-best-fit)
    - [4.4 Subconclusion](#44-subconclusion)
- [5. Chapter 2: Measuring 4 parking spaces with one ESP32-S3](#5-chapter-2-measuring-4-parking-spaces-with-one-esp32-s3)
    - [5.1 Introduction](#51-introduction)
    - [5.2 Multi-sensor setup on one controller](#52-multi-sensor-setup-on-one-controller)
    - [5.3 Reading order and measurement stability](#53-reading-order-and-measurement-stability)
    - [5.4 Subconclusion](#54-subconclusion)
- [6. Chapter 3: Requirements, risks and best direction](#6-chapter-3-requirements-risks-and-best-direction)
    - [6.1 Introduction](#61-introduction)
    - [6.2 Functional and non-functional requirements](#62-functional-and-non-functional-requirements)
    - [6.3 Risks and recommended implementation direction](#63-risks-and-recommended-implementation-direction)
    - [6.4 Subconclusion](#64-subconclusion)
- [7. Final conclusion](#7-final-conclusion)
- [8. Recommendations](#8-recommendations)
- [9. References](#9-references)



## 1. Introduction

This document was written for The Embedded Alliance and Mayor Mats Otten.  
The context of this document is the current stage of the Smart City project, where a smart parking prototype must be developed for four parking spaces.

The purpose of this document is to investigate how parking occupancy can be detected in a simple and reliable way with one ESP32-S3. This is relevant because a smart parking system can reduce search time, improve traffic flow in the city model, and make parking status visible in real time (Tips, 2026).

The goal is to build a working and clear prototype that fits the current project stage.

This document is written for the internal project team and the project client. It assumes that the reader has basic prior knowledge of ESP32 boards, sensors, and embedded prototypes.

## 2. Main research question and subquestions

The main research question of this document is:

**How can four parking spaces be reliably monitored with one ESP32-S3 using a suitable sensor setup?**

To answer this main research question, the following subquestions were formulated:

1. Which sensor is most suitable for detecting parking occupancy in this prototype?
2. How can four parking spaces be measured with one ESP32-S3?
3. How can the measurements be kept stable and reliable?
4. Which functional and non-functional requirements should the prototype meet?
5. What is the best implementation direction at this stage of the project?

## 3. Methodology

This document was created using the following methods:

- literature research on smart parking and ultrasonic distance measurement;
- analysis of the HC-SR04 datasheet and its technical limits;
- review of practical Arduino examples with one and multiple ultrasonic sensors;
- comparison of possible sensor options for this prototype;
- translation of the findings into a realistic implementation direction.

These methods were chosen because this project needs both technical understanding and practical decisions. The goal of the analysis is not only to describe how a sensor works, but to decide which solution fits the project best and how it should be implemented.

---

## 4. Chapter 1: Suitable sensor for parking occupancy detection

### 4.1 Introduction

This chapter answers the following subquestion:

**Which sensor is most suitable for detecting parking occupancy in this prototype?**

To answer this, the chapter discusses possible sensor options and explains why one option fits this prototype better than the others.

### 4.2 Possible sensor options for this prototype

A parking occupancy prototype can be built with different types of sensors. Examples are ultrasonic sensors, infrared sensors, pressure sensors, and camera-based systems.

In theory, all of these can detect whether a parking space is free or occupied.

For this prototype, not every option is equally practical. Pressure sensors would require a different construction in the parking surface. Camera-based systems add more complexity than needed for a first working version. Infrared sensors can work, but they are more sensitive to surface color, lighting conditions, and alignment.

Ultrasonic sensors fit this use case better because they measure distance directly. That makes them useful when the goal is simple: check whether an object is present above a parking space.

### 4.3 Why the HC-SR04 is the best fit

The HC-SR04 is the most suitable sensor for this prototype. The datasheet states that the module supports non-contact measurement from 2 cm to 400 cm, with an accuracy up to 3 mm (Tech Support, z.d.). It works with a 10 microsecond trigger pulse and uses the echo signal duration to calculate distance (Tech Support, z.d.). The sensor also has a measuring angle of 15 degrees (Tech Support, z.d.). The datasheet also states that the module works on 5V and has a working frequency of 40Hz (Tech Support, z.d.).

These values are good enough for a model parking setup. The range is more than enough, the accuracy is sufficient, and the hardware connection is simple.

The module only needs 5V power, ground, trigger pulse input, echo pulse output (Tech Support, z.d.).

A practical parking sensor example also shows that the HC-SR04 can be used in a parking context and that measured distance can be translated into status logic (Parking Sensor, z.d.).

### 4.4 Subconclusion

Based on this chapter, it can be concluded that the HC-SR04 ultrasonic sensor is the most suitable sensor for this prototype because it is simple, low cost, accurate enough, and directly supports distance-based parking occupancy detection.

---

## 5. Chapter 2: Measuring 4 parking spaces with one ESP32-S3

### 5.1 Introduction

This chapter answers the following subquestion:

**How can four parking spaces be measured with one ESP32-S3?**

To answer this, the chapter discusses how multiple sensors can be connected to one controller and how the readings should be handled.

### 5.2 Multi-sensor setup on one controller

One ESP32-S3 can measure four parking spaces by using four ultrasonic sensors.

A practical way to do this is one shared trigger pin, four separate echo pins, shared power and ground (Handling Multiple Ultrasonic Sensors With Arduino!, z.d.).

This reduces the number of required pins and keeps the wiring cleaner (Handling Multiple Ultrasonic Sensors With Arduino!, z.d.).

The ESP32-S3 has enough available pins for this setup. Using one shared trigger also makes the hardware layout easier to manage.

This fits the project because a clean setup is easier to explain, easier to test, and easier to expand later.

### 5.3 Reading order and measurement stability

In the practical example, the sensors are read one by one with short delays between measurements (Handling Multiple Ultrasonic Sensors With Arduino!, z.d.). The HC-SR04 datasheet also recommends using a measurement cycle above 60 ms, which supports leaving enough time between measurements (Tech Support, z.d.). If several sensors send sound waves at once, their signals can interfere with each other.

The best reading method is sequential, trigger sensor 1 and read the result, short wait, trigger sensor 2 and read the result, repeat for sensors 3 and 4 (Handling Multiple Ultrasonic Sensors With Arduino!, z.d.).

After that, each measured distance can be translated into parking status. A practical parking sensor example also shows that measured distance can be used to trigger a status response when an object gets closer to the sensor (Parking Sensor, z.d.).

Example:

- short distance = occupied  
- larger distance = free

To improve stability, two threshold values should be used. One threshold changes to occupied, and another threshold changes back to free. This prevents constant switching when values move slightly.

### 5.4 Subconclusion

Based on this chapter, it can be concluded that four parking spaces can be measured with one ESP32-S3 by using four HC-SR04 sensors, a shared trigger pin, separate echo pins, and sequential measurement logic.

---

## 6. Chapter 3: Requirements, risks and best direction

### 6.1 Introduction

This chapter answers the following subquestion:

**What is the best implementation direction at this stage of the project?**

To answer this, the chapter discusses requirements, risks, and the most practical direction for the build phase.

### 6.2 Functional and non-functional requirements

The prototype should meet the following functional requirements:

1. detect whether each of the four parking spaces is free or occupied
2. update parking status automatically
3. show the result through OLED
4. count how many spaces are free

The prototype should also meet the following non-functional requirements:

1. stable enough for live demonstration
2. simple enough to build at this stage
3. clear code library structure for the team
4. quick enough response time

These requirements matter because the goal is not only to prove that one sensor works, but to build a useful team prototype that can be expanded later.

### 6.3 Risks and recommended implementation direction

The main risks are:

- measurement errors caused by unsuitable object surfaces (Tech Support, z.d.)
- unreliable results when the measured surface is not smooth enough (Tech Support, z.d.)
- When multiple ultrasonic sensors are used, they should be measured one by one to reduce interference and improve reliability (Handling Multiple Ultrasonic Sensors With Arduino!, z.d.)
- blocking code (Ed, 2021)

If sensor angles are poor or reflections are inconsistent, results may become unreliable (Tech Support, z.d.).

Another risk is software structure. Simple blocking delays may work at first, but they become weaker when more features are added. A timer-based non-blocking approach is stronger (Ed, 2021).

The best direction at this stage of the project is:

- use HC-SR04 sensors for all four spaces (Handling Multiple Ultrasonic Sensors With Arduino!, z.d.)
- use one shared trigger and separate echo pins (Handling Multiple Ultrasonic Sensors With Arduino!, z.d.)
- measure sensors one by one (Handling Multiple Ultrasonic Sensors With Arduino!, z.d.)
- use stable threshold logic
- keep output clear
- keep code modular for future integration

### 6.4 Subconclusion

Based on this chapter, it can be concluded that the best direction is a four-sensor parking prototype with simple hardware, sequential measurements, stable logic, and clear output.

---

## 7. Final conclusion

This document examined how four parking spaces can be reliably monitored with one ESP32-S3 using a suitable sensor setup.

First, the HC-SR04 was identified as the most suitable sensor because it is simple and accurate enough for this prototype.  
Second, four parking spaces can be measured with one ESP32-S3 by using multiple sensors with shared trigger logic.  
Third, reliable operation depends on sequential readings, stable thresholds, and good sensor placement.  
Fourth, the best direction is a practical prototype focused on stable parking detection before adding extra features.

Based on the full analysis, it can be concluded that **the best solution is to build a four-space smart parking prototype with HC-SR04 ultrasonic sensors on one ESP32-S3, using sequential distance measurements and stable status logic**.

## 8. Recommendations

Based on the results of this document, the following recommendations are made:

1. Test one parking space first before scaling to four.
2. Use one shared trigger pin and separate echo pins.
3. Read sensors one by one.
4. Use clear status output first, then expand later.
5. Test sensor placement at different heights and angles.

## 9. References

1. Tech Support. (z.d.). Ultrasonic Ranging Module HC - SR04. [https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) viewed on 17 April 2026
2. Handling multiple ultrasonic sensors with Arduino! (z.d.). Arduino Project Hub. [https://projecthub.arduino.cc/bruno_opaiva/handling-multiple-ultrasonic-sensors-with-arduino-0afc4b](https://projecthub.arduino.cc/bruno_opaiva/handling-multiple-ultrasonic-sensors-with-arduino-0afc4b) viewed on 17 April 2026
3. Parking sensor. (z.d.). Arduino Project Hub. [https://projecthub.arduino.cc/jackaless/parking-sensor-4aa9d0](https://projecthub.arduino.cc/jackaless/parking-sensor-4aa9d0) viewed on 17 April 2026
4. Tips, L. (2026, 5 januari). Arduino Smart Car Parking System explained. Lunar Tips. [https://ftp.bills.com.au/lunar-tips/arduino-smart-car-parking-system-explained-1767647834#troubleshooting-common-issues](https://ftp.bills.com.au/lunar-tips/arduino-smart-car-parking-system-explained-1767647834#troubleshooting-common-issues) viewed on 17 April 2026
5. Ed. (2021, 28 september). Arduino PulseIn() with interrupts. The Robotics Back-End. [https://roboticsbackend.com/arduino-pulsein-with-interrupts/](https://roboticsbackend.com/arduino-pulsein-with-interrupts/) viewed on 21 April 2026