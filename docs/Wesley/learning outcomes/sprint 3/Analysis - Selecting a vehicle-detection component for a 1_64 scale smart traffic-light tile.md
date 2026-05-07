# Analysis - Selecting a vehicle-detection component for a 1_64 scale smart traffic-light tile

## Abstract

This research investigates which teacher-provided component is most suitable for detecting whether a model car is standing at or passing a traffic light in a 1:64 scale traffic-light project. The project is built on a 300 × 300 × 3.6 mm tile, which creates strict physical and visual limitations. Because the project is a scale model, the preferred sensor should be hidden under the tile or integrated into the tile construction instead of being clearly visible on the road surface.

The research uses a structured component-selection method. First, the project requirements are defined from the current traffic-light context. After that, all teacher-provided components are screened for suitability. Components that cannot logically detect vehicle presence are removed from the selection. The remaining candidate components are then compared based on hidden integration, detection reliability, scale suitability, ESP32-S3 compatibility, simplicity, and usefulness for detecting both standing and passing model cars.

The research is connected to the existing project direction, where the traffic light becomes smarter when it reacts to real-time input instead of only following fixed timing (Wesley, 2026c; Zavadko, 2026). The final selection follows from the project requirements, the comparison of candidate components, the weighted decision matrix, and the limitations that still need to be validated during physical testing.

---

## Table of Contents


- [1. Introduction](#1-introduction)
  - [1.1 Problem statement](#11-problem-statement)
  - [1.2 Main research question](#12-main-research-question)
  - [1.3 Sub questions](#13-sub-questions)
  - [1.4 Methodology](#14-methodology)
    - [Step 1: Define project requirements](#step-1-define-project-requirements)
    - [Step 2: Screen the complete component list](#step-2-screen-the-complete-component-list)
    - [Step 3: Research possible detection principles](#step-3-research-possible-detection-principles)
    - [Step 4: Compare the candidates](#step-4-compare-the-candidates)
    - [Step 5: Make the final selection in the conclusion](#step-5-make-the-final-selection-in-the-conclusion)
  - [1.5 Requirements for the selected component](#15-requirements-for-the-selected-component)
  - [1.6 Scope and limitations of this research](#16-scope-and-limitations-of-this-research)
- [2. Which Components Are Not Suitable and Why](#2-which-components-are-not-suitable-and-why)
- [3. Sub questions](#3-sub-questions)
  - [3.1 What must the sensor detect in this project?](#31-what-must-the-sensor-detect-in-this-project)
    - [Sub conclusion 3.1](#sub-conclusion-31)
  - [3.2 Which component types from the list can theoretically detect a vehicle?](#32-which-component-types-from-the-list-can-theoretically-detect-a-vehicle)
    - [Sub conclusion 3.2](#sub-conclusion-32)
  - [3.3 Which components can realistically be hidden under the tile or integrated into the tile construction?](#33-which-components-can-realistically-be-hidden-under-the-tile-or-integrated-into-the-tile-construction)
    - [Sub conclusion 3.3](#sub-conclusion-33)
  - [3.4 Which components are practical for detecting both a standing and passing model car?](#34-which-components-are-practical-for-detecting-both-a-standing-and-passing-model-car)
    - [Sub conclusion 3.4](#sub-conclusion-34)
  - [3.5 Which component fits best with the esp32-s3 and the existing traffic-light setup?](#35-which-component-fits-best-with-the-esp32-s3-and-the-existing-traffic-light-setup)
    - [Sub conclusion 3.5](#sub-conclusion-35)
- [4. Comparison and selection](#4-comparison-and-selection)
  - [4.1 Weighted decision matrix](#41-weighted-decision-matrix)
  - [4.2 Technical constraints for the selected solution](#42-technical-constraints-for-the-selected-solution)
  - [4.3 Risk analysis](#43-risk-analysis)
  - [4.4 Validation plan](#44-validation-plan)
- [5. Conclusion](#5-conclusion)
- [6. References](#6-references)
- [Appendix A — Teacher-provided sensor list](#appendix-a--teacher-provided-sensor-list)




---

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

---

### 1.2 Main research question

**Which component from the teacher-provided component list is most suitable for detecting a car standing at or passing a traffic light in a 1:64 scale traffic-light tile, while preferably keeping the sensor hidden under the tile or integrated into the tile construction?**

---

### 1.3 Sub questions

1. **What must the sensor detect in this project?**
2. **Which component types from the teacher-provided list can theoretically detect a vehicle?**
3. **Which components can realistically be hidden under the tile or integrated into the tile construction of a 300 × 300 × 3.6 mm scale-model tile?**
4. **Which components are practical for detecting both a standing and passing model car?**
5. **Which component fits best with the ESP32-S3 and the existing traffic-light setup?**

---

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

---

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

## 2. Which Components Are Not Suitable and Why

The teacher-provided component list contains many useful electronics modules, but most of them are not suitable for detecting a vehicle on a model traffic-light tile. A vehicle-detection component must detect the presence or movement of a model car. Components that only produce output, measure unrelated environmental values, or require manual operation are therefore not suitable. The complete component list is based on the teacher-provided DLO source for the Smart Cities sensor kit (Sensors, Sensors and More Sensors - Smart Cities - Semester 2, n.d.).

| Component                                     |    Suitable? | Reason                                                                                                  |
| --------------------------------------------- | -----------: | ------------------------------------------------------------------------------------------------------- |
| KY-001 Temperature sensor module              |           No | Measures temperature, not vehicle presence.                                                             |
| KY-002 Vibration switch module                |    Weak / No | Can detect vibration or shock, but a small 1:64 car may not create reliable vibration through the tile. |
| KY-004 Key switch module                      |           No | Manual button input, not automatic vehicle detection.                                                   |
| KY-005 Infrared emission sensor module        | No by itself | Only emits infrared light. It would need a receiver and alignment.                                      |
| KY-006 Small passive buzzer module            |           No | Output component, not a sensor for vehicle presence.                                                    |
| KY-008 Laser sensor module                    | No by itself | Laser emitter, not a complete detector. It would need a receiver and alignment.                         |
| KY-009 3-color full-color LED SMD module      |           No | Output component.                                                                                       |
| KY-011 2-color LED module                     |           No | Output component.                                                                                       |
| KY-012 Active buzzer module                   |           No | Output component.                                                                                       |
| KY-013 Temperature sensor module              |           No | Measures temperature.                                                                                   |
| KY-015 Temperature and humidity sensor module |           No | Measures environment, not vehicle presence.                                                             |
| KY-016 3-color LED module                     |           No | Output component.                                                                                       |
| KY-017 Mercury open optical module            |           No | Tilt or position-related switch, not suitable for road vehicle detection.                               |
| KY-018 Photo resistor module                  |    Weak / No | Can detect light changes, but a shadow is not reliable enough because room lighting can change.         |
| KY-019 5V relay module                        |           No | Switching output module, not a sensor.                                                                  |
| KY-020 Tilt switch module                     |           No | Detects tilt, not a car on a road.                                                                      |
| KY-022 Infrared sensor receiver module        | No by itself | Receiver only. It would need an emitter and alignment.                                                  |
| KY-023 XY-axis joystick module                |           No | Manual input component.                                                                                 |
| KY-026 Flame sensor module                    |           No | Detects flame or infrared flame sources, not vehicles.                                                  |
| KY-027 Magic light cup module                 |           No | Not suitable for vehicle presence detection.                                                            |
| KY-028 Temperature sensor module              |           No | Measures temperature.                                                                                   |
| KY-029 Yin Yi 2-color LED module 3MM          |           No | Output component.                                                                                       |
| KY-031 Knock sensor module                    |    Weak / No | Detects knocks or vibration, not reliable vehicle presence.                                             |
| KY-034 Automatic flashing colorful LED module |           No | Output component.                                                                                       |
| KY-036 Metal touch sensor module              |           No | Detects touch, not a car through the tile.                                                              |
| KY-037 Sensitive microphone sensor module     |           No | Detects sound, not reliable car presence.                                                               |
| KY-038 Microphone sound sensor module         |           No | Detects sound, not reliable car presence.                                                               |
| KY-039 Detect the heartbeat module            |           No | Biological sensor, unrelated to the project.                                                            |
| KY-040 Rotary encoder module                  |           No | Rotation input component, not vehicle detection.                                                        |

Some weak candidates could technically react when a car moves nearby, but they are not reliable enough for this project. For example, a photoresistor can react to a shadow, but light level is affected by the room, hand movement, and other shadows. A vibration or knock sensor can react to impact, but a small model car may not create consistent vibration through a thin tile. For a traffic-light input, the signal must be directly linked to the car being present, not to an indirect environmental effect.

---

## 3. Sub questions

### 3.1 What must the sensor detect in this project?

The sensor must detect whether a model car is present at the traffic light. This means the sensor must support a car that stands still at the stop line and a car that passes the detection point. In the existing smart-traffic-light requirements, vehicle detection is important because the system should use vehicle presence as input for traffic-light decisions (Wesley, 2026c). 

For a standing car, the sensor output should remain active while the car is above or near the detection point. For a passing car, the output may only be active briefly. Because of that, the future software should be able to read the input repeatedly and store a short detection event if needed. This fits the existing decision to use non-blocking `millis()` timing, because the program can keep checking inputs while the traffic-light sequence continues (Wesley, 2026b). 

External sensor sources also show that different candidate components detect in different ways. The KY-021 detects a magnetic field by closing a reed contact, while the KY-032 detects an obstacle through reflected infrared light (KY-021 Mini Reed Magnet - SensorKit, n.d.; KY-032 Obstacle Detector - SensorKit, n.d.). This matters because the project does not only need “a sensor”. It needs a sensor principle that fits a hidden scale-model road.

#### Sub conclusion 3.1

The sensor must provide a clear and repeatable signal for car presence. It should work for both a stationary car and a moving car, and the signal should be simple enough to use as the first smart input in the traffic-light system.

---

### 3.2 Which component types from the list can theoretically detect a vehicle?

Only a small group of components from the teacher-provided list can theoretically detect a vehicle. These components use magnetic, infrared, optical, light, or vibration-based detection.

| Component                          | Detection principle                         | Possible use for this project                                  |
| ---------------------------------- | ------------------------------------------- | -------------------------------------------------------------- |
| KY-003 Hall magnetic sensor        | Digital magnetic-field detection            | Could detect a magnet in or under a model car.                 |
| KY-010 Optical broken module       | Light beam interruption                     | Could detect a car passing through an interrupter gap.         |
| KY-018 Photo resistor module       | Light-level change                          | Could detect a shadow, but reliability is weak.                |
| KY-021 Mini magnetic reed module   | Magnetic reed contact                       | Could detect a magnet in or under a model car.                 |
| KY-024 Linear magnetic Hall sensor | Digital and analog magnetic-field detection | Could detect a magnet and possibly field strength.             |
| KY-025 Reed module                 | Magnetic reed detection                     | Could detect a magnet, similar to KY-021.                      |
| KY-032 Obstacle avoidance sensor   | Reflected infrared light                    | Could detect a visible object in front of the sensor.          |
| KY-033 Hunt sensor module          | Surface contrast / reflection               | Could detect contrast, but is mainly meant for line following. |
| KY-035 Hall magnetic sensor        | Analog magnetic-field detection             | Could detect magnetic field strength and polarity.             |

The magnetic group is the strongest theoretical group. The KY-003 uses a Hall-effect switch that outputs a digital signal when a magnetic field is detected (KY-003 Hall Magnetic Field Sensor - SensorKit, n.d.). The KY-021 uses a reed switch that closes when a magnetic field is present (KY-021 Mini Reed Magnet - SensorKit, n.d.). The KY-024 has a digital output for magnetic-field detection and an analog output for the measured sensor value (KY-024  Linear, Magnetic Hall Sensor - SensorKit, n.d.). The KY-035 is an analog Hall sensor where the output voltage changes depending on the magnetic field and pole direction (KY-035 Bihor Magnetic Sensor - SensorKit, n.d.). 

The infrared and optical group can also detect objects, but usually requires visible placement or alignment. The KY-032 uses reflected infrared light from an obstacle (KY-032 Obstacle Detector - SensorKit, n.d.). The KY-010 works as a light barrier where the signal changes when the beam is interrupted (KY-10 Light Barrier - SensorKit, n.d.). These can detect movement, but they are harder to hide cleanly under a road surface.

#### Sub conclusion 3.2

The most relevant candidate group is the magnetic sensor group. Infrared and optical sensors can detect objects, but magnetic sensors fit the hidden scale-model requirement better.

---

### 3.3 Which components can realistically be hidden under the tile or integrated into the tile construction?

The hidden-integration requirement is important because the project is a scale model. The road should look realistic, and the sensor should preferably not be visible. This does not automatically mean that a full sensor module can fit inside the 3.6 mm tile. Instead, the component should preferably be mounted under the tile or integrated into the tile construction in a way that keeps the road surface visually clean. This makes the physical sensing principle more important than only the electrical signal.

Magnetic sensors are suitable for hidden placement because they do not need direct optical contact with the car. A small magnet can be placed inside or under the model car, and the sensor can be placed under the road surface. The KY-021 is especially simple because the reed switch closes when a magnetic field is detected (KY-021 Mini Reed Magnet - SensorKit, n.d.). However, the full KY-021 module should not automatically be assumed to fit inside the tile. The module dimensions are larger than the 3.6 mm tile thickness, so the realistic mounting approach is to place the module under the tile or position the sensing part as close as possible to the road surface while keeping the road visually clean (KY-021 Mini Reed Magnet - SensorKit, n.d.). ArduinoModules also describes the KY-021 as a digital reed-switch module that can operate at 3.3 V to 5 V, which fits common microcontroller projects (ArduinoModules, 2026).

The KY-024 and KY-035 can also be hidden because they detect magnetic fields. However, they are less simple for a first implementation. The KY-024 has both analog and digital outputs, which means it may need threshold adjustment and calibration (KY-024  Linear, Magnetic Hall Sensor - SensorKit, n.d.). The KY-035 outputs an analog voltage related to magnetic field strength and pole direction, which is useful but requires analog interpretation in the software (KY-035 Bihor Magnetic Sensor - SensorKit, n.d.). 

The KY-032 infrared obstacle sensor is harder to hide because it needs to send infrared light toward the object and receive the reflection. That means the emitter and receiver normally need a visible direction toward the car (KY-032 Obstacle Detector - SensorKit, n.d.). The KY-010 light barrier is also difficult to hide because the car must physically interrupt the light path (KY-10 Light Barrier - SensorKit, n.d.).

#### Sub conclusion 3.3

Magnetic detection is the most realistic hidden solution. Optical and infrared components can work for visible experiments, but they are less suitable when the road surface must stay clean and realistic.

---

### 3.4 Which components are practical for detecting both a standing and passing model car?

A practical sensor must detect a car that remains at the stop line and a car that passes the detection point. This means the sensor must be useful for both a continuous signal and a short event.

A magnetic reed sensor can support both cases. When a car with a magnet stands above the sensor, the magnetic field can keep the reed switch activated. When the car passes over the sensor, the reed switch can activate briefly. This gives the software two possible interpretations: a long signal can mean “car waiting,” and a short signal can mean “car passed.” This is practical for the existing traffic-light software because the system already uses non-blocking timing, which is suitable for repeatedly checking sensor input (Wesley, 2026b). 

A Hall sensor can also detect both standing and passing cars. The KY-003 gives a digital magnetic detection signal (KY-003 Hall Magnetic Field Sensor - SensorKit, n.d.). The KY-024 and KY-035 can give more detailed magnetic information through analog output, but this also makes them more complex to calibrate (KY-024  Linear, Magnetic Hall Sensor - SensorKit, n.d.; KY-035 Bihor Magnetic Sensor - SensorKit, n.d.).

The KY-032 can detect a standing object if the car stays in front of the infrared beam, and it can detect a passing car if the reflection changes. However, the result can depend on the surface, colour, angle, and distance of the model car. Because the KY-032 detects reflected infrared light, the physical placement of the sensor matters strongly (KY-032 Obstacle Detector - SensorKit, n.d.).

The KY-010 can detect passing movement well if the car interrupts the beam, but it is less practical for detecting a car standing on a normal road surface because it requires a slot or aligned light path (KY-10 Light Barrier - SensorKit, n.d.).

#### Sub conclusion 3.4

Magnetic detection is the most practical for both standing and passing cars. A reed switch gives the simplest yes/no signal, while Hall sensors give more flexibility but require more calibration.

---

### 3.5 Which component fits best with the esp32-s3 and the existing traffic-light setup?

The selected sensor must fit the existing ESP32-S3 traffic-light setup. The current traffic-light system already uses the ESP32-S3 as controller, while the MCP23017 and ULN2803 handle the traffic-light outputs. This means the sensor should be added as a separate input and should not disturb the existing output structure.

Electrical compatibility is also important. The ESP32-S3 works with a 3.3 V logic environment. The official ESP32-S3 datasheet lists 3.3 V as the typical recommended input voltage for the main 3.3 V power domains and shows a maximum recommended value of 3.6 V (“ESP32-S3 Series Datasheet Version 2.2”, 2026). It also lists high-level input voltage relative to the chip supply voltage, which means that a sensor output should not simply send a 5 V signal into an ESP32-S3 GPIO pin (“ESP32-S3 Series Datasheet Version 2.2”, 2026). Therefore, the selected sensor should preferably be powered from 3.3 V. If a module is powered from 5 V, the output signal must be checked and, if necessary, reduced with a voltage divider or level shifter before it is connected to the ESP32-S3.

A digital sensor is easiest for this first smart feature. The KY-021 can be read as a digital input because the reed switch changes state when a magnetic field is present (KY-021 Mini Reed Magnet - SensorKit, n.d.; ArduinoModules, 2026). This fits the ESP32-S3 well because the software can read the sensor with a normal GPIO input. It also fits the advice from the previous sprint: add one smart feature first, keep the current hardware structure, and test the new part in stages (Wesley, 2026a). 

The KY-003 is also a realistic magnetic alternative because it gives a digital magnetic detection signal. However, the SensorKit documentation specifies the KY-003 operating voltage as 5 V due to the onboard LED and mentions that a voltage translator is needed when it is used with a 3.3 V microcontroller environment such as the Raspberry Pi Pico (KY-003 Hall Magnetic Field Sensor - SensorKit, n.d.). Since the ESP32-S3 also uses 3.3 V logic, the KY-003 would require extra voltage-level checking or level shifting before it can be safely connected. This makes it less simple than the KY-021 for the first prototype.

The KY-024 and KY-035 are also compatible with a microcontroller, but they are less simple because they involve analog readings, thresholds, or calibration (KY-024 Linear, Magnetic Hall Sensor - SensorKit, n.d.; KY-035 Bihor Magnetic Sensor - SensorKit, n.d.). The KY-032 is also possible as a digital sensor, but the physical integration is less suitable for a hidden tile because the sensor must face the object (KY-032 Obstacle Detector - SensorKit, n.d.).

#### Sub conclusion 3.5

The component that fits best with the current ESP32-S3 setup should be digital, simple, easy to test separately, and safe for 3.3 V GPIO input. Magnetic reed detection fits this better than infrared, optical, or analog magnetic detection, as long as the KY-021 is powered and wired so that the ESP32-S3 input does not receive a 5 V signal.

---

## 4. Comparison and selection

This section compares the realistic candidate components and shows how well each option fits the project requirements.

| Component                          | Hidden integration | Standing car |  Passing car | ESP32-S3 simplicity | Main limitation                                                | Overall assessment                                  |
| ---------------------------------- | -----------------: | -----------: | -----------: | ------------------: | -------------------------------------------------------------- | --------------------------------------------------- |
| KY-021 Mini magnetic reed module   |               High |         High |         High |                High | Needs a magnet in or under the car                             | Strongest candidate                                 |
| KY-025 Reed module                 |      Medium / High |         High |         High |       Medium / High | Similar magnetic reed principle, but less preferred than KY-021 for compact first implementation | Good alternative |
| KY-003 Hall magnetic sensor        |               High |         High |         High |      Medium / Lower | Needs correct magnet orientation and may require level shifting because the SensorKit version is specified for 5 V operation | Good alternative, but less simple for ESP32-S3 |
| KY-024 Linear magnetic Hall sensor |               High |         High |         High |              Medium | Needs threshold or analog calibration                          | Technically strong, but more complex                |
| KY-035 Analog Hall sensor          |               High |         High |         High |               Lower | Requires analog interpretation                                 | Useful for later advanced testing                   |
| KY-032 Obstacle avoidance sensor   |       Low / Medium |       Medium |       Medium |              Medium | Needs visible line-of-sight and depends on reflection          | Useful for visible prototype testing                |
| KY-010 Optical broken module       |                Low |       Medium |         High |              Medium | Requires beam interruption and physical gap                    | Better for counting than hidden stop-line detection |
| KY-033 Hunt sensor module          |       Low / Medium | Low / Medium |       Medium |              Medium | Designed for surface contrast, not car presence                | Not recommended                                     |
| KY-018 Photo resistor module       |             Medium |          Low | Low / Medium |              Medium | Sensitive to ambient light and shadows                         | Not recommended                                     |

The comparison shows that the strongest candidates are the magnetic sensors. The KY-021, KY-003, KY-024, and KY-035 can all detect a magnet. However, the KY-021 has the best balance between hidden placement, simple digital detection, and easy implementation. The KY-024 and KY-035 are more flexible but also require more calibration because they use analog behaviour or adjustable thresholds (KY-024  Linear, Magnetic Hall Sensor - SensorKit, n.d.; KY-035 Bihor Magnetic Sensor - SensorKit, n.d.).

The infrared and optical sensors are less suitable for the tile because they are harder to hide. The KY-032 detects reflected infrared light, so it needs a suitable angle and line-of-sight to the vehicle (KY-032 Obstacle Detector - SensorKit, n.d.). The KY-010 detects interruption of a light barrier, which is useful for movement detection but not ideal for a hidden road surface (KY-10 Light Barrier - SensorKit, n.d.).

A practical setup for the strongest candidate would be: A small magnet is placed inside or underneath the model car. When the car moves over the road surface, the magnet comes close to the sensor position under the 3.6 mm tile. The magnetic sensor is hidden under the stop line, so it is not visible on the road surface. When the magnet is close enough, the sensor changes its signal. The ESP32-S3 reads this signal as a digital input and can then use it in the traffic-light decision logic to know that a car is standing at or passing the traffic light.

This approach is not the same as a real inductive traffic loop. It does not detect the metal body of a car by itself. Instead, it detects a magnet added to the model car. For this scale-model project, that is acceptable because the goal is to create a practical and testable vehicle-presence input within a very small physical tile.

---

### 4.1 Weighted decision matrix

To make the component selection more measurable, a weighted decision matrix is used. The matrix only includes components that remained realistic candidates after the earlier qualitative screening. Components that were already rejected because they do not directly detect vehicle presence are not included again, because that would add unnecessary comparison without improving the final decision.

The score range is 1 to 5. Because this research is desk-based and the components have not yet been physically tested, the scores represent the expected fit for this project based on the component principle, available documentation, project constraints, and expected integration effort. A score of 5 therefore does not mean that the component has already been proven in practice. It means that the component is theoretically the strongest fit for that criterion and should be validated during the realisation phase.

| Score | Meaning                                                                                                |
| ----: | ------------------------------------------------------------------------------------------------------ |
|     1 | Poor fit. The component has a major limitation for this criterion.                                     |
|     2 | Weak fit. The component could work in theory, but the limitation is large.                             |
|     3 | Acceptable fit. The component is usable, but with clear practical limitations.                         |
|     4 | Good fit. The component fits the criterion well, but still needs some attention during implementation. |
|     5 | Strong fit. The component directly supports the criterion with low expected complexity.                |

The weights are based on the project priorities. Hidden integration receives the highest weight because the sensor should preferably not be visible on the 1:64 scale tile. Reliable standing detection is also weighted strongly because the traffic light must be able to detect a car waiting at the stop line. Future expandability receives a lower weight because this research focuses on selecting the best first prototype component, not on designing the final full-scale sensor system.

| Criterion                   |   Weight |   KY-021 |   KY-025 |   KY-003 |   KY-024 |   KY-035 |   KY-032 |   KY-010 |
| --------------------------- | -------: | -------: | -------: | -------: | -------: | -------: | -------: | -------: |
| Hidden integration          |      25% |        5 |        4 |        5 |        5 |        5 |        2 |        2 |
| Reliable standing detection |      20% |        5 |        5 |        4 |        4 |        4 |        3 |        2 |
| Passing detection           |      15% |        4 |        4 |        4 |        4 |        4 |        3 |        4 |
| ESP32-S3 simplicity         |      15% |        5 |        4 |        3 |        3 |        3 |        4 |        3 |
| Scale suitability           |      10% |        4 |        4 |        4 |        4 |        4 |        2 |        2 |
| Calibration effort          |      10% |        5 |        5 |        4 |        3 |        3 |        3 |        3 |
| Future expandability        |       5% |        3 |        3 |        4 |        5 |        5 |        3 |        3 |
| **Weighted total**          | **100%** | **4.65** | **4.25** | **4.10** | **4.05** | **4.05** | **2.80** | **2.60** |

The KY-018 photoresistor and KY-033 hunt sensor are not included in the weighted matrix because the qualitative comparison already showed that their detection principles are too indirect for this project. The KY-018 depends on changes in light level, which makes it sensitive to room lighting, shadows, and hand movement. The KY-033 is mainly intended for detecting surface contrast or reflection, which makes it more suitable for line-following behaviour than for hidden vehicle presence detection. These components were therefore considered theoretically possible but not realistic enough for the final shortlist.

The KY-025 is included in the weighted matrix because it uses a similar magnetic reed principle to the KY-021. This makes it a relevant alternative. However, the KY-021 still scores higher because it is treated as the more compact and straightforward first choice for this project. Both reed-based options can detect a magnet placed inside or underneath a model car, but the KY-021 has the strongest balance between hidden integration, simple digital reading, low calibration effort, and staged prototype testing (KY-021 Mini Reed Magnet - SensorKit, n.d.).

The KY-003 also scores well because it is a digital Hall magnetic sensor and can detect a magnetic field without a mechanical reed contact (KY-003 Hall Magnetic Field Sensor - SensorKit, n.d.). However, it receives a lower score for ESP32-S3 simplicity because the SensorKit version is specified for 5 V operation due to the onboard LED. The documentation also mentions that a voltage translator is needed when the module is used with a 3.3 V microcontroller environment, which is relevant because the ESP32-S3 also uses 3.3 V logic (KY-003 Hall Magnetic Field Sensor - SensorKit, n.d.). This does not make the KY-003 unsuitable, but it makes it less straightforward than the KY-021 for the first prototype.

The KY-024 and KY-035 are technically strong because they can provide more detailed magnetic information. However, their analog behaviour or adjustable threshold makes them less simple for the first prototype. They may be useful in a later version if measuring magnetic field strength becomes relevant, but for this sprint their extra flexibility does not outweigh the added calibration effort (KY-024 Linear, Magnetic Hall Sensor - SensorKit, n.d.; KY-035 Bihor Magnetic Sensor - SensorKit, n.d.).

The KY-032 and KY-010 score lower because they are harder to hide in the tile. The KY-032 obstacle sensor depends on reflected infrared light and therefore needs a visible direction toward the vehicle. The KY-010 light barrier can detect passing movement well, but it requires a beam interruption path or physical gap, which conflicts with the clean hidden-road requirement (KY-032 Obstacle Detector - SensorKit, n.d.; KY-10 Light Barrier - SensorKit, n.d.).

Based on the weighted decision matrix, the KY-021 remains the strongest candidate. It scores highest because it directly supports the most important project criteria: hidden integration, standing-car detection, simple ESP32-S3 input reading, and low calibration effort. The result also supports the earlier qualitative comparison, while making the final selection more transparent and repeatable.

### 4.2 Technical constraints for the selected solution

The KY-021 is the strongest first choice, but it has several technical constraints that must be handled during implementation.

First, the KY-021 does not detect the model car itself. It detects a magnetic field. Therefore, a small magnet must be placed inside or underneath the model car. This makes the KY-021 a scale-model replacement for vehicle detection, not a real inductive traffic loop.

Second, the detection depends on the distance and orientation between the magnet and the reed switch. The 3.6 mm tile thickness, road material, mounting height, and magnet strength can all influence whether the sensor triggers reliably. This must be tested physically before the solution can be considered proven.

Third, the KY-021 uses a reed switch, which is a mechanical contact. Mechanical contacts can bounce when they open or close. Because of that, the software should include debouncing so one detection does not accidentally become multiple detections.

Fourth, the ESP32-S3 input must be protected from unsafe voltage levels. The recommended implementation is to power the KY-021 module at 3.3 V and read it with a normal ESP32-S3 digital input. If a 5 V supply is used for the module, the output voltage must be checked before connecting it to the ESP32-S3. If the module output can become 5 V, a voltage divider or level shifter is required (“ESP32-S3 Series Datasheet Version 2.2”, 2026).

These constraints do not make the KY-021 unsuitable, but they show what must be verified during the realisation phase.

### 4.3 Risk analysis

| Risk | Possible impact | Mitigation |
|---|---|---|
| Magnet is too weak through the 3.6 mm tile | The sensor does not trigger reliably. | Test multiple magnet strengths and place the sensor as close as possible to the road surface. |
| Magnet orientation is wrong | The sensor may not activate when the car is above it. | Test different magnet orientations under the model car. |
| Reed switch bounce | One car detection may be counted multiple times. | Add software debouncing using `millis()`. |
| Passing car trigger is too short | The ESP32-S3 may miss the detection event. | Poll the input frequently or store the detection as a short event latch. |
| Sensor detection zone is too narrow | The car may not be detected if it is not placed exactly above the sensor. | Place the sensor under the expected stop-line position and test different car positions. |
| 5 V output is connected directly to ESP32-S3 GPIO | The ESP32-S3 input could be damaged. | Use 3.3 V sensor power or add a voltage divider/level shifter if 5 V output is possible. |
| Nearby magnets or multiple cars influence the sensor | False detection may occur. | Test the sensor with one and multiple cars and define the detection area clearly. |
| The full KY-021 module does not physically fit inside the 3.6 mm tile | The sensor cannot be hidden inside the tile without protruding or requiring a recess. | Mount the module under the tile or place only the sensing position as close as possible to the road surface while keeping the module underneath. |

### 4.4 Validation plan

Because this research is desk-based, the selected component still needs to be validated physically. The validation should be done before the KY-021 is integrated into the full traffic-light logic.

| Test | What is tested | Expected result |
|---|---|---|
| 1. Basic ESP32-S3 input test | Connect the KY-021 to the ESP32-S3 at 3.3 V and print the sensor state in the serial monitor. | The serial monitor clearly shows when the sensor is active and inactive. |
| 2. Magnet distance test | Move a small magnet toward the KY-021 from different distances. | The sensor triggers at a distance that is usable for the tile setup. |
| 3. Tile thickness test | Place the KY-021 under the 3.6 mm tile and move the magnet above the road surface. | The sensor still detects the magnet through the tile. |
| 4. Standing car test | Place a model car with a magnet above the sensor. | The sensor remains active while the car is standing at the stop line. |
| 5. Passing car test | Move the model car over the sensor by hand for 10 trials. | At least 9 out of 10 passes are detected and stored as a short event. |
| 6. Magnet orientation test | Test different magnet positions and orientations under the car. | The most reliable magnet position is found. |
| 7. Debounce test | Check whether one detection causes multiple false triggers. | Software debouncing prevents repeated false detections. |
| 8. Integration test | Connect the sensor state to the traffic-light decision logic. | The traffic-light system can use the detection without blocking the existing `millis()` based state machine. |

In the software, a short passing detection should not be missed. Therefore, the final implementation should store a detection event for a short time using `millis()`. This keeps the sensor logic compatible with the non-blocking traffic-light approach that was already used in the previous realisation (Wesley, 2026b).

## 5. Conclusion

The main research question was:

**Which component from the teacher-provided component list is most suitable for detecting a car standing at or passing a traffic light in a 1:64 scale traffic-light tile, while preferably keeping the sensor hidden under the tile or integrated into the tile construction?**

Based on the research, the most suitable component is the **KY-021 Mini Magnetic Reed Module**.

The KY-021 is the best choice because it matches the most important requirements of this project. It can be mounted under the road surface, it can detect a small magnet placed inside or under a model car, it gives a simple digital signal, and it can detect both a standing car and a passing car. The KY-021 works by closing a reed contact when a magnetic field is present, which makes it suitable for simple magnet-based presence detection (KY-021 Mini Reed Magnet - SensorKit, n.d.). However, the full KY-021 module should not be described as fitting inside the 3.6 mm tile without further physical validation. If the full module is too thick for the tile, only the sensing position should be placed as close as possible to the road surface while the module is mounted underneath. It is also practical for microcontroller projects because the module provides a digital output and can operate in the 3.3 V to 5 V range (ArduinoModules, 2026).

The KY-003 Hall sensor is also a realistic alternative because it gives digital magnetic detection and does not use a mechanical reed contact. However, the SensorKit version is specified for 5 V operation due to the onboard LED and may require voltage-level checking or level shifting before it can be connected safely to the ESP32-S3 (KY-003 Hall Magnetic Field Sensor - SensorKit, n.d.). For this first implementation, the KY-021 is therefore preferred because it behaves like a simple switch and is easier to understand, wire, debug, and explain.

The KY-024 and KY-035 are good alternatives, but they are more complex because they use analog values or adjustable thresholds. They may be useful for a later version where magnetic field strength needs to be measured, but they are not the simplest first choice for this sprint (KY-024 Linear, Magnetic Hall Sensor - SensorKit, n.d.; KY-035 Bihor Magnetic Sensor - SensorKit, n.d.). The KY-032 and KY-010 can also detect objects or movement, but they are less suitable because they normally need visible line-of-sight or a beam interruption path (KY-032 Obstacle Detector - SensorKit, n.d.; KY-10 Light Barrier - SensorKit, n.d.).

The most important limitation is that the KY-021 does not detect a normal model car by itself. A small magnet must be added to the car. Therefore, I should describe the KY-021 honestly as a **scale-model replacement for vehicle detection**, not as a real inductive loop. For this project, that limitation is acceptable because the tile is only 3.6 mm thick and the project is focused on a 1:64 model environment.

The recommended next implementation step is to place one KY-021 under the stop line of one road section and add a small magnet to one test car. The KY-021 should first be powered and tested at 3.3 V with the ESP32-S3 before it is connected to the traffic-light logic. The first tests should verify magnet detection through the 3.6 mm tile, continuous detection of a standing car, short-pulse detection of a passing car, and software debouncing. If these tests are reliable, the same detection principle can later be repeated for other road sections.

---

## 6. References


ArduinoModules. (2026, April 24). Arduino Mini Magnetic Reed Switch Module KY-021 — Wiring & Code. ArduinoModulesInfo. https://arduinomodules.info/ky-021-mini-magnetic-reed-switch-module/


ESP32-S3 Series Datasheet Version 2.2. (2026). In Espressif. Espressif Systems. Retrieved May 2, 2026, from https://documentation.espressif.com/esp32-s3_datasheet_en.pdf

KY-003 Hall magnetic field sensor - SensorKit. (n.d.). https://sensorkit.joy-it.net/en/sensors/ky-003

KY-10 Light barrier - SensorKit. (n.d.). https://sensorkit.joy-it.net/en/sensors/ky-010

KY-021 Mini reed magnet - SensorKit. (n.d.). https://sensorkit.joy-it.net/en/sensors/ky-021

KY-024  Linear, magnetic hall sensor - SensorKit. (n.d.). https://sensorkit.joy-it.net/en/sensors/ky-024

KY-032 Obstacle detector - SensorKit. (n.d.). https://sensorkit.joy-it.net/en/sensors/ky-032

KY-035 Bihor magnetic sensor - SensorKit. (n.d.). https://sensorkit.joy-it.net/en/sensors/ky-035

Sensors, Sensors and more Sensors - Smart Cities - semester 2. (n.d.). https://dlo.mijnhva.nl/d2l/le/news/691654/363628/view?ou=691654

Wesley. (2026a). *Advice: Learning goal 1 — Expanding the traffic-light system to multiple traffic lights* [Advice deliverable].

Wesley. (2026b). *Realisation: Breadboard realisation of the four-way traffic-light setup* [Realisation deliverable].

Wesley. (2026c). *Smart traffic light: When does a traffic light become smart?* [Project document].

Zavadko, V. (2026, January 14). How do smart traffic lights work? technical architecture and use cases explained. Intellias. https://intellias.com/smart-traffic-signals/





---

## Appendix A — Teacher-provided sensor list

The sensor list below comes from the Smart Cities semester 2 DLO source (Sensors, Sensors and More Sensors - Smart Cities - Semester 2, n.d.)

- KY-001 Temperature sensor module
- KY-002 Vibration switch module
- KY-003 Hall magnetic sensor module
- KY-004 Key switch module
- KY-005 Infrared emission sensor module
- KY-006 Small passive buzzer module
- KY-008 Laser sensor module
- KY-009 3-color full-color LED SMD modules
- KY-010 Optical broken module
- KY-011 2-color LED module
- KY-012 Active buzzer module
- KY-013 Temperature sensor module
- KY-015 Temperature and humidity sensor module
- KY-016 3-color LED module
- KY-017 Mercury open optical module
- KY-018 Photo resistor module
- KY-019 5V relay module
- KY-020 Tilt switch module
- KY-021 Mini magnetic reed modules
- KY-022 Infrared sensor receiver module
- KY-023 XY-axis joystick module
- KY-024 Linear magnetic Hall sensors
- KY-025 Reed module
- KY-026 Flame sensor module
- KY-027 Magic light cup module
- KY-028 Temperature sensor module
- KY-029 Yin Yi 2-color LED module 3MM
- KY-031 Knock Sensor module
- KY-032 Obstacle avoidance sensor module
- KY-033 Hunt sensor module
- KY-034 Automatic flashing colorful LED module
- KY-035 Class Bihor magnetic sensor
- KY-036 Metal touch sensor module
- KY-037 Sensitive microphone sensor module
- KY-038 Microphone sound sensor module
- KY-039 Detect the heartbeat module
- KY-040 Rotary encoder module