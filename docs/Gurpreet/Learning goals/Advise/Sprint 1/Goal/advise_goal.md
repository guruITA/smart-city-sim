# City sim learning group – Advice

### Learning Question

Which switching solution is the most suitable for further development of the smart streetlight prototype with the ESP32-S3: the current relay-based solution or a MOSFET-based alternative?

### S - Situation

In the first sprint of the City Sim Learning Group, I am working on the smart streetlight for my wooden city tile. In the previous phases, I first analysed which components, GPIO connections, and power supply configuration were needed, then translated those outcomes into a design, and after that realised the prototype as a working Sprint 1 system. At this point, the prototype already works in practice with an LDR module, a relay module, and an LED circuit that switches on and off based on ambient light. Because of that, the next step is no longer only to prove that the prototype works, but also to determine whether the chosen switching solution is still the most suitable direction for further development.

### T - Task

My task in this phase is to evaluate the current relay-based solution and determine whether it should remain the preferred switching solution for the smart streetlight prototype, or whether a MOSFET-based alternative would be more suitable for a next version. To do that, I need to describe the starting point from the previous phases, identify the strengths and limitations of the relay module, study the MOSFET as an alternative switching solution, compare both options against the current and possible future requirements of the prototype, and formulate a clear recommendation for further development in a structured advice report.

### A - Action

To complete this task, I first reviewed the outcomes of the previous phases. I used the analysis, design, and realisation reports to determine how the relay module had been selected, how it had been represented in the design, and how it had performed in the realised Sprint 1 prototype. In this way, I used the current relay-based prototype as the practical starting point for the advice phase.

After that, I analysed the relay module as the current switching solution. I described why it was suitable for Sprint 1, especially because it supported simple and understandable on/off switching and had already been proven to work in practice. At the same time, I also examined its limitations, especially in relation to more advanced future behaviour such as dimming, more efficient switching, and further expansion of the prototype.

Next, I explored a MOSFET-based switching solution as a possible alternative for a next version of the prototype. I studied technical information about MOSFET switching and examined how it differs from relay-based switching in terms of switching behaviour, efficiency, and suitability for more advanced control such as PWM.

After that, I compared both solutions using the same criteria. I looked at their suitability for the current Sprint 1 prototype, their suitability for future development, their efficiency and switching behaviour, and their scalability in relation to GPIO usage and possible future additions such as a PIR sensor. Based on this comparison, I formulated a recommendation for which switching solution should be preferred in the next stage of the project.

### R — Result

The result of this phase is a complete advice report in which I determined which switching solution is the most suitable for further development of the smart streetlight prototype. In that report, I concluded that the relay module remains a suitable solution for the current Sprint 1 prototype, because it is simple, understandable, and already proven to work in practice. At the same time, I concluded that a MOSFET-based solution is the better recommendation for a future version of the prototype, because it is more suitable for advanced control behaviour such as dimming, more efficient switching, and further expansion with extra functionality.

This means that the advice phase did not reject the relay module as a wrong choice, but instead placed it in the right project context. The relay module was the right solution for the first realised version, while the MOSFET is the more suitable direction for further development.

### R - Reflection

By completing this advice phase, I learned that a working prototype is not automatically the final or best technical solution. In the realisation phase, the most important question was whether the prototype could function in practice. In the advice phase, I had to take a step back and look at the same prototype from a different perspective, namely whether the chosen solution is still the best option once the project becomes more advanced.

What helped me in this phase was that I already had a working prototype as a reference point. Because of that, I was not comparing abstract theory only, but was able to judge the relay solution based on something that had already been designed, built, and tested. This made the advice more realistic and more grounded in the actual project situation.

At the same time, this phase also showed me that technical advice is not only about choosing the most advanced option. A MOSFET may be technically stronger for future development, but that does not mean the relay module was a bad choice. I learned that the quality of an advice depends on how well it matches the project stage, the learning context, and the future direction of the prototype. That made this phase different from the previous ones, because I had to balance what works now with what will be more suitable later.

### T - Transfer

I will use the results of this advice phase directly in  further development of the smart streetlight prototype in future spints. The recommendation from this phase will serve as the basis for deciding whether the next prototype version should remain relay-based or should move toward a MOSFET-based switching design. In that way, the advice phase does not remain theoretical, but becomes the decision-making bridge between the current realised prototype and a more advanced future version.

I will also be able to use the same working method in later sprints, where I may again need to compare a current working solution with a more advanced alternative. The combination of looking back at earlier phases, analysing strengths and limitations, comparing alternatives, and ending with a justified recommendation has therefore become a reusable approach for future technical advice within this project.

### Appendix

The full advice document that belongs to this learning goal can be found [here](https://city-the-embedded-alliance-city-sim-learning-group-4a0740.dev.hihva.nl/Gurpreet/Learning%20goals/Advise/Sprint%201/Proof/advice/).