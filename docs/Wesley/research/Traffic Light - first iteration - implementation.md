# Implementation Learning Log — First Physical Traffic Light with ESP32-S3

## Summary

In this first implementation iteration, I used the analysed requirements and the earlier Fritzing and Wokwi design as the basis for building the traffic light on real ESP32-S3 hardware. In practice, I built the same three-LED traffic light physically, connected the LEDs to GPIO 4, 5, and 6, used one 220Ω resistor per LED, and uploaded the final Arduino code to the board. After testing, the physical prototype behaved the same as the Wokwi simulation: it started safely in red, followed the fixed order red → green → yellow → red, used the intended timing, and repeated the cycle without conflicting outputs. The main learning in this step was not what the traffic-light logic is in theory, because that had already been documented earlier, but how to apply that design correctly on real hardware and verify that the realized system behaved exactly like the simulation.

## Table of Contents

- [Summary](#summary)
- [Table of Contents](#table-of-contents)
- [Overview](#overview)
- [Goal of the Implementation](#goal-of-the-implementation)
- [Hardware and Implementation Setup](#hardware-and-implementation-setup)
  - [GPIO mapping](#gpio-mapping)
  - [Control logic](#control-logic)
- [What I Built](#what-i-built)
- [What I Learned](#what-i-learned)
  - [1. I learned how to move from design validation to real hardware](#1-i-learned-how-to-move-from-design-validation-to-real-hardware)
  - [2. I learned that physical realization still needs careful checking even when the simulation already works](#2-i-learned-that-physical-realization-still-needs-careful-checking-even-when-the-simulation-already-works)
  - [3. I learned that my real implementation matched the first-iteration requirements](#3-i-learned-that-my-real-implementation-matched-the-first-iteration-requirements)
  - [4. I learned that using Fritzing and Wokwi together made the realization easier](#4-i-learned-that-using-fritzing-and-wokwi-together-made-the-realization-easier)
  - [5. I learned that simple proof is enough when it directly shows the realized result](#5-i-learned-that-simple-proof-is-enough-when-it-directly-shows-the-realized-result)
- [Results and Evidence](#results-and-evidence)
  - [Evidence collected](#evidence-collected)
- [Reflection](#reflection)
- [Conclusion](#conclusion)
- [References](#references)
- [Appendix A — Proof of Realization on Real Hardware](#appendix-a--proof-of-realization-on-real-hardware)
  - [A.1 Visual proof of the realized circuit](#a1-visual-proof-of-the-realized-circuit)
  - [A.2 Final code proof](#a2-final-code-proof)
  - [A.3 Short test notes](#a3-short-test-notes)
    - [Test overview](#test-overview)
    - [Test 1 — Startup behavior after power-up/reset](#test-1--startup-behavior-after-power-upreset)
    - [Test 2 — Correct traffic-light order](#test-2--correct-traffic-light-order)
    - [Test 3 — Fixed timing per state](#test-3--fixed-timing-per-state)
    - [Test 4 — No conflicting outputs](#test-4--no-conflicting-outputs)
    - [Test 5 — Repeated cycle stability](#test-5--repeated-cycle-stability)
    - [Final conclusion](#final-conclusion)



---

## Overview

During the analysis phase, I had already defined the first iteration as a basic traffic light with three LED outputs, a fixed red → green → yellow → red sequence, fixed timing, safe startup in red, and no conflicting outputs. During the design phase, I translated that into a documented Fritzing and Wokwi design using GPIO 4, 5, and 6, three 220Ω resistors, and a shared ground return. In this implementation step, I applied that earlier work on real ESP32-S3 hardware and verified that the physical system behaved the same way as the validated simulation.

---

## Goal of the Implementation

The goal of this implementation was to realize the first working physical version of my traffic-light project and verify that the hardware and software matched the analysed requirements and the earlier design. The physical prototype had to control the red, yellow, and green LEDs, follow the fixed order red → green → yellow → red, start in red after power-up, prevent conflicting outputs, and keep repeating the cycle reliably. After testing, the realized traffic light met those goals and behaved the same as in Wokwi.

---

## Hardware and Implementation Setup

For the physical implementation, I used the ESP32-S3, three LEDs, and three resistors on a breadboard. I followed the same structure as the design: one output pin per LED, one resistor per LED, and a shared ground return. The realized wiring matched the Fritzing design and the code matched the Wokwi-validated behavior, which made it possible to compare the physical prototype directly to the earlier digital design.

### GPIO mapping

| Component  | GPIO pin |
| ---------- | -------: |
| Red LED    |        4 |
| Yellow LED |        5 |
| Green LED  |        6 |

Each LED was connected through its own 220Ω resistor to ground. This kept the hardware simple and matched the first-iteration design exactly. 

### Control logic

The control logic of the implementation followed the same structure as in the simulation. I used separate functions for the three light states and made sure that the other outputs were switched off before a new state became active. That way, only one traffic-light output was active at a time. The implemented sequence was:

* red
* green
* yellow
* red

The realized timing also matched the design and simulation:

* red: 5000 ms
* green: 5000 ms
* yellow: 2000 ms

The system also started safely in red after upload, reset, or power-up. This matched both the analysed requirements and the design decisions documented earlier.

---

## What I Built

I built the first physical version of my traffic-light prototype on a breadboard with the ESP32-S3 as controller. The prototype consists of one red LED, one yellow LED, and one green LED, each connected to its own GPIO pin and resistor. After uploading the firmware, the traffic light ran exactly as intended. The red LED turned on first, then the green LED, then the yellow LED, and then the sequence repeated continuously.

Because I had already validated the design in Wokwi, the realization step was mainly about making sure that the real wiring and the uploaded code matched the earlier design. After testing, the physical prototype showed the same behavior as the simulation. This confirmed that the design was not only valid digitally, but also buildable and functional on real hardware.

---

## What I Learned

### 1. I learned how to move from design validation to real hardware

Before this step, I had already proven in Fritzing and Wokwi that the first traffic-light design was clear and workable. In this implementation step, I learned how to take that documented design and use it as a direct build guide for the real prototype. That made the realization process much more structured, because I was no longer experimenting without direction.

### 2. I learned that physical realization still needs careful checking even when the simulation already works

Even though the Wokwi simulation already behaved correctly, I still had to check the real breadboard wiring carefully. On real hardware, LED polarity, resistor placement, GPIO connections, and ground wiring all matter directly. This taught me that simulation reduces risk, but real implementation still requires attention to detail.

### 3. I learned that my real implementation matched the first-iteration requirements

This step confirmed that the requirements from the analysis were realistic and implementable. The physical traffic light controls the three outputs, follows the intended state order, starts safely in red, avoids conflicting outputs, and repeats the cycle correctly. That means the implementation does not only look correct, but also directly satisfies the earlier first-iteration scope. 

### 4. I learned that using Fritzing and Wokwi together made the realization easier

Fritzing helped me as a hardware reference while building the breadboard circuit, and Wokwi helped me as a behavior reference while testing the logic. Together, they made debugging easier because I could compare the physical prototype to both the circuit layout and the simulated output sequence. This made the implementation step more efficient and more reliable.

### 5. I learned that simple proof is enough when it directly shows the realized result

This implementation phase also taught me that realization proof does not have to be complicated. A clear photo of the real wiring, a capture of the final code, and short test notes are already enough to show that the prototype was physically built, programmed, and verified. The strength of the proof comes from the fact that it directly shows the realized hardware and its tested behavior.

---

## Results and Evidence

The results of this implementation step are:

* a working physical ESP32-S3 traffic-light prototype
* a realized breadboard circuit that matches the design
* uploaded firmware for the first traffic-light iteration
* successful behavior on real hardware with the same result as the Wokwi simulation
* proof that startup, sequence, timing, and repetition all work correctly

### Evidence collected

| Evidence          | What it proves                                                    |
| ----------------- | ----------------------------------------------------------------- |
| Real wiring photo | I physically built the first traffic-light prototype              |
| Final code        | I implemented the traffic-light logic on the ESP32-S3             |
| Short test notes  | I verified that the physical behavior matched the intended design |
| Fritzing design   | The physical circuit was built from a documented hardware design  |
| Wokwi validation  | The realized behavior matched the earlier validated simulation    |

---

## Reflection

This implementation step was useful because it connected the earlier analysis and design work to an actual working prototype. During the analysis phase, I had already documented the requirements and hardware basis. During the design phase, I had already documented the circuit, the pin choices, the timing, and the state sequence. In this implementation step, I learned how those earlier documents function as practical tools during realization.

The most important reflection is that the physical prototype behaved the same as the Wokwi simulation. That confirmed that the design decisions were correct and that the realization was done accurately. It also showed me that the workflow of analyse → design → implement works well for embedded development, because each earlier phase reduced uncertainty in the next one.

---

## Conclusion

This first implementation step showed that I can take the earlier analysed requirements and documented design and turn them into a working real hardware prototype. I built the first traffic light physically on the ESP32-S3, uploaded the firmware, and tested that the behavior matched the intended first iteration. The realized traffic light starts in red, follows the order red → green → yellow → red, uses fixed timing, prevents conflicting outputs, and repeats continuously. Because the real hardware behaved the same as the Wokwi simulation, this implementation gives practical proof that the first traffic-light iteration works both digitally and physically.

---

## References

_Wesley. *Learning Outcome - Analysis.*

_Wesley. *Learning Outcome - Design.*

_Wesley. *Traffic Light - first iteration - fritzing.*

_Wesley. *Traffic Light - first iteration - wokwi.*

---

## Appendix A — Proof of Realization on Real Hardware

This appendix contains the direct proof for my first physical realization of the traffic-light project. It supports the first-iteration requirements that I had already defined earlier: three outputs, fixed order, fixed timing, safe startup in red, no conflicting outputs, and continuous repetition. The realized hardware behaved the same as the Wokwi simulation and therefore confirms that the first design also works physically.

### A.1 Visual proof of the realized circuit

**Figure A1. First physical traffic light circuit**

![Figure A1](../assets/Traffic%20Light%20-%20first%20iteration%20-%20implementation/20260325_180750.jpg)

Figure A1 shows the realized traffic-light prototype on the breadboard. The circuit contains the ESP32-S3, one red LED, one yellow LED, one green LED, and one resistor for each LED. The wiring matches the Fritzing design that I created earlier.

From the realized circuit, the implementation can be observed as:

* GPIO 4 → red LED → resistor → GND
* GPIO 5 → yellow LED → resistor → GND
* GPIO 6 → green LED → resistor → GND

This proves that I physically built the same first-iteration traffic-light circuit that I had already validated in the design and simulation phase.

---

### A.2 Final code proof

**Figure A2. Final Arduino code used for the physical traffic-light prototype**

```ino
#include <Arduino.h>

const int RED_PIN = 4;
const int YELLOW_PIN = 5;
const int GREEN_PIN = 6;


void allOff() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void redOn() {
  allOff();
  digitalWrite(RED_PIN, HIGH);
  Serial.println("RED");
  delay(5000);
}

void yellowOn() {
  allOff();
  digitalWrite(YELLOW_PIN, HIGH);
  Serial.println("YELLOW");
  delay(2000);
}

void greenOn() {
  allOff();
  digitalWrite(GREEN_PIN, HIGH);
  Serial.println("GREEN");
  delay(5000);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  //register pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // Safe startup: red first
  digitalWrite(LED_BUILTIN, LOW);
  redOn();
}

void loop() {
  redOn();
  greenOn();
  yellowOn();
}
```


Figure A2 shows the final code that I uploaded to the ESP32-S3. In this implementation, the code controls the red, yellow, and green LEDs through GPIO 4, 5, and 6. It starts safely in red, switches through the sequence red → green → yellow → red, and repeats the cycle continuously.

The final code proves several important points:

* I implemented three separate outputs for red, yellow, and green.
* I implemented the fixed state order required for the first iteration.
* I used fixed timing for each state.
* I prevented conflicting outputs by switching the other LEDs off before activating the next one.
* I kept the system behavior the same as in Wokwi.

---

### A.3 Short test notes

I tested the first physical traffic-light prototype after building the circuit and uploading the final code to the ESP32-S3. The goal of these tests was to verify that the realized hardware behaved the same as the Wokwi simulation and matched the first-iteration requirements.

#### Test overview

| Test | What I checked | Result |
|------|----------------|--------|
| 1 | Startup behavior after power-up/reset | Passed |
| 2 | Correct traffic-light order | Passed |
| 3 | Fixed timing per state | Passed |
| 4 | No conflicting outputs | Passed |
| 5 | Repeated cycle stability | Passed |

#### Test 1 — Startup behavior after power-up/reset

I powered on and reset the ESP32-S3 multiple times to check the startup state.

**Expected result:**  
The system starts safely in red.

**Observed result:**  
Each time the prototype started, the red LED turned on first.

**Conclusion:**  
Passed. The startup behavior matches the design and the Wokwi simulation.

#### Test 2 — Correct traffic-light order

I observed the full cycle several times.

**Expected result:**  
The LEDs follow the order: red → green → yellow → red.

**Observed result:**  
The prototype consistently followed the correct state sequence.

**Conclusion:**  
Passed. The physical implementation matches the intended traffic-light order.

#### Test 3 — Fixed timing per state

I observed the duration of each light state during multiple cycles.

**Expected result:**  
- Red: 5 seconds  
- Green: 5 seconds  
- Yellow: 2 seconds

**Observed result:**  
The timing of the physical prototype matched the programmed timing and behaved the same as the simulation.

**Conclusion:**  
Passed. The timing remained stable during testing.

#### Test 4 — No conflicting outputs

I checked whether more than one traffic-light LED could become active at the same time.

**Expected result:**  
Only one LED is active in each state.

**Observed result:**  
During testing, only the active state LED was on. No conflicting outputs were observed.

**Conclusion:**  
Passed. The implementation correctly switches the other LEDs off before activating the next state.

#### Test 5 — Repeated cycle stability

I let the traffic light run through multiple full cycles without changing the wiring or code.

**Expected result:**  
The cycle repeats continuously without errors or unexpected behavior.

**Observed result:**  
The prototype continued cycling correctly and remained stable over repeated runs.

**Conclusion:**  
Passed. The implementation works reliably for repeated cycles.

#### Final conclusion

The short tests confirm that the first physical traffic-light prototype works correctly. The realized hardware starts safely in red, follows the intended order red → green → yellow → red, uses fixed timing, prevents conflicting outputs, and behaves the same as the Wokwi simulation.

---
