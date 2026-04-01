# Learning Goal – Smart Pedestrian Crossing System

## Learning Question

**How can I understand and conceptually design the components and system logic of a Smart pedestrian crossing subsystem for a T-junction using an ESP32-S3?**

--- 

# S — Situation

Within the Smart City Sim project, each student designs and builds a tile that represents a part of a smart city. My tile represents a T-junction that includes smart infrastructure such as lighting and interactive elements. The goal of the project is to simulate a smart city where embedded systems monitor and control different processes.

For my tile, I will design a smart pedestrian crossing subsystem that focuses on pedestrian interaction. The system will consist of two pedestrian crossings placed opposite each other, each with its own button and signal lights.

This pedestrian crossing system is part of a larger traffic system, where other components, such as car traffic lights, are developed by other team members.

When a pedestrian presses a button on one side, the system should safely allow crossing by activating both pedestrian signals in a synchronized way.

The system will be built using an ESP32-S3 microcontroller and basic electronic components such as LEDs and push buttons.

However, I currently do not fully understand:

- which components are required,
- how they should be connected,
- and how the system logic should work for multiple pedestrian inputs.

Because embedded systems development is new to me, I want to first analyze and understand the system before building it. By researching components and studying how pedestrian crossings operate, I can design a clear and functional system for my tile.

---

# T — Task

Based on my learning question, I want to:
- Identify which electronic components are needed for a dual-sided pedestrian crossing system.
- Understand the function of each component (e.g., push buttons, LEDs, optional sensors).
- Learn how these components can be connected to the ESP32-S3.
- Understand how the system should react when a pedestrian presses the button.
- Design a clear system logic that handles input from both sides.- Create a basic prototype idea for how the system could work on my tile.
- Ensure that both pedestrian signals behave consistently and safely.
- Create a basic prototype concept for implementation on my tile.
- Document my findings so that my team members can understand the design.

---

# A — Action

## 1. Research

First, I will research how pedestrian crossings normally work.

To do this I will:

- Study examples of pedestrian crossing systems
- Analyze how user input (button press) affects system behavior
- Study simple traffic light systems to understand their behavior.

This will help me understand what the system should do.

## 2. Component Analysis
Next, I will explore the electronic components available in my embedded kit.

I will focus on:

- Push buttons (input)
- LEDs (output signals)
- Resistors (current limitation)
- Breadbord 

For each component, I will:

- Identify its function
- Understand how it works in a circuit
- Learn how it connects to the ESP32-S3

## 3. Interaction and System Logic

After understanding the components, I will define how the system should behave.

I will:

- Describe how the system reacts to a button press
- Define how both pedestrian sides interact with each other
- Ensure the system behaves consistently and safely

This will result in a step-by-step system behavior description.

## 4. Conceptual Design 
Finally, I will create a conceptual design of the system.

This includes:

- A simple diagram of the components and connections
- A description of how the ESP32-S3 controls the system
- A clear interaction flow between input (buttons) and output (signals)

This conceptual design will be used as a foundation for the next phase: the detailed system design and prototype implementation.