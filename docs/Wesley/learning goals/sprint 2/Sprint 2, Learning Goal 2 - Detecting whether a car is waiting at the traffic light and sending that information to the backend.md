# Sprint 2, Learning Goal 2 - Detecting whether a car is waiting at the traffic light and sending that information to the backend

## Learning Question

How can I make my traffic-light project recognize whether a car is waiting at the traffic light and send that information to the backend in a clear, reliable, and testable way?

## S — Situation

At the start of this learning goal, I already have a working traffic-light prototype. The system runs on an ESP32-S3 and controls the traffic lights in a fixed order. That means the traffic light can already switch between its states correctly, but it still follows the same sequence every time. At this point, the system does not yet react to what is happening around it. It does not know whether a car is actually waiting at the traffic light, and it does not share information with the backend.

That is the main limitation of the current situation. The traffic light works, but it behaves like every traffic situation is the same. In practice, that is not how traffic works. Sometimes a car is waiting, and sometimes there is no traffic at all. If the system cannot notice that difference, it cannot become smarter or more useful. Right now, I already have the traffic-light setup itself, the controller, and the basic behaviour working. What I want to add in this step is the ability to notice whether a car is present and to pass that information on to the backend.

This is important because it moves the project from a fixed local prototype toward a system that can respond to real situations. In practice, that means the traffic light is no longer only following a pre-set pattern, but starts collecting useful input from the outside world. That is an important step if I want to build a smarter traffic-light system later on.


## T — Task

For this learning goal, my task is to expand the traffic-light project so that it can detect whether a car is waiting at the traffic light and send that information to the backend. The purpose of this step is not only to add a new technical feature, but to make the project more meaningful in practice. I want to solve the problem that the current traffic light has no awareness of what is happening nearby and therefore cannot yet support smarter decisions.

To do that, I will first work out what kind of information I need to detect and what the system should do with that information. After that, I will prepare how this detection is connected to the ESP32-S3 and how the information is passed on to the backend in a clear and usable way. I will then build and test the setup so I can check whether the traffic-light project can correctly recognize the situation at the traffic light and whether that information arrives properly at the backend. The goal is to create a working flow in which the project can notice whether a car is present and communicate that clearly, reliably, and in a way that can be tested.

### Deliverables

**1. Analysis document**

In this document, I explain what kind of input is relevant for this iteration and what the system needs to recognize in order to know whether a car is waiting at the traffic light. I describe what information needs to be read, how often that information should be checked and sent, what the communication between the ESP32-S3 and the backend needs to support, which risks can occur when the detected values are unstable or when communication fails, and when I can say that this part of the project works correctly.

**2. Design document**

In this document, I show how the detection part is connected and how it fits into the existing traffic-light project. I explain which pins and components I use, how the detected information moves through the embedded application, how the message to the backend is structured, which communication method is used, and how the system should deal with missing, unclear, or incorrect information.

**3. Realise document**

In this document, I record how I build and test the setup on the breadboard, how I verify that the ESP32-S3 correctly reads whether a car is present, how I send that information from the ESP32-S3 to the backend, how I transfer the working setup into the project tile, and which tests show that the information is sent correctly and reliably.

**4. Advice document**

In this document, I evaluate whether the chosen setup is suitable for detecting whether a car is waiting at the traffic light, whether the way of sending the information to the backend is reliable enough, which problems or limitations I encountered during testing, and what I would improve in the next iteration of the connected traffic-light system.

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

