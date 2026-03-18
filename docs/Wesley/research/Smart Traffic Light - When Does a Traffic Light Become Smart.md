# Smart Traffic Light: When Does a Traffic Light Become Smart?


## Table of Contents

1. [Introduction](#introduction)  
2. [Functional requirements](#1-functional-requirements)  
   2.1 [First iteration: basic traffic light](#11-first-iteration-basic-traffic-light)  
   2.2 [Second iteration: sensor-based vehicle detection](#12-second-iteration-sensor-based-vehicle-detection)  
   2.3 [Third iteration: reacting to pedestrians on the sidewalk](#13-third-iteration-reacting-to-pedestrians-on-the-sidewalk)  
   2.4 [Fourth iteration: advanced smart traffic light behavior](#14-fourth-iteration-advanced-smart-traffic-light-behavior)  
3. [Non-functional requirements](#2-non-functional-requirements)  
   3.1 [First iteration: basic traffic light](#21-first-iteration-basic-traffic-light)  
   3.2 [Second iteration: sensor-based vehicle detection](#22-second-iteration-sensor-based-vehicle-detection)  
   3.3 [Third iteration: reacting to pedestrians on the sidewalk](#23-third-iteration-reacting-to-pedestrians-on-the-sidewalk)  
   3.4 [Fourth iteration: advanced smart traffic light behavior](#24-fourth-iteration-advanced-smart-traffic-light-behavior)  
4. [Acceptance criteria](#3-acceptance-criteria)  
   4.1 [First iteration: basic traffic light](#31-first-iteration-basic-traffic-light)  
   4.2 [Second iteration: sensor-based vehicle detection](#32-second-iteration-sensor-based-vehicle-detection)  
   4.3 [Third iteration: reacting to pedestrians on the sidewalk](#33-third-iteration-reacting-to-pedestrians-on-the-sidewalk)  
   4.4 [Fourth iteration: advanced smart traffic light behavior](#34-fourth-iteration-advanced-smart-traffic-light-behavior)  
5. [Testing of acceptance criteria](#4-testing-of-acceptance-criteria)  
   5.1 [First iteration: basic traffic light](#41-first-iteration-basic-traffic-light)  
   5.2 [Second iteration: sensor-based vehicle detection](#42-second-iteration-sensor-based-vehicle-detection)  
   5.3 [Third iteration: reacting to pedestrians on the sidewalk](#43-third-iteration-reacting-to-pedestrians-on-the-sidewalk)  
   5.4 [Fourth iteration: advanced smart traffic light behavior](#44-fourth-iteration-advanced-smart-traffic-light-behavior)  
6. [Conclusion](#conclusion)  
7. [References](#references)  


## Introduction

For this project, I chose the traffic light challenge. The main question in this documentation is not only how to build a traffic light, but also when a traffic light can be considered smart.

A normal traffic light follows a fixed cycle and changes between red, yellow, and green based on pre-set timing. A smart traffic light does more than that. It reacts to real-time input from its environment, such as sensors, cameras, or other connected systems, and uses that information to adjust its behavior. This means that a traffic light becomes smart when it is no longer only time-based, but also responsive to what is happening around it (Zavadko, 2026).

For my project, I will build this up step by step. I will begin with a basic traffic light, then add vehicle detection through sensors, then make it react to pedestrians waiting on the sidewalk, and finally extend it with more advanced smart behavior. This makes the project manageable for a first hardware implementation while still showing clearly when the system starts becoming a smart traffic light (Zavadko, 2026).

The sections below focus on the most important parts of the documentation: functional requirements, non-functional requirements, acceptance criteria, and testing.

---

## 1. Functional requirements

### 1.1 First iteration: basic traffic light

This first iteration is the foundation. At this stage, the system is still a normal traffic light because it only follows a fixed sequence. I need this version first so I can safely understand the basic logic before adding smart behavior.

- **FR1.** The system shall control three LEDs that represent red, yellow, and green.
    
- **FR2.** The system shall switch between traffic light states in the order red, yellow, green, red.
    
- **FR3.** The system shall use fixed timing values for each state.
    
- **FR4.** The system shall start in a safe default state, which is red.
    
- **FR5.** The system shall prevent conflicting outputs, such as red and green being active at the same time.
    
- **FR6.** The system shall keep repeating the traffic light cycle until the board is reset or turned off.
    

### 1.2 Second iteration: sensor-based vehicle detection

A traffic light starts becoming smart when it reacts to real-time traffic conditions. Smart traffic lights can collect data from sensors and use that information to adjust signal timing. Vehicle counting and traffic flow detection are examples of this kind of smart behavior (Zavadko, 2026).

- **FR7.** The system shall detect the presence of vehicles using a sensor.
    
- **FR8.** The system shall use vehicle detection as input for traffic light decisions.
    
- **FR9.** The system shall adjust the waiting time or green time based on detected vehicle presence.
    
- **FR10.** The system shall only change timing within safe minimum and maximum limits.
    
- **FR11.** The system shall continue using a default cycle if no valid sensor input is available.
    
- **FR12.** The system shall process sensor input continuously while the traffic light is running.
    

### 1.3 Third iteration: reacting to pedestrians on the sidewalk

A smart traffic light should not only react to vehicles, but also to pedestrians. The source explains that connected cameras and smart traffic systems can detect pedestrian flow and support safer crossings. It also describes solutions where traffic lights can adjust signal timing when a pedestrian wants to cross (Zavadko, 2026). 

- **FR13.** The system shall detect or receive a request from a pedestrian waiting on the sidewalk.
    
- **FR14.** The system shall store the pedestrian request until it reaches a safe moment to respond.
    
- **FR15.** The system shall stop vehicle traffic safely before allowing a pedestrian crossing phase.
    
- **FR16.** The system shall provide a separate pedestrian crossing phase.
    
- **FR17.** The system shall ignore repeated pedestrian requests while one request is already active.
    
- **FR18.** The system shall return to the normal traffic cycle after the pedestrian phase is completed.
    

### 1.4 Fourth iteration: advanced smart traffic light behavior

More advanced smart traffic lights can use adaptive traffic signal control, emergency priority, and broader road-user logic. The source describes adaptive timing, emergency vehicle signal preemption, and support for pedestrians as important smart traffic light features (Zavadko, 2026).

- **FR19.** The system shall support adaptive timing based on traffic conditions.
    
- **FR20.** The system shall support an emergency priority mode.
    
- **FR21.** The system shall log traffic events and state changes for analysis.
    
- **FR22.** The software shall separate sensing, decision-making, and LED control into different parts.
    
- **FR23.** The system shall be extendable for future support of other road users.
    
- **FR24.** The system shall fall back to a safe default cycle when advanced smart input is unavailable.
    

---

## 2. Non-functional requirements

### 2.1 First iteration: basic traffic light

- **NFR1.** The prototype shall run on low-voltage hardware only.
    
- **NFR2.** The code shall be understandable for a first embedded systems project.
    
- **NFR3.** The traffic light timing shall be predictable and repeatable.
    
- **NFR4.** The LED state shall be clearly visible.
    
- **NFR5.** The system shall return to a safe state after reset.
    

### 2.2 Second iteration: sensor-based vehicle detection

Because smart traffic lights rely on real-time data, response time and reliability are important. The source also explains that processing traffic data at the edge reduces latency, which is useful for time-sensitive control decisions (Zavadko, 2026).

- **NFR6.** Sensor input shall be read reliably and consistently.
    
- **NFR7.** Vehicle detection shall be processed quickly enough to influence the next safe traffic phase.
    
- **NFR8.** The detection logic shall remain understandable and maintainable.
    
- **NFR9.** Safety shall have priority over shorter waiting times.
    
- **NFR10.** The sensor setup shall remain simple enough for a prototype.
    

### 2.3 Third iteration: reacting to pedestrians on the sidewalk

- **NFR11.** Pedestrian input shall be handled safely and without creating conflicting signals.
    
- **NFR12.** The waiting time for pedestrians shall remain reasonable.
    
- **NFR13.** The system shall not skip required safety transitions when responding to a pedestrian.
    
- **NFR14.** The pedestrian logic shall be easy to test and observe.
    
- **NFR15.** The system shall remain understandable while adding pedestrian behavior.
    

### 2.4 Fourth iteration: advanced smart traffic light behavior

- **NFR16.** The software shall be modular so that new smart features can be added later.
    
- **NFR17.** Adaptive decisions shall be made fast enough to remain useful in real time.
    
- **NFR18.** If an input source fails, the system shall continue in a safe fallback mode.
    
- **NFR19.** Logged data shall be readable and useful for analysis.
    
- **NFR20.** The project shall stay understandable as a learning project and not become unnecessarily complex.
    

---

## 3. Acceptance criteria

### 3.1 First iteration: basic traffic light

- **AC1.** When the system starts, the red LED turns on first.
    
- **AC2.** The system always follows the order red, yellow, green, red.
    
- **AC3.** No conflicting LED combination occurs during ten full cycles.
    
- **AC4.** Each state remains active for the configured duration.
    
- **AC5.** After reset, the system starts again in the red state.
    

### 3.2 Second iteration: sensor-based vehicle detection

- **AC6.** The sensor detects vehicle presence correctly.
    
- **AC7.** When a vehicle is detected, the traffic light timing changes according to the programmed rules.
    
- **AC8.** The system does not exceed the defined minimum or maximum phase durations.
    
- **AC9.** If no valid sensor value is available, the traffic light continues in the default cycle.
    
- **AC10.** Sensor input does not create unsafe signal transitions.
    

### 3.3 Third iteration: reacting to pedestrians on the sidewalk

- **AC11.** A pedestrian request is detected and stored correctly.
    
- **AC12.** The pedestrian phase starts only after a safe transition.
    
- **AC13.** During the pedestrian phase, vehicle green is never active.
    
- **AC14.** Repeated pedestrian requests do not create duplicate crossing phases.
    
- **AC15.** After the pedestrian phase, the system returns to the regular cycle.
    

### 3.4 Fourth iteration: advanced smart traffic light behavior

- **AC16.** Under simulated heavier traffic, the system adjusts the green phase within configured safety limits.
    
- **AC17.** When emergency mode is activated, the system reaches the priority state through a safe transition.
    
- **AC18.** Traffic events and state changes are logged correctly.
    
- **AC19.** If advanced smart input is removed, the system returns to the default traffic cycle.
    
- **AC20.** The software can be extended with one additional road-user scenario without breaking the current implementation.
    

---

## 4. Testing of acceptance criteria

### 4.1 First iteration: basic traffic light

- **Test AC1:** Power on the board and observe which LED turns on first. The test passes if the red LED is active first.
    
- **Test AC2:** Observe at least five complete cycles and write down the order of the states. The test passes if the order is always red, green, yellow, red.
    
- **Test AC3:** Run the traffic light for ten cycles and check for invalid LED combinations. The test passes if conflicting states never occur.
    
- **Test AC4:** Measure the duration of each state using a stopwatch or serial timestamps. The test passes if the measured time matches the configured value within a small tolerance.
    
- **Test AC5:** Reset the board multiple times. The test passes if the red LED is always the first active state after reset.
    

### 4.2 Second iteration: sensor-based vehicle detection

- **Test AC6:** Place a vehicle or simulate vehicle presence in front of the sensor. The test passes if detection is registered correctly.
    
- **Test AC7:** Compare traffic light behavior with and without detected vehicles. The test passes if timing changes according to the programmed rules.
    
- **Test AC8:** Measure the shortest and longest active phases during detection tests. The test passes if all phases remain within configured limits.
    
- **Test AC9:** Disconnect the sensor or simulate invalid input. The test passes if the system continues with the default cycle.
    
- **Test AC10:** Trigger detection during different phases. The test passes if the system still performs only safe transitions.
    

### 4.3 Third iteration: reacting to pedestrians on the sidewalk

- **Test AC11:** Trigger a pedestrian request from the sidewalk input. The test passes if the request is registered.
    
- **Test AC12:** Trigger the request during green and yellow. The test passes if the crossing phase only begins after a safe transition.
    
- **Test AC13:** Observe all LED outputs during the pedestrian phase. The test passes if vehicle green never appears while pedestrians are allowed to cross.
    
- **Test AC14:** Trigger the pedestrian input repeatedly in a short time. The test passes if the system handles it as one request.
    
- **Test AC15:** Observe the system after the pedestrian phase ends. The test passes if the normal cycle continues correctly.
    

### 4.4 Fourth iteration: advanced smart traffic light behavior

- **Test AC16:** Simulate low and high traffic conditions. The test passes if the green phase adjusts only within safe programmed limits.
    
- **Test AC17:** Activate emergency mode during different phases. The test passes if the system safely transitions to the priority state.
    
- **Test AC18:** Compare observed behavior with serial logging output. The test passes if phase changes and input events match the log.
    
- **Test AC19:** Disable advanced input features. The test passes if the system continues using the default cycle.
    
- **Test AC20:** Add one extra feature. The test passes if the original logic still works correctly.
    

---

## Conclusion

A traffic light becomes smart when it can react to real-time situations instead of only following a fixed timer. In my project, that smart behavior is introduced in steps. The first iteration is a normal traffic light. The second iteration makes the system react to vehicle detection. The third iteration adds response to pedestrians waiting on the sidewalk. The fourth iteration extends the system with more advanced smart traffic light behavior such as adaptive timing and priority logic. This structure helps me keep the project realistic, safe, and understandable while still showing clearly when the system can be called a smart traffic light (Zavadko, 2026).

## References

Zavadko, V. (2026, January 14). _How do smart traffic lights work? technical architecture and use cases explained_. Intellias. https://intellias.com/smart-traffic-signals/


