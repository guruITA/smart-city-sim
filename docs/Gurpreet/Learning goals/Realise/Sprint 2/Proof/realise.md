# Smart cities learning group goal: Realise

- Name: Gurpreet Singh
- Date: 30-03-2026

## Table of Contents

- [Table of Contents](#table-of-contents)
- [1. Introduction](#1-introduction)
- [2. Methodology](#2-methodology)
- [3. Working method](#3-working-method)
- [4. Main realisation question and subquestions](#4-main-realisation-question-and-subquestions)
- [5. Tools used](#5-tools-used)
- [6. Goal of the realisation phase](#6-goal-of-the-realisation-phase)
- [7. Starting point from the previous phases](#7-starting-point-from-the-previous-phases)
- [8. Components and setup used](#8-components-and-setup-used)
- [9. Implementation process](#9-implementation-process)
    - [9.1 Starting point: the original standalone prototype code](#91-starting-point-the-original-standalone-prototype-code)
    - [9.2 Creating the library-like project structure](#92-creating-the-library-like-project-structure)
    - [9.3 Implementing the header file](#93-implementing-the-header-file)
    - [9.4 Implementing the cpp file](#94-implementing-the-cpp-file)
    - [9.5 Integrating the smart streetlight into the shared main ino file](#95-integrating-the-smart-streetlight-into-the-shared-main-ino-file)
    - [9.6 Testing the structured implementation](#96-testing-the-structured-implementation)
    - [9.7 Subconclusion](#97-subconclusion)
- [10. Problems encountered and solutions](#10-problems-encountered-and-solutions)
- [11. Result of the realisation](#11-result-of-the-realisation)
- [12. Evaluation of the implementation](#12-evaluation-of-the-implementation)
- [13. Final conclusion](#13-final-conclusion)
- [14. Recommendations](#14-recommendations)
- [15. Sources](#15-sources)

## 1. Introduction

This realise report is part of the Smart Cities Learning Group project. In Sprint 1, the smart streetlight was developed as a standalone prototype in which all logic was placed directly inside one sketch. That version was suitable for proving that the LDR module, relay module, and ESP32 S3 could work together to switch the streetlight on and off automatically. However, in Sprint 2 the project context changed.

The ESP32 boards are now treated as multifunctional City Hubs, which means that the code of multiple team members must be able to work together inside one shared project. Because of that, the original standalone smart streetlight code was no longer sufficient. The next step was therefore to restructure the code into a clearer and more reusable form that can be integrated into a shared ESP32 S3 project.

The purpose of this realisation phase is to implement that new software structure in practice. This report focuses on how the original standalone smart streetlight code was converted into a library like structure with a header file, an implementation file, and a shared main `.ino` file, and how this structured version was tested in the wider City Hub project context.

## 2. Methodology

For this realise report, several methods were used:

- reviewing the original standalone Sprint 1 smart streetlight code;
- creating a library like project structure inside the shared ESP32 S3 project;
- separating the smart streetlight functionality into a header file, an implementation file, and a shared main `.ino` file;
- integrating the smart streetlight functionality into the wider shared project together with other modules;
- testing whether the structured code still behaves the same as the original standalone version;
- evaluating whether the new code structure is clearer, more reusable, and more suitable for merging than the original version.

This approach was chosen because the realisation phase in Sprint 2 is no longer mainly about hardware construction, but about implementing a software structure that supports reuse and integration in a shared embedded project.

## 3. Working method

The realisation process was carried out step by step. First, the original standalone smart streetlight code from Sprint 1 was used as the starting point. That version already contained the working logic for reading the LDR module, comparing the light value with a threshold, and controlling the relay module.

After that, the project structure was reorganised so that the smart streetlight functionality was no longer placed directly inside one single sketch. Instead, a library like structure was created inside the `lib` folder of the shared City Hub project. Within that structure, the smart streetlight functionality was divided into a header file and a cpp file.

Next, the main `.ino` file of the shared project was adapted so that it no longer contained the full streetlight logic itself, but instead used the smart streetlight as a reusable component. Finally, the structured version was tested to check whether the streetlight still worked correctly and whether it could function together with other project modules in one shared project.

By working in this way, the realisation phase translated the results of the analysis phase into an implemented and testable software structure.

## 4. Main realisation question and subquestions

The main realisation question of this report is:

How can I structure the smart streetlight functionality as reusable and mergeable code within one shared ESP32-S3 project, so that it can be integrated with the work of other team members in a clear, maintainable, and testable way?

To answer this main realisation question, the following subquestions were formulated:

1. How can the original standalone smart streetlight code be restructured into a library-like format?
2. Which parts of the smart streetlight functionality should be placed in the header file, the cpp file, and the main `.ino` file?
3. Does the new structured implementation still perform the same smart streetlight behaviour as the original standalone version?
4. How can the structured smart streetlight functionality be integrated into the shared City Hub project together with other modules?
5. To what extent is the new structure clearer, more reusable, and more suitable for merging than the original code?

## 5. Tools used

For this realise report, the following tools were used:

- the original Sprint 1 smart streetlight code as the starting point;
- the Sprint 2 analysis document as the basis for the chosen software structure;
- VS Code for implementing and testing the code;
- the shared City Hub project structure with a `lib` folder and a central `.ino` file;
- Scribbr for formatting references correctly;
- ChatGPT for support with language use, phrasing, spelling, and grammar.

## 6. Goal of the realisation phase

The goal of this realisation phase was to convert the original standalone smart streetlight logic into reusable and structured code inside the shared ESP32 S3 City Hub project.

The intention was that the smart streetlight functionality would no longer exist only as a local sketch, but as a clearer component that can be integrated into the shared project, maintained more easily, and merged more safely with the work of other team members.

## 7. Starting point from the previous phases

This realisation phase was based on the results of the previous phases.

In Sprint 1, the smart streetlight was analysed, designed, and realised as a working local prototype. That version used an ESP32 S3, an LDR module on GPIO4, a relay module on GPIO5, and a threshold-based on/off control logic. The result was a working streetlight that could detect light and switch automatically.

In the Sprint 2 analysis phase, it was concluded that the standalone structure of that code was no longer sufficient for the shared City Hub project. The analysis showed that the most suitable direction was a library like structure in which the smart streetlight functionality would be separated into a header file, a cpp file, and a main `.ino` integration point.

The realisation phase therefore started from an already working standalone smart streetlight behaviour, but implemented it in a new software structure.

## 8. Components and setup used

For this implementation, the following code structure and setup were used:

- the original standalone smart streetlight sketch from Sprint 1;
- a `lib/StreetLight/` folder inside the shared City Hub project;
- `StreetLight.h` as the header file;
- `StreetLight.cpp` as the implementation file;
- `city-sim.ino` as the shared main file;
- the existing ESP32 S3 project setup used by the learning group;
- integration together with other modules such as `NetworkController` and `TrainPredictionSignal`.

## 9. Implementation process

This chapter describes how the standalone smart streetlight code was converted into a reusable and structured implementation.

### 9.1 Starting point: the original standalone prototype code

The starting point of this realisation phase was the original standalone smart streetlight code from Sprint 1. In that version, all logic was placed inside one sketch. The code contained the pin definitions for the LDR module and relay module, the threshold value, the `setup()` function, the `loop()` function, and the `updateStreetLight()` function.

That structure was sufficient for a local prototype, but it meant that all responsibilities were mixed together in one file. Sensor reading, relay control, timing, pin setup, and debug output were all embedded directly inside the same sketch. This made the code less suitable for reuse and integration in the shared City Hub project.

### 9.2 Creating the library-like project structure

To solve this, a library like structure was created inside the shared project. A separate `StreetLight` folder was added inside the `lib` directory. Inside that folder, two files were created: `StreetLight.h` and `StreetLight.cpp`.

This new structure made it possible to treat the smart streetlight as its own software component instead of leaving it as a loose block of code inside the central sketch. It also matched the direction that was identified in the analysis phase, namely that a library like structure is the most suitable way to support clarity, reuse, and merging inside one shared project.

### 9.3 Implementing the header file

The header file was used to define the reusable smart streetlight component.

In this file, the `StreetLight` class was declared. The file contains the private attributes for the LDR pin, relay pin, threshold value, interval, and timing variable. It also contains the public constructor and the public methods `begin()` and `update()`.

```cpp
  #ifndef STREETLIGHT_H
  #define STREETLIGHT_H

  #include <Arduino.h>

  class StreetLight {

  private:
    int _ldrPin;
    int _relayPin;
    int _threshold;

    int _interval;
    unsigned long _previousMillis;

  public:
    StreetLight(int ldrPin, int relayPin, int threshold, int interval);

    void begin();
    void update();
  };

  #endif
```

This implementation follows the intended role of the header file, because it defines the interface of the smart streetlight component without placing the full working logic in the main project file.

### 9.4 Implementing the cpp file

The `StreetLight.cpp` file was used to implement the actual behaviour of the smart streetlight.

This file contains the constructor, the initialisation logic inside `begin()`, and the operational logic inside `update()`. In this way, the original standalone behaviour was moved out of the main sketch and placed into the internal implementation of the component.



Compared with the original standalone sketch, the logic is now grouped more clearly. The sensor reading, threshold comparison, relay switching, and timed updates are all handled by the smart streetlight component itself.

### 9.5 Integrating the smart streetlight into the shared main ino file

After creating the reusable component, the smart streetlight was integrated into the shared `city-sim.ino` file. In this file, the smart streetlight is no longer implemented directly. Instead, the main project includes the smart streetlight header, creates a `StreetLight` object, and calls its methods from the central `setup()` and `loop()` functions.



This shows clearly that the smart streetlight now functions as one module inside a wider City Hub project together with other modules. The shared main file is now more focused on project integration instead of containing all smart streetlight logic itself.

### 9.6 Testing the structured implementation

After the restructuring, the implementation was tested to verify whether the smart streetlight still behaved correctly inside the shared project.

The most important test points were:

1. whether the smart streetlight component could still be initialised correctly through `lamp.begin()`;
2. whether `lamp.update()` still read the LDR value and controlled the relay correctly;
3. whether the structured smart streetlight functionality still switched the streetlight on and off based on the threshold value;
4. whether the smart streetlight could run together with another module inside the shared `loop()` without breaking the structure of the project.

The test results showed that the structured version still preserved the original behaviour of the standalone smart streetlight. At the same time, the main project became more organised because the full streetlight behaviour was no longer mixed directly into the central file.

### 9.7 Subconclusion

The implementation process successfully converted the original standalone smart streetlight sketch into a librar like structure inside the shared ESP32 S3 project. The `StreetLight.h` file now defines the reusable component, the `StreetLight.cpp` file contains the working logic, and the main `.ino` file uses the smart streetlight as one module within the wider City Hub project.

This means that the standalone logic was successfully translated into a clearer, more reusable, and more mergeable implementation.

## 10. Problems encountered and solutions

During this realisation phase, no major technical problems occurred that prevented the smart streetlight from being restructured and integrated into the shared project.

The main challenge was no longer hardware wiring, but software organisation. The original code was simple and easy to understand as a local prototype, but it was not yet suitable for a shared project in which multiple modules must coexist. The main issue was therefore not whether the code worked, but how it should be reorganised without losing the original behaviour.

This was solved by separating the smart streetlight into a reusable class with its own files. In this way, the internal logic remained grouped together while the shared main file only handled integration.

Another important point was to verify that the new structure still behaved the same as the original version. This was addressed by testing the `begin()` and `update()` behaviour after the restructuring and by checking that the streetlight still responded correctly to the LDR input and threshold logic.

## 11. Result of the realisation

The result of this phase is a structured smart streetlight implementation inside the shared ESP32 S3 City Hub project.

The original standalone sketch was successfully converted into:

- a separate `StreetLight.h` header file;
- a separate `StreetLight.cpp` implementation file;
- a shared main `.ino` file in which the streetlight is used as a component.

As a result, the smart streetlight functionality is now:

- easier to understand because its responsibilities are grouped together;
- easier to reuse because it can be called from the shared main project;
- easier to merge because the detailed logic is no longer placed directly in the central file;
- still functionally equivalent to the original standalone Sprint 1 version.

## 12. Evaluation of the implementation

The new implementation is stronger than the original standalone sketch in terms of structure and project suitability.

A strong point of the new structure is that it fits the City Hub idea much better. The main `.ino` file now acts more as the central coordination point, while the smart streetlight manages its own internal behaviour inside its own files. This makes the project easier to read and easier to extend with other modules.

Another strong point is that the original functionality was preserved. The smart streetlight still performs the same basic behaviour as before, namely reading the light level and switching the relay based on the threshold value. This means that the restructuring improved the organisation without breaking the functional behaviour.

At the same time, this realisation phase also shows that software structure itself is an important part of embedded development. A working local prototype is not automatically suitable for a larger shared system. The implementation had to move from “working code” to “working code in the right structure”.

Overall, the realisation phase can be considered successful, because the standalone smart streetlight logic was reimplemented as a structured and reusable component inside the shared project.

## 13. Final conclusion

This realise report described how the original standalone smart streetlight logic was implemented as reusable and structured code inside one shared ESP32 S3 City Hub project.

First, the standalone Sprint 1 sketch was taken as the starting point. That version already proved that the smart streetlight could function correctly, but it was not yet suitable for integration into a shared project.

Second, the code was restructured into a library like format. A separate header file was created to define the reusable smart streetlight component, and a separate cpp file was created to contain the internal behaviour of that component. The shared main `.ino` file was then adapted so that it only handled project-level integration by creating the component and calling its methods.

Third, the structured smart streetlight was integrated into the wider shared City Hub project together with other modules. Testing showed that the smart streetlight still behaved correctly and that the original standalone behaviour was preserved.

Based on this realisation report, it can be concluded that the Sprint 2 implementation goal was achieved: the original standalone smart streetlight logic was successfully converted into a clearer, reusable, and mergeable software structure within the shared ESP32 S3 project. This implementation now forms the practical basis for the next phase, in which the chosen software structure can be evaluated and advised on.

## 14. Recommendations

Based on this realise report, the following recommendations are made for the next phase of the project:

1. Keep the structured smart streetlight component as the new basis for further development.
2. Use the new implementation as the reference when evaluating whether the chosen structure is the best solution for the City Hub project.
3. Test the structured smart streetlight further together with additional modules to confirm that the integration remains stable as the shared project grows.
4. Continue applying the same separation principle to other project functions so that the shared project remains organised.
5. Use the results of this realisation phase directly in the advice phase in order to evaluate whether the implemented structure is sufficiently clear, reusable, and scalable for future development.

## 15. Sources

*Reference formatting was made using Scribbr APA Generator.*

1. Sprint 2 [analysis document](https://city-the-embedded-alliance-city-sim-learning-group-4a0740.dev.hihva.nl/Gurpreet/Learning%20goals/Realise/Sprint%201/Proof/realise/).
2. Previous Sprint 1 [realisation document](https://city-the-embedded-alliance-city-sim-learning-group-4a0740.dev.hihva.nl/Gurpreet/Learning%20goals/Realise/Sprint%201/Proof/realise/).