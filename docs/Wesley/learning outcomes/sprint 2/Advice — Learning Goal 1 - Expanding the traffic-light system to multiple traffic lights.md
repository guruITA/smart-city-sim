# Advice — Learning Goal 1 - Expanding the traffic-light system to multiple traffic lights


## Table of Contents

- [1. Introduction](#1-introduction)
- [2. Advice based on this sprint](#2-advice-based-on-this-sprint)
  - [2.1 I should keep the current hardware structure](#21-i-should-keep-the-current-hardware-structure)
  - [2.2 I should keep the simplified crossroads model](#22-i-should-keep-the-simplified-crossroads-model)
  - [2.3 I should keep the `millis()` based state machine](#23-i-should-keep-the-millis-based-state-machine)
  - [2.4 I should make the software more modular before adding too much new behaviour](#24-i-should-make-the-software-more-modular-before-adding-too-much-new-behaviour)
  - [2.5 I should freeze one final mapping before continuing](#25-i-should-freeze-one-final-mapping-before-continuing)
  - [2.6 I should add only one smart feature first](#26-i-should-add-only-one-smart-feature-first)
  - [2.7 I should keep safety rules central in every next step](#27-i-should-keep-safety-rules-central-in-every-next-step)
  - [2.8 I should keep using staged tests](#28-i-should-keep-using-staged-tests)
- [3. Advice based on the current limitations](#3-advice-based-on-the-current-limitations)
- [4. Advice I take into the next sprint](#4-advice-i-take-into-the-next-sprint)
- [5. Advice for my next step](#5-advice-for-my-next-step)
- [6. Conclusion](#6-conclusion)
- [References](#references)


## 1. Introduction

In this advice document, I look back at Learning Goal 1 of Sprint 2. In this sprint, I expanded my first simple traffic light into a small crossroads system with multiple traffic lights that work together safely and clearly.

In the earlier deliverables, I already described the analysis, design, and realisation of that system. In this document, I focus on the advice I take from that work for the next sprint. I explain what I should keep, what I should improve, and what I should be careful with before I continue toward smarter traffic-light behaviour.

The purpose of this document is to show how I judge my own chosen structure after building and testing it, and what I think is the best next step for the project.

## 2. Advice based on this sprint

### 2.1 I should keep the current hardware structure

My first advice is that I should keep the current hardware structure for the next sprint.

In this sprint, I moved from one simple traffic light with direct GPIO outputs to a four-way setup based on:

* ESP32-S3
* MCP23017
* two ULN2803 chips
* external LED power

For this project, I think that was the right decision. The first prototype was useful for learning the basic traffic-light logic, but it was not suitable for scaling up to a full crossing with twelve lamp channels. The new structure solves that problem much better.

By using the MCP23017, I need fewer ESP32 pins. By using the ULN2803 chips and external LED power, I also avoid having the ESP32 directly handle the lamp current. That makes the setup more realistic and also gives me more room for later expansion.

Because of that, I do not think I should redesign the hardware concept again in the next sprint. The current structure is already a good enough base to continue with.

### 2.2 I should keep the simplified crossroads model

My second advice is that I should keep the current simplified traffic model while adding the next feature.

In this sprint, I grouped the crossing into two compatible directions:

* North and South
* East and West

That made the traffic cycle much easier to understand and test. The phase order stayed clear:

1. all red
2. North/South green
3. North/South yellow
4. all red
5. East/West green
6. East/West yellow

For this stage of the project, I think that is the correct level. It already represents a real coordinated crossing better than my first single-light prototype, but it is still simple enough to manage.

Because of that, my advice is not to make the crossing more complicated too early. I should not immediately add turning lanes, pedestrian phases, emergency priority, and other advanced behaviour all at once. First, I should keep this clear base stable and then extend it step by step.

### 2.3 I should keep the `millis()` based state machine

One of the most important improvements in this sprint was changing from `delay()` to a `millis()` based state machine.

My advice is to definitely keep that approach in the next sprint.

The reason is simple. In the first prototype, `delay()` was acceptable because the project was very small. In the expanded version, that becomes less practical. If I later add sensors, pedestrian input, backend commands, or logging, then blocking the controller with long delays would create problems.

By using `millis()`, the program can keep running while the traffic-light timing is active. That makes the code more suitable for combination with other features later. So for the next sprint, I should continue building on this non-blocking structure.

### 2.4 I should make the software more modular before adding too much new behaviour

Even though the current code works, my advice is that I should make the software structure clearer before I add too many extra features.

At this moment, the project already contains multiple parts:

* traffic-light phase logic
* timing logic
* output mapping
* I2C communication
* switching through the MCP23017 and ULN2803

That is still manageable now, but it will become harder if I start adding smart behaviour without separating responsibilities more clearly.

For the next sprint, I think I should separate the software more into parts such as:

* input handling
* decision logic
* output control

That would make it easier to add new behaviour without making the code confusing or breaking the parts that already work.

### 2.5 I should freeze one final mapping before continuing

A practical advice for myself is that I should create one final version of the real pin and channel mapping before I continue.

During the project, some details changed between design and realisation, which is normal while building and testing. But before I continue, I need one final overview that clearly shows:

* which ESP32 pins I use
* which pins are SDA and SCL
* which MCP23017 outputs belong to which traffic-light lamps
* which code names match which physical channels

That will make the project easier to understand later and will reduce confusion when I add new modules or update the code.

### 2.6 I should add only one smart feature first

The current system is a coordinated multi-traffic-light setup, but it is still based on a fixed cycle. That means it is a better and more realistic version than the first prototype, but it is not yet truly smart.

Because of that, my advice is that in the next sprint I should add only one smart feature first.

The best next step would be something like:

* vehicle detection for one road direction

or

* a stored pedestrian request

That would be enough to make the system more responsive to real input, without making the project too complex too quickly.

If I try to add too many smart features at once, I increase the chance that the code, wiring, and testing become harder to manage. So my advice is to keep the next step small and controlled.

### 2.7 I should keep safety rules central in every next step

A very important advice for the next sprint is that I should keep the safety rules central in both the code and the testing.

In this sprint, the system worked because the basic traffic safety remained clear:

* conflicting directions never become green at the same time
* yellow is only used as a transition phase
* all-red is used between incompatible directions
* the system starts safely

As I add smarter behaviour later, it becomes even more important that these rules stay protected. Smarter behaviour should never make the system less safe.

So in the next sprint, every added feature should still be checked against these same core safety rules.

### 2.8 I should keep using staged tests

Another advice I want to keep for myself is the way I tested during this sprint.

Instead of building and debugging everything at once, I tested the project in smaller steps:

1. serial communication
2. I2C communication
3. MCP23017 behaviour
4. ULN2803 behaviour
5. combined signal path
6. full breadboard setup

That worked well, because it helped me isolate problems more easily and gave me more confidence before I moved to the next stage.

For the next sprint, I should use the same method again. If I add a sensor or another input, I should first test that part on its own, then test how it affects the logic, and only after that connect it to the full traffic-light behaviour.

## 3. Advice based on the current limitations

This sprint gave me a good intermediate version, but it also showed me a few limits that I should keep in mind when I continue.

My first advice is that I should stay realistic about what the current system already is and what it is not yet. At this moment, it is a fixed-cycle crossroads controller with a much better hardware structure than my first prototype, but it is still not yet a fully smart traffic light. Because of that, I should use this version as the base for the next step, but I should not treat it as if the smart part is already finished.

My second advice is that I should remember that the realised setup is still a breadboard prototype. It works in practice, which is important, but it is still more vulnerable to wiring mistakes and layout issues than a cleaner final build. Because of that, I should keep my next hardware changes controlled and avoid making the setup unnecessarily messy.

My third advice is that I should keep the scope under control. The current model does not yet include turning lanes, pedestrians, emergency vehicles, adaptive traffic control, or backend-driven behaviour. That is acceptable for this sprint, but it also means I should add new behaviour in small steps instead of trying to solve everything at once.

My fourth advice is that I should keep my documentation aligned with the actual build. Now that the project has become more complex, it is more important that the mapping, code, and implementation still match each other clearly. If I let those drift apart, the next sprint will become harder than necessary.

## 4. Advice I take into the next sprint

The most important advice I take into the next sprint is that scaling a project is not only about adding more outputs. It also means I need a better structure.

In the first prototype, one ESP32 with three LEDs was enough. In the expanded version, that direct approach was no longer a good fit. My advice for the next sprint is therefore to keep separating controller logic, output expansion, switching, and power. That structure made the project more manageable, so I should continue building on it instead of going back to a more direct but less scalable design.

A second advice I take into the next sprint is that timing design should stay an important part of my implementation choices. `delay()` was simple, but it would become a problem when combining this system with future sensors or backend logic. Because of that, I should continue using the `millis()` based approach and treat non-blocking timing as a requirement for later features.

A third advice I take into the next sprint is that staged testing should stay part of my way of working. Testing one part at a time helped me reduce confusion and solve problems earlier. Because of that, I should use the same method again for the next added feature instead of waiting until the whole system is built before testing.

## 5. Advice for my next step

Based on this sprint, my advice for my own next step is:

* keep the current hardware architecture
* keep the simplified crossroads model
* keep the `millis()` based state machine
* make the software more modular
* freeze one final mapping document
* add only one smart feature first
* keep safety rules central
* continue testing in stages

I think that is the best path forward because it builds on what already works instead of changing too much again.

## 6. Conclusion

My advice after this sprint is to treat the current four-way traffic-light system as a successful intermediate version and as the base for the next iteration.

The current setup is much stronger than the first simple prototype. It uses fewer controller pins, separates logic from switching and power, and gives me a clearer structure for future expansion. The move to a `millis()` based state machine also makes the software much more suitable for the next sprint.

At the same time, I should stay aware that this is still not the final smart-traffic version. Before continuing, I should make the current setup more consistent and stable by freezing the final mapping and by keeping the code structure clear.

After that, I should take one controlled next step by adding one real smart feature, while keeping the crossing safe and understandable.

## References

Wesley. *Sprint 2, Learning Goal 1 - Expanding the traffic-light system to multiple traffic lights.*

Wesley. *Analysis - Expanding the Traffic-Light Prototype to a Four-Way Crossing.*

Wesley. *Design - Four-Way Traffic-Light Crossing with Low Pin Usage and External LED Power.*

Wesley. *Realisation - Breadboard Realisation of the Four-Way Traffic-Light Setup.*

Wesley. *Realisation - Developing and Using Tests During Realisation for MCP23017 and ULN2803 Integration.*

Wesley. *Smart Traffic Light - When Does a Traffic Light Become Smart?*
