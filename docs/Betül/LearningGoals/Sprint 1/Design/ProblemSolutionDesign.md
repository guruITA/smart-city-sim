# Problem Solution Design

### Proposed Solution

The proposed solution is to develop a **Smart Pedestrian Crossing prototype** for the T-junction tile using an **ESP32-S3 microcontroller**.

The system simulates a real pedestrian crossing by controlling **car traffic lights and pedestrian signals** when a pedestrian requests to cross the road.

The goal is to create a simple embedded system that improves pedestrian safety and demonstrates how smart infrastructure can work within the Smart City simulation.

---

## System Components

The prototype requires several components to simulate a traffic crossing system.

**Controller**

- ESP32-S3 microcontroller

**Input**

- Push button (pedestrian crossing request)

**Outputs**

Car traffic lights

- Red LED
- Yellow LED
- Green LED

Pedestrian lights

- Red LED
- Green LED

Optional component:

- Buzzer for an audio crossing signal.

---

### System Behaviour

The pedestrian crossing follows a simplified traffic control sequence.

**Normal traffic**

- Car light → Green
- Pedestrian light → Red

**Pedestrian request**

When the button is pressed, the system registers the request.

**Traffic stops**

The car traffic lights change:

Green → Yellow → Red

**Pedestrian crossing**

The pedestrian light turns **Green**, allowing pedestrians to cross safely.

**Return to normal traffic**

After a short timer:

- Pedestrian light → Red
- Car light → Green

Traffic continues normally.

---

## Position in the City Tile

The pedestrian crossing will be implemented on the **T-junction tile** in the Smart City model.

The ESP32-S3 will control the traffic signals and process pedestrian crossing requests.

---


# Prototype Design - Smart Pedestrian Crossing System 


## From Idea to Prototype Design

After analysing the problem of pedestrian safety at a T-junction, the next step is translating the concept of a **smart pedestrian crossing** into a technical design.

The purpose of this design phase is to determine **which components are required**, how they should be connected, and how the system should behave.

To do this, I created a **prototype circuit design** that simulates a pedestrian crossing system using an **ESP32-S3 microcontroller** and basic electronic components.

The design focuses on simplicity so that it can be implemented and tested during the first sprint of the Smart City simulation project.

## Components Required for the Design

To simulate a pedestrian crossing system, several basic electronic components are required. Each component has a specific role within the system.

### ESP32-S3 Microcontroller

The **ESP32-S3** functions as the central controller of the system.

It reads inputs from the pedestrian button and controls the LED signals that represent the pedestrian traffic lights.

Because the ESP32 can process digital inputs and outputs, it is suitable for controlling simple embedded systems such as traffic signal simulations.

---

### Push Button

The push button acts as the **pedestrian request input**.

When a pedestrian presses the button, the ESP32 detects the signal and triggers the crossing sequence. This mimics the real-world behaviour of pedestrian crossings in cities.

---

### LEDs

To simulate the traffic system, multiple LEDs are used.

Car traffic lights:

- Red LED
- Yellow LED
- Green LED

Pedestrian signal:

- Red LED
- Green LED

These LEDs visually represent the behaviour of a real traffic light system.

---

### Resistors

Each LED must be connected with a **current-limiting resistor**.

Resistors prevent too much electrical current from flowing through the LED, which could otherwise damage the component or the microcontroller.