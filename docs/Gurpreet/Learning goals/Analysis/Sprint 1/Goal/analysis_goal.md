# City Sim Learning Group - Analysis

### Learning Question

How can I analyse which components, GPIO connections, and power supply configuration are needed to build a safe, reliable, and scalable automatic smart streetlight prototype with the ESP32-S3 for Sprint 1?

### S - Situation

In the first sprint of the City Sim Learning Group, I am working on a smart streetlight for my wooden city tile. The idea is that the streetlight should not simply stay on all the time, but should respond automatically to the environment by switching on when it becomes dark and switching off again when it becomes light. At the start of this sprint, I am still new to Embedded Systems and Robotics, and I do not yet clearly understand how the ESP32-S3 works, which GPIO pins are suitable, which components are needed, or how the power supply should be set up in a safe, reliable, and scalable way. Because of that, I first need to analyse the problem before moving on to the design and realisation phases.

### T - Task

My task in this phase is to determine what the automatic smart streetlight prototype should do and what is technically needed to realise it with the ESP32-S3. To do that, I need to describe the context and project goal of the prototype, formulate the functional and non-functional requirements, identify which components are needed and explain their function, determine which GPIO connections are suitable, estimate the total current consumption of the prototype, and analyse which power supply configuration is the most appropriate. I also need to structure these findings in a clear analysis document.

### A - Action

To complete this task, I first explored existing examples of smart streetlight by reviewing multiple YouTube videos and tutorials. I used this orientation phase to understand which components returned frequently in practice and which basic logic was commonly used in simple automatic lighting systems. After that, I described the context of a smart streetlight and translated the project idea into a more concrete Sprint 1 goal. I then formulated functional requirements, such as automatic switching based on ambient light and an adjustable threshold value, and non-functional requirements, such as electrical correctness, reliability, clarity, reproducibility, and scalability.

Next, I analysed the main components needed for the prototype, especially the LDR module, the relay module, the LEDs, and the resistors, and studied how each of these components works and why it is relevant for the system. In parallel, I compared tutorial examples with the actual ESP32-S3 used in the project, because I noticed that many examples used different ESP32 boards with different GPIO options. I therefore checked the official documentation of the ESP32-S3 and the available clone documentation to determine which pins were suitable for connecting the LDR module and the relay module.

After that, I analysed the estimated total current consumption of the prototype by calculating the expected current of the LEDs, the relay module, the LDR module, and the ESP32-S3. Based on those calculations, I compared different power supply options. I first considered a separate 5 V battery, but after further analysis I compared that idea with a breadboard power supply and an external 5 V 1 A adapter with additional protection and stability components such as a fuse, diode, and capacitor.

### R - Result

The result of this phase is a complete analysis document in which I determined which components, GPIO connections, and power supply configuration are most suitable for the Sprint 1 automatic smart streetlight prototype. In that document, I concluded that the prototype should use an ESP32-S3, an LDR module as the input component, a relay module as the switching component, and 20 white LEDs with separate 220 ohm resistors as the output. I also concluded that GPIO4 is a suitable choice for the LDR module and GPIO5 for the relay module. In addition, I estimated the total current consumption of the prototype at approximately 280 mA to 347 mA without Wi-Fi and concluded that the most suitable power solution for further development is a protected 5 V 1 A external supply with a fuse, a 1N4007 diode, and a 1000 µF / 25 V electrolytic capacitor. This analysis document then became the direct basis for the design phase.

### R - Reflection

By completing this analysis, I gained a much clearer understanding of what is actually needed to build the smart streetlight prototype. At the start, I mainly saw the project as a simple lamp that should turn on in the dark, but during the analysis I realised that even a relatively simple prototype already requires multiple technical choices about components, GPIO usage, current consumption, and power design. I also noticed that tutorial examples can be helpful as orientation, but that they cannot simply be copied without checking whether they fit the actual microcontroller and project situation.

At the same time, this phase also showed me where my beginner level still limited me. I found it challenging to assess the power design and to connect the calculations to practical safety choices such as the fuse, diode, and capacitor. I also noticed that I initially underestimated how many technical decisions were needed before I could even start designing the prototype. This made clear to me that a technically working idea is not enough by itself, because it also has to be properly analysed and supported before it can be translated into a design.

### T — Transfer

I will use the results of this analysis directly in the next phase, namely the design phase. The identified components, the chosen GPIO pins, the current calculations, and the power design will be translated into the Fritzing design and breadboard layout of the prototype. In that way, the analysis will not remain theoretical, but will function as the technical foundation for the next learning goal. I will also be able to use the same working method in later sprints, for example when analysing a speed camera with additional inputs and outputs. The combination of orientation, documentation study, technical comparison, calculations, and structured reporting will therefore become a reusable approach for my further development in Embedded Systems and Robotics.

### Appendix

The full analysis document that belongs to this learning goal can be found [here]()