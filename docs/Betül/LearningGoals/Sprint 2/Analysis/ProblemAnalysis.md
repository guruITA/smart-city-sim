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
- Understand the function of each component (e.g., push buttons, LEDs, resistors).
- Learn how these components interact within an embedded system.
- Understand how the system should react when a pedestrian presses a button.
- Analyze how multiple inputs (two buttons) influence the system behavior.
- Describe a clear and logical system behavior for safe pedestrian crossing.
- Document my findings so that they can be used in the next phase (design).
- Understand how the ESP32-S3 functions as a microcontroller within the system.
- Learn how the ESP32-S3 is programmed and how code controls inputs and outputs.
- Understand how the ESP32 connects to a laptop and how code is uploaded and executed.
- Understand how the ESP32-S3 operates independently after uploading code.
- Investigate how the microcontroller can be powered without a constant laptop connection.
- Analyze how embedded systems function as standalone systems in real-world scenarios.

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
- Breadboard 

For each component, I will:

- Identify its function
- Understand how it works in a circuit
- Learn how it connects to the ESP32-S3

## 3. Interaction and System Logic

After understanding the components, I will define how the system should behave.

I will:

- Describe how the system reacts to a button press
- Analyze how both pedestrian sides interact with each other
- Examine how the system ensures safe and consistent behavior

This will result in a **clear, step-by-step description of the system behavior**, which will serve as input for the design phase.

## 4. ESP32 Functionality and Programming

In addition to analyzing the components and system behavior, I will study how the ESP32-S3 operates within an embedded system.

To do this, I will:

- Explore how the ESP32-S3 functions as the central controller of the system
- Learn how digital inputs (buttons) and outputs (LEDs) are handled in code
- Understand how GPIO pins are used to control hardware components
- Practice uploading code from my laptop to the ESP32-S3
- Analyze how the written code translates into physical behavior of the system

This will help me understand how the system logic is implemented in practice and how software and hardware interact.

## 5. ESP32 Standalone Operation

To better understand how the system will function in a real-world scenario, I will analyze how the ESP32-S3 operates independently.

To do this, I will:

- Investigate how code is uploaded from a laptop to the ESP32-S3
- Understand how the ESP32 stores and executes code without being connected to a computer
- Explore different ways to power the ESP32 (e.g., USB adapter, battery, powerbank)
- Analyze how the system continues to function after being disconnected from the laptop

This will help me ensure that the pedestrian crossing system can operate as a **standalone embedded system**, which is essential for real-world smart city applications.