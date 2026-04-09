# Learning Goal - Design

## Learning Question – Problem Solution Design

**How can I design a functional and synchronized smart pedestrian crossing system with two opposite pedestrian crossings using an ESP32-S3, including components, circuit layout, and control logic?**

--- 

# S — Situation
In Sprint 2 of the City Sim project, I am responsible for a T-junction tile that allows pedestrians to cross safely. The goal is to design a smart pedestrian crossing system with two opposite crossings, each with a button and signal lights.

I am new to embedded systems and microcontroller programming, so this sprint will involve researching and understanding components, wiring, and control logic before any physical prototype is built. This design will form the blueprint for the prototype

--- 

# T — Task

I aim to:

- Select and connect the required components (LEDs, buttons, resistors, optional sensors).
- Design a circuit layout for two opposite pedestrian crossings.
- Develop the control logic for button presses and synchronized pedestrian lights.
- Test and document the working system to ensure functionality and safety.

# A — Action

To achieve the learning goal for Problem Solution Design, I will take the following actions:

**Map Control Logic**
 - Create a step-by-step control logic for the buttons and pedestrian signals.
 - Define how the two pedestrian crossings synchronize when a button is pressed.
 - Establish the timing for red/green signals to ensure the system is safe and consistent.

**Create Circuit Diagram**
 - Draw a conceptual circuit diagram showing how:
 - Push buttons
 - LEDs (red/green)
 - Resistors
 - ESP32-S3 pins are connected.
 - This diagram is intended to visualize the logic and connections before physically building the prototype.

**Plan Component Placement**
- Decide where each component will be placed on the tile.
- Ensure buttons and LEDs are clearly and functionally arranged for easy assembly in the Realise phase.

**Document Design Decisions**
- Record all design choices:
- Why specific pins were selected.
- Why the timing was set as it is.
- How the interaction between the two pedestrian crossings works.
- This documentation will serve as a reference for the Realise phase.

**Wokwi Simulation**
- Create a simulation in Wokwi to test the concept:
- Virtually connect LEDs and buttons as shown in the diagram.
- Simulate the control logic on the ESP32.
- Check that the system behaves as intended without physical errors.
- Use feedback from the simulation to refine the design before building the physical prototype.