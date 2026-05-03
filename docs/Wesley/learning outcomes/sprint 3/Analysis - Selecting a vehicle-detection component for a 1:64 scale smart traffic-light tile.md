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

### 1.4 Methodology

This research uses a structured component-selection method. The goal is to make a reasoned choice before implementation. This is useful because the teacher-provided component list contains many modules that are not intended for vehicle detection.

The method consists of five steps.

#### Step 1: Define project requirements

First, I define what the sensor must do in this project. The requirements are based on the physical tile, the 1:64 scale, the need for hidden placement, and the existing ESP32-S3 traffic-light system. The earlier project advice also states that the next step should add only one smart feature first, keep the current hardware structure, keep the `millis()` based state machine, and continue using staged tests (Wesley, 2026a). 

#### Step 2: Screen the complete component list

Second, I screen the full teacher-provided component list. I use the component list from the Smart Cities semester 2 DLO page as the starting point for the selection (Sensors, Sensors and More Sensors - Smart Cities - Semester 2, n.d.). Components that are clearly not vehicle-detection sensors are removed from the candidate list. For example, temperature sensors, buzzers, LED modules, joystick modules, and flame sensors do not directly detect a model car.

#### Step 3: Research possible detection principles

Third, I look at the working principle of the remaining candidate sensors. Magnetic sensors, reed switches, infrared obstacle sensors, optical interrupters, photoresistors, and vibration sensors are considered. For example, the KY-021 mini reed module detects a magnetic field by closing a reed contact, while the KY-032 obstacle sensor detects objects through reflected infrared light (KY-021 Mini Reed Magnet - SensorKit, n.d.; KY-032 Obstacle Detector - SensorKit, n.d.).

#### Step 4: Compare the candidates

Fourth, I compare the candidate components using criteria that matter for this project:

| Criterion              | Meaning in this project                                                       |
| ---------------------- | ----------------------------------------------------------------------------- |
| Hidden integration | The sensor should preferably be hidden under the tile or integrated into the tile construction without being visible on the road surface. |
| Standing detection     | The sensor should detect a car that waits at the traffic light.               |
| Passing detection      | The sensor should detect a car that moves past the detection point.           |
| Reliability            | The signal should clearly represent car presence.                             |
| Scale suitability      | The sensor should work with a 1:64 model car and a 3.6 mm tile.               |
| Simplicity             | The component should be understandable and testable for this sprint.          |
| ESP32-S3 compatibility | The sensor should be readable by the ESP32-S3 without unnecessary complexity. |
| Future usefulness      | The component should support later smart traffic-light behaviour.             |

#### Step 5: Make the final selection in the conclusion

Finally, I use the comparison and sub conclusions to answer the main research question. The selected component must fit the technical requirements, the scale-model context, and the requirement that the sensor should preferably be hidden under the tile or integrated into the tile construction.

### 1.5 Requirements for the selected component

To make the component selection less subjective, I define the requirements before comparing the candidate components. These requirements are based on the physical scale-model context, the existing ESP32-S3 traffic-light setup, the teacher-provided sensor list, and the project goal of adding one smart vehicle-detection feature first (Sensors, Sensors and More Sensors - Smart Cities - Semester 2, n.d.; Wesley, 2026a; Wesley, 2026b; Wesley, 2026c).

| ID | Requirement | Priority | Reason |
|---|---|---|---|
| R1 | The component must detect a model car at the stop line. | Must | The traffic light needs to know whether a car is waiting. |
| R2 | The component must detect a car passing the detection point. | Must | The system should also notice a short passing event. |
| R3 | The component should be hidden under the tile or integrated into the tile construction. | Should | A hidden sensor keeps the 1:64 scale model visually realistic, while avoiding unrealistic visible sensor placement on the road surface. |
| R4 | The component must be readable by the ESP32-S3 without unsafe voltage levels. | Must | The ESP32-S3 uses 3.3 V logic and the GPIO input must not receive an unsafe voltage. |
| R5 | The component must work through or near the 3.6 mm tile material. | Must | The sensor must still trigger when placed under the road surface. |
| R6 | The component should require minimal calibration. | Should | This sprint focuses on the first smart feature, so the solution should remain understandable and testable. |
| R7 | The component should support staged testing. | Should | The earlier project approach used staged tests successfully, so the sensor should also be testable separately first. |
| R8 | The component should not disturb the existing traffic-light output architecture. | Must | The current ESP32-S3, MCP23017, and ULN2803 structure should remain usable. |

### 1.6 Scope and limitations of this research

This research is a desk-based component selection. The candidate components are not physically tested yet. The comparison is therefore based on the teacher-provided component list, external module documentation, earlier project documentation, and expected integration effort.

This means that some assumptions still need to be validated later. For example, the detection distance through the 3.6 mm tile, the required magnet strength, the magnet orientation, and the exact sensor mounting position cannot be fully proven without a physical test. Because of that, the selected component should be seen as the best component for the first prototype test, not yet as a fully validated final solution.

This limitation is important because the chosen magnetic detection approach depends on practical factors such as the distance between the magnet and the sensor, the thickness and material of the road surface, and the position of the magnet inside or under the model car.