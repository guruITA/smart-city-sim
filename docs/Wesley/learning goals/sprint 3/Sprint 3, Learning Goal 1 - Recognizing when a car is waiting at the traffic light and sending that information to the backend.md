# Sprint 3, Learning Goal 1 - Recognizing when a car is waiting at the traffic light and sending that information to the backend

## Learning Question

How can I make my traffic-light project notice when a car is waiting at the traffic light and send that information to the backend in a clear, reliable, and testable way?

## S — Situation

At the start of this learning goal, I already have a working traffic-light project. I have the controller, the traffic lights, and the basic switching logic working. The traffic light can already change between red, green, and yellow in the correct order and keep running on its own.

What is still missing is that the project does not notice what is happening at the road itself. If a car arrives and has to wait, my project does not know that. It treats every situation the same, whether there is traffic or not. That means the traffic light is still only following a fixed pattern.

In practice, that is a problem. A traffic light becomes more useful when it can respond to what is really happening instead of always acting as if the road is the same. In this step, I want to add the ability to recognize whether a car is waiting at the traffic light and pass that information on to the backend. That is important because it is the first step from a fixed prototype toward a traffic-light system that can react to real situations.

## T — Task

My task in this learning goal is to extend the current traffic-light project so it can recognize whether a car is waiting at the traffic light and send that information to the backend.

The main goal is not just to add another part to the project, but to solve a practical problem. Right now, my traffic light cannot tell the difference between an empty road and a road where a car is waiting. I want to change that.

To do this, I will first determine what the project must recognize before I can say that a car is waiting. After that, I will work out how this information can be read by the controller and passed on to the backend in a useful way. Then I will build and test the setup to check whether the project can correctly recognize the situation at the traffic light and whether that information arrives correctly at the backend.

The result I want is a working flow in which my project can notice that a car is waiting, share that clearly with the backend, and be tested properly.

### Deliverables

**1. Analysis document**

In this document, I explain what my project needs to recognize before I can say that a car is waiting at the traffic light. I describe the current situation, what information is important, how often this should be checked, what can go wrong if the reading is unclear or unstable, and when I can conclude that this part works correctly.

**2. Design document**

In this document, I show how this new part fits into the existing traffic-light project. I explain how the recognition of a waiting car is connected to the controller, how the information moves through the application, how it is sent to the backend, and how the system should handle unclear, missing, or incorrect input.

**3. Realise document**

In this document, I record how I build and test the setup. I explain how I check whether the project correctly recognizes that a car is waiting, how I verify that this information is sent to the backend, and which tests show that the full flow works in a reliable way.

**4. Advice document**

In this document, I evaluate whether my chosen solution is suitable for recognizing when a car is waiting at the traffic light and for sending that information to the backend. I describe the problems or limitations I found, what worked well, and what I would improve in the next iteration.

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

