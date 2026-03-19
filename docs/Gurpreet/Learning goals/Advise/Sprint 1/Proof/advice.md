# Smart cities learning group goal: Advise

- Name: Gurpreet Singh
- Date: 18-03-2026

## Table of Contents
- [Smart cities learning group goal: Advise](#smart-cities-learning-group-goal-advise)
  - [Table of Contents](#table-of-contents)
  - [1. Introduction](#1-introduction)
  - [2. Methodology](#2-methodology)
  - [3. Working method](#3-working-method)
  - [4. Main advice question and subquestions](#4-main-advice-question-and-subquestions)
  - [5. Tools used](#5-tools-used)
  - [6. Goal of the advice phase](#6-goal-of-the-advice-phase)
  - [7. Starting point from the previous phases](#7-starting-point-from-the-previous-phases)
  - [8. Components and alternatives considered](#8-components-and-alternatives-considered)
    - [8.1 Current solution: relay module](#81-current-solution-relay-module)
    - [8.2 Alternative solution: MOSFET](#82-alternative-solution-mosfet)
    - [8.3 Subconclusion](#83-subconclusion)
  - [9. Comparison of both solutions](#9-comparison-of-both-solutions)
    - [9.1 Suitability for the current Sprint 1 prototype](#91-suitability-for-the-current-sprint-1-prototype)
    - [9.2 Suitability for future development](#92-suitability-for-future-development)
    - [9.3 Efficiency and switching behaviour](#93-efficiency-and-switching-behaviour)
    - [9.4 Scalability and GPIO usage](#94-scalability-and-gpio-usage)
    - [9.5 Subconclusion](#95-subconclusion)
  - [10. Key considerations in the advice phase](#10-key-considerations-in-the-advice-phase)
  - [11. Result of the advice phase](#11-result-of-the-advice-phase)
  - [12. Evaluation of both solutions](#12-evaluation-of-both-solutions)
  - [13. Final conclusion](#13-final-conclusion)
  - [14. Recommendations](#14-recommendations)
  - [15. Sources](#15-sources)

## 1. Introduction

This advice report is part of the Smart Cities Learning Group project. In the previous phases, the smart streetlight prototype was first analysed, then translated into a design, and after that realised as a working Sprint 1 prototype. Because the prototype now works in practice, the next step is no longer only to build, but also to evaluate whether the chosen technical solution is the most suitable direction for further development.

In the realised prototype, a relay module was used to switch the LED circuit on and off based on the light value measured by the LDR module. This worked well for Sprint 1, because the prototype only had to demonstrate automatic switching between light and dark conditions. However, during further reflection it became clear that later versions of the smart streetlight may require more than only switching fully on or fully off.

The purpose of this advice phase is therefore to compare the current relay-based solution with a possible MOSFET-based alternative. The report focuses on determining which switching solution is the most suitable for further development of the smart streetlight prototype, especially if later sprints require features such as dimming, more efficient switching, or the addition of extra sensors such as a PIR sensor.

## 2. Methodology

For this advice report, several methods were used:

- reviewing the results of the previous analysis, design, and realisation phases;
- using the realised relay-based prototype as the practical starting point for evaluation;
- studying technical information about relay-based and MOSFET-based switching;
- comparing both solutions against the requirements of the current and possible future prototype versions;
- evaluating both options in terms of switching behaviour, efficiency, scalability, and practical applicability;
- formulating a recommendation for the next version of the smart streetlight prototype.

This approach was chosen because the advice phase must not be based on theory alone. The current relay-based prototype already works in practice, so the advice must be based on both realised project experience and technical comparison.

## 3. Working method

The advice process was carried out step by step. First, the realised Sprint 1 prototype was taken as the starting point. This made it possible to look back at what already works well in practice and what limitations may appear when the prototype is developed further.

After that, the current relay module was evaluated as the switching solution used in Sprint 1. Then, a MOSFET-based switching solution was explored as a possible alternative for a next version of the prototype. Both options were compared using the same criteria, namely suitability for the current Sprint 1 prototype, suitability for future development, efficiency and switching behaviour, and scalability in relation to GPIO usage and extra modules.

By working in this way, the advice phase continues the same project structure as the previous phases. The earlier phases established what should be built, how it should be designed, and whether it works in practice. This phase builds on that by determining which direction is the most suitable for further development.

## 4. Main advice question and subquestions

The main advice question of this report is:

Which switching solution is the most suitable for further development of the smart streetlight prototype with the ESP32-S3: the current relay-based solution or a MOSFET-based alternative?

To answer this main advice question, the following subquestions were formulated:

1. What are the strengths and limitations of the relay module used in the realised Sprint 1 prototype?
2. What is a MOSFET-based switching solution, and how could it be applied in this project?
3. Which solution is more suitable for the current Sprint 1 prototype?
4. Which solution is more suitable for future development of the smart streetlight prototype?
5. Which solution offers the best balance between simplicity, efficiency, and scalability?

## 5. Tools used

For this advice report, the following tools were used:

- the earlier analysis, design, and realisation reports as project references;
- technical sources about relays and MOSFETs;
- Scribbr for formatting references correctly;
- ChatGPT for support with language use, phrasing, spelling, and grammar.

## 6. Goal of the advice phase

The goal of this advice phase was to determine which switching solution should be recommended for the further development of the smart streetlight prototype. The current realised prototype uses a relay module and has already proven that automatic light-based switching works in practice.

The purpose of this phase is to decide whether that relay-based approach should remain the preferred solution, or whether a MOSFET-based solution would be a better choice for a future version of the prototype.

## 7. Starting point from the previous phases

This advice phase builds directly on the previous phases of the project.

In the analysis phase, the relay module was chosen as the switching component because the LED load should not be powered directly from an ESP32-S3 GPIO pin. In the design phase, this choice was translated into a breadboard design in which the relay formed the bridge between the control side and the LED circuit. In the realisation phase, that design was physically built and tested successfully. The result was a working Sprint 1 prototype that automatically switched the LED circuit on and off based on ambient light.

This means that the relay-based solution is not only a theoretical option, but a proven working solution within this project. At the same time, the realised prototype also makes it possible to reflect on whether that solution remains the best option when the prototype becomes more advanced.

## 8. Components and alternatives considered

This chapter describes the two switching solutions considered in this advice phase: the current relay-based solution and a MOSFET-based alternative.

### 8.1 Current solution: relay module

The current Sprint 1 prototype uses a relay module as the switching component for the LED circuit. In this setup, the ESP32-S3 sends a digital control signal to the relay module, and the relay draws approximately 70mA coil current when energized [(Jain, 2026)](https://zbotic.in/mosfet-vs-relay-for-motor-switching-which-is-better/).

A strong point of the relay module is that it is simple to understand and easy to use in a beginner project. It also creates a clear separation between the control side and the switched circuit. This made it a suitable choice for Sprint 1, where the main goal was to demonstrate automatic switching in a reliable way.

At the same time, the relay is mainly suitable for on/off switching. It is less suitable if the circuit later needs more refined control, such as dimming the LEDs or switching very frequently.

### 8.2 Alternative solution: MOSFET

A MOSFET is an electronic switching component that can also be used to control a load such as an LED circuit. In a project like this, a logic-level N-channel MOSFET could be used to switch the LED circuit through the ESP32-S3.

Compared with a relay, a MOSFET is not a mechanical switch. It switches electronically in microseconds rather than relay's 10-20ms. This makes it more suitable for functions such as PWM control [(Jain, 2026)](https://zbotic.in/mosfet-vs-relay-for-motor-switching-which-is-better/).

A MOSFET-based solution is therefore relevant as an alternative for a future version of the smart streetlight prototype, especially if the project later moves beyond simple binary switching.

### 8.3 Subconclusion

The advice phase compares two realistic switching options. The relay module is the current proven solution in the realised Sprint 1 prototype, while the MOSFET is a possible alternative for further development. Both options are technically relevant, but they are suitable for different levels of prototype complexity.

## 9. Comparison of both solutions

This chapter compares the relay module and the MOSFET-based alternative against the project requirements and future development direction.

### 9.1 Suitability for the current Sprint 1 prototype

For the current Sprint 1 prototype, the relay module is a suitable solution. The prototype only needs to switch the LED circuit on and off based on the light value measured by the LDR module. The realised prototype already proved that this works in practice.

A MOSFET could also perform this switching task, but for the current prototype it would not necessarily provide a major additional benefit if the only requirement remains simple on/off behaviour. In that sense, the relay module already fulfils the Sprint 1 goal sufficiently.

For the current prototype alone, the relay module is therefore the most direct and understandable solution.

### 9.2 Suitability for future development

When future development is considered, the situation changes. Later versions of the smart streetlight may require more advanced behaviour, for example dimming the LEDs instead of only switching them fully on or off, or combining the lighting function with extra inputs such as a PIR motion sensor.

For that kind of development, a MOSFET is more suitable than a relay. A relay mainly supports basic switching, while a MOSFET can also support fast electronic control such as PWM. This makes the MOSFET more flexible for a more advanced smart streetlight concept.

This means that, although the relay is sufficient for Sprint 1, the MOSFET is the more future-oriented solution.

### 9.3 Efficiency and switching behaviour

A relay and a MOSFET differ significantly in their switching behaviour. A relay switches mechanically in 10-20ms [(Jain, 2026)](https://zbotic.in/mosfet-vs-relay-for-motor-switching-which-is-better/), which means it is slower and less suitable for frequent switching. A MOSFET switches electronically in microseconds [(Jain, 2026)](https://zbotic.in/mosfet-vs-relay-for-motor-switching-which-is-better/), which means it can switch much faster and more efficiently.

This difference becomes especially important when dimming is considered. Dimming requires PWM with fast switching, which relays cannot achieve due to mechanical limitations. A MOSFET is much more appropriate in such a situation.

In addition, relays consume continuous 70mA coil current when energized [(Jain, 2026)](https://zbotic.in/mosfet-vs-relay-for-motor-switching-which-is-better/), while MOSFETs have zero gate current and no mechanical wear because they have no moving parts [(Jain, 2026)](https://zbotic.in/mosfet-vs-relay-for-motor-switching-which-is-better/). This makes MOSFETs a more efficient and durable option for repeated switching behaviour.

### 9.4 Scalability and GPIO usage

Scalability is also important for later sprints. If the smart streetlight is expanded with a PIR sensor or additional behaviour, the ESP32-S3 must still have enough flexibility in the way the switching design is used.

The current relay-based solution already uses one GPIO pin for relay control. A MOSFET would also require a control connection, so the difference is not only about the number of pins used, but more about what kind of functionality becomes possible through that control pin.

If the project develops toward dimming, motion-based behaviour, or more refined control logic, the MOSFET makes better use of the controller’s capabilities. In that sense, it supports a more scalable design direction.

### 9.5 Subconclusion

The comparison shows that the relay module is the most suitable solution for the current Sprint 1 prototype, because it is simple, clear, and already proven in practice. However, the MOSFET is more suitable for further development because it supports faster switching and more efficient operation (0mA vs 70mA coil curren) [(Jain, 2026)](https://zbotic.in/mosfet-vs-relay-for-motor-switching-which-is-better/).and advanced behaviour such as dimming [(Lisleapex, 2024)](https://www.lisleapex.com/blog-mosfet-vs-relay-what-are-differences-and-how-to-choose)

## 10. Key considerations in the advice phase

During the advice phase, no major practical problems occurred, because the purpose of this phase was not to build a new prototype, but to evaluate the current realised solution and compare it with an alternative.

The most important consideration was that the advice had to stay connected to the actual project situation. It would have been easy to recommend a MOSFET immediately as the technically stronger solution, but that would ignore the fact that the relay-based prototype already works and is easier to understand in an introductory learning context.

For that reason, the advice had to balance two perspectives. On the one hand, the relay module is fully acceptable for the current prototype and for the current level of project complexity. On the other hand, if the project develops further, the MOSFET becomes the stronger recommendation. This balance is central to the advice in this report.

## 11. Result of the advice phase

The result of this advice phase is a clear recommendation about the switching solution for the smart streetlight prototype.

For the current Sprint 1 prototype, the relay module remains an appropriate choice because it has already been realised successfully and fits the current goal of automatic on/off switching.

For the next version of the prototype, however, a MOSFET-based switching solution is advised as the better direction. This is because it is more suitable for future functions such as dimming, more efficient switching, and possible expansion with extra sensors and more advanced behaviour [(Jain, 2026)](https://zbotic.in/mosfet-vs-relay-for-motor-switching-which-is-better/)..

## 12. Evaluation of both solutions

Both solutions have value within this project, but they fit different development stages.

The relay module is valuable because it helped make the first working prototype possible. It fits a beginner-friendly Sprint 1 context in which the most important goal was to make the system function clearly and reliably.

The MOSFET is valuable because it fits a more advanced stage of the project. Once the prototype already works, the question changes from “can it switch?” to “can it switch in a better, more efficient, and more scalable way?”. In that context, the MOSFET becomes the more appropriate solution.

This means that the relay should not be seen as a wrong choice. It was the right choice for the first realised version. The MOSFET should instead be seen as the recommended next step.

## 13. Final conclusion

This advice report evaluated which switching solution is the most suitable for further development of the smart streetlight prototype with the ESP32-S3.

First, the current relay-based solution was reviewed from the perspective of the previous phases. The relay module was chosen in the analysis phase, translated into the design phase, and successfully implemented in the realisation phase. This means that the relay module is a proven working solution for the current Sprint 1 prototype.

Second, a MOSFET-based switching solution was explored as an alternative for a future version of the prototype. The comparison showed that a MOSFET is more suitable for advanced control behaviour, especially when dimming, efficiency, and further expansion are taken into account.

Based on this comparison, it can be concluded that the relay module remains the most suitable choice for the current Sprint 1 prototype, because it is simple, understandable, and already works in practice. At the same time, the MOSFET is the more suitable recommendation for a next version of the smart streetlight prototype, because it better supports future development.

This advice phase therefore completes the progression from analysis, design, and realisation by moving from a working prototype to a justified recommendation for further development. The relay-based solution proved suitable for Sprint 1, but the MOSFET-based solution is advised as the better direction for a next version of the smart streetlight prototype.

## 14. Recommendations

Based on this advice report, the following recommendations are made:

1. Keep the current relay-based prototype as the working reference for Sprint 1.
2. Use the realised relay prototype as the comparison baseline for further development.
3. Explore a MOSFET-based switching design for the next version of the smart streetlight prototype.
4. Test whether a MOSFET-based design can support LED dimming in practice.
5. Evaluate in a later phase whether the MOSFET-based solution also improves efficiency and scalability when extra functionality, such as a PIR sensor, is added.
6. Document the next prototype version clearly so that the transition from relay-based switching to MOSFET-based switching remains understandable and reproducible.

## 15. Sources

1. [analysis.md](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/main/docs/Gurpreet/Learning%20goals/Analysis/Sprint%201/analysis.md?ref_type=heads#smart-cities-learning-group-goal-analysis)

2. [design.md](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/main/docs/Gurpreet/Learning%20goals/Design/Sprint%201/design.md?ref_type=heads#smart-cities-learning-group-goal-design)

3. [realise.md](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/main/docs/Gurpreet/Learning%20goals/Realise/Sprint%201/realise.md?ref_type=heads#smart-cities-learning-group-goal-realise)

4. Jain, J. (2026, March 11). MOSFET vs Relay for Motor Switching: Which Is Better? - Zbotic. Zbotic. [https://zbotic.in/mosfet-vs-relay-for-motor-switching-which-is-better/](https://zbotic.in/mosfet-vs-relay-for-motor-switching-which-is-better/)

5. Lisleapex. (2024, May 29). MOSFET vs Relay: What are Differences and How to Choose. Lisleapex Electronics. [https://www.lisleapex.com/blog-mosfet-vs-relay-what-are-differences-and-how-to-choose](https://www.lisleapex.com/blog-mosfet-vs-relay-what-are-differences-and-how-to-choose)