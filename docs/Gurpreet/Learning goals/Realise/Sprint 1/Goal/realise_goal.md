# City sim learning group – Realise 

### Learning Question

How can the designed automatic smart streetlight prototype with the ESP32-S3 be physically built and tested as a working Sprint 1 prototype?

### S - Situation

In the first sprint of the City Sim Learning Group, I am working on the realisation of a smart streetlight for my wooden city tile. In the analysis phase, I already determined which components, GPIO connections, and power supply configuration were most suitable for the prototype. In the design phase, I translated those outcomes into a clear breadboard and Fritzing design. However, at this point, the prototype still only exists in a digital design. Because of that, I now need to physically build the prototype and test whether the design also works in practice. This is important, because only after the realisation phase it becomes clear whether the earlier choices were not only theoretically correct, but also practically workable.

### T - Task

My task in this phase is to translate the design into a physical breadboard setup and test whether the automatic smart streetlight works as intended in practice. To do that, I need to build the prototype with the ESP32-S3, the LDR module, the relay module, the LED circuit, and the protected power input section. I also need to upload and test the code, observe whether the prototype responds correctly to light and dark conditions, identify any practical problems in the setup, and document the results in a clear realisation report so that the outcome can be evaluated and used in the next phase.

### A - Action

To complete this task, I first used the design from the previous phase as the practical starting point for the breadboard setup. Based on that design, I placed the ESP32-S3 on the breadboard and prepared the power and ground rails. After that, I connected the main components one by one, namely the LDR module as the input, the relay module as the switching component, and the repeated LED-resistor structure as the light output.

Next, I also added the protected power input section to the realised setup. This included the fuse holder with a 1 A fuse, the 1N4007 diode, and the 1000 µF / 25 V electrolytic capacitor. In this way, the realised prototype did not only follow the main control and output logic, but also the protected power design that had already been chosen in the analysis and design phases.

After building the hardware, I uploaded the code to the ESP32-S3. The code used in this prototype was based on the logic from the YouTube orientation source that was also discussed earlier in `analysis.md`, namely *ESP32 Light Sensor Relay Control - Smart Automation with Wokwi!* by sm Tronics. After that, I checked the code against the Arduino Language Reference to better understand and verify the functions and structure that were used, such as `pinMode()`, `digitalWrite()`, `analogRead()`, and `delay()`.

Finally, I tested the working behaviour of the prototype by changing the amount of light reaching the LDR module. During these tests, I checked whether the measured light value changed correctly, whether the relay switched as expected, and whether the LED circuit turned on and off in response to light and dark conditions. I also observed the practical build quality of the setup and noted which parts of the physical wiring could still be improved.

### R - Result

The result of this phase is a physically built and working Sprint 1 prototype of the automatic smart streetlight. In this realised setup, the ESP32-S3 functions as the controller, the LDR module as the light sensor input, the relay module as the switching component, and the white LEDs with separate 220 ohm resistors as the light output. In addition, the prototype includes a protected power input section consisting of an external 5 V / 1 A adapter, a fuse holder with fuse, a 1N4007 diode, and a 1000 µF / 25 V electrolytic capacitor.

The realised prototype is able to detect ambient light through the LDR module, process that value with the ESP32-S3, switch the relay module on and off, and turn the LED circuit on in darker conditions and off in lighter conditions. This means that the designed prototype was successfully translated into a working breadboard implementation. At the same time, the realisation phase also made visible that the breadboard became crowded because of the large number of jumper wires, which reduced the visual clarity.

### R - Reflection

By completing this realisation phase, I learned that there is an important difference between a technically correct design and a physically workable prototype. In the earlier phases, I mainly focused on what the prototype should do and how the design should look, but during the realisation phase I experienced what happens when all of those choices actually come together on the breadboard. This helped me understand the prototype much better as a complete system.

I also noticed that the earlier analysis and design phases gave me a strong basis during implementation. Because the components, GPIO pins, and power setup had already been thought through, I was able to build the prototype with more direction and less guessing. At the same time, this phase also showed me that a working prototype can still have weaknesses. In my case, the main issue was not that the prototype failed technically, but that the breadboard became visually crowded because of the large number of jumper wires. That made it harder to immediately see which connection belonged where.

This phase made clear to me that realisation is not only about making something work once, but also about evaluating whether the build quality, clarity, and reproducibility are good enough for further development. So even though the prototype worked, I also learned that there is still room for improvement in how clearly and neatly the setup is built.

### T - Transfer

I will use the results of this realisation phase directly in the next phase, namely the advising phase. Because the prototype now works in practice, I can move beyond the question of whether the relay-based solution functions at all and instead evaluate whether it is also the most suitable solution for further development. In the advising phase, I will therefore use the realised prototype as the practical starting point for comparing the current relay-based design with possible alternatives, such as a MOSFET-based switching design.

In that way, the realisation phase will not only serve as the practical confirmation of the earlier analysis and design, but also as the foundation for making a more informed technical recommendation for the next version of the smart streetlight. I will also be able to use the same working method in later sprints, where analysis, design, realisation, and advice will again build on each other step by step.

### Appendix

The full realisation document that belongs to this learning goal can be found [here](https://city-the-embedded-alliance-city-sim-learning-group-4a0740.dev.hihva.nl/Gurpreet/Learning%20goals/Realise/Sprint%201/Proof/realise/).