# Analysis Document

## Table of Contents

1. [Introduction](#1-introduction)  
2. [Problem Description](#2-problem-description)  
3. [Objective](#3-objective)  
4. [Hardware Analysis](#4-hardware-analysis)  
   4.1 [E-ink Display](#41-e-ink-display)  
   4.2 [Documentation Challenges](#42-documentation-challenges)  
5. [Functional Requirements](#5-functional-requirements)  
6. [Non-Functional Requirements](#6-non-functional-requirements)  
7. [System Design Considerations](#7-system-design-considerations)  
   7.1 [Backend Communication](#71-backend-communication)  
   7.2 [Data Handling](#72-data-handling)  
   7.3 [Display Strategy](#73-display-strategy)  
   7.4 [Reusability Approach](#74-reusability-approach)  
8. [Limitations of E-ink Displays](#8-limitations-of-e-ink-displays)  
9. [Risks and Challenges](#9-risks-and-challenges)  
10. [Success Criteria](#10-success-criteria)  
11. [Conclusion](#11-conclusion)  

---

## 1. Introduction

In this project, an ESP32 is used as part of a smart city simulation where physical tiles represent different cities. In previous sprints, the tiles were able to send information to a backend system, but there was no visual feedback on the tiles themselves.

The goal of this sprint is to improve interaction by adding an e-ink display to each tile. This display will show relevant information such as the city name and a logo. The content should not be fixed in the code but must be dynamically retrieved from the backend. This ensures flexibility and allows updates without reprogramming the device.

---

## 2. Problem Description

At the moment, the system only sends data to the backend without displaying anything on the physical tiles. This creates a disconnect between the digital system and the physical representation of the cities.

If a simple solution is used where the display content is hardcoded, it would not be suitable for a scalable system. Every change would require updating and reflashing the ESP32, which is inefficient and impractical when multiple tiles are involved. In addition, such a solution would not support dynamic data coming from the backend.

Because of this, a more flexible and reusable approach is needed, where the displayed information can be updated remotely.

---

## 3. Objective

The objective of this assignment is to design and implement a system in which an ESP32 with an e-ink display can show a city name and a logo, while retrieving this information from a backend API.

The solution should be reusable across multiple tiles without requiring changes in the code. It should also integrate well with the existing system and support reliable updates despite the limitations of the e-ink display.

---

## 4. Hardware Analysis

### 4.1 E-ink Display

The selected display is the MH-ET Live 2.9-inch e-paper display with a resolution of 296 by 128 pixels and support for three colors: black, white, and red. The display communicates via SPI and supports both 3-line and 4-line interfaces.

A key advantage of this display is its low power consumption, as it only uses power when refreshing the screen. However, it has a relatively slow refresh rate compared to other display types such as LCD or OLED.

### 4.2 Documentation Challenges

One of the main challenges is that this specific display is not well documented. However, it appears to be compatible with similar displays from Waveshare and Seengreat.

This means that existing libraries and documentation for those displays can likely be reused, although some experimentation may be required to ensure compatibility.

---

## 5. Functional Requirements

The system should be able to display the city name clearly on the screen and show a logo alongside it. The information must be retrieved from a backend API instead of being hardcoded.

The display should update whenever new data is available, and the same firmware should work on multiple ESP32 devices without requiring modifications. This ensures that the solution is scalable and easy to reuse.

---

## 6. Non-Functional Requirements

The solution must be reusable so that it can be applied to multiple tiles without code changes. It should also be scalable, allowing new tiles to be added easily.

Performance is important because the e-ink display has a slow refresh rate, so updates must be handled efficiently. The system should be reliable in maintaining a connection with the backend and should be structured in a way that makes it easy to maintain and extend.

---

## 7. System Design Considerations

### 7.1 Backend Communication

The ESP32 will communicate with the backend using an HTTP-based API. The backend will provide the necessary data, such as the city name and a reference to the logo image.

By retrieving this information dynamically, the system can adapt to changes without requiring updates to the device itself.

### 7.2 Data Handling

The city name can be displayed directly as text on the screen. The logo, however, must be converted into a format that is compatible with the e-ink display.

This may require downloading the image and processing it on the ESP32, or preparing the image in advance on the backend so that it can be displayed directly.

### 7.3 Display Strategy

Because the e-ink display has a slow refresh rate, it is important to minimize the number of updates. The display should only refresh when the data changes.

Frequent updates should be avoided to prevent slow performance and visual artifacts such as ghosting. Storing the last displayed content can help determine whether an update is necessary.

### 7.4 Reusability Approach

To ensure reusability, the system should avoid hardcoded values. Each ESP32 can be identified using a unique identifier, such as its MAC address, allowing the backend to send specific data for each tile.

This approach ensures that the same code can be used across all devices while still allowing different content to be displayed.

---

## 8. Limitations of E-ink Displays

E-ink displays have several limitations that must be considered. The refresh rate is slow, meaning updates can take several seconds. There may also be ghosting effects if the display is refreshed too often.

Additionally, the display only supports a limited number of colors, which restricts the design of logos and visuals. These limitations require careful handling of updates and simple visual designs.

---

## 9. Risks and Challenges

There is a risk that the lack of proper documentation will make the display difficult to implement. This can be mitigated by using libraries designed for similar displays.

Another challenge is handling image conversion, as logos may not display correctly if they are not in the right format. This can be addressed by preprocessing images on the backend.

Network issues may prevent the ESP32 from receiving data, so retry mechanisms should be implemented. Memory limitations on the ESP32 may also affect performance when handling images, which means optimization is necessary.

---

## 10. Success Criteria

The solution is considered successful when the ESP32 is able to display both the city name and logo, and when this data is retrieved dynamically from the backend.

The system should function without hardcoded content and should work across multiple tiles using the same firmware. It must update reliably and integrate smoothly with the existing project.

---

## 11. Conclusion

Adding an e-ink display to the ESP32 tiles improves the interaction between the physical and digital components of the project. However, the limitations of the hardware and the lack of documentation require a thoughtful approach.

By separating the data from the display logic and retrieving content from the backend, a scalable and reusable solution can be created. This ensures that the system can grow and adapt without requiring constant updates to the devices.

---

## References

*2.9inch SPI e-INK Display Expansion Module HAT 296x128 - Wiki.* (z.d.). Seengreat. Geraadpleegd op 22 april 2026, van [https://seengreat.com/wiki/132/29inch-e-ink-display#toc2](https://seengreat.com/wiki/132/29inch-e-ink-display#toc2)

*2.9inch e-Paper module Manual - Waveshare Wiki.* (z.d.). Waveshare. Geraadpleegd op 22 april 2026, van [https://www.waveshare.com/wiki/2.9inch_e-Paper_Module_Manual](https://www.waveshare.com/wiki/2.9inch_e-Paper_Module_Manual)

Kravec, M. (2025, 23 februari). Control 4-color MH-ET Live Epaper using Arduino. *kravemir.org.* Geraadpleegd op 22 april 2026, van [https://kravemir.org/how-to/control-4-color-mh-et-live-epaper-using-arduino/](https://kravemir.org/how-to/control-4-color-mh-et-live-epaper-using-arduino/)
