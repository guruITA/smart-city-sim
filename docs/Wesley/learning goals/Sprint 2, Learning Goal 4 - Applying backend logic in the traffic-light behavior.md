# Sprint 2 — Learning Goal 4: Applying backend logic in the traffic-light behavior

## Learning Question

How can I apply backend-driven logic in my traffic-light prototype while keeping the local traffic-light behavior safe, predictable, and technically clear?

## S — Situation

Once the traffic-light system can send data to the backend and receive information back, the next challenge is how the local controller should use that backend information in its actual behavior. This is an important step, because a smart traffic-light system does not become smart only by communicating with a backend. It becomes smart when that communication influences decisions in the system in a useful and controlled way.

For Sprint 2, I therefore want to focus on the relationship between backend logic and local safety. The backend may decide or suggest what should happen, but the local traffic-light controller still needs to remain safe and understandable. That means I need to think clearly about what logic belongs in the backend, what safety rules must remain local, and how to prevent invalid or conflicting traffic-light behavior.

## T — Task

For this learning goal, I will investigate and prepare how backend logic can influence the local traffic-light behavior without removing local control over safety. The main goal is to create a clear and testable approach in which backend decisions are applied only within safe boundaries.

I will make this learning goal concrete by producing the following deliverables:

### Deliverables

**1. Analysis document**
A document in which I describe:

* what decisions may come from the backend,
* what decisions must remain local on the ESP32-S3,
* what safety rules must always be enforced locally,
* the risks of invalid or unsafe backend instructions,
* the acceptance criteria for backend-driven traffic-light control.

**2. Design document**
A document in which I show:

* the division of responsibility between backend logic and local control logic,
* the state model for applying backend-driven behavior,
* the fallback behavior when backend input is invalid or unavailable,
* the safety checks that block unsafe state changes,
* how the design prevents conflicting traffic-light states.

**3. Realise document**
A document in which I record:

* how I implement backend-influenced logic in the breadboard setup,
* how I test whether the local controller accepts or rejects backend instructions correctly,
* how I verify that local safety rules always remain active,
* how I transfer the working setup into the project tile,
* what tests prove that backend-driven behavior works without unsafe states.

**4. Advice document**
A document in which I evaluate:

* whether the backend-driven logic is suitable for the project,
* whether the safety strategy is strong enough,
* what limitations or risks remain,
* what should be changed before this logic is expanded further in a next iteration.

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

A strong next refinement would be to make the deliverables even more uniform by giving them fixed names, such as **Analysis Report**, **Design Report**, **Realisation Log**, and **Advice Report**, across all four learning goals.