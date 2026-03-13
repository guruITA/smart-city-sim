# Smart Cities Learning group goal: Advise

- Name: Gurpreet Singh
- Date: 11-3-2026

## Table Contents 
- [Smart Cities Learning group goal: Advise](#smart-cities-learning-group-goal-advise)
  - [Table Contents](#table-contents)
  - [Show \& Tell: Smart streetlight prototype](#show--tell-smart-streetlight-prototype)
  - [Explanation of the prototype](#explanation-of-the-prototype)
  - [Technical evaluetion](#technical-evaluetion)
  - [Total Power Consumption Summary](#total-power-consumption-summary)
  - [Design choice](#design-choice)


## Show & Tell: Smart streetlight prototype 

At the end of Sprint 1 of the Smart Cities Learning Group project, I present the prototype of my smart streetlight. The goal of this prototype is to demonstrate how the streetlights can automatically switch on and off depending on the amount of ambient light.

This is built on the ESP32-S3 that we got that reads the light level from an LDR module. Based on this value, the ESP32 controls a relay module that switches the streetlight LEDs on or off.

The prototype represents a simplified version of how an automatic streetlight system could function inside a Smart City environment. Instead of a fixed timer, the lighting responds dynamically to environmental conditions.

## Explanation of the prototype 

![breadboard_streetlight_wiring](images/breadboard_streetlight_wiring.png)

The prototype consists of the following components:

- ESP32-S3
- LDR module
- 1-channel 5V relay module
- White LEDs with individual 220Ω resistors
- Breadboard power system with fuse, diode and elco

The LDR module measures the ambient light level and sends an analog signal through the AO pin to an ADC pin on the ESP32-S3 the GPIO4. The ESP32 reads this value and compares it to a threshold defined in the code.

When the measured light level drops below the threshold, the ESP32 activates the relay module via pin 5 GPIO5. The relay then connects the external 5V power supply to the LED circuit, turning on the streetlight LEDs.

When the light level increases again, the ESP32 deactivates the relay and the LEDs turn off.

This simple control logic allows the streetlight to automatically react to environmental lighting conditions.

## Technical evaluetion 

During the analysis phase, I calculated the expected power consumption of the system to make sure if the breadboard power supply would be sufficient.

The estimated current consumption of the system is:

## Total Power Consumption Summary

| Component        | Power Consumption |
| ---------------- | ----------------- |
| **20x LEDs**     | 26 - 36 mA        |
| **Relay module** | 80 - 100 mA       |
| **LDR module**   | 15 mA             |
| **ESP32-S3**     | round about 50 mA |

The total current consumption is 145 – 182 mA but not every LED will be implemented on my ESP32-S3 so it will be 4x LEDs per teammate.

The breadboard power supply provided school supports up to 500 mA based on de specs I have read, which means the prototype fits within the safe operating range for Sprint 1 but It isn't future proof.

However, the analysis also showed that the system could become less stable if more modules are added in later sprints. Because of this, the design includes additional protection and stability components.

The external power supply line includes:

- a 1A fuse to prevent excessive current in case of short circuits
- a 1N4007 diode to protect against reverse polarity
- a 1000µF 25V capacitor to smooth voltage dips and spikes when the relay switches. I don't have it on my prototype because I got the wrong one and I need to do some more research on which is the correct one to use based on my design en logic.

This improve the prototype and reduce the risk of voltage drops and spikes that could reset the ESP32-S3.

## Design choice

One of the design choices in this prototype was to switch the full LED group ON/OFF with one relay module or other component if possible, instead of controlling multiple LEDs separately through different GPIO pins.

I chose this because of my beginner knowlegde the goal of the Sprint 1 smart streetlight is only automatic ON/OFF behaviour. Individual LED control is not necessary for now. By using one relay module, the ESP32-S3 only has to send one digital control signal, while the relay switches the complete LED group.

This has several advantages:

- it keeps the wiring simpler
- it reduces the number of GPIO pins needed
- it avoids drawing the LED load current directly from the ESP32-S3 pins
- it makes the design easier to understand for the rest of the team.

The LEDs are treated as one lamp group instead of many individually controlled. For a first working prototype, this is a logical and beginner-friendly solution for me but ofcourse it could be improved.