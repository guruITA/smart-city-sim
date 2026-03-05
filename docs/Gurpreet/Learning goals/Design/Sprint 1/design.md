# Smart Cities Learning group goal: Design

Naam: Gurpreet Singh

Datum: 11-2-2026

Smart Streetlight – Fritzing Schematic

Learning Question
How can I design a clear and correct schematic for a smart streetlight using an ESP32-S3, a light sensor and a lamp, so that I can actually build it later?

S — Situation
After analysing what my smart streetlight should do and which components I probably need, the next step is to turn this into a concrete design. At this moment I only have a rough idea in my head and some notes about the ESP32-S3 and possible sensors. I have never made a hardware schematic before, and I only know Fritzing from examples. For Sprint 1 I need a design that I can both understand myself and use as a basis for the prototype on my wooden tile.

T — Task
My goal in this phase is to create a simple but correct schematic for my smart streetlight. The design should show how the ESP32-S3-DevKitC-1, the chosen light sensor and the streetlight LED are connected. The schematic must be clear enough that I can use it later to wire everything without guessing.

A — Action
To reach this goal I will first choose one concrete light sensor based on my earlier analysis. I will look up the wiring in datasheets and example projects to see how this sensor is normally connected to a microcontroller. Using the ESP32-S3 documentation I will research, based on datasheets and example projects or my own decision, which pins I will use for the sensor input and for controlling the LED. Then I will open Fritzing and build the full circuit step by step. I will pay attention to labelling wires, naming pins and keeping the layout readable instead of just dropping components randomly. If I notice that Fritzing becomes unclear, I will briefly check if a simple block diagram or hand-drawn overview helps me to see the logic of the design better, even if the main schematic stays in Fritzing.

R — Result
The expected result is a complete Fritzing schematic of my smart streetlight that clearly shows how every component is connected to the ESP32-S3 and how the system is supposed to work electrically. The design should be consistent with the problem-analysis, use realistic components and use pins that are actually usable on the ESP32-S3. This schematic will be stored in my project repository so it can be reused and reviewed.

R — Reflection
After finishing the design I will reflect on how easily this schematic can be translated into a real build. I will look at which parts of the design I feel confident wiring in practice and which parts still raise questions, for example about power, grounding or pin choice. This should make clear whether my design is really ready for prototyping or if I need to adjust it before moving on.

T — Transfer
In the next phase I will use this schematic as the direct blueprint for the realisation of my smart streetlight prototype. During wiring and testing I will check where the design holds up and where it causes problems, so I can update the schematic accordingly. What I have learned about making a design that is actually buildable will be reused in future designs for the traffic light and pedestrian crossing, so that each next design is better aligned with the realisation phase from the start.

Appendix (optional)

Detailed logs
Additional figures/tables
Full code/artefacts

References

# Feedback from Mats 
Mats feedback is that the design is not only meant as a final product, but also as a helpful tool for the rest of the team. When the design is properly worked out, I can hand it over to the other team members so they clearly understand how everything should work together, which components need to be used, and how everything should be connected. This way, team members can already start building certain parts, such as the lamppost. In addition, Mats advises setting a clear moment to ask for feedback. According to him, it would be better to ask Gerald for this, since Gerald is responsible for embedded systems. Apart from that, his feedback is largely the same as the feedback given during the analysis.