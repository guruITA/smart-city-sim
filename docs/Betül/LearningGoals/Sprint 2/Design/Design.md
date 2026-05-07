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

# R — Result
By following my design actions, I created a complete and functional blueprint for the smart pedestrian crossing subsystem. I developed a clear system concept, selected the required components, and created a Wokwi simulation that accurately represents the wiring and logic. I mapped all GPIO pins, designed the control logic for synchronized crossings, and documented how each component connects to the ESP32‑S3. I also printed a first version of the pedestrian signal housing to explore physical placement and scale. Although the model was too large, it helped me understand how the LEDs could fit inside and how the housing integrates with the tile.
This full design package now serves as the foundation for building the physical prototype in the Realise phase.

# R — Reflection
This sprint helped me understand how to translate a problem into a technical design. Because I am new to embedded systems, breaking the system down into components, wiring, and logic made the project manageable. The Wokwi simulation was especially valuable because it allowed me to test ideas without making physical mistakes. I also learned the importance of documenting pin choices and logic flow, because this makes the next sprint much easier. The 3D print, even though I didn’t design it myself, showed me that physical testing is important for understanding scale and placement. Overall, I feel more confident in designing embedded systems and I understand how important it is to validate ideas early.

# T — Transfer
In the next sprint, I will use this design to build the real prototype on the breadboard and integrate it into the tile. The GPIO mapping, logic flow, and Wokwi simulation will guide my wiring and coding. I will also refine the 3D housing by scaling it down and adjusting the dimensions so it fits the LEDs properly. The structured design approach I used here—breaking the system into components, logic, wiring, and physical layout—is something I can reuse in future embedded projects. It helps me stay organized, avoid mistakes, and build prototypes more efficiently.

**References**

Wokwi - World’s most advanced ESP32 Simulator. (z.d.). https://wokwi.com/

Lab, M. (2025, 2 december). How to use Push Button with ATMEGA32 AVR Microcontroller. Microcontrollers Lab. https://microcontrollerslab.com/use-push-button-atmega32-avr-microcontroller/#:~:text=In%20this%20tutorial%2C%20we%20will%20learn%20about%20the,use%20of%20%E2%80%9Cif%20and%20else%E2%80%9D%20statements%20in%20C.