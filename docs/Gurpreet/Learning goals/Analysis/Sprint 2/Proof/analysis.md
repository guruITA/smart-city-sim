# Smart cities learning group goal: Analysis

- Name: Gurpreet Singh
- Date: 25-03-2026

## Table of Contents

- [Table of Contents](#table-of-contents)
- [1. Introduction](#1-introduction)
- [2. Methodology](#2-methodology)
- [3. Working method](#3-working-method)
- [4. Main research question and subquestions](#4-main-research-question-and-subquestions)
- [5. Tools used](#5-tools-used)
- [6. Context of the Sprint 2 development step](#6-context-of-the-sprint-2-development-step)
    - [6.1 From standalone prototype to shared City Hub project](#61-from-standalone-prototype-to-shared-city-hub-project)
    - [6.2 Why the current code structure is no longer sufficient](#62-why-the-current-code-structure-is-no-longer-sufficient)
    - [6.3 Functional requirements for the software structure](#63-functional-requirements-for-the-software-structure)
    - [6.4 Non-functional requirements for the software structure](#64-non-functional-requirements-for-the-software-structure)
    - [6.5 Subconclusion](#65-subconclusion)
- [7. Reusable and mergeable code in the City Sim Learning Group](#7-reusable-and-mergeable-code-in-the-city-sim-learning-group)
    - [7.1 What reusable code means in this project](#71-what-reusable-code-means-in-this-project)
    - [7.2 What mergeable code means in this project](#72-what-mergeable-code-means-in-this-project)
    - [7.3 Risks of unclear code structure](#73-risks-of-unclear-code-structure)
    - [7.4 Subconclusion](#74-subconclusion)
- [8. Possible ways to organise the smart streetlight code](#8-possible-ways-to-organise-the-smart-streetlight-code)
    - [8.1 Keeping everything in one main sketch](#81-keeping-everything-in-one-main-sketch)
    - [8.2 Dividing the code into multiple files](#82-dividing-the-code-into-multiple-files)
    - [8.3 Organising the code as reusable classes or components](#83-organising-the-code-as-reusable-classes-or-components)
    - [8.4 Using a library like structure inside the shared project](#84-using-a-library-like-structure-inside-the-shared-project)
    - [8.5 Subconclusion](#85-subconclusion)
- [9. Which parts of the current smart streetlight functionality should be separated](#9-which-parts-of-the-current-smart-streetlight-functionality-should-be-separated)
    - [9.1 What should be moved to the header file](#91-what-should-be-moved-to-the-header-file)
    - [9.2 What should be moved to the implementation file](#92-what-should-be-moved-to-the-implementation-file)
    - [9.3 What should remain in the main `.ino` file](#93-what-should-remain-in-the-main-ino-file)
    - [9.4 Why this separation fits the City Hub idea](#94-why-this-separation-fits-the-city-hub-idea)
    - [9.5 Subconclusion](#95-subconclusion)
- [10. Acceptance criteria for the Sprint 2 code structure](#10-acceptance-criteria-for-the-sprint-2-code-structure)
    - [10.1 Criteria for clarity and maintainability](#101-criteria-for-clarity-and-maintainability)
    - [10.2 Criteria for reuse and integration](#102-criteria-for-reuse-and-integration)
    - [10.3 Criteria for testing and further development](#103-criteria-for-testing-and-further-development)
    - [10.4 Subconclusion](#104-subconclusion)
- [11. Final conclusion](#11-final-conclusion)
- [12. Recommendations](#12-recommendations)
- [13. Sources](#13-sources)


## 1. Introduction

This analysis is part of the Smart Cities Learning Group project. In Sprint 1, the project focused on building a working automatic smart streetlight prototype with the ESP32-S3. In that phase, the most important goal was to make the hardware and the basic control logic work correctly. The result was a standalone prototype that could switch the streetlight on and off based on the measured light level.

In Sprint 2, the situation changes. The project now moves from a single working prototype towards a broader shared software structure in which the code of all team members must be able to work together in one ESP32-S3 project. Gerald explained that the ESP32 boards should be seen as multifunctional City Hubs. This means that the smart streetlight functionality should no longer remain only a standalone sketch, but should be organised in a way that supports reuse, integration, and merging with the work of other team members.

At the start of this sprint, it is not yet clear which software structure is the most suitable for this next step. It is necessary to analyse what reusable and mergeable code means in the context of this learning group, which parts of the current smart streetlight code should be separated, and which requirements the code structure must meet in order to support a shared project. It is also important to identify the risks of poor structure, such as duplicated logic, unclear responsibilities, and merge conflicts.

The purpose of this analysis is therefore to determine how the smart streetlight functionality should be structured as reusable and mergeable code within one shared ESP32-S3 project. The outcome of this analysis provides the foundation for the design, implementation, and evaluation of the Sprint 2 software structure.

## 2. Methodology

For this analysis, several methods were used:

- reviewing the current standalone smart streetlight code and the results of Sprint 1;
- analysing the new project context in which all code must be combined in one shared ESP32-S3 project;
- studying software organisation approaches for projects, such as functions, multiple files, reusable libraries, and object-oriented structure;
- identifying which parts of the current smart streetlight functionality should be separated into clearer software responsibilities;
- analysing the risks related to unclear responsibilities, code duplication, and merge conflicts;
- defining acceptance criteria for a software structure that is suitable for the design and realisation phase.

This combination of methods was chosen because the Sprint 2 problem is no longer mainly about hardware, but about software structure. The analysis therefore focuses on clarity, maintainability, reusability, and integration within a shared embedded project.

## 3. Working method

The analysis was carried out step by step. First, the context of the Sprint 2 development step was described. This was necessary to explain why the current standalone smart streetlight code is no longer sufficient now that the ESP32 boards must function as multifunctional City Hubs inside one shared project.

After that, the meaning of reusable and mergeable code was analysed in the context of the City Sim Learning Group. This included looking at what kind of software structure is needed when multiple team members work on one shared project and when one ESP32-S3 must eventually support multiple functions.

Next, different ways of organising the smart streetlight code were compared. The analysis looked at keeping everything in one main sketch, dividing code into multiple files, separating logic into classes or components, and using a library-like structure within the shared project. After that, the current smart streetlight functionality was examined to determine which parts should be separated into clearer modules or responsibilities.

Finally, acceptance criteria were defined for a code structure that is suitable for the design and realisation phase. By following this working method, the analysis was built up from project context to software evaluation and finally to a justified direction for Sprint 2.

## 4. Main research question and subquestions

The main research question of this analysis is:

How should the smart streetlight functionality be structured as reusable and mergeable code within one shared ESP32-S3 project for the City Sim Learning Group?

To answer this main research question, the following subquestions were formulated:

1. Why is the current standalone smart streetlight code no longer sufficient in the Sprint 2 project context?
2. What do reusable and mergeable code mean in the context of the City Sim Learning Group?
3. Which software organisation approaches are suitable for structuring the smart streetlight functionality?
4. Which parts of the current smart streetlight functionality should be separated into modules, classes, or components?
5. Which risks arise when the code structure remains unclear or unstructured?
6. Which acceptance criteria should the Sprint 2 code structure meet in order to support the design and realisation phase?

## 5. Tools used

For this analysis, the following tools were used:

- the current smart streetlight code from Sprint 1 as the starting point;
- the previous Sprint 1 analysis, design, and realisation documents as project references;
- technical sources about code organisation and reusable software structure;
- Scribbr for formatting references correctly;
- ChatGPT for support with language use, phrasing, spelling, and grammar.

## 6. Context of the Sprint 2 development step

Before analysing the software structure itself, it is first necessary to explain the project context of Sprint 2. This chapter describes why the development direction has changed and which requirements now follow from that change.

### 6.1 From standalone prototype to shared City Hub project

In Sprint 1, the smart streetlight was mainly developed as a local prototype on one tile. The most important goal in that phase was to prove that the streetlight could respond to the light level and switch correctly between light and dark conditions. That goal was suitable for a first prototype, because the project first had to demonstrate the basic concept.

In Sprint 2, the situation changes because the code of all team members must eventually be merged into one shared ESP32-S3 project. The ESP32 boards are therefore no longer treated as isolated controllers for one local function, but as multifunctional City Hubs that may need to support multiple functions within the learning group project. This means that code written only for one isolated use case becomes less suitable.

### 6.2 Why the current code structure is no longer sufficient

The current standalone smart streetlight code is useful as a first working version, but it is mainly focused on solving one function in one place. Such a structure can be acceptable in an early prototype, but it becomes problematic when more functions, more files, and more contributors are added.

As projects grow in complexity, proper code organisation becomes more important because it improves readability, reduces duplication, makes debugging easier, supports reuse across projects, and helps multiple people work on the same project [(Arduino Code Organization | Compile N Run, z.d.)](https://www.compilenrun.com/docs/iot/arduino/arduino-project-structure/arduino-code-organization/). In the context of the City Sim Learning Group, this means that the software structure should now support not only local functionality, but also shared development and future extension.

### 6.3 Functional requirements for the software structure

The software structure for Sprint 2 must support the following functional requirements:

1. The smart streetlight functionality must remain able to read the light value and control the streetlight behaviour.
2. The code must be organised in a way that allows it to be called from one shared main project.
3. The structure must make it possible to separate the smart streetlight behaviour from unrelated functionality of other team members.
4. The structure must support further extension, for example with extra sensors, other logic, or additional control behaviour.

### 6.4 Non-functional requirements for the software structure

The software structure must also support the following non-functional requirements:

1. The code must be clear and understandable for me and other team members.
2. The structure must reduce duplication and unclear responsibilities.
3. The structure must support safer merging in a shared repository.
4. The structure must be maintainable and testable.
5. The structure must fit the City Hub idea, in which one ESP32-S3 can support the logic of multiple prototypes.

### 6.5 Subconclusion

The Sprint 2 context differs from Sprint 1 because the smart streetlight is no longer only a local standalone prototype, but must become part of one shared ESP32-S3 project. This makes the current standalone structure less suitable. The code structure must now support readability, reuse, maintainability, merging, and future extension.

## 7. Reusable and mergeable code in the City Sim Learning Group

This chapter explains what reusable and mergeable code mean in the context of this project.

### 7.1 What reusable code means in this project

Reusable code is code that is organised in such a way that the same logic can be used again without rewriting it every time. In this project, that means that the smart streetlight functionality should not be buried inside one large sketch in a way that only works for one file and one developer. Instead, the behaviour should be separated into parts that can be called, maintained, and extended more easily.

Good code organisation supports this by breaking code into smaller focused functions, multiple files, reusable libraries, and classes that encapsulate related data and behaviour [(Arduino Code Organization | Compile N Run, z.d.)](https://www.compilenrun.com/docs/iot/arduino/arduino-project-structure/arduino-code-organization/). For the smart streetlight, this means that reading sensor input, controlling switching logic, and handling configuration should no longer be treated as one inseparable block.

### 7.2 What mergeable code means in this project

Mergeable code is code that can be combined with the work of others with a lower risk of conflicts and confusion. In the City Sim Learning Group, this is important because the work of multiple prototypes must eventually come together in one shared ESP32-S3 project.

When code remains as one large standalone sketch, responsibilities are often mixed together, which increases the risk that different team members will change the same files or duplicate similar logic. By contrast, using multiple files and logical components helps keep code organised by feature and makes larger projects easier to manage [(Arduino Code Organization | Compile N Run, z.d.)](https://www.compilenrun.com/docs/iot/arduino/arduino-project-structure/arduino-code-organization/).

### 7.3 Risks of unclear code structure

If the software structure remains unclear, several risks can arise:

- responsibilities may remain mixed together in one file;
- logic may be duplicated because there is no clear reusable structure;
- code from multiple team members may become harder to merge;
- future extensions may require rewriting instead of extending.

These risks are especially relevant in a shared project, because unclear structure affects not only one developer, but the whole team.

### 7.4 Subconclusion

In this project, reusable code means code that is separated into clear responsibilities and can be used again without unnecessary rewriting. Mergeable code means code that is organised in a way that supports teamwork inside one shared ESP32-S3 project. Both goals require a clearer software structure than the standalone Sprint 1 sketch.

## 8. Possible ways to organise the smart streetlight code

This chapter compares different ways to organise the smart streetlight functionality.

### 8.1 Keeping everything in one main sketch

The simplest option is to keep everything in one main sketch. This can work for very small projects, but it quickly becomes difficult to manage when the project grows. As projects become more complex, writing everything only in `setup()` and `loop()` becomes less suitable and better organisation techniques become necessary [(Arduino Code Organization | Compile N Run, z.d.)](https://www.compilenrun.com/docs/iot/arduino/arduino-project-structure/arduino-code-organization/).

For the shared City Hub project, this option is the least suitable, because it keeps responsibilities mixed together and makes collaboration harder.

### 8.2 Dividing the code into multiple files

A stronger option is to divide the code into multiple files. This allows the code to be organised by logical components instead of placing everything in one sketch. Using multiple files helps keep code organised by feature and makes it easier to manage larger projects [(Arduino Code Organization | Compile N Run, z.d.)](https://www.compilenrun.com/docs/iot/arduino/arduino-project-structure/arduino-code-organization/).

For the smart streetlight, this would already be an improvement, because sensor input, switching behaviour, and configuration could be placed more clearly.

### 8.3 Organising the code as reusable classes or components

A further improvement is to use classes or components so that related data and behaviour are grouped together. Object-oriented organisation can make code more modular and reusable by encapsulating related functionality [(Arduino Code Organization | Compile N Run, z.d.)](https://www.compilenrun.com/docs/iot/arduino/arduino-project-structure/arduino-code-organization/)

For this project, that means that the smart streetlight could be represented as a component with its own responsibilities instead of as a loose collection of code fragments.

### 8.4 Using a library like structure inside the shared project

For functions that must be reused across projects or across a wider codebase, a library like structure is the strongest option. The source explains that custom libraries provide maximum reusability and organisation, usually with a header file and an implementation file [(Arduino Code Organization | Compile N Run, z.d.)](https://www.compilenrun.com/docs/iot/arduino/arduino-project-structure/arduino-code-organization/)

In the City Sim Learning Group, this does not necessarily mean publishing a separate external library, but it does mean structuring the smart streetlight code in a similar way inside the shared project, for example with its own `.h` and `.cpp` files.

### 8.5 Subconclusion

Keeping everything in one sketch is the weakest option for Sprint 2. Dividing code into multiple files is already better, but a library-like structure with clear classes or components is the most suitable direction because it best supports reuse, maintainability, and merging inside one shared project.

## 9. Which parts of the current smart streetlight functionality should be separated

This chapter explains which parts of the original standalone smart streetlight code should be separated in the new software structure.

### 9.1 What should be moved to the header file

The header file should contain the declaration of the reusable smart streetlight component. This means that it should describe what the smart streetlight is and which public functionality it offers to the rest of the project, without already containing the full working logic itself.

Based on the original code, the header file should contain the class definition of the smart streetlight component, the private attributes for the LDR pin, relay pin, threshold value, interval, and timing variable, and the public methods that the main project must be able to call, such as the constructor, `begin()`, and `update()`.

The reason for putting these parts in the header file is that the shared main project must be able to know which functionality the smart streetlight component provides, without needing to see all implementation details in the main file itself.

### 9.2 What should be moved to the implementation file

The implementation file should contain the actual working logic of the smart streetlight. In the original standalone sketch, this logic was still mixed directly into `setup()`, `loop()`, and the `updateStreetLight()` function. In the new structure, these parts should be moved into the `.cpp` file.

This means that the constructor logic, the pin setup, the serial start, the timed update logic, the `analogRead()` of the LDR module, the comparison with the threshold value, the relay control with `digitalWrite()`, and the serial debug messages should all be placed in the implementation file.

The reason for this is that the `.cpp` file should contain the internal behaviour of the smart streetlight component. In this way, the smart streetlight logic remains grouped together in one place instead of being spread across the shared main project.

### 9.3 What should remain in the main `.ino` file

The main `.ino` file should only contain the project-level control that is needed to use the smart streetlight component inside the shared ESP32-S3 project. Based on the original code, the main file should therefore no longer contain the full sensor reading and relay switching logic itself.

Instead, the main `.ino` file should include the smart streetlight header, create an instance of the smart streetlight component with the required configuration values, call `begin()` inside `setup()`, and call `update()` inside `loop()`.

This is important because the main `.ino` file will eventually need to work together with the code of other team members. If the smart streetlight logic remains fully inside the main file, the shared project would become harder to read, harder to merge, and harder to extend.

### 9.4 Why this separation fits the City Hub idea

This separation supports the idea that the ESP32-S3 should function as a multifunctional City Hub. In such a project, the main file should act more as the central coordination point, while each separate city function should manage its own internal logic inside its own files.

For the smart streetlight, this means that the old standalone sketch should be restructured into a component with a clear boundary. The header file defines the interface, the implementation file contains the behaviour, and the main `.ino` file only controls how the component is used within the shared project. This makes it easier to combine the smart streetlight with other modules later, such as traffic lights, sensors, or additional smart city functions.

### 9.5 Subconclusion

The original standalone smart streetlight code should be separated into three levels. The header file should contain the declaration of the reusable smart streetlight component, including its attributes and public methods. The implementation file should contain the actual working logic, including setup behaviour, sensor reading, threshold comparison, relay control, and timed updates. The main `.ino` file should only contain the integration of the component into the wider shared project by creating the object and calling its methods.

This separation makes the code structure clearer, more reusable, and more suitable for merging into one shared ESP32-S3 project.
## 10. Acceptance criteria for the Sprint 2 code structure

This chapter defines the acceptance criteria for the code structure that will be designed and implemented in the next phases.

### 10.1 Criteria for clarity and maintainability

The code structure must meet the following criteria:

1. responsibilities are clearly separated;
2. the main project does not contain unnecessary detailed smart streetlight logic;
3. files, classes, or components have clear names and clear purposes;
4. the structure remains understandable for both the author and team members.

### 10.2 Criteria for reuse and integration

The code structure must also meet these criteria:

1. the smart streetlight functionality can be called from one shared ESP32-S3 project;
2. the structure supports integration with other team members’ work;
3. the design reduces the risk of duplication and unclear overlap;
4. the smart streetlight functionality can be extended without rewriting the whole structure.

### 10.3 Criteria for testing and further development

Finally, the structure must meet the following criteria:

1. the separated functionality can still be tested in practice;
2. the behaviour of the smart streetlight remains equivalent to the original standalone version;
3. the structure is suitable for the design and realisation phase;
4. the structure supports later extensions in the City Hub project.

### 10.4 Subconclusion

A suitable Sprint 2 code structure must be clear, maintainable, reusable, mergeable, and testable. These acceptance criteria define the standard that the design and realisation phase must meet.

## 11. Final conclusion

This analysis examined how the smart streetlight functionality should be structured as reusable and mergeable code within one shared ESP32-S3 project for the City Sim Learning Group.

First, it was established that the Sprint 2 context differs from Sprint 1 because the smart streetlight is no longer only a standalone prototype, but must become part of a shared City Hub project. This means that the original standalone structure is no longer sufficient when the work of multiple team members must be combined.

Second, the analysis showed that reusable code in this project means code with clearly separated responsibilities that can be used again without unnecessary rewriting, while mergeable code means code that can be integrated more safely with the work of others. This requires a more structured approach than one large sketch.

Third, different software organisation approaches were compared. Keeping everything in one main sketch was found to be the weakest option, while multiple files, classes, and a library-like structure provide stronger support for reuse and maintainability. The analysis showed that a library-like structure inside the shared project is the most suitable direction.

Fourth, the analysis identified that the current smart streetlight functionality should be separated into a clearer three level structure. The declaration of the reusable smart streetlight component should be placed in a header file, the actual working behaviour should be placed in an `cpp` file file, and the shared main `.ino` file should only contain the project level integration of that component. This makes the code easier to understand, test, reuse, and merge.

Finally, acceptance criteria were defined for the Sprint 2 code structure. These criteria show that the new structure must support clarity, maintainability, reuse, integration, testing, and future extension.

Based on this full analysis, it can be concluded that the most suitable direction for Sprint 2 is to restructure the current standalone smart streetlight code into a clearer, library-like software structure within the shared ESP32-S3 project. This direction best supports the City Hub idea and provides the strongest basis for the design and realisation phase.

## 12. Recommendations

Based on the results of this analysis, the following recommendations are made for the next phase of the project.

1. Use this analysis as the basis for the design phase.  
The results of this analysis should be translated into a clear software design with separated responsibilities and a visual UML representation.

2. Design the smart streetlight as a reusable component within the shared project.  
The next phase should focus on a structure in which the smart streetlight can be initialised and updated from the shared main project without embedding all logic directly in that project file.

3. Separate the current standalone logic into clear software responsibilities.  
At minimum, sensor input, switching logic, configuration, and project integration should be separated in the new design.

4. Reduce the risk of merge conflicts by limiting overlap in shared files.  
The structure should be designed in such a way that the smart streetlight code can be changed mostly inside its own files instead of constantly requiring edits in the same central logic as other team members.

5. Keep the new structure testable against the original behaviour.  
The new code structure should still allow verification that the smart streetlight behaves the same as the original working Sprint 1 version.

## 13. Sources

*Reference formatting was made using Scribbr APA Generator.*

1. Previous Sprint 1 [analysis document](https://city-the-embedded-alliance-city-sim-learning-group-4a0740.dev.hihva.nl/Gurpreet/Learning%20goals/Analysis/Sprint%201/Proof/analysis/).
2. Previous Sprint 1 [design document](https://city-the-embedded-alliance-city-sim-learning-group-4a0740.dev.hihva.nl/Gurpreet/Learning%20goals/Design/Sprint%201/Proof/design/).
3. Previous Sprint 1 [realisation document](https://city-the-embedded-alliance-city-sim-learning-group-4a0740.dev.hihva.nl/Gurpreet/Learning%20goals/Realise/Sprint%201/Proof/realise/).
4. Arduino Code Organization | Compile n run. (z.d.). Compile N Run. [https://www.compilenrun.com/docs/iot/arduino/arduino-project-structure/arduino-code-organization/](https://www.compilenrun.com/docs/iot/arduino/arduino-project-structure/arduino-code-organization/) viewed on 25 March 2026.
5. Writing a library for Arduino | Arduino Documentation. (n.d.). [https://docs.arduino.cc/learn/contributions/arduino-creating-library-guide/](https://docs.arduino.cc/learn/contributions/arduino-creating-library-guide/) viewed on 25 March 2026
