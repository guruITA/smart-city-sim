# **Analysis**

# 1) Goal of this document

This document explains how a smart pedestrian crossing works and how its behavior is translated into an ESP32‑S3 project. It focuses on system logic (input → processing → output), digital button behavior (HIGH/LOW, pull‑up/pull‑down), resistor usage, LED polarity, and the electrical principles behind the breadboard used during prototyping.


# 2) How a smart pedestrian crossing works in real life

## 2.1 What “smart” means

A smart pedestrian crossing improves safety by combining:

- **User input** (push button)
- **A controller** that manages timing and decisions
- **Signals** for drivers and pedestrians

The goal is predictable and safe behavior.

## 2.2 System model: input → processing → output

1. **Input:** A pedestrian presses a button.
2. **Processing:** The controller checks safety, uses timers, and prevents unsafe combinations.
3. **Output:** Drivers are warned, cars stop, pedestrians get a green signal.

## 2.3 Why the system does not switch instantly

Delays are required for:

- driver reaction time
- braking distance
- smoother traffic flow



# 3) Digital input logic on the ESP32‑S3

## 3.1 HIGH vs LOW

A digital pin reads:

- **LOW** ≈ 0V
- **HIGH** ≈ 3.3V

The ESP32 checks this repeatedly to detect button presses.

## 3.2 Floating inputs

Without a pull resistor, a pin becomes **floating**, meaning it randomly switches between HIGH and LOW due to electrical noise. This causes false button presses.



# 4) Pull-up and pull-down resistors

Pull resistors give the input a stable default state.

## 4.1 Pull-down (direct logic)

- Default (unpressed): **LOW**
- Pressed: **HIGH**
- Button connects the pin to **3.3V**
- Resistor connects the pin to **GND**

This is the logic used in the project.

## 4.2 Pull-up (inverted logic)

- Default: **HIGH**
- Pressed: **LOW**
- Button connects the pin to **GND**
- Resistor connects the pin to **3.3V**

## 4.3 Internal pull-down on ESP32‑S3

The ESP32‑S3 supports internal pull-down resistors:

```
pinMode(buttonPin, INPUT_PULLDOWN);
```

This removes the need for an external 10kΩ resistor.


# 5) Why resistors matter

## 5.1 Button resistors

- Typical value: **10kΩ**
- Purpose: **signal stability**, not current limiting

## 5.2 LED resistors

- Typical value: **220Ω – 470Ω**
- Purpose: **limit current** to protect the LED and the ESP32 pin
- **Each LED needs its own resistor**


# 6) LED polarity and correct wiring

LEDs are polarity‑sensitive:

- **Anode (+)** = long leg
- **Cathode (–)** = short leg

Correct wiring for ESP32‑S3:

```
GPIO pin → 220Ω resistor → LED anode (+)
LED cathode (–) → GND
```

The LED must **not** be connected directly to the + rail, because:

- it would always be on
- the ESP32 cannot control it
- the LED may burn out



# 7) Breadboard analysis

A breadboard is a solderless prototyping tool that allows quick testing of circuits. Inside the breadboard, metal strips connect groups of holes.

### Power rails

- Long vertical lines on the sides
- Red = + (3.3V or 5V)
- Blue/black = GND
- Used to distribute power across the board

### Terminal rows

- Horizontal rows in the center
- Each row (A–E or F–J) is internally connected
- Rows do **not** connect to each other

### Center gap

- Separates the left and right halves
- Prevents short circuits
- Designed for microcontrollers like the ESP32‑S3 so each pin gets its own row

### Why this matters

Understanding the breadboard helps you:

- place components correctly
- avoid short circuits
- understand how signals travel
- test LEDs, resistors, and buttons safely before building the final version

The breadboard makes it easy to experiment, fix mistakes, and verify that the digital input logic (HIGH/LOW, pull‑down behavior) works reliably.



# 8) Connecting button logic to the crossing sequence

When the button is pressed:

1. Input changes from LOW → HIGH
2. ESP32 detects the change
3. A timed sequence begins
4. LEDs and signals activate in order

Extra presses are ignored until the cycle finishes.



# 9) Conclusion

- A smart pedestrian crossing is a **timed safety system**
- Digital inputs must never float
- Pull‑down logic gives stable behavior
- LED resistors protect the hardware
- Breadboards help test and understand the circuit safely



# 10) Glossary

- **Input:** button or sensor
- **Processing:** logic + timing
- **Output:** LEDs and signals
- **HIGH/LOW:** digital voltage states
- **Floating pin:** unstable input
- **Pull-up/pull-down:** resistor methods for stability