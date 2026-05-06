# Analysis - Recognizing when a car is waiting at the traffic light and sending that information to the backend

## Summary

This analysis shows that the main challenge in this sprint is not only detecting a vehicle, but correctly interpreting what that detection means in the context of the traffic light. The project has already moved beyond a single simple traffic light and now uses a coordinated four-way crossing with safe startup, fixed phase order, all-red transitions, no conflicting green states, and a non-blocking millis() based controller. Because that foundation is already working, the next step is to make the system respond to traffic input instead of only following a fixed cycle.

For this learning goal, the system must determine whether a car is actually waiting at the traffic light and send that interpreted result to the backend. A simple raw sensor trigger is not enough, because the same signal could represent either a waiting vehicle or a passing vehicle. In this analysis, the relevant interpreted states are defined as no vehicle, waiting vehicle, passing vehicle, and unclear input. A stable active signal during red is treated as a waiting car, while a short signal during green is treated as passing traffic.

The chosen prototype direction uses the KY-021 as a simplified sensor so the focus remains on the logic of interpretation rather than on complex hardware. The backend should receive a meaningful message that includes the sensor, the current phase, the interpreted state, the time of change, and whether the reading was accepted as valid. The most important requirement is that sensor logic must never break the safe timing and safe phase transitions of the traffic-light controller. A correct result for this sprint is therefore a prototype that can detect presence, classify it meaningfully, send that interpretation to the backend, and remain safe when input is missing, unstable, or unclear.

## Table of Contents

- [Introduction](#introduction)
- [Current Situation](#current-situation)
- [Problem Definition](#problem-definition)
- [What the System Should Do in Practice](#what-the-system-should-do-in-practice)
- [Interpretation Rules Used in This Analysis](#interpretation-rules-used-in-this-analysis)
- [Why a Simple Sensor Trigger Is Not Enough](#why-a-simple-sensor-trigger-is-not-enough)
- [Chosen Prototype Direction](#chosen-prototype-direction)
- [What Information Must Be Sent to the Backend](#what-information-must-be-sent-to-the-backend)
- [Important Risks](#important-risks)
- [Current Limits](#current-limits)
- [Acceptance Criteria for This Step](#acceptance-criteria-for-this-step)
  - [AC1. The sensor detects vehicle presence at the stop line correctly](#ac1-the-sensor-detects-vehicle-presence-at-the-stop-line-correctly)
  - [AC2. A stable detection during red is interpreted as a waiting car](#ac2-a-stable-detection-during-red-is-interpreted-as-a-waiting-car)
  - [AC3. A short detection during green is interpreted as passing traffic and not as a waiting car](#ac3-a-short-detection-during-green-is-interpreted-as-passing-traffic-and-not-as-a-waiting-car)
  - [AC4. The interpreted traffic state is sent correctly to the backend](#ac4-the-interpreted-traffic-state-is-sent-correctly-to-the-backend)
  - [AC5. Invalid or unstable input does not create unsafe traffic-light behaviour](#ac5-invalid-or-unstable-input-does-not-create-unsafe-traffic-light-behaviour)
  - [AC6. The system stays within the safe timing limits of the project](#ac6-the-system-stays-within-the-safe-timing-limits-of-the-project)
- [What Counts as a Correct Result in This Step](#what-counts-as-a-correct-result-in-this-step)
- [Conclusion](#conclusion)
- [References](#references)




## Introduction

At the start of this sprint, I was no longer at the stage of only one simple traffic light. Before this learning goal, I had already completed the first physical traffic-light prototype and then expanded that prototype in Sprint 2 into a coordinated four-way crossing with multiple traffic lights that work together safely and clearly. The first version already proved the basic traffic-light logic, safe startup in red, fixed state order, stable timing, and repeated cycling on real hardware (Wesley, 2026e).  

In Sprint 2, I then developed the project from analysis to design, realisation, and advice. That resulted in a coordinated crossing based on an ESP32-S3, an MCP23017, two ULN2803 chips, and external LED power. That step solved the scaling problem of the first prototype and created a better structure for later extension with sensors, backend communication, and smarter behaviour (Wesley, 2026a; Wesley, 2026d).  

The realised Sprint 2 setup already showed safe startup, the intended phase sequence, all-red transitions, and no conflicting green states. I also already changed the software from `delay()` to a `millis()` based state machine so the controller would be easier to combine later with sensors, backend logic, and other modules on the same ESP32-S3 (Wesley, 2026b; Wesley, 2026f).   

That means the next challenge is no longer how to make a traffic light work at all, and also no longer how to scale it to a multi-light crossroads. Those parts were already completed earlier. The next step is to make that coordinated crossing respond to traffic input. In my wider project structure, this is the stage where the system starts moving from a fixed-cycle traffic light toward a smart traffic light that reacts to real-time input from its environment (Wesley, 2026c).  

## Current Situation

The current system is already more advanced than the first prototype. It is no longer one standalone traffic light with three direct LED outputs. At this stage, I already have a coordinated four-way setup that groups compatible directions together and controls them as one crossing instead of as separate loose traffic lights (Wesley, 2026d).  

The hardware structure already separates controller logic, output expansion, and output switching. The ESP32-S3 handles the logic, the MCP23017 expands the outputs over I2C, and the ULN2803 chips switch the LED channels that use external power. I chose that structure in Sprint 2 because the earlier one-pin-per-LED design no longer scaled well to a full crossing and left too little room for later extension (Wesley, 2026a; Wesley, 2026d).  

The realised setup already proved that the crossing starts safely, follows the intended traffic-light sequence, avoids conflicting green states, and cycles stably over time. The software was also already improved to a `millis()` based state machine, because blocking `delay()` timing would create problems in later integration with sensors or backend logic (Wesley, 2026b; Wesley, 2026f).  

So the main limitation of the current version is no longer the traffic-light coordination itself. The remaining limitation is that the crossing still behaves as a fixed-cycle controller. It can already switch multiple traffic lights safely, but it does not yet recognise whether a car is waiting at the stop line and it does not yet send that interpreted situation to the backend as meaningful traffic input. That missing step matches the next smart stage in the wider project, where vehicle detection becomes an input for traffic-light decisions (Wesley, 2026c).  

## Problem Definition

The main problem in this learning goal is not only how to detect that something is present near the traffic light, but also how to interpret that signal correctly. In the wider project, the second smart step is sensor-based vehicle detection. That step is not only about reading a sensor, but about using vehicle detection as input for traffic-light decisions, keeping the timing within safe limits, and continuing safely if no valid sensor input is available (Wesley, 2026c).  

For this project, the solution therefore has to meet several conditions. It must be feasible in the prototype, allow the controller to detect presence near the stop line, distinguish between waiting and passing traffic, send meaningful information to the backend instead of only a raw trigger, remain understandable as a learning project, and remain safe if the input is missing, unstable, or unclear.

So the real problem is not only sensing. The real problem is turning a simple sensor event into a traffic meaning that the rest of the system can actually use.

## What the System Should Do in Practice

In practice, the system should not only notice that the sensor changed. It should recognise a traffic situation that matters to the traffic light. For this learning goal, the central practical question is whether there is a car waiting at the traffic light, yes or no.

That means the output of this sprint should become an interpreted traffic state rather than only a raw sensor value. This follows the wider project direction in which the traffic light starts becoming smart when it uses real-time input for decisions instead of only following a fixed timer (Wesley, 2026c).  

For this analysis, I define the relevant interpreted states as:

* no vehicle
* waiting vehicle
* passing vehicle
* unclear input

I use these categories because the goal of this sprint is to add meaningful road-input interpretation, not only raw detection.

## Interpretation Rules Used in This Analysis

The interpretation rules below are definitions for this project analysis. They are not presented as external traffic-engineering laws, but as the rule set I will use to make the prototype behaviour consistent and testable.

For this learning goal, I define a stable active signal during red as waiting traffic. I define a short signal during green as passing traffic rather than waiting traffic. I treat signals during yellow or phase changes as less certain and therefore requiring extra context before they are accepted as a waiting-car state.

This fits the wider project requirement that sensor input must be used meaningfully and safely, not in a way that creates unsafe or confusing behaviour (Wesley, 2026c).  

## Why a Simple Sensor Trigger Is Not Enough

A simple trigger is not enough in this project because the same raw signal can belong to different traffic situations. If the system only checks whether the sensor is active, then a car moving across the sensor during green could be misread as a waiting car. That would send the backend the wrong meaning and reduce the value of the smart part of the system.

For that reason, this sprint focuses on interpretation as well as detection. That also matches the wider project direction, where a traffic light becomes smart when it reacts meaningfully to real-time input rather than only following a fixed timer (Wesley, 2026c).  

## Chosen Prototype Direction

For this learning goal, I chose the KY-021 as the prototype sensor. In this project, I use it as a simplified stand-in for vehicle presence so I can focus first on the logic of recognising and interpreting waiting traffic before making the hardware more complex.

This also fits the staged development of the wider project. My project is deliberately built up in steps: first a basic traffic light, then sensor-based vehicle detection, then pedestrian behaviour, and only after that more advanced smart functions. That makes the project manageable and keeps the scope suitable for a learning project (Wesley, 2026c; Wesley, 2026f).   

## What Information Must Be Sent to the Backend

For this step, the backend does not need a large amount of data. It needs a small amount of information that it can interpret consistently.

In this prototype, the most useful backend message is one that includes:

* which sensor triggered
* the current traffic-light phase
* the interpreted traffic state
* the time of the state change
* whether the reading was accepted as valid

This is better than sending only an active or inactive sensor value, because the backend then receives a state with meaning instead of having to infer the situation from a raw hardware signal.

## Important Risks

There are several important risks in this step. Passing traffic may be mistaken for waiting traffic if the project only checks whether the sensor is active. A real waiting car may be missed if the signal is too short, the placement is poor, or the input is unstable. Repeated or unclear triggers may also create noisy backend messages if every change is treated as equally meaningful.

There is also a project-level safety risk. In the wider smart-traffic-light plan, sensor input must not create unsafe signal transitions, and the system must continue safely if no valid input is available. That means the sensor logic may influence interpretation, but it must not break the safe timing structure or the safe phase transitions of the traffic-light controller (Wesley, 2026c).  

A further practical risk is integration with the existing controller. In Sprint 2, I already concluded that timing design and staged testing matter when different modules are combined. Because of that, I should continue using non-blocking timing, keep the safety rules central, and keep testing new behaviour in small stages instead of adding everything at once (Wesley, 2026b; Wesley, 2026f).   

## Current Limits

At this moment, my project is already at the stage of a working coordinated four-way crossing, but it is still not yet a fully smart traffic light. My own Sprint 2 advice already states that the current system should be treated as a strong intermediate version and as the base for the next step, not as the finished smart version (Wesley, 2026f).  

The current setup is still a fixed-cycle crossroads controller. Even though the hardware structure is much stronger than the first prototype, it does not yet include vehicle detection, pedestrian requests, emergency priority, adaptive traffic control, or backend-driven behaviour. My own advice for the next step was therefore to keep the current architecture, keep the simplified crossroads model, and add only one smart feature first, such as vehicle detection for one direction (Wesley, 2026f).  

The scope of this learning goal is therefore deliberately limited. It focuses only on recognising whether a car is waiting and sending that interpreted state to the backend. It does not yet include queue length, multi-car detection, full adaptive control, or more advanced road-user behaviour, because those belong to later iterations in the project structure (Wesley, 2026c).  

## Acceptance Criteria for This Step

For this analysis, I define the following acceptance criteria for the prototype.

### AC1. The sensor detects vehicle presence at the stop line correctly

The system must be able to register when a vehicle is present at the detection point. This fits the wider project requirement that the second smart iteration must detect vehicle presence correctly (Wesley, 2026c). 

### AC2. A stable detection during red is interpreted as a waiting car

If the light is red and the signal remains stable, the system must classify that as waiting traffic.

### AC3. A short detection during green is interpreted as passing traffic and not as a waiting car

If the light is green and the signal is only active briefly, the system must not create a waiting-car state.

### AC4. The interpreted traffic state is sent correctly to the backend

The backend must receive a meaningful traffic state such as waiting, passing, no vehicle, or unclear input.

### AC5. Invalid or unstable input does not create unsafe traffic-light behaviour

If the input is missing, unstable, or unclear, the system must remain safe. This fits the wider project requirement that the traffic light must continue safely if no valid input is available (Wesley, 2026c).  

### AC6. The system stays within the safe timing limits of the project

Vehicle detection may influence interpretation, but the system must still remain inside the safe timing structure already defined in the wider project (Wesley, 2026c).  

## What Counts as a Correct Result in This Step

For this step, I consider the result correct when the system can detect a vehicle at the stop line, interpret stable red-phase presence as waiting traffic, avoid misclassifying short green-phase passing traffic as waiting, send the interpreted state clearly to the backend, and remain safe when input is unclear or missing.

This sprint does not yet need to make the system fully smart. Instead, it needs to deliver the first meaningful step from a fixed-cycle coordinated crossing toward a traffic light that reacts to road input. That also fits the staged project structure in which vehicle detection is the first smart feature added after the crossing itself already works (Wesley, 2026c; Wesley, 2026f).  

## Conclusion

The main conclusion of this analysis is that the challenge in this sprint is not only sensor detection, but sensor interpretation. For this learning goal, the project should not define a waiting car as only “sensor active.” It should define a waiting car as a stable presence at the stop line interpreted together with the current traffic-light phase.

That means the controller should combine sensor state, signal duration, and traffic-light phase before sending a result to the backend. In the context of the broader project, that is the first practical step from a fixed coordinated crossing toward smarter traffic-light behaviour based on real-time input (Wesley, 2026c).  

## References

ESP32-S3-DEVKITC-1 v1.1. (n.d.). espressif.com. https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32s3/esp32-s3-devkitc-1/user_guide_v1.1.html

Wesley. (2026a). *Analysis - Expanding the traffic-light prototype to a four-way crossing v2* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/learning%20outcomes/sprint%202/Analysis%20-%20Expanding%20the%20Traffic-Light%20Prototype%20to%20a%20Four-Way%20Crossing%20v2.md

Wesley. (2026b). *Realisation - Breadboard realisation of the four-way traffic-light setup* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/learning%20outcomes/sprint%202/Realisation%20-%20Breadboard%20Realisation%20of%20the%20Four-Way%20Traffic-Light%20Setup.md

Wesley. (2026c). *Smart traffic light - When does a traffic light become smart?* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/research/Smart%20Traffic%20Light%20-%20When%20Does%20a%20Traffic%20Light%20Become%20Smart.md

Wesley. (2026d). *Sprint 2, learning goal 1: Expanding the traffic-light system to multiple traffic lights* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/learning%20goals/sprint%202/Sprint%202,%20Learning%20Goal%201%20-%20Expanding%20the%20traffic-light%20system%20to%20multiple%20traffic%20lights.md

Wesley. (2026e). *Traffic Light - first iteration - implementation* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/research/Traffic%20Light%20-%20first%20iteration%20-%20implementation.md

Wesley. (2026f). *Advice — Learning Goal 1 - Expanding the traffic-light system to multiple traffic lights* https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/8febfd00c6ceb887593016ed8d4fcd316f0c3ac8/docs/Wesley/learning%20outcomes/sprint%202/Advice%20%E2%80%94%20Learning%20Goal%201%20-%20Expanding%20the%20traffic-light%20system%20to%20multiple%20traffic%20lights.md

