# Sprint 1 — Learning Outcome (Design)

- [Learning Question](#learning-question)
- [S — Situation](#s--situation)
- [T — Task](#t--task)
- [A — Action](#a--action)
- [R — Result](#r--result)
- [R — Reflection](#r--reflection)
- [T — Transfer](#t--transfer)
- [Appendix A — Linked design proof and supporting files](#appendix-a--linked-design-proof-and-supporting-files)
  - [A.1 Analysis basis](#a1-analysis-basis)
  - [A.2 Fritzing design documentation](#a2-fritzing-design-documentation)
  - [A.3 Wokwi design validation](#a3-wokwi-design-validation)


## Learning Question

How can I translate the analysed requirements and ESP32-S3 hardware constraints into a clear, safe, and buildable first-iteration traffic light design, and document that design with enough proof before moving to realization. 

## S — Situation

After switching from the streetlight idea to a traffic light project, I first completed the analysis of the first iteration. In that analysis, I defined the first version as a basic traffic light with three LED outputs, a fixed red → green → yellow → red sequence, fixed timing, safe startup in red, and no conflicting outputs. I also analysed the ESP32-S3 hardware basis and the Wokwi workflow so that the design step could be based on clear requirements and constraints instead of trial and error. 

## T — Task

For the design part of this sprint, my task was to turn that analysis into a concrete and well-documented prototype design. In practice, this meant creating a buildable hardware design for the ESP32-S3 traffic light and documenting the key design decisions clearly enough that I could later realize and test the system. The design proof needed to show the wiring, the chosen GPIO pins, the traffic-light states, the timing, and the reasoning behind those choices. 

## A — Action

I started from the analysed first-iteration requirements and used them as design constraints. I kept the scope intentionally simple: three LEDs, fixed cycle, fixed timing, startup in red, and no conflicting outputs. I then chose GPIO 4, 5, and 6 as the three output pins for red, yellow, and green, because this first version only required simple digital outputs and I wanted to keep the design on ordinary GPIO pins. After that, I documented the circuit in Fritzing with both a schematic view and a breadboard view, each showing the ESP32-S3, the three LEDs, the three 220Ω resistors, the GPIO connections, and the shared ground return. 

To make the design more explicit, I also added a pin selection table with reason and risk/check, a state table for the traffic-light sequence, a timing table, and a short design rationale. In that documentation, I explained why GPIO 4, 5, and 6 were chosen, why the startup state is red, why the order is red → green → yellow → red, how conflicting outputs are prevented in the design, and why this design is suitable for the first iteration only. 

Finally, I used Wokwi as digital design validation. I built the same first-iteration circuit in Wokwi, compiled the firmware, linked it through `wokwi.toml`, and checked whether the simulated behavior matched the intended design. That let me confirm that the chosen pin mapping, the circuit structure, and the designed state sequence all worked together before physical realization. 

## R — Result

The result of this design step is a documented first-iteration traffic light design that I can use as the basis for realization. My design proof now consists of a Fritzing schematic, a Fritzing breadboard layout, a GPIO pin selection table, a state table, a timing table, a design decisions section, and Wokwi design validation. Together, these show that the design is not only drawable, but also understandable, buildable, and traceable back to the analysed requirements.  

## R — Reflection

This design step taught me that good embedded design is more than only drawing a circuit. By documenting the pin choices, the state logic, the timing, and the design rationale, I made the first traffic-light iteration much easier to understand and review. I also learned that Fritzing and Wokwi serve different design purposes: Fritzing helped me communicate the hardware structure clearly, while Wokwi helped me validate that the designed behavior matched the requirements before building the real circuit. That made the design phase more systematic and gave me stronger proof than only showing code or a single wiring image would have done.  

## T — Transfer

I will reuse this same design approach in later iterations of the project. For more advanced versions, such as adding multiple traffic lights, pedestrian inputs, or sensor-based behavior, I can use the same structure again: start from analysed requirements, choose and justify the hardware connections, document the wiring, model the states and timing, explain the design decisions, and validate the design digitally before realizing it physically. 

---

## Appendix A — Linked design proof and supporting files

### A.1 Analysis basis

This file contains the analysed scope, requirements, acceptance criteria, and hardware constraints that I used as the basis for the design step. 
[Learning Outcome - Analysis.pdf](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3bdf1039bb10a02b979b892f43c5b37dd055f290/docs/Wesley/learning%20outcomes/sprint%201/Learning%20Outcome%20-%20Analysis.md)

### A.2 Fritzing design documentation

This file contains the hardware design documentation for the first iteration, including the schematic view, breadboard view, pin selection table, state table, timing table, design rationale, and requirement-to-proof traceability table. 
[Traffic Light - first iterion - fritzing.pdf](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3bdf1039bb10a02b979b892f43c5b37dd055f290/docs/Wesley/research/Traffic%20Light%20-%20first%20iterion%20-%20fritzing.md)

### A.3 Wokwi design validation

This file contains the Wokwi-based validation of the same first-iteration design, including the simulated circuit, build workflow, configuration files, source code proof, and requirement-to-proof traceability. 
[Traffic Light - first iteration - wokwi.pdf](https://gitlab.fdmci.hva.nl/studio/smart-cities/projecten/2025-2026-semester-2/city-sim-learning-group/city-the-embedded-alliance-city-sim-learning-group/-/blob/3bdf1039bb10a02b979b892f43c5b37dd055f290/docs/Wesley/research/Traffic%20Light%20-%20first%20iteration%20-%20wokwi.md)

