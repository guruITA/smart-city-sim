# Sprint 1 — Learning Outcome (Analysis)

## Table of Contents

1. [Learning Question](#learning-question)
2. [S — Situation](#s--situation)
3. [T — Task](#t--task)
4. [A — Action](#a--action)
5. [R — Result](#r--result)
6. [R — Reflection](#r--reflection)
7. [T — Transfer](#t--transfer)
8. [Appendix A — Research Documents Used in the Analysis](#appendix-a--research-documents-used-in-the-analysis)


## Learning Question

How can I translate my traffic light idea into clear functional and non-functional requirements, measurable acceptance criteria, and a safe ESP32-S3 hardware basis for the first iteration of the prototype?  

## S — Situation

I am starting the Embedded & Robotics minor and this is my first time working with real embedded hardware. I switched from a streetlight idea to a traffic light project. During the analysis, I found that the full traffic light concept quickly becomes broader than only a fixed red-yellow-green sequence, because later versions can also include vehicle detection, pedestrian requests, and more advanced smart behavior. That is why this sprint focuses on the **first iteration**: a basic traffic light with a fixed cycle. This document only covers the **analysis** step of that first iteration and is meant to prepare the later phases of the sprint: design, realize, and advice.  

## T — Task

For the analysis part of this sprint, my task is to create the research needed to define the first iteration of the traffic light project. That means I need to establish the project scope, describe the functional and non-functional requirements, define measurable acceptance criteria and tests, identify the relevant ESP32-S3 hardware constraints, and document how simulation can support the next phases. The goal of this analysis is not yet to build the system, but to produce a clear and usable foundation for the design, realize, and advice steps that follow in the same sprint cycle.   

## A — Action

For the analysis of this sprint, I created multiple research documents, with each document focused on one subject. In my traffic light research document, I analysed the project scope and defined the first iteration as a basic traffic light with three LED outputs, a fixed red-green-yellow-red sequence, fixed timing, a safe startup in red, and no conflicting outputs. In my ESP32-S3 research document, I analysed the development board, the GPIO groups, ADC-capable pins, PWM possibilities, and the pins that require extra care, such as the flash and PSRAM pins, the strapping pins, and the UART-related pins. In my Wokwi research document, I analysed how simulation in VS Code can be prepared using `wokwi.toml` and `diagram.json`, so that hardware setup and firmware behavior can later be tested in a virtual environment. Together, these research documents form the analysis of this sprint.   

## R — Result

The result of this analysis is that the first iteration is now clearly scoped as a **basic traffic light prototype** instead of the full smart traffic light system. The research already defines the main first-iteration functional requirements: the system controls red, yellow, and green LEDs, follows the order red-green-yellow-red, uses fixed timing, starts safely in red, prevents conflicting outputs, and keeps repeating the cycle. It also defines the first-iteration non-functional requirements and acceptance criteria, such as predictable timing, visible LED states, low-voltage hardware use, understandable code, safe behavior after reset, startup in red, correct state order, no conflicting LED combinations, and correct timing during repeated cycles. In addition, the ESP32-S3 analysis identifies the hardware basis for the next phases, including general GPIO use, ADC availability on GPIO1-GPIO20, PWM through LEDC, reserved flash or PSRAM pins on GPIO26-GPIO32, and the need to handle GPIO0, GPIO3, GPIO45, GPIO46, GPIO43, and GPIO44 with care.  

## R — Reflection

By creating separate research documents for each subject, I noticed that the analysis became much clearer and easier to structure. Each subject had its own focus: the traffic light document helped me define the project logic, requirements, acceptance criteria, and the difference between the first basic iteration and later smart behavior. The ESP32-S3 document helped me document the hardware functions, pin groups, and wiring constraints. The Wokwi document helped me document the simulation setup separately from the hardware and project logic. Because I documented each research subject on its own, it became easier to keep the findings organised, to avoid mixing different kinds of information together, and to use the results later in the design, realize, and advice phases. It also became easier to trace where a specific decision came from, such as a requirement from the traffic light analysis, a pin choice from the ESP32-S3 analysis, or a testing preparation step from the Wokwi analysis. This way of working made the analysis more systematic, more readable, and easier to reuse in the next steps of the sprint.   

## T — Transfer

I will use the outcome of this analysis directly in the next phases of this sprint. In the **design** step, I will translate the analysed requirements and hardware constraints into a concrete circuit and software structure. In the **realize** step, I will build and test the first traffic light iteration. In the **advice** step, I will evaluate what worked well, what needs improvement, and what should be recommended for the next iteration. I will also reuse this sprint structure in later work: first analyse, then design, then realize, then advise.   

## Appendix A — Research Documents Used in the Analysis

1. **Smart Traffic Light: When Does a Traffic Light Become Smart?**
   Used to define the project scope, separate the first basic iteration from later smart iterations, and document functional requirements, non-functional requirements, acceptance criteria, and tests for the traffic light project. 

   [permalink Smart Traffic](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/08d00c7c5ae667fbf8fb431da6c9015ff4cee5a8/docs/Wesley/research/Smart%20Traffic%20Light:%20When%20Does%20a%20Traffic%20Light%20Become%20Smart%3F.md)

2. **ESP32-S3 Development Board: Pinout, Interfaces, and Practical Board Comparison**
   Used to document the ESP32-S3 hardware basis, including GPIO groups, ADC pins, PWM support, flash and PSRAM pin restrictions, strapping pins, and board-specific wiring considerations. 
   
   [permalink ESP32-S3](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/08d00c7c5ae667fbf8fb431da6c9015ff4cee5a8/docs/Wesley/research/ESP32-S3%20Development%20Board:%20Pinout,%20Interfaces,%20and%20Practical%20Board%20Comparison.md)

3. **Wokwi for VS Code, Setup and Configuration for ESP32 Projects**
   Used to document how simulation can be prepared in VS Code with Wokwi, including the required files and the workflow for testing the setup in a virtual environment. 

   [permalink Wokwi](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/08d00c7c5ae667fbf8fb431da6c9015ff4cee5a8/docs/Wesley/research/Wokwi%20for%20VS%20Code,%20Setup%20and%20Configuration%20for%20ESP32%20Projects.md)


