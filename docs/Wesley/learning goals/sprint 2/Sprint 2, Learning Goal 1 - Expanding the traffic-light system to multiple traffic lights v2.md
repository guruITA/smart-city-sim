# Sprint 2 — Learning Goal 1: Expanding the traffic-light system to multiple traffic lights

## Learning Question

How can I turn my first simple traffic light into a small crossroads system with multiple traffic lights that work together safely and clearly?

## S — Situation

At the start of Sprint 2, I already have one working traffic light. I built it with an ESP32-S3, three LEDs, and one resistor for each LED. The system starts safely on red, changes in a fixed order from red to green to yellow, and repeats that cycle correctly. I tested it in simulation and on real hardware, so I know the first version works.

That first version helped me understand the basics: how to connect the lights, how to control them, and how to make sure the system stays safe by never turning on conflicting lights at the same time. Right now, the setup only represents one traffic light. That is fine for learning the basics, but it does not yet reflect a real traffic situation. A real crossroads has more than one traffic light, and those lights must work together.

In this sprint, I want to take the next step by expanding my project from one traffic light to multiple traffic lights. The reason is practical: traffic lights are not useful on their own at a crossroads. They need to be coordinated so road users can clearly understand when they must stop and when they may go. If that coordination is wrong, the situation becomes confusing or unsafe. By adding multiple traffic lights, I move from a simple demo to a more realistic traffic situation.

## T — Task

For this learning goal, my task is to turn my first working traffic light into a small crossroads system with multiple traffic lights that can work together properly. The purpose of this step is not only to add more lights, but to make sure the full situation remains understandable and safe. I want to solve the practical problem that one traffic light on its own does not yet show how traffic control works in a real environment.

To do that, I will first work out what the expanded system needs to do and what rules it has to follow. After that, I will design how the traffic lights should be connected and how they should take turns. I will then build and test the setup, first on the breadboard and later in the project tile. During that process, I will check whether the lights work together in a logical way and whether unsafe combinations are prevented. The goal is to create a working multi-traffic-light system that is not only technically correct, but also makes sense from the perspective of real traffic use.

### Deliverables

1. Analysis document

In this document, I explain what I am adding in this second version of the project and what the crossroads system should do in practice. I describe the most important safety rules that the system has to follow so that the traffic lights work together in a clear and safe way. I also explain what I see as a correct result for this step and which limits I still have in my current hardware setup.

2. Design document

In this document, I show how the multiple traffic lights are connected and which pins and components I use for the full setup. I explain how the traffic lights are grouped and how they take turns during the traffic cycle. I also include a diagram or table that makes clear how the full system works as one coordinated setup.

3. Realisation document

In this document, I record how I first build the setup on a breadboard and how I test whether the traffic lights work together correctly. I describe how I transfer the working setup into the project tile and which code I use to control the different traffic lights. I also show the test results that prove the system works safely and as intended.

4. Advice document

In this document, I evaluate whether the system is easy to understand and whether the chosen structure is good enough for the next sprint. I reflect on the problems or limitations I ran into during this step and explain what I would improve before continuing with smarter behaviour in a later iteration.

## A — Action

I worked on this learning goal in four clear steps.

First, I made the **analysis deliverable**. In that document, I examined how I could expand my first simple traffic light into a four-way crossing with multiple traffic lights, while still keeping the system understandable, safe, and suitable for later extension. I compared possible approaches and chose the direction with an **ESP32-S3, MCP23017, two ULN2803 chips, and external LED power**, because that structure uses fewer controller pins, avoids multiplexing, and leaves room for later sensors and backend control.

After that, I made the **design deliverable**. In that document, I translated the analysis into a concrete design for the crossroads system. I worked out the grouping of the traffic lights into **North/South** and **East/West**, defined the coordinated traffic cycle, described the channel mapping, and documented the full hardware structure from controller to lamp output. I also made clear how the design stays safe by preventing conflicting green states and by using all-red transition phases.

Next, I worked on the **realisation phase**. I first used staged tests during building, starting with serial communication, then I2C communication, then standalone testing of the MCP23017, standalone testing of the ULN2803, and then the combined signal path. After that, I built the full four-way setup on the breadboard and tested whether the system started safely, followed the correct sequence, avoided conflicting green states, and cycled stably. In this phase, I also improved the software by replacing `delay()` with a `millis()` based state machine so the code is more suitable for later integration with other parts of the project.

Finally, I made the **advice deliverable**. In that document, I looked back on the choices I made and described what I should keep for the next sprint. My advice was to keep the current hardware architecture, keep the simplified crossroads model, continue with the `millis()` based approach, keep testing in stages, and add only one smart feature first in the next iteration.

## R — Result


This learning goal resulted in a set of deliverables that together document the full step from first prototype to a coordinated multi-traffic-light system.

The first result was my **Analysis - Expanding the Traffic-Light Prototype to a Four-Way Crossing**. In this document, I showed why the first one-pin-per-LED setup was no longer suitable for a full crossing and why the chosen solution with **ESP32-S3 -> MCP23017 -> 2x ULN2803 -> external LED power supply -> 4 traffic lights** was the best fit for this sprint.

The second result was my **Design - Four-Way Traffic-Light Crossing with Low Pin Usage and External LED Power**. In this document, I worked out the concrete design of the four-way crossing, including the hardware architecture, traffic-light grouping, coordinated cycle, output mapping, and design proof with schematic and breadboard views.

The third result was my **Realisation - Developing and Using Tests During Realisation for MCP23017 and ULN2803 Integration**. In this document, I recorded the staged tests I created and used during the building process, so I could verify each part before expanding to the full system. 

The fourth result was my **Realisation - Breadboard Realisation of the Four-Way Traffic-Light Setup**. In this document, I showed the complete breadboard implementation of the designed system, including the realised mapping, the breadboard build process, the `millis()` based control approach, and the final test results. Those tests showed that the setup started safely, followed the intended sequence, avoided conflicting green states, and cycled stably. 

The fifth result was my **Advice — Learning Goal 1 - Expanding the traffic-light system to multiple traffic lights**. In this document, I evaluated the chosen structure and described what I should carry into the next sprint, such as keeping the current hardware structure, keeping the simplified crossroads model, and adding only one smart feature first.

Together, these results show that I completed the learning goal by moving from analysis, to design, to realisation, to advice, and by documenting each of those steps in a separate deliverable. This matches the structure that I originally described in the learning goal. 


## R — Reflection

This learning goal helped me answer my learning question, because I did not stay at one simple traffic light. I expanded the project into a small crossroads system with multiple traffic lights that work together in a coordinated and safe way. In the analysis and design, I worked out that a direct one-pin-per-LED approach was no longer a good fit for a four-way crossing, and that I needed a structure that used fewer controller pins, kept the outputs stable, and left room for later extension. That is why I chose the setup with the ESP32-S3, MCP23017, two ULN2803 chips, and external LED power.

The most important meaning of the result is that the project became more realistic. Instead of one isolated traffic light, I now have a system that behaves as one coordinated crossing with safe phase changes, all-red transitions, and no conflicting green states. The breadboard tests showed that the setup starts safely, follows the intended sequence, avoids unsafe combinations, and keeps cycling stably. That means the result is not only correct on paper, but also works in practice.

Another important reflection is that scaling the project was not only about adding more LEDs. I learned that expanding the system required a better structure in both hardware and software. The realised setup showed that the traffic-light system can be built in a modular way, because the ESP32-S3 handles the logic, the MCP23017 expands the outputs, and the ULN2803 chips handle the switching. I also learned that replacing `delay()` with `millis()` was an important improvement, because it makes the code more suitable for later integration with sensors or backend communication.

A final reflection is that the staged way of working helped me a lot. I did not build everything at once, but first tested serial communication, then I2C, then the MCP23017, then the ULN2803, and then the combined signal path before expanding to the full setup. That made the building process more controlled and reduced confusion during debugging. Because of that, this learning goal also taught me that good structure in testing is just as important as good structure in the design itself.


## T — Transfer

I will transfer this learning to the next sprint by keeping the current hardware architecture as the base for further development. The current setup already solves the main scaling problem of the first prototype, because it uses fewer controller pins, separates logic from switching and power, and is easier to extend later. That means I do not need to redesign the whole hardware concept again, but can continue building on what already works.

I will also transfer this learning by keeping the simplified crossroads model and the `millis()` based state machine. The simplified grouping of North/South and East/West is still the right level for this project stage, because it keeps the system understandable while already behaving like a real coordinated crossing. The `millis()` based approach is also something I will keep, because it is more suitable than `delay()` when I later add sensors, backend communication, or other modules to the same ESP32-S3.

For the next context, I will apply the same staged testing method again. If I add a new input such as vehicle detection or a pedestrian request, I will first test that part on its own, then test how it affects the logic, and only after that connect it to the full traffic-light system. In that way, I can reuse the same step-by-step workflow that already helped me during this sprint.

My next step is therefore not to add many new features at once, but to make one controlled step toward smarter behaviour. Based on my advice deliverable, the best transfer of this learning is to keep the current structure, make the software more modular, freeze one final mapping, and then add only one smart feature first, for example vehicle detection for one road direction or a stored pedestrian request. That is how I will use what I learned here in a new context without making the project unnecessarily complex too quickly.


## Appendix

The appendix for this learning goal consists of the separate deliverables and their included proof material, such as mapping tables, staged tests, breadboard implementation details, schematic and breadboard design views, and advice for the next sprint.

## References

Wesley. (2026). *Sprint 2, learning goal 1: Expanding the traffic-light system to multiple traffic lights* [Learning goal document].

Wesley. (2026). *Analysis: Expanding the traffic-light prototype to a four-way crossing* [Analysis deliverable].

Wesley. (2026). *Design: Four-way traffic-light crossing with low pin usage and external LED power* [Design deliverable].

Wesley. (2026). *Realisation: Developing and using tests during realisation for MCP23017 and ULN2803 integration* [Realisation deliverable].

Wesley. (2026). *Realisation: Breadboard realisation of the four-way traffic-light setup* [Realisation deliverable].

Wesley. (2026). *Advice: Learning goal 1 - Expanding the traffic-light system to multiple traffic lights* [Advice deliverable].

