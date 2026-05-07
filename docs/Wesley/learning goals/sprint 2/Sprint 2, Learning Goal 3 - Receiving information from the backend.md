# Sprint 2, Learning Goal 3: Receiving information from the backend

## Learning Question

How can my traffic-light prototype receive information from the backend and translate that information into usable input for the embedded system?

## S — Situation

After adding the ability to send information from the traffic-light system to the backend, the next step is to make the communication two-way. In the first iteration, the traffic light was fully local and only followed a fixed programmed sequence. In Sprint 2, I want the prototype not only to send information outward, but also to receive information back from the backend.

This step is important because a connected traffic-light system should be able to receive updates, commands, or settings from another system. That means I now need to learn how the ESP32-S3 can receive data, how that data should be validated, and how I can integrate it into the local traffic-light software without making the behavior unclear or unreliable.

## T — Task

For this learning goal, I will investigate and prepare how the ESP32-S3 can receive information from the backend and how that information can be processed inside the traffic-light prototype. The main goal is to make backend input a controlled and testable part of the embedded system.

I will make this learning goal concrete by producing the following deliverables:

### Deliverables

**1. Analysis document**
A document in which I describe:

* what type of information the backend should send,
* what the embedded system needs to receive for this iteration,
* the communication requirements for receiving data,
* the risks of missing, invalid, delayed, or conflicting backend input,
* the acceptance criteria for correct data reception.

**2. Design document**
A document in which I show:

* the communication flow from backend to ESP32-S3,
* how incoming messages are structured,
* how backend input is validated before use,
* how received data is stored or processed in the application,
* how the software handles communication errors.

**3. Realise document**
A document in which I record:

* how I test receiving backend data on the breadboard setup,
* how I verify that the ESP32-S3 can interpret the received information correctly,
* how I connect the received input to the local software structure,
* how I transfer the working implementation into the project tile,
* what tests prove that backend information is received and processed correctly.

**4. Advice document**
A document in which I evaluate:

* whether the data-receiving mechanism is stable and understandable,
* whether the validation strategy is good enough,
* what issues occurred in communication or parsing,
* what should be improved before this mechanism is used in more advanced logic.

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

