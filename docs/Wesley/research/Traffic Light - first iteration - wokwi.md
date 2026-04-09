# Wokwi Learning Log — First Hardware Simulation with ESP32-S3

## Summary

In this first iteration, I used Wokwi to apply what I had already researched during the analysis phase about Wokwi setup, ESP32-S3 hardware, and the requirements of the first traffic-light version. In practice, I built a simple traffic light simulation with an ESP32-S3, three LEDs, and three resistors, and connected the LEDs to GPIO 4, 5, and 6. This helped me move from theoretical preparation to an actual working simulation. The main learning was not what Wokwi is in general, because I had already documented that earlier, but how to use that knowledge in my own project by creating the circuit, compiling the firmware, linking the build output in `wokwi.toml`, and validating the first traffic-light behavior against the requirements of the basic iteration.    The build-and-simulate workflow also matches the official Wokwi VS Code project configuration and Arduino CLI compile behavior.

## Table of Contents

- [Overview](#overview)
- [Goal of the Wokwi Simulation](#goal-of-the-wokwi-simulation)
- [Project Setup](#project-setup)
  - [Build step](#build-step)
  - [`wokwi.toml`](#wokwitoml)
  - [`diagram.json`](#diagramjson)
  - [`.gitignore`](#gitignore)
- [What I Built](#what-i-built)
- [What I Learned](#what-i-learned)
  - [1. I learned how to apply my earlier Wokwi research in a real project](#1-i-learned-how-to-apply-my-earlier-wokwi-research-in-a-real-project)
  - [2. I learned that simulation depends on a complete workflow, not only on code](#2-i-learned-that-simulation-depends-on-a-complete-workflow-not-only-on-code)
  - [3. I learned how my code maps directly onto the requirements of the first iteration](#3-i-learned-how-my-code-maps-directly-onto-the-requirements-of-the-first-iteration)
  - [4. I learned how my ESP32-S3 research helped with practical pin use](#4-i-learned-how-my-esp32-s3-research-helped-with-practical-pin-use)
  - [5. I learned that simulation is useful as design validation](#5-i-learned-that-simulation-is-useful-as-design-validation)
  - [6. I learned that local tooling decisions also matter](#6-i-learned-that-local-tooling-decisions-also-matter)
- [Results and Evidence](#results-and-evidence)
  - [Evidence collected](#evidence-collected)
- [Reflection](#reflection)
- [Conclusion](#conclusion)
- [References](#references)
- [Appendix A — Proof of Realization in Wokwi](#appendix-a--proof-of-realization-in-wokwi)
  - [A.1 Visual proof of the simulated circuit](#a1-visual-proof-of-the-simulated-circuit)
  - [A.2 Source code proof](#a2-source-code-proof)
  - [A.3 Circuit configuration proof from `diagram.json`](#a3-circuit-configuration-proof-from-diagramjson)
  - [A.4 Build and simulation configuration proof](#a4-build-and-simulation-configuration-proof)
  - [A.5 Requirement-to-proof traceability table](#a5-requirement-to-proof-traceability-table)


---

## Overview

During the analysis phase, I had already documented how Wokwi works in VS Code, how `wokwi.toml` and `diagram.json` are used, and how an Arduino CLI workflow can be connected to simulation. I had also already documented the first iteration requirements of the traffic light and researched the ESP32-S3 board, its GPIO use, and practical wiring considerations. In this learning step, I applied that earlier knowledge in practice by creating my own first simulation and checking whether the traffic-light logic could run correctly on an ESP32-S3 setup.   

---

## Goal of the Wokwi Simulation

The goal of this simulation was to create the first working version of my traffic-light project in a safe digital environment before building it physically. Based on my requirements document, the first iteration had to control three LEDs, follow the order red → green → yellow → red, use fixed timing, start in red, prevent conflicting outputs, and keep repeating the cycle. The Wokwi simulation was therefore used as a first practical check of those requirements before moving to real hardware. 

---

## Project Setup

I used three project elements together:

* `traffic_light.ino` for the control logic
* `diagram.json` for the circuit layout
* `wokwi.toml` for linking the simulation to the compiled firmware

This setup follows the same workflow I had already documented in my Wokwi analysis file: Wokwi in VS Code uses `wokwi.toml` and `diagram.json`, and the firmware must be compiled first before the simulator can run correctly.  The same workflow is also described in the official Wokwi and Arduino CLI documentation.

### Build step

To generate the files used by `wokwi.toml`, I ran this command in the folder of the `.ino` file:

```bash
arduino-cli compile -v -b esp32:esp32:esp32s3 --output-dir ./build_wokwi
```

This was an important practical learning moment, because I had already researched that Wokwi for VS Code needs compiled firmware output, but now I actually used that workflow in my own project. The command produced the `.merged.bin` and `.elf` files that I then referenced in `wokwi.toml`.  This matches the documented meaning of `arduino-cli compile` with a board FQBN and `--output-dir`. 

### `wokwi.toml`

In my project, the simulation configuration was:

```toml
[wokwi]
version = 1
firmware = 'embedded/Wesley/traffic_light/build_wokwi/traffic_light.ino.merged.bin'
elf = 'embedded/Wesley/traffic_light/build_wokwi/traffic_light.ino.elf'
```

This reflects the same type of configuration I had already described in my analysis document: `wokwi.toml` points to the compiled firmware and ELF output so the simulator knows what to run. By using it in practice, I learned how the theoretical setup becomes part of the real project workflow. 

### `diagram.json`

For the circuit itself, I added:

* one red LED
* one yellow LED
* one green LED
* one resistor for each LED
* connections from the LEDs to GPIO 4, 5, and 6
* connections from the resistors to GND

This was the first time I used Wokwi to create and connect my own traffic-light circuit for an ESP32-S3. My earlier Wokwi analysis already documented that `diagram.json` defines the hardware layout, and here I applied that by making my own simple traffic-light wiring. 

### `.gitignore`

To prevent my local simulation files and build artifacts from cluttering the shared repository, I added the following:

```gitignore
wokwi.toml
diagram.json

/*/Wesley/*/build/*
/*/Wesley/*/build_wokwi/
```

This was a practical project-organization decision. The simulation setup and build output were useful for my own workflow, but they were not intended to flood the shared learning repository. That made me more aware that simulation is not only about coding and wiring, but also about organizing project files properly. Git’s ignore mechanism is intended for intentionally untracked files such as generated build output. 

---

## What I Built

I built a first simulated traffic light with this GPIO mapping:

| Component  | GPIO pin |
| ---------- | -------: |
| Red LED    |        4 |
| Yellow LED |        5 |
| Green LED  |        6 |

Each LED had its own resistor and was connected to ground through that resistor. The ESP32-S3 board controlled the LEDs one at a time. This matched the first-iteration traffic-light concept from my requirements document, where the first version is a normal fixed-cycle traffic light with three outputs and no smart behavior yet. 

I also created the first working control code:

* `allOff()`
* `redOn()`
* `yellowOn()`
* `greenOn()`

These functions made the code easier to read and made the traffic-light sequence clearer.

---

## What I Learned

### 1. I learned how to apply my earlier Wokwi research in a real project

Before this step, I had already documented how Wokwi for VS Code works. In this learning activity, I moved from analysis to implementation. That meant I no longer only understood the required files conceptually, but also used them in a working project with my own circuit and build output. 

### 2. I learned that simulation depends on a complete workflow, not only on code

This step taught me that the simulation only works when several elements fit together correctly:

* the code must compile
* the firmware paths in `wokwi.toml` must be correct
* the circuit in `diagram.json` must match the GPIO pins used in the code

So the learning here was that hardware simulation is not only visual. It is a combination of configuration, compilation, circuit design, and code. 

### 3. I learned how my code maps directly onto the requirements of the first iteration

My earlier traffic-light requirements document defined that the first version had to use three LEDs, fixed timing, a fixed order, safe startup in red, and no conflicting outputs. By simulating my code in Wokwi, I could see that the program structure directly reflected those requirements. The red, green, and yellow functions represented the states, the delays represented the configured timing, and the repeated loop represented the continuous cycle. 

### 4. I learned how my ESP32-S3 research helped with practical pin use

In my ESP32-S3 research, I had already documented that the ESP32-S3 has flexible GPIO routing, but also that some pins should be handled carefully because of flash/PSRAM use or startup behavior. That earlier research helped me understand why pin choice matters even in a basic LED project, and it gave context for using ordinary GPIO pins for the traffic-light outputs. 

### 5. I learned that simulation is useful as design validation

The Wokwi simulation confirmed that the first traffic-light design works as intended before physical assembly. This is important because my requirements document already included acceptance criteria such as red starting first, the correct state order, and the absence of conflicting outputs. The simulation gave me early evidence for these criteria and therefore supported both my design and realization learning outcomes. 

### 6. I learned that local tooling decisions also matter

By building the firmware in a separate `build_wokwi` folder and ignoring those files in Git, I learned that embedded development is also about project hygiene. This was not a theory point from the traffic-light logic itself, but a practical workflow lesson from doing the simulation locally.

---

## Results and Evidence

The results of this learning step are:

* a working ESP32-S3 traffic-light simulation
* a configured `wokwi.toml` file linked to the compiled output
* a `diagram.json` file with the LED and resistor circuit
* a working `traffic_light.ino` program
* a successful local build process using Arduino CLI
* a cleaner local workflow through `.gitignore` rules

### Evidence collected

| Evidence                                             | What it proves                                                         |
| ---------------------------------------------------- | ---------------------------------------------------------------------- |
| `traffic_light.ino`                                  | I implemented the first traffic-light sequence in code                 |
| `diagram.json`                                       | I created the simulated LED circuit                                    |
| `wokwi.toml`                                         | I linked the simulator to the compiled firmware                        |
| `arduino-cli compile ... --output-dir ./build_wokwi` | I generated the required simulation files                              |
| `.gitignore` rules                                   | I managed local simulation files separately from the shared repository |
| Wokwi screenshot                                     | I visually completed and ran the simulated circuit                     |

This evidence is also meaningful in relation to the earlier requirements and testing plan for the first iteration, especially for startup in red, fixed order, repeatable timing, and checking for conflicting states. 

---

## Reflection

This learning step was useful because it connected three things I had already researched separately: the Wokwi workflow, the ESP32-S3 platform, and the first traffic-light requirements. During the analysis phase, these were still separate documents. By actually building and running the simulation, I learned how they come together in practice.

The most important reflection is that I did not mainly learn “what Wokwi is,” because I had already documented that before. What I learned here was how to use that prior knowledge in my own workflow: compile the project, connect the build files, wire the simulated hardware, and check whether the implementation matches the first iteration requirements. That makes this step less about tool introduction and more about practical application and validation.   

---

## Conclusion

This first Wokwi implementation step showed that I can take earlier research from the analysis phase and apply it in a working embedded simulation. I used my previous Wokwi documentation to set up the workflow, my ESP32-S3 research to understand the board and GPIO use, and my traffic-light requirements to guide what the first iteration had to do. The result was a working ESP32-S3 traffic-light simulation that cycles through red, green, and yellow and gives me practical evidence for the first iteration of the project.   

---

## References

_Wesley. *Wokwi for VS Code, Setup and Configuration for ESP32 Projects.* Analysis document used earlier in the project. 

_Wesley. *ESP32-S3 Development Board – Pinout, Interfaces, and Practical Board Comparison.* Analysis document used earlier in the project. 

_Wesley. *Smart Traffic Light – When Does a Traffic Light Become Smart?* Requirements and project-scope document used earlier in the project. 

_Wesley. *Learning Outcome - Analysis.* 

_Arduino CLI | Arduino documentation. (n.d.). https://docs.arduino.cc/arduino-cli/

_Getting started with Wokwi for VS Code. (n.d.). Wokwi. https://docs.wokwi.com/vscode/getting-started

_Configuring your project (wokwi.toml). (n.d.). Wokwi. https://docs.wokwi.com/vscode/project-config

_Diagram.json file format. (n.d.). Wokwi. https://docs.wokwi.com/diagram-format







## Appendix A — Proof of Realization in Wokwi

This appendix contains the direct proof for my first Wokwi realization of the traffic light project. It supports the first iteration requirements I defined earlier: three outputs, fixed order, fixed timing, safe startup in red, no conflicting outputs, and continuous repetition. These requirements were already established in my earlier analysis and traffic-light requirement documents.  

### A.1 Visual proof of the simulated circuit

**Figure A1. First Wokwi traffic light circuit**
[!Open Figure A1](../assets/Traffic%20Light%20-%20first%20iteration%20-%20wokwi/wokwi%20traffic%20light%20-%20first%20iteration.png)

Figure A1 shows my first traffic light circuit in Wokwi. The circuit contains one ESP32-S3 board, one red LED, one yellow LED, one green LED, and one resistor for each LED. The wiring shown in the simulation matches the intended first-iteration design.

From the circuit, the wiring can be observed as:

* GPIO 4 → red LED → resistor → GND
* GPIO 5 → yellow LED → resistor → GND
* GPIO 6 → green LED → resistor → GND

This proves that I created a three-output traffic light circuit that matches the first iteration scope. It also matches the ESP32-S3 research I documented earlier, where I reviewed the board and its GPIO use for practical embedded development. 

---

### A.2 Source code proof

The following code was used in the first Wokwi simulation:

```cpp
#include <Arduino.h>

const int RED_PIN = 4;
const int YELLOW_PIN = 5;
const int GREEN_PIN = 6;

void allOff() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void redOn() {
  allOff();
  digitalWrite(RED_PIN, HIGH);
  Serial.println("RED");
  delay(5000);
}

void yellowOn() {
  allOff();
  digitalWrite(YELLOW_PIN, HIGH);
  Serial.println("YELLOW");
  delay(2000);
}

void greenOn() {
  allOff();
  digitalWrite(GREEN_PIN, HIGH);
  Serial.println("GREEN");
  delay(5000);
}

void setup() {
  Serial.begin(115200);
  
  //register pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // Safe startup: red first
  redOn();
}

void loop() {
  redOn();
  greenOn();
  yellowOn();
}
```

This code proves several important points:

* I defined three separate output pins for red, yellow, and green.
* I implemented the fixed order `red → green → yellow → red`.
* I configured fixed timing with 5000 ms for red, 5000 ms for green, and 2000 ms for yellow.
* I made the startup state safe by activating red first in `setup()`.
* I prevented conflicting outputs by calling `allOff()` before activating a new light.
* I created continuous repetition through the `loop()` function.

This code therefore gives direct proof for the first iteration behavior that I described earlier in my requirements and analysis documents.  

---

### A.3 Circuit configuration proof from `diagram.json`

The Wokwi circuit was defined in `diagram.json` as follows:

```json
{
  "version": 1,
  "author": "Your Name",
  "editor": "wokwi",
  "parts": [
    { "type": "board-esp32-s3-devkitc-1", "id": "esp", "top": 0, "left": 0, "attrs": {} },
    {
      "type": "wokwi-led",
      "id": "ledRed",
      "top": -109.2,
      "left": -127.2,
      "attrs": { "color": "red" }
    },
    {
      "type": "wokwi-led",
      "id": "ledYellow",
      "top": -49.2,
      "left": -127.2,
      "attrs": { "color": "yellow" }
    },
    {
      "type": "wokwi-led",
      "id": "ledGreen",
      "top": 6,
      "left": -130.6,
      "attrs": { "color": "green" }
    },
    {
      "type": "wokwi-resistor",
      "id": "rRed",
      "top": -69.6,
      "left": -217.8,
      "rotate": 180,
      "attrs": { "value": "220" }
    },
    {
      "type": "wokwi-resistor",
      "id": "rYellow",
      "top": -2.4,
      "left": -217.8,
      "rotate": 180,
      "attrs": { "value": "220" }
    },
    {
      "type": "wokwi-resistor",
      "id": "rGreen",
      "top": 54.65,
      "left": -218.65,
      "rotate": 180,
      "attrs": { "value": "220" }
    }
  ],
  "connections": [
    [ "esp:TX", "$serialMonitor:RX", "", [] ],
    [ "esp:RX", "$serialMonitor:TX", "", [] ],
    [ "esp:4", "ledRed:A", "red", [ "h-53.08", "v-124.98" ] ],
    [ "ledRed:C", "rRed:1", "black", [] ],
    [ "rRed:2", "esp:GND.1", "black", [ "h-12.8", "v294.83" ] ],
    [ "esp:5", "ledYellow:A", "gold", [ "h-81.88", "v-74.58" ] ],
    [ "ledYellow:C", "rYellow:1", "black", [] ],
    [ "rYellow:2", "esp:GND.1", "black", [ "h-12.8", "v227.63" ] ],
    [ "esp:6", "ledGreen:A", "green", [] ],
    [ "ledGreen:C", "rGreen:1", "black", [] ],
    [ "rGreen:2", "esp:GND.1", "black", [ "h-11.95", "v170.58" ] ]
  ],
  "dependencies": {}
}
```

This file proves that the simulated hardware matches the code and the design:

* the board is an ESP32-S3,
* there are exactly three LEDs,
* there are exactly three resistors,
* GPIO 4 is connected to the red LED,
* GPIO 5 is connected to the yellow LED,
* GPIO 6 is connected to the green LED,
* each LED returns to ground through its own resistor.

This is important because it shows that the software pin mapping and the simulated wiring correspond exactly. In my earlier Wokwi analysis, I already described `diagram.json` as the file that defines the simulated hardware layout, and this project is the practical proof of that. 

---

### A.4 Build and simulation configuration proof

To generate the firmware files for Wokwi, I used the following command in the folder of the `.ino` file:

```bash
arduino-cli compile -v -b esp32:esp32:esp32s3 --output-dir ./build_wokwi
```

The Wokwi simulation used this configuration:

```toml
[wokwi]
version = 1
firmware = 'embedded/Wesley/traffic_light/build_wokwi/traffic_light.ino.merged.bin'
elf = 'embedded/Wesley/traffic_light/build_wokwi/traffic_light.ino.elf'
```

This proves that I did not only draw a circuit, but also completed the build-and-simulate workflow:

* I compiled the Arduino project for the ESP32-S3 board,
* I generated the `.merged.bin` and `.elf` files,
* I linked those files in `wokwi.toml`,
* I then used those compiled files for simulation.

This matches the workflow I had already researched earlier during the analysis phase. 

---

### A.5 Requirement-to-proof traceability table

| Requirement / criterion     | Proof in code                                    | Proof in circuit / setup                                  | Status         |
| --------------------------- | ------------------------------------------------ | --------------------------------------------------------- | -------------- |
| Three outputs are used      | `RED_PIN`, `YELLOW_PIN`, `GREEN_PIN` are defined | Three LEDs are present in Figure A1 and in `diagram.json` | Proven         |
| Fixed order is implemented  | `loop()` uses `redOn(); greenOn(); yellowOn();`  | Simulation is wired for the same three outputs            | Proven         |
| Fixed timing is implemented | `delay(5000)`, `delay(5000)`, `delay(2000)`      | Timing is configured in the program                       | Proven in code |
| Safe startup in red         | `setup()` starts with `redOn();`                 | Startup behavior is explicitly designed in code           | Proven         |
| No conflicting outputs      | `allOff()` is called before every state change   | Only one LED path is activated at a time in code          | Proven in code |
| Repetition is continuous    | Sequence is placed inside `loop()`               | Simulation keeps cycling through the states               | Proven         |

This table shows the connection between my earlier requirements and the actual implementation proof. Instead of only showing files, it shows how those files demonstrate that the first iteration behaves as intended.  

---






