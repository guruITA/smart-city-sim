# Sprint 2, Learning Goal 2: Adding sensors and sending sensor data to the backend

## Learning Question

How can I add sensors to my traffic-light prototype and send the measured data from the ESP32-S3 to a backend in a structured, reliable, and testable way?

## S — Situation

The first iteration of my traffic-light project was a fixed local system. The traffic light followed a pre-programmed sequence and did not use input from its environment. For Sprint 2, I want to move toward a smarter traffic-light system. A smart traffic-light system should not only run on fixed timing, but also respond to information from outside the controller itself.

That is why the next step is to add sensors and send the measured data to a backend. This is important because it changes the project from a standalone embedded prototype into a connected system that can provide live input to other parts of the solution. To do this properly, I need to decide what I want to measure, how I read that data on the ESP32-S3, and how I send it to the backend in a format that is understandable and usable.

## T — Task

For this learning goal, I will investigate and prepare how sensor input can be integrated into the traffic-light system and how the ESP32-S3 can send that data to the backend. The main goal is to create a working flow from sensor measurement to backend communication.

I will make this learning goal concrete by producing the following deliverables:

### Deliverables

**1. Analysis document**
A document in which I describe:

* which sensor or sensors are relevant for this iteration,
* what data needs to be measured,
* how often that data should be read and sent,
* the communication requirements between the ESP32-S3 and the backend,
* the risks related to unstable sensor values or failed communication,
* the acceptance criteria for successful sensor integration.

**2. Design document**
A document in which I show:

* the sensor wiring and pin mapping,
* how sensor data moves through the embedded application,
* the structure of the data message sent to the backend,
* the communication method used,
* the error handling for missing or invalid data.

**3. Realise document**
A document in which I record:

* how I build and test the sensor setup on the breadboard,
* how I verify that the ESP32-S3 reads the correct sensor values,
* how I send the data from the ESP32-S3 to the backend,
* how I transfer the working setup into the project tile,
* what tests prove that the sensor data is sent correctly.

**4. Advice document**
A document in which I evaluate:

* whether the chosen sensor setup is suitable for the traffic-light project,
* whether the communication method is reliable enough,
* what issues occurred during testing,
* what should be improved in the next iteration of the connected system.

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

