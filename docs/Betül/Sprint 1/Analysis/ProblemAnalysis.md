# Problem Analyse

### Smart Pedestrian Crossing at a T-Junction

## 1. Context: Pedestrian Crossings at Intersections

In urban environments, intersections are locations where different types of road users meet, including vehicles, cyclists, and pedestrians. One common intersection type is the **T-junction**, where one road ends and joins another road.

At these junctions, pedestrians often need to cross the main road or the connecting road. Without proper traffic control, this can create dangerous situations because drivers may focus on turning or merging traffic instead of pedestrians.

Pedestrian crossings are therefore used to regulate when pedestrians can safely cross the road. These crossings typically use **traffic lights or signalized pedestrian systems** that coordinate vehicle and pedestrian movement.

---

## 2. Current Situation

Traditional pedestrian crossings operate using a **push-button request system**. When a pedestrian presses the button, the traffic control system eventually changes the lights to allow pedestrians to cross.

The standard process is usually:

1. Cars have a green light.
2. A pedestrian presses the crossing button.
3. The traffic light controller waits for the correct moment.
4. Car lights change from **green → yellow → red**.
5. The pedestrian signal changes to **green**.
6. Pedestrians cross safely.
7. The system returns to normal traffic flow.

Although this system works, it does not always respond efficiently to real-time situations.

---

## 3. Problems at a T-Junction

At a **T-junction**, traffic patterns are slightly different from a normal cross intersection. Vehicles may:

- Drive straight on the main road
- Turn into the side road
- Exit the side road and join the main road

Because of this, pedestrians crossing near the junction may face several challenges.

### 3.1 Limited Driver Visibility

Drivers turning into the side road may focus on incoming traffic and may not immediately notice pedestrians waiting to cross.

### 3.2 Inefficient Waiting Times

In traditional systems, pedestrians must press a button and wait for the next traffic cycle. This can lead to unnecessary waiting times.

### 3.3 Lack of Adaptive Control

Most basic systems do not adjust to the presence of pedestrians or changing traffic conditions. The crossing signal simply follows fixed timing rules.

---

## 4. Smart Pedestrian Crossing Concept

A **smart pedestrian crossing system** can improve safety and efficiency at intersections by using sensors and microcontrollers.

Instead of only relying on fixed timing, a smart system can:

- Detect pedestrian crossing requests
- Control traffic signals dynamically
- Ensure safe crossing time
- Reduce unnecessary delays

In a prototype system, a microcontroller such as an **ESP32-S3** can control LED traffic lights, read input from buttons or sensors, and manage the timing logic of the crossing.

---

## 5. Problem Statement

At a T-junction, pedestrians need a safe and efficient way to cross the road. Traditional pedestrian crossing systems rely on fixed timing and simple button requests, which may not always provide optimal safety or efficiency.

Therefore, there is a need for a **smart pedestrian crossing system** that can control traffic signals and manage pedestrian crossing requests in a safer and more efficient way.

---


# Learning Goal – Smart Pedestrian Crossing System

## Learning Question

**What components and basic system logic are required to design and implement a smart pedestrian crossing system at a T-junction using an ESP32-S3?**

---

# S — Situation

Within the City Sim project, every student designs and builds their own tile that represents a part of a smart city. My tile represents a **T-junction** that will include smart infrastructure such as lighting and sensors. The goal of the project is to create a small smart city where embedded systems collect data and control different parts of the city. 
Ons-idee-voor-nu

For my tile I want to design a **smart pedestrian crossing** that allows pedestrians to safely cross the road when they press a button. The system should use an **ESP32-S3 microcontroller** and simple electronic components such as LEDs and buttons.

However, I still do not fully understand which **components** are needed and how they work together in an embedded system. I also do not yet know how the system should decide **when the pedestrian light should turn green or red**.

Because embedded development is new to me, I want to first analyze the system before building it. By researching the components and understanding how a simple pedestrian crossing works, I can design a system that I will later implement on my tile.

To do this, I will use the **Studio Smart Cities knowledge base** and other resources to better understand how smart infrastructure systems are designed.

---

# T — Task

Based on my learning question, I want to:

- Identify which **electronic components** are needed for a smart pedestrian crossing system.
- Understand what each component does (for example: button, LED, sensor).
- Learn how these components can be connected to the **ESP32-S3**.
- Understand how the system should react when a **pedestrian presses the button**.
- Design a **simple step-by-step behavior** for the pedestrian crossing.
- Create a **basic prototype idea** for how the system could work on my tile.
- Document my findings so that my team members can understand the design.

---

# A — Action