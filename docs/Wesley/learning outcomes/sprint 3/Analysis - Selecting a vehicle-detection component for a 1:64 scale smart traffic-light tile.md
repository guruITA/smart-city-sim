## 1. Introduction

For this project, I am working on a traffic-light system for a scale model. The model is built on a 300 × 300 × 3.6 mm tile and uses a scale of approximately 1:64. In the earlier project phases, I already developed a traffic-light setup that moved from a single traffic light to a more realistic four-way crossing. That system uses an ESP32-S3 as controller, an MCP23017 as output expander, two ULN2803 chips as output drivers, and external LED power. This structure was chosen because it leaves room for future expansion with sensors and backend communication (Wesley, 2026b). 

The next step is to let the traffic-light system react to a car. A normal fixed traffic light only follows timing. A smarter traffic light reacts to real-time input from the environment, such as vehicles or pedestrians (Zavadko, 2026). In the earlier project documentation, vehicle detection is already described as an important step toward sensor-based smart traffic-light behaviour (Wesley, 2026c). 

For this project, I need to choose a component from the list that the teachers can provide. The chosen component must be suitable for detecting whether a model car is standing at the traffic light or passing by. Because the project is a scale model, the sensor should preferably not be visible on the tile. A visible sensor would make the road less realistic, while a hidden sensor would fit better with the goal of creating a clean scale-model traffic scene.

---

### 1.1 Problem statement

The problem is that I need a reliable way to detect a small model car on a 1:64 scale traffic-light tile, but I am limited to the components that are available from the teachers. The sensor must fit the small physical context of the tile and should preferably be hidden under the tile or integrated into the tile construction.

A real traffic-light crossing can use larger traffic-detection technologies, such as inductive loops, cameras, radar, or advanced traffic sensors. Those options are not realistic for this project because the whole tile is only 300 × 300 mm and only 3.6 mm thick. Therefore, the goal is not to copy a real traffic sensor exactly. The goal is to choose the most practical component from the available list that can act as a scale-model replacement for vehicle detection.

The selected component must support two situations:

1. A car is standing still at the traffic light.
2. A car is passing over or past the detection point.

The component must also fit the current ESP32-S3 setup. The existing project already uses a modular hardware structure, and the software has been improved by using `millis()` instead of `delay()` so that future sensors and communication can be added more safely (Wesley, 2026b). 

### 1.2 Main research question

**Which component from the teacher-provided component list is most suitable for detecting a car standing at or passing a traffic light in a 1:64 scale traffic-light tile, while preferably keeping the sensor hidden under the tile or integrated into the tile construction?**

---

### 1.3 Sub questions

1. **What must the sensor detect in this project?**
2. **Which component types from the teacher-provided list can theoretically detect a vehicle?**
3. **Which components can realistically be hidden under the tile or integrated into the tile construction of a 300 × 300 × 3.6 mm scale-model tile?**
4. **Which components are practical for detecting both a standing and passing model car?**
5. **Which component fits best with the ESP32-S3 and the existing traffic-light setup?**
