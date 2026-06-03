# Professionalization of a Breadboard Prototype into a Perfboard or PCB Solution

* **Author:** Thijmen Walter (Embedded & Robotics Engineer Student)
* **Date:** 31-05-2026
* **Version:** 2.0
* **Classification:** Internal
* **Client:** Mayor Mats Otten
* **Company:** The Embedded Alliance

---

## Table of Contents

* [1. Introduction](#1-introduction)  
* [2. Main research question and subquestions](#2-main-research-question-and-subquestions)  
* [3. Methodology](#3-methodology)  
* [4. Chapter 1: Breadboard limitations](#4-chapter-1-breadboard-limitations)  
* [5. Chapter 2: Perfboard implementation](#5-chapter-2-perfboard-implementation)  
* [6. Chapter 3: PCB design analysis](#6-chapter-3-pcb-design-analysis)  
* [7. Comparison of solutions](#7-comparison-of-solutions)  
* [8. Final conclusion](#8-final-conclusion)  
* [9. Recommendations](#9-recommendations)  
* [10. References](#10-references)  

---

# 1. Introduction

This document was written for The Embedded Alliance, Mayor Mats Otten, and other stakeholders involved in the Smart City project. The document is intended for readers with low to intermediate prior knowledge of embedded systems, electronics, and hardware prototyping.

The context of this document is a Smart City embedded hardware project consisting of a train railroad crossing tile with warning lights and automatic barrier control when a train is detected. In addition, the project contains a separate hardware module with an e-ink display used for information visualization.

Embedded hardware projects are often developed in multiple stages. During the early stages, developers commonly use a breadboard, which is a reusable prototyping platform that allows electronic components to be connected without soldering [(Adafruit, 2024)](https://learn.adafruit.com/breadboards-for-beginners/perma-protos). When a design becomes more stable, it can be transferred to a perfboard, a board with pre-drilled holes that allows components to be permanently soldered into place. For fully professional products, a Printed Circuit Board (PCB) is often used. A PCB contains custom-designed copper traces that replace manual wiring and provide a compact, reliable, and scalable hardware solution [(TechTarget, 2024)](https://www.techtarget.com/whatis/definition/printed-circuit-board-PCB). Understanding the differences between these implementation methods is important when determining how a prototype can be transformed into a more professional system.

During development, the hardware was implemented using breadboards for rapid prototyping and testing. While this allowed quick experimentation, the setup became less suitable for reliable demonstrations, transport, and long-term usage.

The purpose of this document is to investigate how the current breadboard implementation can be transformed into a more reliable and professional solution using either a perfboard or a custom PCB design. This research is relevant because professional hardware implementations require stability, maintainability, and improved presentation quality [(SparkFun Electronics, 2025)](https://learn.sparkfun.com/tutorials/pcb-basics/all).

This document compares the advantages and disadvantages of breadboards, perfboards, and PCBs in order to determine which solution is most suitable for the current project situation.

---

# 2. Main research question and subquestions

The main research question of this document is:

**How can the current breadboard setup be professionalized into a more reliable and maintainable hardware solution?**

To answer this question, the following subquestions were formulated:

1. What are the limitations of the current breadboard setup?
2. What are the advantages and disadvantages of using a perfboard?
3. What are the advantages and disadvantages of designing a PCB?
4. Which solution is the most suitable for the current project requirements?

---

# 3. Methodology

This document was created using the following research methods:

* Literature research about breadboards, perfboards, soldering, and PCB development
* Analysis of the current hardware prototype
* Comparison of hardware implementation methods
* Evaluation of reliability, maintainability, and scalability
* Feedback from teammates and lecturers

These methods were selected because they provide both theoretical knowledge and practical insights into improving the current hardware implementation.

---

# 4. Chapter 1: Breadboard limitations

## 4.1 Introduction

Breadboards are widely used during the prototyping phase of electronics projects because they allow circuits to be assembled and modified without soldering [(Adafruit, 2024)](https://learn.adafruit.com/breadboards-for-beginners/perma-protos). Although they provide flexibility during development, breadboards are generally not intended for permanent implementations. This chapter examines the limitations of the current breadboard setup and evaluates how these limitations affect the Smart City railroad crossing prototype and e-ink display module.

This chapter answers the following subquestion:

**What are the limitations of the current breadboard setup?**

The chapter focuses on reliability, maintainability, scalability, and presentation quality.

---

## 4.2 Reliability issues

Breadboards are designed for rapid prototyping and experimentation. Components can easily be inserted, removed, and modified without soldering. This makes breadboards highly suitable during the early development phase.

However, breadboards also introduce reliability problems. Jumper wires can disconnect accidentally, connections may become unstable, and components can shift during transport or movement. These problems increase as the circuit becomes larger and more complex (Monk, 2016).

For the current railroad crossing prototype, unstable breadboard connections may cause incorrect behaviour of warning lights or barrier control mechanisms. Movement of the prototype during transport or demonstrations can accidentally disconnect jumper wires.

The e-ink display module also requires stable wiring connections because interrupted communication or power delivery can result in incomplete or failed display updates.

Because of these risks, the current breadboard implementation is not ideal for long-term or professional usage [(SparkFun Electronics, 2025)](https://learn.sparkfun.com/tutorials/pcb-basics/all).

---

## 4.3 Maintainability and presentation

Another limitation of breadboards is reduced maintainability. Large numbers of jumper wires make the circuit difficult to read and troubleshoot. When changes are required, developers may accidentally disconnect unrelated connections.

In the current implementation, the railroad crossing tile contains multiple hardware components such as LEDs, train detection logic, and barrier control connections. As the number of connections increases, the breadboard setup becomes more difficult to organize and maintain.

In addition, breadboard prototypes often appear unfinished or temporary. This negatively affects presentations to stakeholders, clients, and lecturers because the implementation does not visually represent a finalized product [(Adafruit, 2024)](https://learn.adafruit.com/breadboards-for-beginners/perma-protos).

A more permanent implementation would improve organization, readability, and professional appearance.

---

## 4.4 Subconclusion

Based on this chapter, it can be concluded that breadboards are useful for rapid prototyping but unsuitable for professional long-term implementations due to limitations in reliability, maintainability, and presentation quality.

---

# 5. Chapter 2: Perfboard implementation

## 5.1 Introduction

A perfboard is a prototyping board containing a grid of holes that allows electronic components to be soldered permanently [MKTPCB (2023)](https://www.mktpcb.com/perfboard/). Unlike breadboards, perfboards create fixed electrical connections, resulting in improved reliability and durability. This chapter evaluates whether a perfboard could serve as an effective intermediate solution for professionalizing the current prototype.

This chapter answers the following subquestion:

**What are the advantages and disadvantages of using a perfboard?**

This chapter discusses implementation complexity, durability, maintainability, and practical use cases.

---

## 5.2 Advantages of perfboards

Perfboards provide a more permanent implementation compared to breadboards because components are soldered directly onto the board. This significantly reduces the risk of loose connections and improves overall circuit stability [(Adafruit, 2024)](https://learn.adafruit.com/breadboards-for-beginners/perma-protos).

According to [MKTPCB (2023)](https://www.mktpcb.com/perfboard/), perfboards are widely used in the electronics industry as a cost-effective alternative to printed circuit boards for rapid prototyping and flexible circuit construction. Because components are soldered directly onto the board, perfboards provide stronger and more durable connections compared to solderless breadboards. This makes them suitable for small embedded projects and prototype systems that require improved stability.

Another advantage is affordability. Perfboards are inexpensive and widely available, making them suitable for student projects and small prototypes. They also allow developers to create a cleaner and more compact layout.

For the current project, a perfboard implementation would make the railroad crossing tile more durable by permanently soldering the warning lights, sensors, and barrier control connections. This would reduce instability during demonstrations and improve cable organization.

The e-ink display module could also benefit from a compact soldered implementation because the display requires reliable communication between the microcontroller and display driver.

---

## 5.3 Disadvantages of perfboards

Despite these advantages, perfboards also have limitations. Connections on the backside of the board must often be created manually using solder bridges or wires. This process can become messy and difficult to maintain when the circuit grows larger.

Additionally, modifying a soldered circuit is more difficult than modifying a breadboard. Incorrect soldering may also damage components or require partial reconstruction of the implementation.

[MKTPCB (2023)](https://www.mktpcb.com/perfboard/) also explains that perfboards require manual soldering and routing of electrical connections. As circuits become larger or more complex, troubleshooting and modifications may become more difficult compared to breadboard implementations.

Compared to PCBs, perfboards are less optimized in terms of routing efficiency and scalability [(SparkFun Electronics, 2025)](https://learn.sparkfun.com/tutorials/pcb-basics/all).

For future expansions of the railroad crossing tile or e-ink display system, perfboards may eventually become too limited for efficient hardware organization.

---

## 5.4 Subconclusion

Based on this chapter, it can be concluded that perfboards provide a reliable and affordable intermediate solution between breadboards and fully custom PCBs. They significantly improve stability while remaining relatively accessible for small-scale projects.

---

# 6. Chapter 3: PCB design analysis

## 6.1 Introduction

A Printed Circuit Board (PCB) is a custom-designed board that contains conductive traces, pads, and mounting points for electronic components [(TechTarget, 2024)](https://www.techtarget.com/whatis/definition/printed-circuit-board-PCB). PCBs are commonly used in commercial products because they provide high reliability, compactness, and scalability. This chapter investigates whether a PCB solution would be suitable for the current Smart City project.

This chapter answers the following subquestion:

**What are the advantages and disadvantages of designing a PCB?**

The chapter focuses on design quality, scalability, manufacturing, and professional implementation standards.

---

## 6.2 Advantages of PCBs

PCBs provide the most professional implementation method for embedded hardware projects. Components and electrical traces are designed digitally, resulting in a clean and optimized layout (KiCad, 2025).

Professional PCB manufacturers such as [MKTPCB (2023)](https://www.mktpcb.com/) also provide rapid prototyping and PCB assembly services, making PCB production increasingly accessible for student and prototype projects.

This improves:

* Reliability
* Readability
* Compactness
* Reproducibility
* Scalability

A PCB also reduces wiring complexity and minimizes the chance of incorrect manual connections.

For the current project, a custom PCB could combine the train detection system, warning lights, barrier control circuitry, and microcontroller into a single compact board. This would reduce wiring complexity and improve the professional appearance of the prototype.

A separate PCB for the e-ink display module could also simplify integration and make the display easier to mount inside a final enclosure.

---

## 6.3 Disadvantages of PCBs

The largest disadvantage of PCB development is the increased complexity. PCB design requires knowledge of electronic schematics, routing, component footprints, and manufacturing requirements.

Additionally, manufacturing a PCB introduces additional costs and waiting time. Design mistakes may require a complete redesign and reordering process [(Adafruit, 2024)](https://learn.adafruit.com/breadboards-for-beginners/perma-protos).

PCB manufacturing also requires preparation of design files such as schematics, Gerber files, and component lists before production can begin (MKTPCB, 2023).

For rapidly changing prototypes, PCB development may therefore be less practical during early project stages.

For the current project, this means that a PCB may be more suitable after the hardware functionality has been finalized and tested thoroughly.

---

## 6.4 Subconclusion

Based on this chapter, it can be concluded that PCBs provide the highest level of professionalism, reliability, and scalability. However, they also require more development time, technical knowledge, and financial investment.

---

# 7. Comparison of solutions

The following table compares the three hardware implementation methods discussed in this document. The comparison focuses on the criteria that are most relevant to the Smart City project, including reliability, ease of modification, appearance, cost, scalability, and required technical experience.

**Table 1: Comparison of Breadboard, Perfboard, and PCB Solutions**

| Property                | Breadboard | Perfboard | PCB         |
| ----------------------- | ---------- | --------- | ----------- |
| Reliability             | Low        | Medium    | High        |
| Ease of modification    | High       | Medium    | Low         |
| Professional appearance | Low        | Medium    | High        |
| Cost                    | Low        | Low       | Medium/High |
| Scalability             | Low        | Medium    | High        |
| Required experience     | Low        | Medium    | High        |

Table 1 shows that breadboards provide the highest flexibility during development, while PCBs provide the highest reliability and professional quality. Perfboards offer a compromise between the two approaches by improving stability while remaining relatively inexpensive and easy to implement.

---

# 8. Final conclusion

This document examined how the current breadboard setup can be professionalized into a more reliable and maintainable hardware solution.

The analysis showed that breadboards are highly useful during development but unsuitable for professional long-term implementations due to instability and poor maintainability.

Perfboards provide a stronger and more stable solution while remaining affordable and relatively easy to implement.

PCBs provide the most professional and scalable implementation method. However, they also require additional design knowledge, manufacturing time, and financial investment.

Based on the full analysis, it can be concluded that transitioning the railroad crossing tile and e-ink display modules from breadboards to a perfboard or PCB solution would significantly improve reliability, maintainability, and presentation quality.

For the current project scope, a perfboard implementation is the most realistic short-term solution, while PCB development is the most suitable long-term solution.

---

# 9. Recommendations

Based on the results of this document, the following recommendations are made:

1. Replace the current breadboard setup with a perfboard implementation to improve connection stability and reduce the risk of loose wiring.

2. Use the perfboard solution as an intermediate step before developing a fully custom PCB, because the project is still in an active development phase with possible hardware changes.

3. Create a digital PCB design using software such as [KiCad](https://www.kicad.org) to explore how the project could be further professionalized in the future.

4. Document the hardware implementation process with photos, circuit diagrams, and explanations to improve maintainability and project transparency.

5. Continue evaluating the hardware setup during testing to determine whether a future transition from perfboard to PCB is justified based on reliability, scalability, and project requirements.

---

# 10. References

1. Monk, S. (2016). *Programming Arduino: Getting Started with Sketches*. McGraw-Hill Education.
2. KiCad. (2025). *KiCad Documentation*. Retrieved May 20, 2026, from [https://docs.kicad.org/](https://docs.kicad.org/)
3. SparkFun Electronics. (2025). *PCB Basics*. Retrieved May 20, 2026, from [https://learn.sparkfun.com/tutorials/pcb-basics/all](https://learn.sparkfun.com/tutorials/pcb-basics/all)
4. Adafruit. (2024). *Perma-Proto Guide*. Retrieved May 20, 2026, from [https://learn.adafruit.com/breadboards-for-beginners/perma-protos](https://learn.adafruit.com/breadboards-for-beginners/perma-protos)
5. Autodesk. (2024). *PCB Design Resources*. Retrieved May 20, 2026, from [https://www.autodesk.com/solutions/pcb-design-software](https://www.autodesk.com/solutions/pcb-design-software)
6. MKTPCB. (2023). *Perfboard | A Quick Guide | Types, Uses, Techniques, and More*. Retrieved May 20, 2026, from [https://www.mktpcb.com/perfboard/](https://www.mktpcb.com/perfboard/)
7. TechTarget. (2024). *What is a Printed Circuit Board (PCB)?*. Retrieved May 31, 2026, from [https://www.techtarget.com/whatis/definition/printed-circuit-board-PCB](https://www.techtarget.com/whatis/definition/printed-circuit-board-PCB)

---

