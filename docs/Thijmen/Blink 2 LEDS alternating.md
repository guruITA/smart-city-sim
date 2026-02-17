# Blink 2 LEDS alternating demo

## 1. Overview

This demo alternates two LEDs using non-blocking timing.  
Every 750 milliseconds, one LED turns ON while the other turns OFF, creating a left-right blinking pattern.

This behavior simulates the warning lights at a railway crossing, where two red lights flash alternately to alert traffic that a train is approaching.

This demo is located in the folder ```embedded/Thijmen/blink-alternating```

---

## 2. LED Configuration

- One LED is connected to pin 18  
- One LED is connected to pin 19  

The LEDs alternate between ON and OFF states to create the flashing pattern.

---

## 3. Functional Description

The system creates an alternating blinking effect:

- When the left LED is ON, the right LED is OFF.
- After 750 ms, they switch states.
- This pattern repeats continuously.

This mimics the alternating red flashing lights used at railway crossings to warn road users.

---

## 4. Timing Mechanism

The program uses a non-blocking timing approach with `millis()`.

When the elapsed time reaches 750 milliseconds:

- The active LED switches.
- The timing reference is updated.
- The cycle continues.

This ensures smooth operation without blocking the main loop.

---

## 5. Educational Purpose

This demo demonstrates:

- Non-blocking timing using `millis()`
- Alternating signal logic
- Basic simulation of railway crossing warning lights
- Time-based state switching in embedded systems
