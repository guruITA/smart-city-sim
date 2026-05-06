# Design Document

## Table of Contents

1. [Introduction](#1-introduction)  
2. [System Overview](#2-system-overview)  
3. [Architecture Design](#3-architecture-design)  
4. [Hardware Design](#4-hardware-design)  
5. [Software Design](#5-software-design)  
6. [Data Flow](#6-data-flow)  
7. [API Design](#7-api-design)  
8. [Display Rendering Strategy](#8-display-rendering-strategy)  
9. [Update Mechanism](#9-update-mechanism)  
10. [Error Handling](#10-error-handling)  
11. [Reusability & Scalability](#11-reusability--scalability)  
12. [Bill of Materials (BOM)](#12-bill-of-materials-bom)  
13. [Circuit Design (Fritzing)](#13-circuit-design-fritzing)  
14. [Conclusion](#14-conclusion)  

---

## 1. Introduction

This document describes the design and implementation of an ESP32-based system with an e-ink display. The system is part of a smart city simulation in which physical tiles represent cities.

The purpose of this design is to enable each tile to display dynamic information retrieved from a backend system. The displayed content must be remotely configurable without requiring firmware updates.

---

## 2. System Overview

Each tile consists of:

* An ESP32 microcontroller
* A 2.9-inch e-ink display
* A Wi-Fi connection

The ESP32 retrieves text data from a backend API every 10 seconds and updates the display only when the content has changed.

---

## 3. Architecture Design

The system follows a client-server architecture.

### Components

#### ESP32 (Client):

* Connects to Wi-Fi
* Sends HTTP requests
* Processes responses
* Controls the display

#### Backend (Server):

* Stores display content
* Exposes API endpoints
* Returns text data

### Architecture Flow

1. ESP32 connects to Wi-Fi
2. Sends HTTP request to backend
3. Receives response
4. Compares with previous data
5. Updates display if needed

---

## 4. Hardware Design

### Components

* ESP32
* MH-ET Live 2.9" e-ink display (296x128, black/white/red)
* Jumper wires
* Power supply (USB or battery)

### Communication Protocol

The display communicates via SPI:

* VCC → 3.3V
* GND → GND
* DIN → MOSI
* CLK → SCK
* CS → GPIO
* DC → GPIO
* RST → GPIO
* BUSY → GPIO

---

## 5. Software Design

The software is modular:

### 5.1 Wi-Fi Module

* Connects to network
* Handles reconnection

### 5.2 API Client

* Sends HTTP GET request every 10 seconds
* Retrieves plain text

### 5.3 Data Manager

* Stores previous text
* Compares new vs old

### 5.4 Display Controller

* Renders text on e-ink display
* Handles refresh logic

---

## 6. Data Flow

1. ESP32 sends request:

```
GET /eink-display/text
```

2. Backend responds:

```json
The Embedded alliance
```

3. ESP32:

* Reads text
* Compares with previous value
* Updates display if changed

---

## 7. API Design

The backend is implemented using FastAPI.

### 7.1 Data Models

#### Request Model

```json
{
  "text": "string (1–120 characters)"
}
```

#### Response Model

```json
{
  "text": "string"
}
```

---

### 7.2 Endpoints

#### Get Display Text (JSON)

```
GET /eink-display
```

Response:

```json
{
  "text": "The Embedded alliance"
}
```

---

#### Get Display Text (Plain Text)

```
GET /eink-display/text
```

Response:

```
The Embedded alliance
```

This endpoint is used by the ESP32 for efficiency.

---

#### Update Display Text

```
PUT /eink-display
```

Request:

```json
{
  "text": "New city name"
}
```

Response:

```json
{
  "text": "New city name"
}
```

---

### 7.3 State Management

The backend stores the current text in memory:

```python
_display_state = {"text": "The Embedded alliance"}
```

---

## 8. Display Rendering Strategy

Due to e-ink limitations:

* Only update when content changes
* Use simple text layout
* Avoid frequent refreshes

### Layout

```bash
+----------------------+
|                      |
|   City Name          |
|                      |
|                      |
+----------------------+
```

---

## 9. Update Mechanism

The ESP32 polls the backend every 10 seconds.

### Logic

```cpp
if (newText != oldText):
    updateDisplay()
```

### Benefits

* Reduces unnecessary refreshes
* Minimizes ghosting
* Saves power

---

## 10. Error Handling

### Network Errors

* Retry connection
* Attempt reconnect to Wi-Fi

### Invalid Data

* Ignore response
* Keep previous display

### Display Errors

* Reinitialize display
* Reset SPI communication

---

## 11. Reusability & Scalability

The system is designed to be reusable:

* No hardcoded display content
* Same firmware for all devices
* Backend controls content

### Scalability Options

* Add device identification (MAC address)
* Store data in PostgreSQL
* Extend API for multiple tiles

---

## 12. Bill of Materials (BOM)

| Component          | Quantity |
| ------------------ | -------- |
| ESP32              | 1        |
| 2.9" E-ink Display | 1        |
| Jumper Wires       | 9        |
| Power Supply       | 1        |

---

## 13. Circuit Design (Fritzing)

A Fritzing diagram is included to visualize the wiring between the ESP32 and the e-ink display.

![Fritzing diagram E-ink screen](./assets/E-ink-screen-fritzing.png)

Key connections:

* VCC -> 3.3V
* GND -> GND
* DIN -> GPIO 11
* CLK -> GPIO 13
* CS -> GPIO 10
* DC -> GPIO 9
* RST -> GPIO 8
* BUSY -> GPIO 7

---

## 14. Conclusion

This design provides a practical and scalable solution for displaying dynamic text on an e-ink display using an ESP32.

By using a backend-driven approach and polling mechanism, the system avoids hardcoded data and allows easy updates. The design accounts for the limitations of e-ink displays and ensures efficient performance by minimizing unnecessary refreshes.

The result is a reusable system that integrates smoothly into the smart city simulation and can be extended in future iterations.

---