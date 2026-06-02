# Advice: Professionalization of a Breadboard Prototype into a Perfboard or PCB Solution

**Author:** Thijmen Walter (Embedded & Robotics Engineer Student)  
**Date:** 02-06-2026  
**Version:** 1.0  
**Classification:** Internal  
**Client:** Mayor Mats Otten  
**Company:** The Embedded Alliance  

---

## Table of Contents

* [1. Introduction](#1-introduction)
* [2. Main advice question and subquestions](#2-main-advice-question-and-subquestions)
* [3. Methodology](#3-methodology)
* [4. Chapter 1: Comparison of implementation options](#4-chapter-1-comparison-of-implementation-options)
    * [4.1 Introduction](#41-introduction)
    * [4.2 Central decision problem](#42-central-decision-problem)
    * [4.3 Implementation options](#43-implementation-options)
    * [4.4 Comparison of implementation options](#44-comparison-of-implementation-options)
    * [4.5 Selection of the best solution](#45-selection-of-the-best-solution)
    * [4.6 Second choice](#46-second-choice)
    * [4.7 Subconclusion](#47-subconclusion)
* [5. Chapter 2: Advice for a professional hardware setup](#5-chapter-2-advice-for-a-professional-hardware-setup)
    * [5.1 Introduction](#51-introduction)
    * [5.2 Recommended implementation approach](#52-recommended-implementation-approach)
    * [5.3 Why the perfboard is advised](#53-why-the-perfboard-is-advised)
    * [5.4 Maintaining project flexibility](#54-maintaining-project-flexibility)
    * [5.5 Subconclusion](#55-subconclusion)
* [6. Chapter 3: Advice, risks and implementation direction](#6-chapter-3-advice-risks-and-implementation-direction)
    * [6.1 Introduction](#61-introduction)
    * [6.2 Advice based on project requirements](#62-advice-based-on-project-requirements)
    * [6.3 Risks and recommendations](#63-risks-and-recommendations)
    * [6.4 Subconclusion](#64-subconclusion)
* [7. Final conclusion](#7-final-conclusion)
* [8. Recommendations](#8-recommendations)
* [9. Previous work](#9-previous-work)
* [10. References](#10-references)

---

## 1. Introduction

Embedded systems are commonly developed in multiple stages. During the early stages of development, breadboards are often used because they allow developers to quickly test circuits without soldering. According to Adafruit (2024), breadboards provide flexibility during prototyping and make it easy to modify circuits while requirements are still changing.

As a project matures, however, the limitations of breadboards become increasingly visible. Loose jumper wires, unstable connections, and complex wiring layouts can negatively affect reliability, maintainability, and presentation quality. For this reason, many embedded projects eventually transition to more permanent implementation methods such as perfboards or Printed Circuit Boards (PCBs).

This advice document was written for Mayor Mats Otten and other stakeholders involved in the Smart City project. The project currently contains a railroad crossing tile with train detection, warning lights, and barrier control functionality. In addition, the project contains a separate e-ink display module that is used to visualize information.

The current hardware implementation uses breadboards for both modules. While this approach was appropriate during the development phase, the system is approaching a stage where reliability, presentation quality, and maintainability have become more important.

This advice document builds upon the findings of the analysis document *Professionalization of a Breadboard Prototype into a Perfboard or PCB Solution*. The analysis examined the limitations of breadboards and compared perfboards and PCBs as potential alternatives.

The analysis concluded that both perfboards and PCBs offer significant improvements over breadboards. However, differences exist in implementation complexity, costs, scalability, and suitability for the current project phase.

Based on these findings, this advice document determines which implementation method should be recommended to professionalize the current prototype while balancing reliability, maintainability, cost, and project maturity.

---

## 2. Main advice question and subquestions

The main advice question of this document is:

**Which hardware implementation method should be advised to professionalize the current breadboard prototype while balancing reliability, maintainability, cost, and project maturity?**

To answer this question, the following subquestions are used:

1. Which implementation options are most relevant based on the analysis?
2. How do breadboards, perfboards, and PCBs compare based on practical decision criteria?
3. Which implementation method best fits the current Smart City project?
4. Which implementation method should be considered as the next step after implementation?
5. What risks and considerations must be taken into account during the transition?

These subquestions provide a structured basis for determining the most suitable implementation strategy.

---

## 3. Methodology

This advice document is based on the findings from the analysis document *Professionalization of a Breadboard Prototype into a Perfboard or PCB Solution*.

The analysis examined three implementation methods:

* Breadboards
* Perfboards
* Printed Circuit Boards (PCBs)

The options are evaluated using practical decision criteria that are relevant to the current project situation.

The following criteria are used:

* reliability;
* maintainability;
* ease of modification;
* professional appearance;
* implementation complexity;
* cost;
* scalability;
* suitability for the current project phase.

These criteria were selected because previous literature identifies reliability, maintainability, and scalability as important factors when transitioning from prototype hardware to professional implementations (SparkFun Electronics, 2025; TechTarget, 2024).

The comparison combines literature research, practical observations from the Smart City project, and feedback from project stakeholders.

---

## 4. Chapter 1: Comparison of implementation options

### 4.1 Introduction

This chapter answers the following subquestion:

**How do the relevant implementation options compare based on practical decision criteria?**

The analysis document identified three implementation methods that are relevant to the Smart City project. This chapter compares these options and determines which solution best supports the project's current needs.

---

### 4.2 Central decision problem

The current breadboard implementation successfully supports hardware development and testing. Breadboards are specifically designed for rapid prototyping because components can be inserted and removed without soldering, making circuit modifications straightforward during development (Adafruit, 2024). However, this flexibility comes at the cost of long-term reliability. At the same time, the project is increasingly being used for demonstrations, evaluations, and stakeholder presentations.

As a result, reliability and presentation quality have become more important. SparkFun Electronics (2025) explains that professional hardware implementations should prioritize stable electrical connections and organized circuit layouts, particularly when systems are used outside laboratory environments. At the same time, the project is still evolving, which means future modifications remain possible.

The selected implementation method must therefore:

* improve reliability;
* improve maintainability;
* improve professional appearance;
* remain practical to implement;
* support future development.

These requirements form the basis for the comparison.

---

### 4.3 Implementation options

Based on the analysis, three implementation options are relevant.

The first option is the **breadboard**. Adafruit (2024) explains that breadboards allow components to be connected without soldering, making them highly suitable during the experimentation phase of a project.

The second option is the perfboard. According to MKTPCB (2023), perfboards are widely used as an intermediate prototyping solution because they allow components to be soldered permanently while maintaining flexibility in circuit construction. Compared to breadboards, this improves connection stability and durability. Perfboards allow components to be permanently soldered onto a board while still offering flexibility in circuit construction. This makes perfboards a common intermediate step between prototyping and fully custom hardware.

The third option is the **Printed Circuit Board (PCB)**. TechTarget (2024) describes PCBs as custom-designed circuit boards containing conductive traces that replace manual wiring. This approach improves reliability, reproducibility, and manufacturing consistency.

---

### 4.4 Comparison of implementation options

**Table 1: Comparison of Breadboard, Perfboard, and PCB Solutions.**

Table 1 compares the three implementation methods using the criteria identified in the [3 Methodology](#3-methodology). The purpose of the comparison is to determine which implementation method provides the best balance between reliability, maintainability, cost, scalability, and suitability for the current project phase. The ratings are based on findings from the literature and observations from the Smart City project.

| Property                              | Breadboard | Perfboard      | PCB            |
| ------------------------------------- | ---------- | -------------- | -------------- |
| Reliability                           | Low        | Medium to High | High           |
| Ease of modification                  | High       | Medium         | Low            |
| Professional appearance               | Low        | Medium to High | High           |
| Cost                                  | Low        | Low            | Medium to High |
| Scalability                           | Low        | Medium         | High           |
| Maintainability                       | Low        | Medium         | High           |
| Required experience                   | Low        | Medium         | High           |
| Suitability for current project phase | Medium     | High           | Medium         |

Source: Based on Adafruit (2024), MKTPCB (2023), SparkFun Electronics (2025), and TechTarget (2024).

Table 1 shows that breadboards provide the highest flexibility but perform poorly in terms of reliability and professional presentation. PCBs provide the strongest long-term solution but require additional design effort, manufacturing time, and technical expertise.

The comparison criteria are supported by findings from the literature. Adafruit (2024) and MKTPCB (2023) identify improved connection stability as one of the primary benefits of soldered implementations, while SparkFun Electronics (2025) highlights the scalability and maintainability advantages of PCB-based designs.

The perfboard provides the most balanced solution because it significantly improves reliability and presentation quality while remaining affordable and relatively easy to implement.

---

### 4.5 Selection of the best solution

The recommended solution is the perfboard implementation.

The perfboard offers the best balance between reliability, maintainability, cost, and implementation effort. According to Adafruit (2024), soldered prototyping boards provide significantly stronger and more permanent connections than traditional breadboards. MKTPCB (2023) also describes perfboards as a practical and cost-effective intermediate step between temporary prototyping and fully custom PCB development. These characteristics make the perfboard particularly suitable for the current Smart City project phase.

Compared to the current breadboard setup, a perfboard significantly reduces the risk of loose connections because components are permanently soldered. This improves durability during transport and demonstrations.

The perfboard also provides a more organized hardware layout. This improves troubleshooting and creates a more professional appearance for stakeholders and project evaluators.

Unlike a PCB, a perfboard can be implemented without requiring a complete redesign of the hardware. This reduces implementation effort compared to PCB development, which typically requires schematic creation, routing, manufacturing preparation, and production (TechTarget, 2024).

For the current project phase, the perfboard therefore provides the strongest overall balance between effort and benefits.

---

### 4.6 Second choice

The second-best solution is the **Printed Circuit Board (PCB)**.

A PCB provides the highest level of reliability, maintainability, and professional quality. According to SparkFun Electronics (2025), PCB layouts improve circuit organization, reproducibility, and long-term reliability compared to manually wired implementations.

The custom layout reduces wiring complexity because electrical connections are integrated directly into the board design through copper traces rather than external jumper wires (TechTarget, 2024).

However, PCB development requires:

* schematic creation;
* PCB routing;
* footprint management;
* manufacturing preparation;
* production lead times.

PCB development typically requires schematic capture and board routing using dedicated electronic design software such as Autodesk Eagle or KiCad (Autodesk, 2024; KiCad, 2025).

Because the current project is still subject to change, these additional requirements increase project risk.

For this reason, the PCB is not advised as the immediate next step but rather as the preferred long-term solution after the hardware design has been validated.

---

### 4.7 Subconclusion

Based on the comparison, the perfboard is the most suitable implementation method for the current Smart City project.

It significantly improves reliability, maintainability, and presentation quality while remaining affordable and practical to implement.

The PCB remains the strongest long-term solution once the hardware design becomes stable. SparkFun Electronics (2025) states that PCB-based designs provide improved scalability and reproducibility once a circuit design has been validated.

---

## 5. Chapter 2: Advice for a professional hardware setup

### 5.1 Introduction

This chapter answers the following subquestion:

**How should the advised solution be implemented within the current Smart City project?**

---

### 5.2 Recommended implementation approach

The recommended implementation consists of:

* maintaining the existing hardware functionality;
* transferring circuits from breadboards to perfboards;
* soldering permanent electrical connections;
* organizing wiring systematically;
* documenting all hardware connections;
* validating functionality after migration;
* preparing for future PCB development.

This approach minimizes project risk while significantly improving hardware quality.

---

### 5.3 Why the perfboard is advised

The perfboard is advised because it directly addresses the limitations identified in the analysis. Adafruit (2024) explains that soldered prototyping boards provide more durable and reliable electrical connections than breadboards. Furthermore, MKTPCB (2023) identifies perfboards as a cost-effective solution for projects that require greater stability without the additional complexity of PCB manufacturing.

The soldered connections improve reliability by eliminating accidental wire disconnections, which are common risks associated with breadboard implementations (Adafruit, 2024).

The more compact layout improves maintainability because hardware connections become easier to trace and understand. According to SparkFun Electronics (2025), organized hardware layouts simplify troubleshooting and future modifications.

The improved appearance also benefits stakeholder demonstrations and project presentations because soldered implementations generally appear more organized and permanent than breadboard prototypes (MKTPCB, 2023).

Additionally, perfboards require relatively little financial investment compared to PCB manufacturing while still providing substantial improvements in stability and durability (MKTPCB, 2023).

These advantages make the perfboard the most practical solution for the current stage of development.

---

### 5.4 Maintaining project flexibility

One important advantage of the perfboard solution is that it preserves flexibility.

Although modifications become more difficult than on a breadboard, they remain significantly easier than redesigning and manufacturing a new PCB, which may require updated design files and new manufacturing runs (TechTarget, 2024).

This allows the project team to continue improving the railroad crossing tile and e-ink display module while benefiting from improved hardware stability.

The perfboard implementation can therefore serve as an intermediate validation stage before committing to a final PCB design.

---

### 5.5 Subconclusion

The recommended implementation strategy is to migrate the current breadboard circuits to perfboards while maintaining the existing functionality.

This approach improves reliability and professionalism while preserving sufficient flexibility for future development.

---

## 6. Chapter 3: Advice, risks and implementation direction

### 6.1 Introduction

This chapter answers the following subquestion:

**What risks must be considered during the transition to a more professional hardware implementation?**

---

### 6.2 Advice based on project requirements

The recommended solution should meet the following functional requirements.:

* maintain existing system functionality;
* support reliable operation;
* support demonstrations and presentations;
* improve hardware organization.

The recommended solution should also meet the following non-functional requirements:

* affordable implementation;
* maintainability;
* scalability;
* professional appearance;
* reasonable implementation effort.

The perfboard solution satisfies these requirements most effectively because it combines the reliability advantages of soldered connections with lower implementation complexity than PCB manufacturing (MKTPCB, 2023; TechTarget, 2024).

---

### 6.3 Risks and recommendations

#### Risk 1: Soldering errors

Incorrect solder joints may introduce hardware faults.

Adafruit (2024) notes that soldered circuits require careful assembly because mistakes may require desoldering or component replacement.

**Recommendation:**
Perform continuity testing after each soldering stage.

---

#### Risk 2: Future hardware modifications

Changes become more difficult after soldering.

MKTPCB (2023) explains that modifications become increasingly difficult once permanent solder connections have been created.

**Recommendation:**
Verify circuit designs before transferring them to the perfboard.

---

#### Risk 3: Insufficient documentation

Future maintenance may become difficult without proper documentation.

**Recommendation:**
Update schematics, wiring diagrams, and photographs throughout implementation.

---

#### Risk 4: Transitioning to a PCB too early

PCB manufacturing may lock the design before it has been fully validated.

According to SparkFun Electronics (2025), PCB development is most effective when hardware requirements have been validated and are unlikely to change significantly.

**Recommendation:**
Use the perfboard implementation to collect validation data before creating a PCB.

---

#### Risk 5: Increased implementation time

Migrating hardware requires additional assembly and testing effort.

**Recommendation:**
Plan the migration in phases and test each subsystem separately.

---

### 6.4 Subconclusion

The risks associated with the perfboard implementation are manageable and can be reduced through proper planning, testing, and documentation.

The benefits of improved reliability and presentation quality outweigh the implementation effort.

---

## 7. Final conclusion

This advice document examined which hardware implementation method should be advised to professionalize the current Smart City prototype.

The comparison showed that breadboards remain useful for development because they support rapid prototyping and experimentation (Adafruit, 2024). However, literature indicates that breadboard implementations are less suitable for long-term use because their temporary connections are more susceptible to accidental disconnections and maintenance issues (SparkFun Electronics, 2025).

The analysis also showed that PCBs provide the highest level of professionalism, scalability, and reliability. According to TechTarget (2024) and SparkFun Electronics (2025), custom PCB designs improve circuit organization, reproducibility, and long-term maintainability.

The perfboard provides the strongest balance between implementation effort and hardware quality. It significantly improves reliability, maintainability, and presentation quality while remaining affordable and suitable for a project that is still evolving.

Based on the findings presented throughout this advice document, the final recommendation is:

**Replace the current breadboard implementation with a perfboard solution and use this implementation as the foundation for future PCB development.**

This approach provides immediate improvements while supporting long-term professionalization of the Smart City project.

---

## 8. Recommendations

Based on this advice document, the following recommendations are made:

1. Replace the current breadboard implementation with a perfboard solution.

2. Permanently solder all stable circuit connections.

3. Organize hardware layouts clearly to improve maintainability.

4. Test all hardware functionality after migration.

5. Document all hardware changes with diagrams and photographs.

6. Use the perfboard implementation during stakeholder demonstrations.

7. Create a digital PCB design based on the validated perfboard implementation.

8. Delay PCB manufacturing until the design is fully validated.

9. Continue evaluating reliability and maintainability after migration.

10. Treat the perfboard as an intermediate professionalization step toward a future PCB implementation.

11. Preserve modularity between the railroad crossing tile and e-ink display module.

12. Use testing results from the perfboard implementation to guide future PCB development decisions.

---

## 9. Previous work

This advice document is based on the following analysis document:

**Professionalization of a Breadboard Prototype into a Perfboard or PCB Solution**

The analysis document investigated breadboards, perfboards, and PCBs and compared their suitability for improving the Smart City hardware implementation.

The findings of that analysis form the foundation of this advice.

---

## 10. References

1. Adafruit. (2024). *Perma-Proto Guide*. Retrieved May 20, 2026, from [https://learn.adafruit.com/breadboards-for-beginners/perma-protos](https://learn.adafruit.com/breadboards-for-beginners/perma-protos)


2. Autodesk. (2024). *PCB Design Resources*. Retrieved May 20, 2026, from [https://www.autodesk.com/solutions/pcb-design-software](https://www.autodesk.com/solutions/pcb-design-software)

3. KiCad. (2025). *KiCad Documentation*. Retrieved May 20, 2026, from [https://docs.kicad.org/](https://docs.kicad.org/)

4. MKTPCB. (2023). *Perfboard | A Quick Guide | Types, Uses, Techniques, and More*. Retrieved May 20, 2026, from [https://www.mktpcb.com/perfboard/](https://www.mktpcb.com/perfboard/)

5. SparkFun Electronics. (2025). *PCB Basics*. Retrieved May 20, 2026, from [https://learn.sparkfun.com/tutorials/pcb-basics/all](https://learn.sparkfun.com/tutorials/pcb-basics/all)

6. TechTarget. (2024). *What is a Printed Circuit Board (PCB)?*. Retrieved May 31, 2026, from [https://www.techtarget.com/whatis/definition/printed-circuit-board-PCB](https://www.techtarget.com/whatis/definition/printed-circuit-board-PCB)

---

