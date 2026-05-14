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

Document the steps you take. Be detailed: tools, datasets, parameters, prompts, code snippets, design sketches, and decisions. Include dead-ends—it shows learning.

## R — Result

Present the outcomes clearly and objectively. Include visuals or tables if helpful.

## R — Reflection

Analyze the meaning of the results. Connect back to your Learning Question and to theory/practice.

## T — Transfer

State how you will transfer this learning to new contexts. What’s next?

## Appendix (optional)

Detailed logs
Additional figures/tables
Full code/artefacts

## References

List all sources you used. Use a consistent citation style (e.g., APA/IEEE). Include URLs with access dates for web resources.

Author, A. (Year). Title. Publisher. [https://link](https://link) (accessed YYYY-MM-DD)

Organization. (Year). Title of webpage/report. [https://link](https://link) (accessed YYYY-MM-DD)

Dataset/Tool. Version. Provider. [https://link](https://link) (accessed YYYY-MM-DD)
