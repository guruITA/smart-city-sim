# City sim learning group – Design

### Learning Question

How can the results of the analysis phase be translated into a clear, practical, and reproducible breadboard design for the automatic smart streetlight prototype with the ESP32-S3?

### S - Situation

In the first sprint of the City Sim Learning Group, I am working on the design of a smart streetlight for my wooden city tile. In the analysis phase, I already determined which components, GPIO connections, and power supply configuration are most suitable for the prototype. However, at this point, the prototype still only exists as a technical idea and not yet as a clear visual design. Because I am still a beginner in Embedded Systems and Robotics, it is important for me to make the setup understandable before physically building it. For that reason, I first need to translate the analysis into a practical breadboard design that shows clearly how the ESP32-S3, the LDR module, the relay module, the LEDs, the resistors, and the protected power input section should be connected.

### T - Task

My task in this phase is to convert the outcomes of the analysis phase into a clear and reproducible design for the Sprint 1 smart streetlight prototype. To do that, I need to determine which design requirements the prototype must meet, explain why a breadboard and Fritzing are suitable design tools, describe how the main components should be placed in the design, and show how the power path should be structured. I also need to document these findings in a clear design report, so that the result can be used directly in the realisation phase and can also be understood and reproduced by other team members.

### A - Action

To complete this task, I first reviewed the most important outcomes of the analysis phase. I used these results as the technical basis for the design, especially the chosen components, the selected GPIO pins, the repeated LED-resistor structure, and the preferred external power setup with a fuse, diode, and electrolytic capacitor.

After that, I studied how a breadboard works and how Fritzing can be used as a visual design tool. I looked at the internal structure of the breadboard, such as the connected rows, the power rails, and the centre gap, because that was necessary to understand how the different parts could be placed logically. I also studied the function of Fritzing as a design tool, so that I could use it not only to draw a circuit, but also to create a visual layout that closely resembles the intended physical prototype.

Next, I translated the technical choices from the analysis into actual design decisions. I determined the placement of the ESP32-S3, the external power connection, the fuse, the diode, the electrolytic capacitor, the LDR module, the relay module, and the repeated LED-resistor structure. I also described how the power distribution should run through the design and how the relay should function as the switching element between the control side and the LED circuit.

Finally, I documented the full design in a structured report and created the Fritzing breadboard view that visually shows how the prototype should be wired in practice. In this way, I converted the earlier analysis into a design that is both technically grounded and practically usable.

### R - Result

The result of this phase is a complete design document in which I translated the analysis into a clear and reproducible breadboard design for the Sprint 1 smart streetlight prototype. In that document, I concluded that the prototype should be designed around an ESP32-S3 as the central controller, an LDR module as the input component, a relay module as the switching component, and a repeated LED-resistor structure as the output. I also included the protected external power input section with an external power connection, a fuse, a diode, and an electrolytic capacitor.

The result of this phase is not only a written design report, but also a Fritzing breadboard design that shows how the circuit should be wired physically. This made the design phase the direct bridge between the technical conclusions from the analysis phase and the practical implementation in the realisation phase.

### R - Reflection

By completing this design phase, I learned that designing a prototype is not only about drawing components, but mainly about translating technical choices into a clear and logical structure. During the analysis phase, I mainly focused on what was needed and why, but in the design phase I had to think much more carefully about how everything should be placed and connected in a way that remains understandable for myself and for others.

I also noticed that the breadboard and Fritzing were useful tools for me as a beginner, because they helped me visualise the prototype before physically building it. At the same time, I realised that a digital design does not automatically solve everything, because software does not always contain the exact same parts as the real components used in practice.

What this phase made especially clear to me is that a technically correct idea still needs a clear structure before it can be implemented successfully. The design phase forced me to think more carefully about placement, reproducibility, and readability, and that made the prototype more understandable as a complete system.

### T - Transfer

I will use the results of this design phase directly in the next phase, namely the realisation phase. The Fritzing design, the placement of the components, and the structured power path will be used as the practical basis for physically building the smart streetlight prototype on the breadboard. In that way, the design will function as the direct bridge between analysis and implementation.

I will also be able to use the same working method in later sprints, for example when designing a speed camera with additional inputs and outputs. The combination of technical analysis, visual translation, and structured documentation will therefore become a reusable approach for my further development in Embedded Systems and Robotics.

### Appendix

The full design document that belongs to this learning goal can be found [here](https://city-the-embedded-alliance-city-sim-learning-group-4a0740.dev.hihva.nl/Gurpreet/Learning%20goals/Design/Sprint%201/Proof/design/).