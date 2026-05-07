# DESIGN

---

# **Design (Sprint 2)**

Before building the pedestrian crossing system, I created a clear design that describes how the subsystem should work, which components are needed, and how everything connects to the ESP32‑S3. This design phase helped me understand the system before moving to the breadboard prototype.

I also printed an existing 3D model of a pedestrian signal housing to explore the physical design. I did not design the model myself, but printing it helped me understand the required scale, how the LEDs might fit inside, and how the housing integrates with the tile. The first version was too large, so in Sprint 3 I will scale it down and refine the dimensions

---

## **1. System Concept**

The subsystem contains two opposite pedestrian crossings.

Each side has:

- 1 red LED
- 1 green LED
- 1 push button

Both sides must behave **synchronously**:

when one pedestrian presses a button, **both crossings turn green at the same time**.

The ESP32‑S3 acts as the controller that reads the button inputs and controls the LED outputs.

---

## **2. Component Design**

I selected the following components for the system:

- ESP32‑S3 (controller)
- 4 LEDs (2 red, 2 green)
- 4 × 220Ω resistors (one per LED)
- 2 push buttons
- Breadboard + jumper wires

Each LED uses a **220Ω resistor** to limit current.

Each button uses **INPUT_PULLUP** in code to keep the input stable.

---

## **3. Wiring Design (Based on Wokwi Simulation)**

Before building the real circuit, I designed the wiring in Wokwi.

[](https://portfolio-lti-production.s3.eu-west-1.amazonaws.com/hva_brightspace_productie/i5elzbkjl6j1yw7g4bu094ul4od6?response-content-disposition=inline%3B%20filename%3D%22SmartPedestrianLights.png%22%3B%20filename%2A%3DUTF-8%27%27SmartPedestrianLights.png&response-content-type=image%2Fpng&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAZUHNSBLFLRASQZLQ%2F20260412%2Feu-west-1%2Fs3%2Faws4_request&X-Amz-Date=20260412T173702Z&X-Amz-Expires=43200&X-Amz-SignedHeaders=host&X-Amz-Signature=2afc914b86e1882167311ac57a12a1bc5920076348d7ee8587551b4e1c179f31)

This helped me understand:

- which GPIO pins to use
- how to place LEDs and resistors in series
- how to wire the buttons with pull‑up logic
- how to structure the breadboard layout

The Wokwi design served as a blueprint for the physical prototype.

---

## **4. Logic Design**

I designed the system logic as follows:

1. System starts with **both sides red**
2. When a button is pressed, the ESP32 checks if the system is free
3. A waiting time simulates cars stopping
4. Both sides turn **green** for a fixed crossing time
5. After the crossing, both sides return to **red**
6. A cooldown prevents spamming the system

This logic ensures safety and predictable behavior.

---

## **5. 3D Design (Pedestrian Light Housing)**

I also created a first 3D print of a pedestrian light housing to explore the physical design.

The model turned out **too large**, so I will scale it down and print a smaller version in Sprint 3.

This 3D prototype helped me understand:

- how big the housing should be
- how LEDs might fit inside
- how the design integrates with the tile

Even though the first version was not the right size, it is still part of my design process.

---

## **6. GPIO Pin Mapping & Wiring Design**

For the ESP32‑S3, I used the following pin mapping and wiring for the two synchronized pedestrian crossings:

### **LED Connections (Pedestrian Crossing A & B)**

**Pedestrian A**

- **GPIO 5 → Green LED A**
    - Connected in series with a **220Ω resistor**, then to **GND**
- **GPIO 6 → Red LED A**
    - Connected in series with a **220Ω resistor**, then to **GND**

**Pedestrian B**

- **GPIO 1 → Green LED B**
    - Connected in series with a **220Ω resistor**, then to **GND**
- **GPIO 2 → Red LED B**
    - Connected in series with a **220Ω resistor**, then to **GND**

All LEDs use a **220Ω resistor** to limit current. The **GND rail (–)** on the breadboard is connected to the **GND pin of the ESP32‑S3**.

### **Button Connections**

- **GPIO 12 → Button A**
- **GPIO 13 → Button B**

Both buttons are connected to **GND**, and **do not use external resistors**. This is intentional because the ESP32‑S3 uses:

`INPUT_PULLUP`

This means the logic works as:

- **HIGH = not pressed**
- **LOW = pressed (connected to GND)**

Using the internal pull‑up resistors keeps the circuit simple and stable.

| Component | GPIO Pin | Notes |
| --- | --- | --- |
| Green LED A | 5 | 220Ω → GND |
| Red LED A | 6 | 220Ω → GND |
| Green LED B | 1 | 220Ω → GND |
| Red LED B | 2 | 220Ω → GND |
| Button A | 12 | INPUT_PULLUP |
| Button B | 13 | INPUT_PULLUP |

---

## **6. Summary**

In this design phase, I created:

- a clear wiring plan
- a Wokwi simulation
- a component overview
- a system logic description
- a first 3D model

This design gave me the foundation I needed to build the working prototype in the Realise phase