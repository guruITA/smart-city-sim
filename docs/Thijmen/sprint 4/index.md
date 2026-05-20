# Sprint 4 - Professionalizing the ESP32 City Tile Hardware Setup

# Learning Goal

How can I professionalize my prototype by transferring the working breadboard circuit to a permanent perfboard setup that can be mounted in the city tile, while keeping the wiring safe, clear, testable, and reliable?

---

## S - Situation

In the previous sprints, I worked on the train tile within the city simulation project. This tile included multiple functional features such as barrier control, warning lights, and a train prediction system. These elements were connected to the ESP32 and allowed the tile to react dynamically to incoming train events.

I also implemented backend communication where train data was sent from the ESP32 to the backend system. Later, I extended this by integrating an e-ink display that showed dynamic information such as the city name and logo, which was retrieved from the backend. This made the system more interactive and visually understandable, since digital information could now also be displayed physically on the tile.

Throughout these sprints, all hardware components were built and tested using a breadboard setup. This was useful during development because it allowed quick changes, easy debugging, and flexible wiring while experimenting with different features like sensors, lights, and the display.

However, while the system functions correctly, the breadboard setup is not suitable for long-term or real-world use inside the city tile. The wiring is not fixed, connections can become loose over time, and the overall structure is not stable enough for repeated demonstrations or transport. In addition, as more features were added (such as train prediction logic, backend communication, and the e-ink display), the circuit became more complex and harder to manage on a breadboard.

There is currently no permanent hardware structure inside the tile, meaning everything still depends on temporary wiring. If this is not improved, it could lead to reliability issues, unclear maintenance, and difficulties when teammates need to understand or reuse the hardware setup.

Because of this, there is a clear need to move from a prototype breadboard setup to a permanent and professional hardware solution using a perfboard. This will make the system more stable, easier to maintain, and better suited for integration within the shared city simulation project.

---

## T - Task

The main goal is to create a professional and reusable hardware setup in which the temporary breadboard wiring is replaced with permanent soldered connections on a perfboard. This setup should be designed in a way that keeps all wiring organized, understandable, and safe, while also ensuring that the circuit remains reliable during long-term use and demonstrations inside the city tile. In addition, the hardware should be structured so that all components can be mounted securely within the tile, without loose or fragile connections that could fail over time.

Another important aspect of this task is making sure the system remains testable and maintainable after being transferred to perfboard. Even though the circuit becomes permanent, it should still be possible to debug, extend, or replace parts when needed. The design should also support reuse and future expansion within the shared city simulation project, so that other tiles can adopt a similar structure without needing major changes.

Finally, the solution must fit within the broader collaboration of the project team and align with the "City Hub" concept, where multiple hardware components work together in a consistent and scalable system.

I will make this learning goal concrete by producing several deliverables, including an analysis document where I investigate the need for moving from breadboard to perfboard and identify risks and requirements, a design document where I describe the structure of the permanent hardware setup and how it will be organized and mounted, a realise document where I explain the physical implementation of transferring and soldering the circuit onto perfboard and testing it, and an advice document where I evaluate the final result and give recommendations for improving hardware reliability and future scalability within the project.

I will make this learning goal concrete by producing the following deliverables:

### Deliverables

#### 1. Analysis document

In the analysis document, I describe why the current breadboard setup is no longer sufficient and why a permanent solution is needed. I explain the risks of loose wiring, unstable connections, and increasing complexity due to multiple integrated features like train prediction, warning lights, backend communication, and the e-ink display. I also investigate what is required to create a reliable perfboard-based system, including soldering practices, wiring structure, component placement, and mounting inside the tile. Finally, I define risks such as short circuits, maintenance difficulty, and hardware failure, and determine criteria for success.

#### 2. Design document

In the design document, I explain how the ESP32, sensors, lights, and e-ink display will be structured on the perfboard. I describe how the wiring will be organized clearly to support readability, maintenance, and reuse. I also explain how the hardware will be mounted inside the tile in a stable way and how the design ensures compatibility with backend communication and the existing software structure. A diagram or schematic is included to show the full layout of the system.

#### 3. Realise document

In the realise document, I describe how I physically transferred the circuit from the breadboard to the perfboard. I explain how I soldered components, structured the wiring, and tested each connection step by step. I also describe challenges such as space limitations, preventing short circuits, and ensuring all components still worked after soldering. Additionally, I explain how I tested the full system inside the tile, including the e-ink display, lights, and backend communication. Finally, I document the results and lessons learned.

#### 4. Advice document

In the advice document, I evaluate how successful the perfboard implementation was in improving reliability and professionalism. I reflect on challenges such as soldering accuracy, wiring organization, and system debugging. I explain what worked well and what could be improved in future iterations. Based on this, I provide recommendations such as using modular connectors, designing a custom PCB, improving cable management, and making maintenance easier. I also give advice on how future tiles in the city simulation project can be built in a more standardized and scalable way.

---

## A - Action

---

## R - Result

---

## R - Reflection

---

## T - Transfer

---