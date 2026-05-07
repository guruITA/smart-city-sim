# Realisation — Breadboard Realisation of the Four-Way Traffic-Light Setup

## Table of Contents

- [1. Introduction](#1-introduction)
- [2. What Is Already Handled in the Previous Deliverable](#2-what-is-already-handled-in-the-previous-deliverable)
- [3. Goal of the Realisation](#3-goal-of-the-realisation)
- [4. Realised Breadboard Setup](#4-realised-breadboard-setup)
- [5. Functional Structure of the Setup](#5-functional-structure-of-the-setup)
- [6. Pin and Channel Mapping Used in the Realisation](#6-pin-and-channel-mapping-used-in-the-realisation)
  - [6.1 ESP32-S3 to MCP23017](#61-esp32-s3-to-mcp23017)
  - [6.2 MCP23017 to traffic lights](#62-mcp23017-to-traffic-lights)
- [7. Why I Used `millis()` Instead of `delay()`](#7-why-i-used-millis-instead-of-delay)
- [8. Breadboard Build Process](#8-breadboard-build-process)
- [9. Testing of the Breadboard Realisation](#9-testing-of-the-breadboard-realisation)
  - [Test 1 — Safe startup state](#test-1--safe-startup-state)
  - [Test 2 — Correct traffic-light sequence](#test-2--correct-traffic-light-sequence)
  - [Test 3 — No conflicting green states](#test-3--no-conflicting-green-states)
  - [Test 4 — `millis()` based timing works correctly](#test-4--millis-based-timing-works-correctly)
  - [Test 5 — Stable repeated cycling](#test-5--stable-repeated-cycling)
- [10. Reflection](#10-reflection)
- [11. Conclusion](#11-conclusion)
- [References](#references)
- [Appendix A — Real-Life Realisation Photos](#appendix-a--real-life-realisation-photos)
  - [Figure A1 — Complete breadboard overview](#figure-a1--complete-breadboard-overview)
  - [Figure A2 — ESP32-S3 and MCP23017 side](#figure-a2--esp32-s3-and-mcp23017-side)
  - [Figure A3 — LED channel side](#figure-a3--led-channel-side)
  - [Figure A4 — External power and common ground connection](#figure-a4--external-power-and-common-ground-connection)
- [Appendix B — Final Breadboard Code Used in the Realisation](#appendix-b--final-breadboard-code-used-in-the-realisation)



## 1. Introduction

In this document, I describe how I realised the four-way traffic-light setup on a breadboard based on the earlier analysis and design work. In this phase, I built and tested the complete setup on the breadboard. The transfer to the project tile is not part of this deliverable yet.

This deliverable was written after ***Realisation - Developing and Using Tests During Realisation for MCP23017 and ULN2803 Integration***. That earlier deliverable focused on testing and using the **MCP23017** and **ULN2803** during the realisation phase. Because of that, this document does not repeat all those detailed staged tests again. Instead, this deliverable focuses on how I expanded that tested foundation into the complete four-way breadboard system and how the final full setup behaved in practice.

The goal of this realisation was to verify that the designed hardware and software also work together in the complete four-way configuration. For that reason, I built the circuit with the ESP32-S3, one MCP23017, two ULN2803 chips, and the LED channels for the four traffic lights. After that, I tested whether the system starts safely, whether the traffic-light sequence works correctly, whether conflicting green states are prevented, whether the `millis()` based timing behaves correctly, and whether the system can cycle stably over time.

## 2. What Is Already Handled in the Previous Deliverable

The deliverable ***Realisation - Developing and Using Tests During Realisation for MCP23017 and ULN2803 Integration*** already covers the staged component-level and integration-level testing that I used during building.

That earlier deliverable already documents:

* serial output testing
* I2C communication testing with the MCP23017
* standalone testing of the MCP23017 outputs
* standalone testing of the ULN2803 switching behaviour
* combined testing of the signal path **ESP32-S3 → MCP23017 → ULN2803 → LED**

Because those parts are already documented there, I do not repeat those detailed tests again in this deliverable. In this document, I focus on the complete four-way breadboard setup, the final mapping that was realised, the staged expansion from tested building blocks to the final system, and the behaviour of the full setup after assembly.

## 3. Goal of the Realisation

The goal of this realisation was to build the complete four-way traffic-light controller on a breadboard and verify that the full setup works safely and correctly in practice.

This included checking that:

* the complete setup starts in a safe state
* the traffic-light phases follow the intended order
* conflicting green states do not occur
* the timing works correctly with `millis()`
* the system can keep cycling stably over time
* the breadboard version is suitable as a base for later expansion with sensors or backend communication

## 4. Realised Breadboard Setup

I built the setup on a breadboard according to the earlier design. The ESP32-S3 is used as the controller. The MCP23017 is connected over I2C and is powered from the ESP32-S3 3.3V logic supply. SDA and SCL each use a **5.1 kΩ pull-up resistor** to 3.3V. That was suitable for this short breadboard setup and keeps the I2C lines correctly pulled high.

The MCP23017 address pins **A0, A1, and A2** are connected to ground, so the chip uses I2C address **0x20**. The **RESET** pin is connected with a pull-up resistor to 3.3V.

The outputs of the MCP23017 are connected to two ULN2803 chips. These ULN2803 chips are used as **sink drivers**, which means they do not provide power to the LEDs themselves. Instead, they pull the LED channels to ground when activated. The LEDs are powered from a separate external supply. The negative side of that external supply is connected to the same common ground as the ESP32-S3, the MCP23017, and both ULN2803 chips. This shared ground was necessary so that all control signals and switched outputs use the same reference.

## 5. Functional Structure of the Setup

The realised setup works in four layers.

First, the **ESP32-S3** runs the traffic-light logic.
Second, the **ESP32-S3** sends output commands over I2C to the **MCP23017**.
Third, the **MCP23017** sets the required output pins high or low.
Fourth, the **ULN2803** chips switch the actual LED channels on the breadboard.

This means the ESP32-S3 does not directly drive the twelve LED channels. Instead, the ESP32-S3 controls the logic side, the MCP23017 expands the outputs, and the ULN2803 chips handle the switching side. This matches the earlier design choice to keep the controller side simple and to avoid placing the LED load current directly on the ESP32-S3.

## 6. Pin and Channel Mapping Used in the Realisation

### 6.1 ESP32-S3 to MCP23017

| Function | Connection                                                        |
| -------- | ----------------------------------------------------------------- |
| SDA      | GPIO 5, with 5.1 kΩ pull-up to 3.3V                               |
| SCL      | GPIO 4, with 5.1 kΩ pull-up to 3.3V                               |
| VDD      | 3.3V                                                              |
| VSS      | GND                                                               |
| A0       | GND                                                               |
| A1       | GND                                                               |
| A2       | GND                                                               |
| RESET    | 3.3V through pull-up resistor                                     |


## 6.2 MCP23017 to traffic lights

The final realised output mapping was:

| Traffic light   | Red  | Yellow | Green |
| --------------- | ---- | ------ | ----- |
| Traffic Light 1 | GPB0 | GPB1   | GPB2  |
| Traffic Light 2 | GPB3 | GPB4   | GPB5  |
| Traffic Light 3 | GPA5 | GPA6   | GPA4  |
| Traffic Light 4 | GPA2 | GPA3   | GPA1  |

The ULN2803 inputs are connected to these MCP23017 outputs, and the matching ULN2803 outputs are connected to the cathode side of the LED channels.

## 7. Why I Used `millis()` Instead of `delay()`

In this realisation, I changed the control code to a `millis()` based state machine instead of a `delay()` based sequence.

I did this because `delay()` blocks the microcontroller while it is waiting. That would become a problem if later I combine this traffic-light code with other code on the same ESP32-S3, such as sensors, timing logic, backend commands, or communication with other modules. In that situation, `delay()` could interfere with the rest of the system because the processor would not be free to check other inputs while waiting for the next traffic-light step.

By using `millis()`, the program keeps running continuously and only changes state when enough time has passed. This makes the realised breadboard version more suitable for later integration with other project parts.

## 8. Breadboard Build Process

I did not build the complete four-way setup at once. Instead, I first followed the staged testing approach documented in ***Realisation - Developing and Using Tests During Realisation for MCP23017 and ULN2803 Integration***. That allowed me to verify the most important parts before expanding the system to the final breadboard version.

The build process was therefore divided into stages.

First, I verified that serial output worked on the ESP32-S3, so I could use the serial monitor during the rest of the realisation.
After that, I built and tested the I2C connection between the ESP32-S3 and the MCP23017. This allowed me to confirm that the MCP23017 was detected correctly and that the SDA, SCL, address, power, ground, and RESET connections were correct.
Next, I tested the MCP23017 separately by controlling an LED from one of its outputs.
After that, I tested the ULN2803 separately by driving it directly from the ESP32-S3 and checking whether it switched an LED correctly.
Then, I combined the MCP23017 and ULN2803 into one tested signal chain. This confirmed that the path **ESP32-S3 → MCP23017 → ULN2803 → LED** worked correctly before I expanded it further.

Only after those stages worked correctly did I continue with the full breadboard implementation. At that point, I placed the final MCP23017 and both ULN2803 chips in the breadboard layout, connected the logic power and ground lines, added the I2C pull-up resistors, connected the final output mapping for all four traffic lights, added one resistor per LED channel, and connected the external LED power supply with shared ground.

This staged order made the realisation process more controlled. It reduced the risk of debugging the entire system at once and made it easier to isolate problems before moving to the complete four-way setup.

## 9. Testing of the Breadboard Realisation

After building the full setup, I tested whether the complete system behaved as intended.

### Test 1 — Safe startup state

The first test was whether the complete system starts in a safe state after power-up or reset.

**Expected result**
The system must begin in an all-red state.

**Observed result**
After power-up and after reset, the breadboard setup entered the all-red phase first. This means all four traffic lights started in the safe stop condition before any green phase became active.

**Conclusion**
Passed. The full system starts in a safe state.

### Test 2 — Correct traffic-light sequence

The second test was whether the complete setup followed the intended phase order.

**Expected result**
The sequence should alternate safely between the two road directions, with red and yellow transitions in the correct order.

**Observed result**
During testing, the breadboard followed this sequence:

1. all red
2. road 1 and 2 green while road 3 and 4 stayed red
3. road 1 and 2 yellow while road 3 and 4 stayed red
4. all red
5. road 3 and 4 green while road 1 and 2 stayed red
6. road 3 and 4 yellow while road 1 and 2 stayed red
7. repeat

The transitions were stable and followed the intended order.

**Conclusion**
Passed. The realised setup followed the correct traffic-light sequence.

### Test 3 — No conflicting green states

The third test was whether different road directions could accidentally become green at the same time.

**Expected result**
Only one road direction, together with the opposite light on that same direction, may have green at a time.

**Observed result**
During repeated observation, roads 1 and 2 were green together only while roads 3 and 4 remained red. Roads 3 and 4 were green together only while roads 1 and 2 remained red. No moment was observed in which both directions showed green at the same time.

**Conclusion**
Passed. No conflicting green states occurred.

### Test 4 — `millis()` based timing works correctly

The fourth test was whether the traffic-light timing remained correct while using a non-blocking implementation.

**Expected result**
The traffic lights should still change state at the intended intervals without using `delay()`.

**Observed result**
The visible phase durations matched the configured timing values, and the phase changes occurred correctly while the control logic remained based on `millis()`. This means the realised system kept the intended traffic-light timing while also using a structure that is better suited for later expansion.

**Conclusion**
Passed. The timing worked correctly in the non-blocking implementation.

### Test 5 — Stable repeated cycling

The fifth test was whether the breadboard version could keep cycling without unstable behaviour.

**Expected result**
The sequence should continue repeating without unexpected output changes.

**Observed result**
The complete setup continued cycling through the programmed phases over repeated runs without unexpected LED combinations, freezes, or unstable channel behaviour.

**Conclusion**
Passed. The breadboard version cycled stably.

## 10. Reflection

This realisation phase was useful because it showed whether the design also works outside simulation and documentation. During the design phase, the structure was already clear, but in the realisation phase it became visible whether the different hardware parts also work together physically on a breadboard.

The most important result of this phase is that the traffic-light system can be built in a modular way. The ESP32-S3 only needs two I2C lines to control the MCP23017, the MCP23017 provides the expanded outputs, and the ULN2803 chips handle the switching for the LED channels. That makes the setup easier to expand later than a direct one-pin-per-LED design.

Another important result is the switch to `millis()`. This improves the quality of the software design because it reduces the risk that this code will block future code that runs on the same ESP32-S3. That is important for later stages of the project, where sensors or backend communication may be added.

A final reflection is that ***Realisation - Developing and Using Tests During Realisation for MCP23017 and ULN2803 Integration*** and this deliverable now complement each other well. The earlier deliverable shows how I tested the building blocks during the realisation process. This deliverable shows how I expanded those tested building blocks into the full four-way breadboard setup and verified that the complete system also works correctly.

## 11. Conclusion

In this realisation, I successfully built the designed four-way traffic-light setup on a breadboard with an ESP32-S3, one MCP23017, and two ULN2803 chips. The build follows the design and analysis decisions that were made earlier. The LED channels are powered through an external supply, while the logic side is powered from the ESP32-S3. The use of a common ground ensures that the switching logic works correctly.

This deliverable focuses on the complete breadboard system and the behaviour of the full setup. The detailed component-level and integration-level testing of the MCP23017 and ULN2803 is documented separately in ***Realisation - Developing and Using Tests During Realisation for MCP23017 and ULN2803 Integration***.

The software was also improved in this phase by replacing `delay()` with a `millis()` based approach. Because of that, the realised breadboard version is better prepared for future integration with other code and later project expansion.

At this stage, the realisation covers the breadboard build and the first functional tests of the complete setup. Transfer to the project tile is not part of this document yet.

## References

*Wesley. *Realisation - Developing and Using Tests During Realisation for MCP23017 and ULN2803 Integration.**

*Wesley. *Analysis - Expanding the Traffic-Light Prototype to a Four-Way Crossing.**

*Wesley. *Design - Four-Way Traffic-Light Crossing with Low Pin Usage and External LED Power.**

*Wesley. *Smart Traffic Light - When Does a Traffic Light Become Smart.**

## Appendix A — Real-Life Realisation Photos

### Figure A1 — Complete breadboard overview


### Figure A2 — ESP32-S3 and MCP23017 side



### Figure A3 — LED channel side



### Figure A4 — External power and common ground connection



## Appendix B — Final Breadboard Code Used in the Realisation

```cpp
#include <Wire.h>

/*
  Traffic light controller
  ESP32-S3 -> MCP23017 -> 2x ULN2803 -> 4 traffic lights

  Non-blocking version using millis()
*/

// ============================================================
// I2C settings
// ============================================================

const int I2C_SDA_PIN = 5;
const int I2C_SCL_PIN = 4;

const uint8_t MCP_ADDR = 0x20;

// ============================================================
// MCP23017 registers
// ============================================================

const uint8_t IODIRA = 0x00;
const uint8_t IODIRB = 0x01;
const uint8_t GPIOA  = 0x12;
const uint8_t GPIOB  = 0x13;

// ============================================================
// Output mapping
// ============================================================

// Port B -> bottom ULN2803
const uint8_t TL1_RED_B    = 0;
const uint8_t TL1_YELLOW_B = 1;
const uint8_t TL1_GREEN_B  = 2;
const uint8_t TL2_RED_B    = 3;
const uint8_t TL2_YELLOW_B = 4;
const uint8_t TL2_GREEN_B  = 5;

// Port A -> top ULN2803
const uint8_t TL3_YELLOW_A = 6;
const uint8_t TL3_RED_A    = 5;
const uint8_t TL3_GREEN_A  = 4;
const uint8_t TL4_YELLOW_A = 3;
const uint8_t TL4_RED_A    = 2;
const uint8_t TL4_GREEN_A  = 1;

// ============================================================
// Timing values in milliseconds
// ============================================================

const unsigned long ALL_RED_TIME = 1000;
const unsigned long GREEN_TIME   = 5000;
const unsigned long YELLOW_TIME  = 2000;

// ============================================================
// Phase constants
// ============================================================

const uint8_t PHASE_ALL_RED_1     = 0;
const uint8_t PHASE_ROAD12_GREEN  = 1;
const uint8_t PHASE_ROAD12_YELLOW = 2;
const uint8_t PHASE_ALL_RED_2     = 3;
const uint8_t PHASE_ROAD34_GREEN  = 4;
const uint8_t PHASE_ROAD34_YELLOW = 5;

// ============================================================
// State variables
// ============================================================

uint8_t currentPhase = PHASE_ALL_RED_1;
unsigned long lastPhaseChange = 0;

// ============================================================
// Helper functions
// ============================================================

uint8_t bitMask(uint8_t bit) {
  return (1 << bit);
}

void mcpWriteRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void mcpWriteBoth(uint8_t portAValue, uint8_t portBValue) {
  mcpWriteRegister(GPIOA, portAValue);
  mcpWriteRegister(GPIOB, portBValue);
}

void mcpInit() {
  mcpWriteRegister(IODIRA, 0x00);
  mcpWriteRegister(IODIRB, 0x00);
  mcpWriteBoth(0x00, 0x00);
}

// ============================================================
// Phase output functions
// ============================================================

void allRed() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_RED_B);
  b |= bitMask(TL2_RED_B);

  a |= bitMask(TL3_RED_A);
  a |= bitMask(TL4_RED_A);

  mcpWriteBoth(a, b);
}

void road12Green_road34Red() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_GREEN_B);
  b |= bitMask(TL2_GREEN_B);

  a |= bitMask(TL3_RED_A);
  a |= bitMask(TL4_RED_A);

  mcpWriteBoth(a, b);
}

void road12Yellow_road34Red() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_YELLOW_B);
  b |= bitMask(TL2_YELLOW_B);

  a |= bitMask(TL3_RED_A);
  a |= bitMask(TL4_RED_A);

  mcpWriteBoth(a, b);
}

void road12Red_road34Green() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_RED_B);
  b |= bitMask(TL2_RED_B);

  a |= bitMask(TL3_GREEN_A);
  a |= bitMask(TL4_GREEN_A);

  mcpWriteBoth(a, b);
}

void road12Red_road34Yellow() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_RED_B);
  b |= bitMask(TL2_RED_B);

  a |= bitMask(TL3_YELLOW_A);
  a |= bitMask(TL4_YELLOW_A);

  mcpWriteBoth(a, b);
}

// ============================================================
// State machine functions
// ============================================================

void applyPhase(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
      allRed();
      Serial.println("Phase: All Red");
      break;

    case PHASE_ROAD12_GREEN:
      road12Green_road34Red();
      Serial.println("Phase: Road 1+2 Green | Road 3+4 Red");
      break;

    case PHASE_ROAD12_YELLOW:
      road12Yellow_road34Red();
      Serial.println("Phase: Road 1+2 Yellow | Road 3+4 Red");
      break;

    case PHASE_ALL_RED_2:
      allRed();
      Serial.println("Phase: All Red");
      break;

    case PHASE_ROAD34_GREEN:
      road12Red_road34Green();
      Serial.println("Phase: Road 1+2 Red | Road 3+4 Green");
      break;

    case PHASE_ROAD34_YELLOW:
      road12Red_road34Yellow();
      Serial.println("Phase: Road 1+2 Red | Road 3+4 Yellow");
      break;
  }
}

unsigned long getPhaseDuration(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
    case PHASE_ALL_RED_2:
      return ALL_RED_TIME;

    case PHASE_ROAD12_GREEN:
    case PHASE_ROAD34_GREEN:
      return GREEN_TIME;

    case PHASE_ROAD12_YELLOW:
    case PHASE_ROAD34_YELLOW:
      return YELLOW_TIME;

    default:
      return ALL_RED_TIME;
  }
}

uint8_t getNextPhase(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
      return PHASE_ROAD12_GREEN;

    case PHASE_ROAD12_GREEN:
      return PHASE_ROAD12_YELLOW;

    case PHASE_ROAD12_YELLOW:
      return PHASE_ALL_RED_2;

    case PHASE_ALL_RED_2:
      return PHASE_ROAD34_GREEN;

    case PHASE_ROAD34_GREEN:
      return PHASE_ROAD34_YELLOW;

    case PHASE_ROAD34_YELLOW:
      return PHASE_ALL_RED_1;

    default:
      return PHASE_ALL_RED_1;
  }
}

// ============================================================
// Setup
// ============================================================

void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  mcpInit();

  applyPhase(currentPhase);
  lastPhaseChange = millis();

  Serial.println("Breadboard traffic light controller started");
  Serial.println("Non-blocking timing with millis()");
}

// ============================================================
// Main loop
// ============================================================

void loop() {
  unsigned long now = millis();

  if (now - lastPhaseChange >= getPhaseDuration(currentPhase)) {
    currentPhase = getNextPhase(currentPhase);
    applyPhase(currentPhase);
    lastPhaseChange = now;
  }
}
```
