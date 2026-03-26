# City sim learning group – Design

### Learning Question

How can the results of the Sprint 2 analysis phase be translated into a clear and structured UML design for the smart streetlight code, so that the functionality can be organised as reusable and mergeable code within one shared ESP32-S3 project?

### S - Situation

In Sprint 2 of the City Sim Learning Group, I am no longer only focused on the physical smart streetlight prototype, because the first version has already been analysed, designed, and realised in Sprint 1. In this sprint, the project moves toward a software structure in which the code of all team members must eventually be merged into one shared project that can run on all ESP32 boards of the learning group. Gerald explained that the ESP32 boards should be treated as multifunctional City Hubs, which means that my smart streetlight code should not remain a separate standalone sketch, but must be organised in a way that can work together with the code of other tiles and functions. At this point, the code structure still only exists as an idea from the analysis phase and not yet as a clear software design. Because of that, I first need to translate the analysis into a structured UML design that shows how the smart streetlight functionality can be divided into classes, responsibilities, and relations before moving on to the realisation phase.

### T - Task

My task in this phase is to convert the outcomes of the Sprint 2 analysis phase into a clear and reproducible software design for the smart streetlight functionality. To do that, I need to determine which design requirements the code structure must meet, identify which parts of the smart streetlight logic should be separated into classes, describe the responsibilities of those classes, and define how those classes should work together inside one shared ESP32-S3 project. I also need to visualise these design choices in a UML design and document them clearly, so that the result can be used directly in the realisation phase and can also be understood, reused, and merged with the work of other team members.

### A - Action

### R - Result

### R - Reflection

### T - Transfer

### Appendix

The full design document that belongs to this learning goal can be found [here]().