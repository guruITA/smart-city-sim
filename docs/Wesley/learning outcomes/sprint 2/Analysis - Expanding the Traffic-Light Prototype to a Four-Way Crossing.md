# Analysis: Expanding the Traffic-Light Prototype to a Four-Way Crossing

## Introduction

At the start of this sprint, I already had a working first traffic-light prototype on an ESP32-S3. In that version, one red LED, one yellow LED, and one green LED were each connected directly to their own GPIO pin. That first version was useful because it proved the basic traffic-light logic: safe startup in red, a fixed state order, and stable timing.

For the second version, that approach is no longer sufficient. A full four-way crossing needs four traffic lights, which means twelve separate light channels in total. A direct one-pin-per-LED solution would use too many controller pins and would also make it harder to expand the project later.

In this analysis, I examine how the current prototype can be expanded into a small crossroads system with multiple traffic lights, while using as few controller pins as possible, avoiding multiplexing, and leaving room for future sensor input and backend control.

## Current situation

The current prototype is a single traffic light controlled directly by the ESP32-S3. Each LED has its own GPIO pin and its own resistor. This is simple and understandable, but it does not scale well.

A four-way crossing needs:

- North: red, yellow, green
- East: red, yellow, green
- South: red, yellow, green
- West: red, yellow, green

That means twelve separate light outputs. Using twelve ESP32 pins for this is technically possible, but it is not a good solution for this project. I also want to keep pins available for later additions such as sensors, communication, and backend integration.

Another problem is power. In the first version, the ESP32 pin directly drives each LED. For a larger setup, that is no longer desirable. In the expanded version, the ESP32 should only decide which outputs must be active. The actual LED current should come from an external power source.

## Problem definition

The main problem is how to control twelve traffic-light channels in a clear and safe way without using twelve direct ESP32 outputs.

The solution must meet these conditions:

- it must be feasible for this project;
- it must use fewer controller pins than the number of LEDs;
- it must not use multiplexing, because each traffic-light output should remain a stable real output;
- it must support future traffic-aware behavior, such as reacting when one direction has waiting cars and others do not;
- it must support future commands from a backend;
- it must use external power for the traffic-light LEDs;
- it must remain understandable enough for a learning project.

## What the crossroads system should do in practice

In practice, the system should behave as one coordinated crossing instead of as four separate traffic lights. That means the traffic lights must work together according to safe traffic phases.

For this project, the clearest structure is to treat the crossing as four directions:

- North
- East
- South
- West

Because this version does not include turning traffic, North and South can be grouped together, and East and West can be grouped together. That leads to this basic phase model:

1. all-red
2. North/South green, East/West red
3. North/South yellow, East/West red
4. all-red
5. East/West green, North/South red
6. East/West yellow, North/South red

This is a practical and understandable starting point for a small crossroads model.

## Most important safety rules

The system has to follow a few important safety rules so the crossing remains clear and safe:

- incompatible directions may never have green at the same time;
- yellow is only used as a transition phase;
- an all-red phase must be used between incompatible directions;
- the system must start in a safe state;
- each traffic-light output must remain a stable output and must not depend on multiplexing.

These rules are important because the project should not only work technically, but also make sense as a traffic situation.

## Why the current design is no longer suitable

The first version used one ESP32 pin per LED. That was a good choice for the first iteration, but not for a full crossing.

The main disadvantages are:

- twelve direct outputs would be needed for twelve lamp channels;
- fewer pins would remain available for sensors and communication;
- the ESP32 would still be too closely connected to the load side of the circuit;
- the design becomes less practical if one output later controls more than one visible LED.

Because of that, the one-pin-per-LED design is suitable for learning and early validation, but not as the final structure for the expanded version.

## Design principle for the expanded version

For the second version, I want to separate the system into three clear roles:

1. **controller logic**  
   The ESP32-S3 decides which channels should be active, handles future sensor input, and communicates with the backend.

2. **output expansion**  
   A separate component provides enough output channels without using many ESP32 pins. I/O expanders such as the MCP23017 are designed for this kind of GPIO expansion over I2C (Adafruit, 2024; MakerGuides, 2025).

3. **power switching**  
   A separate driver stage switches the externally powered lamp channels on and off. The ULN2803 is commonly used as a driver array for this type of switching.

This gives the following structure:

```text
ESP32-S3 -> output expansion -> driver stage -> external power supply -> traffic-light LEDs
```

## Considered approaches

### Option 1 — direct ESP32 GPIO outputs

This is the same design style as the first prototype. It is simple to understand, but it uses too many pins and does not separate control from load switching.

**Assessment:** usable as a temporary test, but not suitable as the final solution.

### Option 2 — cascaded shift registers

This reduces the number of required controller pins and is useful to understand the low-pin concept. However, it mainly solves output count, not load switching. In the final hardware, an extra driver stage would still be needed.

**Assessment:** useful for learning and simulation, but not the clearest final structure for this project.

### Option 3 — I2C output expander with driver arrays

This uses an I/O expander, such as an MCP23017, together with driver arrays such as two ULN2803 chips. The MCP23017 provides 16 configurable GPIOs over I2C, which makes it a practical fit for expanding digital outputs in a compact system (Adafruit, 2024; MakerGuides, 2025).

This approach has clear advantages:

- enough outputs for all traffic-light channels;
- only a small number of ESP32 pins are needed;
- the switching is handled by the driver stage;
- the LEDs can use external power instead of ESP32 pin power.

**Assessment:** best fit for this project.

## Selected direction

The most suitable direction for this project is:

**ESP32-S3 + MCP23017 + 2x ULN2803 + external LED power supply**

I chose this direction because it clearly separates control, output expansion, and power switching. It is also easier to explain and easier to expand later.

The main advantages are:

- fewer ESP32 pins are needed;
- no multiplexing is required;
- every lamp channel remains a stable output;
- the design leaves room for future sensors;
- the structure fits future backend control;
- the LED current no longer has to come directly from the ESP32.

## What counts as a correct result in this step

For this step, I consider the result correct when the system can control four traffic lights as one coordinated crossroads setup, without unsafe combinations.

A correct result means:

- North and South can operate together;
- East and West can operate together;
- conflicting directions are never green at the same time;
- all-red transitions are used between incompatible phases;
- the system works as one crossing instead of separate loose traffic lights.

This sprint does not need to make the system fully smart yet. The goal is not advanced traffic optimisation yet, but a safe and understandable multi-traffic-light structure that can later be extended with sensors and backend control.

## Current hardware limits

At this moment, I still have some practical limits.

First, my starting point is a simple prototype where the ESP32 directly powers one red, one yellow, and one green LED. That version is useful for learning, but it is not suitable as the final architecture for a complete crossing.

Second, the expanded version requires extra hardware for output expansion and load switching. Without those components, the full design cannot yet be realized physically in the intended way.

Third, the scope is still simplified. This version does not yet include turning lanes, pedestrian phases, emergency priority, or advanced traffic optimisation. The current model only focuses on straight traffic directions grouped mainly as North/South and East/West.

Finally, simulation can help explain the logic, but it is not enough to fully prove the final electrical design. The switching stage and external power setup still need to be tested on real hardware.

## Important risks

There are also a few important risks to keep in mind:

- the ESP32-S3 pin choice must still be done carefully;
- the controller, output expander, driver stage, and external power source must share a common ground;
- output expansion alone is not enough, because the load still needs a driver stage;
- if one logical output later controls multiple LEDs, the load and resistor design must be handled in the external output stage.

## Feasibility

This design is feasible because it can be built in clear steps:

1. test one externally powered output through the driver stage;
2. test one full traffic light with three channels;
3. expand to all four traffic lights;
4. add sensing and backend communication later.

That staged approach keeps the project manageable and reduces the risk of making the system too complex too early.

## Conclusion

The first one-pin-per-LED prototype was a good first step, but it is not the right final structure for a four-way crossing. For the next version, the ESP32-S3 should no longer power the traffic-light LEDs directly. Instead, it should act as the controller that decides which channels must be active.

The most suitable direction is therefore to separate controller logic, output expansion, and power switching. For this project, the clearest solution is:

**ESP32-S3 -> MCP23017 -> 2x ULN2803 -> external LED power supply -> 4 traffic lights**

This structure uses fewer controller pins, avoids multiplexing, supports external LED power, and leaves room for future extensions such as vehicle detection and backend control. That makes it a strong basis for the next version of the project.

## References for the appendix

Adafruit. (2024, March 7). Adafruit MCP23017 I2C GPIO expander. Adafruit Learning System. https://learn.adafruit.com/adafruit-mcp23017-i2c-gpio-expander

MakerGuides. (2025, June 26). *Using GPIO expander MCP23017 with Arduino (Get up to 128 GPIOs)*. https://www.makerguides.com/using-gpio-expander-mcp23017-with-arduino/

McGaw, P. (n.d.). MCP23017 and ULN2803. PhilipMcGaw.com. https://philipmcgaw.com/mcp23017-and-uln2803/

Rathbone, B. (n.d.). Raspberry PI Traffic Lights (I2C). Bob Rathbone. https://bobrathbone.com/raspberrypi/documents/Raspberry%20PI%20Traffic%20Lights%20(I2C).pdf

