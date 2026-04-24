# Realisation - Recognizing when a car is waiting at the traffic light and sending that information to the backend

## Summary

In this realisation, I extended my existing four-way traffic-light crossing with a first smart input: detecting whether a car is waiting at the traffic light and turning that sensor signal into a meaningful backend-ready traffic state. I did not redesign the whole crossing, because that hardware structure was already completed earlier with the ESP32-S3, MCP23017, two ULN2803 chips, and external LED power. Instead, I built on that existing structure by adding one KY-021 prototype sensor to one road direction and connecting it to the ESP32-S3 as a new digital input.

The most important realisation choice was that the sensor does not directly control the traffic lights. The existing `millis()` based traffic-light controller remains responsible for the safe phase sequence, while the KY-021 is read separately and interpreted in the context of the current traffic-light phase. In this way, the project stays modular and safe. The result of this step is a first working prototype in which the system can detect presence at the stop line, interpret whether that presence means waiting or passing traffic, and produce backend-ready traffic state information without breaking the safe behaviour of the crossing.

## Table of Contents

- [1. Introduction](#1-introduction)
- [2. What is already handled in the previous deliverables](#2-what-is-already-handled-in-the-previous-deliverables)
- [3. Goal of the Realisation](#3-goal-of-the-realisation)
- [4. Realised prototype setup](#4-realised-prototype-setup)
- [5. Functional structure of the realised system](#5-functional-structure-of-the-realised-system)
- [6. Pin and connection mapping used in the realisation](#6-pin-and-connection-mapping-used-in-the-realisation)
  - [6.1 Existing crossing connections](#61-existing-crossing-connections)
  - [6.2 Sensor input connection](#62-sensor-input-connection)
- [7. Why I kept the `millis()` based structure](#7-why-i-kept-the-millis-based-structure)
- [8. Build process](#8-build-process)
- [9. Testing of the realisation](#9-testing-of-the-realisation)
  - [Test 1 — Sensor input is detected correctly](#test-1--sensor-input-is-detected-correctly)
  - [Test 2 — Stable detection during red is interpreted as waiting traffic](#test-2--stable-detection-during-red-is-interpreted-as-waiting-traffic)
  - [Test 3 — Short detection during green is not interpreted as waiting traffic](#test-3--short-detection-during-green-is-not-interpreted-as-waiting-traffic)
  - [Test 4 — Unclear or missing input does not create unsafe behaviour](#test-4--unclear-or-missing-input-does-not-create-unsafe-behaviour)
  - [Test 5 — Backend-ready message is created correctly](#test-5--backend-ready-message-is-created-correctly)
  - [Test 6 — Existing traffic-light behaviour remains stable](#test-6--existing-traffic-light-behaviour-remains-stable)
- [10. Reflection](#10-reflection)
- [11. Conclusion](#11-conclusion)
- [References](#references)
- [Appendix A — Real-Life Realisation Photos](#appendix-a--real-life-realisation-photos)
  - [Figure A1 — Complete prototype overview](#figure-a1--complete-prototype-overview)
  - [Figure A2 — KY-021 sensor placement at the stop line](#figure-a2--ky-021-sensor-placement-at-the-stop-line)
  - [Figure A3 — ESP32-S3 and controller side](#figure-a3--esp32-s3-and-controller-side)
  - [Figure A4 — Serial monitor or backend message proof](#figure-a4--serial-monitor-or-backend-message-proof)
- [Appendix B — Final Realisation Code](#appendix-b--final-realisation-code)




## 1. Introduction

In this document, I describe how I realised the first smart-input extension of my traffic-light project based on the earlier analysis and design work. In this phase, I did not rebuild the full crossing from the beginning. Instead, I used the already working four-way traffic-light setup as the base and added one prototype sensor input to test whether the system can recognise when a car is waiting at the traffic light and send that interpreted result to the backend.

The analysis already defined that this sprint is not only about detection, but about interpretation. It also defined that the project should distinguish between no vehicle, waiting vehicle, passing vehicle, and unclear input, and that the traffic-light phase must be part of that interpretation (Wesley, 2026a). The design then translated that into a concrete hardware and software structure using a KY-021 on GPIO 6, a phase-aware interpretation step, and a backend-ready message structure (Wesley, 2026b).

This realisation phase focuses on building and testing that design in practice.

## 2. What is already handled in the previous deliverables

The analysis deliverable already explains the project background, the practical problem, the interpreted states, the interpretation rules, the risks, and the acceptance criteria for this sprint (Wesley, 2026a). The design deliverable already explains the chosen hardware addition, the use of the KY-021, the proposed software structure, the phase link, and the backend message format (Wesley, 2026b).

Because those parts are already documented there, I do not repeat their full reasoning here. In this realisation document, I focus on how I added the sensor to the current four-way crossing, how I connected it in practice, how I used the current `millis()` based structure, and how the realised prototype behaved during testing.

This also follows the same way of working that I used in Sprint 2, where I first defined the reasoning in analysis and design and then focused the realisation on building, testing, and observing the actual system behaviour (Wesley, 2026c; Wesley, 2026f).

## 3. Goal of the Realisation

The goal of this realisation was to build the designed waiting-car detection extension on top of the already existing four-way traffic-light crossing and verify that the system works safely and correctly in practice.

This included checking that:

* the KY-021 sensor can be read correctly by the ESP32-S3
* the sensor can be integrated without disturbing the existing crossing behaviour
* the current traffic-light phase can be used during interpretation
* stable detection during red can be interpreted as waiting traffic
* short detection during green is not incorrectly treated as waiting traffic
* unclear or missing input does not create unsafe behaviour
* the system can produce a backend-ready traffic state message
* the crossing still keeps its safe phase order and non-blocking timing

These goals follow directly from the current analysis and design (Wesley, 2026a; Wesley, 2026b).

## 4. Realised prototype setup

I built the realised prototype on top of the existing four-way traffic-light setup. That means the current controller side, output-expansion layer, switching layer, and externally powered LED channels all remained in place from the earlier crossing implementation (Wesley, 2026c).

The new part in this realisation was the KY-021 sensor input. I connected one KY-021 to the ESP32-S3 and used it as a prototype detector for one road direction. The sensor was positioned at the stop-line area for that chosen direction on the city tile. A small magnet was used on or in the model vehicle so that the KY-021 could change state when the vehicle was positioned above the detection point, as already defined in the design (Wesley, 2026b).

This means the realised prototype now has two linked parts:

* the existing four-way traffic-light controller
* one new digital sensor input for vehicle presence

The sensor does not switch the lights directly. It only sends input to the controller so that the controller can interpret the signal in the context of the current traffic-light phase.

## 5. Functional structure of the realised system

The realised system now works in six functional steps.

First, the **ESP32-S3** keeps running the existing traffic-light phase logic.

Second, the **ESP32-S3** reads the **KY-021** sensor as a digital input.

Third, the program checks the current traffic-light phase.

Fourth, the program interprets the sensor signal using that phase context.

Fifth, the program checks whether the interpreted state has changed.

Sixth, the program creates a backend-ready message when a meaningful change is detected.

This means the realised system now combines:

```text
traffic-light phase logic + sensor input + interpretation logic + backend-ready output
```

The separation between safe traffic-light control and the new input handling remains important here, because the existing crossing must keep working safely even when the sensor is active, inactive, unclear, or missing (Wesley, 2026a; Wesley, 2026b).

## 6. Pin and connection mapping used in the realisation

### 6.1 Existing crossing connections

The existing crossing setup remained as it was already realised in Sprint 2. That means:

| Function    | Connection                                                                      |
| ----------- | ------------------------------------------------------------------------------- |
| SDA         | GPIO 5 to MCP23017 SDA                                                          |
| SCL         | GPIO 4 to MCP23017 SCL                                                          |
| Logic power | ESP32-S3 3.3V to MCP23017                                                       |
| Ground      | Shared ground between ESP32-S3, MCP23017, ULN2803 chips, and external LED power |

The traffic-light output structure, the MCP23017, the ULN2803 chips, and the external LED power side were not changed in this step (Wesley, 2026c).

### 6.2 Sensor input connection

The new sensor input was added like this:

| ESP32-S3 pin | Connection    | Purpose       |
| ------------ | ------------- | ------------- |
| GPIO 6       | KY-021 signal | Sensor input  |
| 3V3          | KY-021 VCC    | Sensor power  |
| GND          | KY-021 GND    | Common ground |

So the sensor wiring was:

```text
ESP32-S3 3V3   -> KY-021 VCC
ESP32-S3 GND   -> KY-021 GND
ESP32-S3 GPIO6 -> KY-021 signal
```

This matches the design choice to use GPIO 6 as the first new expansion pin, because GPIO 4 and GPIO 5 were already in use for I2C in the crossing design (Wesley, 2026b; Wesley, 2026c).

## 7. Why I kept the `millis()` based structure

I kept the `millis()` based structure instead of switching back to `delay()` because the realised system now combines traffic-light timing with sensor handling and backend-ready state creation. A blocking timing method would reduce the system’s ability to read the sensor continuously and respond cleanly to state changes.

This was already an important lesson in Sprint 2, where replacing `delay()` with `millis()` made the traffic-light controller more suitable for later integration with new features (Wesley, 2026c; Wesley, 2026d). My earlier advice also stated that the non-blocking structure should be kept as a requirement for later features (Wesley, 2026d).

So in this realisation, the phase timing stays non-blocking and the sensor logic is added alongside it instead of interrupting it.

## 8. Build process

I did not build this extension by redesigning the whole crossing from the beginning. I used the already working four-way traffic-light setup as the base and added the new parts in a controlled order.

The build process was divided into stages.

First, I kept the existing crossing setup unchanged and confirmed that the safe traffic-light sequence still worked before I added the new sensor feature.

After that, I connected the KY-021 to the ESP32-S3 with 3.3V, GND, and GPIO 6.

Next, I tested whether the ESP32-S3 could read the KY-021 input correctly on its own.

After that, I linked the sensor reading to the existing phase logic so the software could interpret the signal differently depending on whether the light was in red, green, yellow, or a transition state.

Then, I added the backend-ready message structure so the program could create an interpreted traffic state output instead of only a raw sensor change.

Finally, I tested the full behaviour together: the crossing phases, the sensor reading, the interpreted state, and the backend-ready output.

This follows the same staged way of working that I already found useful in Sprint 2, where testing one part at a time made the build process more controlled and easier to debug (Wesley, 2026d).

## 9. Testing of the realisation

### Test 1 — Sensor input is detected correctly

**Expected result**
The ESP32-S3 must correctly read when the KY-021 changes between inactive and active state.

**Observed result**
During testing, the input changed correctly when the magnet-equipped vehicle was positioned above the detection point and returned again when the vehicle moved away.

**Conclusion**
Passed. The sensor input was detected correctly.

### Test 2 — Stable detection during red is interpreted as waiting traffic

**Expected result**
If the light for the tested direction is red and the sensor remains active longer than the waiting threshold, the system should interpret that as waiting traffic.

**Observed result**
During testing, a vehicle that remained above the stop-line detector during red was interpreted as a waiting vehicle after the stable threshold was reached.

**Conclusion**
Passed. Stable detection during red was interpreted as waiting traffic.

### Test 3 — Short detection during green is not interpreted as waiting traffic

**Expected result**
If the light is green and the vehicle only passes briefly over the detection point, the system must not create a waiting-vehicle result.

**Observed result**
During testing, a short active signal during green was handled as passing traffic and was not treated as a waiting-car state.

**Conclusion**
Passed. Short green-phase detection was not misclassified as waiting traffic.

### Test 4 — Unclear or missing input does not create unsafe behaviour

**Expected result**
If the signal is unstable, unclear, or missing, the crossing must keep its safe normal behaviour.

**Observed result**
During testing, unclear or absent detection did not disturb the traffic-light sequence. The system continued safely in its existing phase structure.

**Conclusion**
Passed. Unclear or missing input did not create unsafe behaviour.

### Test 5 — Backend-ready message is created correctly

**Expected result**
When the interpreted traffic state changes, the program should create a backend-ready message with sensor identifier, direction, phase, interpreted state, timestamp, and validity flag.

**Observed result**
During testing, the system produced message output in the expected structure when a meaningful state change occurred, such as a transition from no vehicle to waiting vehicle.

**Conclusion**
Passed. Backend-ready message creation worked correctly.

### Test 6 — Existing traffic-light behaviour remains stable

**Expected result**
The addition of the KY-021 and the interpretation logic must not break the current safe traffic-light sequence.

**Observed result**
During repeated observation, the existing crossing still started safely, kept the intended phase order, avoided conflicting green states, and continued cycling correctly while the sensor logic was active.

**Conclusion**
Passed. The existing traffic-light behaviour remained stable.

## 10. Reflection

This realisation phase was useful because it showed that the current crossing could be extended with a first smart-input feature without redesigning the full hardware structure. The most important result is that the project is now no longer only a fixed-cycle coordinated crossing. It still behaves as a safe crossing, but it can now also observe one stop-line area and interpret whether a car is waiting there.

Another important result is that the separation between traffic-light control and sensor interpretation worked well in practice. The lights themselves still follow the safe `millis()` based state machine, while the sensor is handled as a separate input layer. That makes the project structure clearer and supports later extension better than mixing all responsibilities together.

A final reflection is that the staged way of working again helped a lot. By keeping the already working crossing unchanged first, then adding the KY-021, then testing the input alone, and only after that combining it with the interpretation and backend-ready output, the build process stayed manageable. That matches the earlier lesson from Sprint 2 that good staged testing is just as important as the design itself (Wesley, 2026c; Wesley, 2026d).

## 11. Conclusion

In this realisation, I successfully extended the existing four-way traffic-light crossing with one prototype sensor input for waiting-car detection. The crossing hardware from Sprint 2 remained unchanged on the output side, while the new KY-021 sensor was added as a digital input on GPIO 6.

The realised system now combines safe phase control, phase-aware sensor interpretation, and backend-ready message creation. During testing, the sensor input was detected correctly, stable red-phase presence was interpreted as waiting traffic, short green-phase detection was not misclassified as waiting traffic, unclear input did not create unsafe behaviour, and the existing crossing behaviour remained stable.

At this stage, the realisation covers the first working prototype of waiting-car recognition on one road direction. It does not yet extend the feature to all directions, and it does not yet include full backend transport, but it already provides a clear and working first smart-input step on top of the coordinated crossing.

## References

Wesley. (2026a). *Analysis - Recognizing when a car is waiting at the traffic light and sending that information to the backend* [Analysis deliverable].

Wesley. (2026b). *Design - Recognizing when a car is waiting at the traffic light and sending that information to the backend* [Design deliverable].

Wesley. (2026c). *Realisation - Breadboard Realisation of the Four-Way Traffic-Light Setup* [Realisation deliverable].

Wesley. (2026d). *Advice — Learning Goal 1 - Expanding the traffic-light system to multiple traffic lights* [Advice deliverable].

Wesley. (2026e). *Smart Traffic Light - When Does a Traffic Light Become Smart?* [Project document].

Wesley. (2026f). *Sprint 2, Learning Goal 1 - Expanding the traffic-light system to multiple traffic lights* [Learning goal document].

Wesley. (2026g). *Traffic Light - first iteration - implementation* [Project deliverable].

## Appendix A — Real-Life Realisation Photos

### Figure A1 — Complete prototype overview

Figure A1 shows the full realised prototype. In this overview, the existing four-way traffic-light crossing remains visible together with the newly added KY-021 detection input. This figure is important because it shows that the new feature is added on top of the already working crossing structure instead of replacing it.

### Figure A2 — KY-021 sensor placement at the stop line

Figure A2 shows the physical placement of the KY-021 sensor at the stop-line area of the chosen road direction. This figure is important because it proves how the simplified waiting-car detection was actually realised on the tile.

### Figure A3 — ESP32-S3 and controller side

Figure A3 shows the controller side of the realised system. It should make clear how the KY-021 is connected to GPIO 6, how the existing I2C lines remain connected to the MCP23017, and how the shared ground is kept across the system.

### Figure A4 — Serial monitor or backend message proof

Figure A4 shows the proof of the interpreted output. This can be a serial monitor view or another output capture that shows the created backend-ready messages for states such as waiting vehicle, passing vehicle, or no vehicle.

## Appendix B — Final Realisation Code

```cpp
#include <Wire.h>

/*
  Existing crossing controller + KY-021 waiting-car detection
  ESP32-S3 -> MCP23017 -> 2x ULN2803 -> 4 traffic lights
  KY-021 -> GPIO 6
  Non-blocking version using millis()
*/

// ============================================================
// I2C settings
// ============================================================

const int I2C_SDA_PIN = 5;
const int I2C_SCL_PIN = 4;

const uint8_t MCP_ADDR = 0x20;

// ============================================================
// KY-021 sensor input
// ============================================================

const int SENSOR_PIN = 6;

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

// Port B
const uint8_t TL1_RED_B    = 0;
const uint8_t TL1_YELLOW_B = 1;
const uint8_t TL1_GREEN_B  = 2;
const uint8_t TL2_RED_B    = 3;
const uint8_t TL2_YELLOW_B = 4;
const uint8_t TL2_GREEN_B  = 5;

// Port A
const uint8_t TL3_YELLOW_A = 6;
const uint8_t TL3_RED_A    = 5;
const uint8_t TL3_GREEN_A  = 4;
const uint8_t TL4_YELLOW_A = 3;
const uint8_t TL4_RED_A    = 2;
const uint8_t TL4_GREEN_A  = 1;

// ============================================================
// Timing values
// ============================================================

const unsigned long ALL_RED_TIME = 1000;
const unsigned long GREEN_TIME   = 5000;
const unsigned long YELLOW_TIME  = 2000;

// Sensor interpretation timing
const unsigned long DEBOUNCE_TIME         = 50;
const unsigned long WAITING_THRESHOLD     = 500;
const unsigned long SENSOR_CLEAR_TIMEOUT  = 200;

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
// Interpreted states
// ============================================================

enum InterpretedState {
  NO_VEHICLE,
  WAITING_VEHICLE,
  PASSING_VEHICLE,
  UNCLEAR_INPUT
};

// ============================================================
// State variables
// ============================================================

uint8_t currentPhase = PHASE_ALL_RED_1;
unsigned long lastPhaseChange = 0;

bool rawSensorState = false;
bool stableSensorState = false;
bool lastRawSensorState = false;
unsigned long lastSensorChange = 0;
unsigned long sensorActiveSince = 0;
unsigned long sensorInactiveSince = 0;

InterpretedState currentInterpretedState = NO_VEHICLE;
InterpretedState lastSentState = NO_VEHICLE;

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
// Phase control
// ============================================================

void applyPhase(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
      allRed();
      break;

    case PHASE_ROAD12_GREEN:
      road12Green_road34Red();
      break;

    case PHASE_ROAD12_YELLOW:
      road12Yellow_road34Red();
      break;

    case PHASE_ALL_RED_2:
      allRed();
      break;

    case PHASE_ROAD34_GREEN:
      road12Red_road34Green();
      break;

    case PHASE_ROAD34_YELLOW:
      road12Red_road34Yellow();
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
    case PHASE_ALL_RED_1:     return PHASE_ROAD12_GREEN;
    case PHASE_ROAD12_GREEN:  return PHASE_ROAD12_YELLOW;
    case PHASE_ROAD12_YELLOW: return PHASE_ALL_RED_2;
    case PHASE_ALL_RED_2:     return PHASE_ROAD34_GREEN;
    case PHASE_ROAD34_GREEN:  return PHASE_ROAD34_YELLOW;
    case PHASE_ROAD34_YELLOW: return PHASE_ALL_RED_1;
    default:                  return PHASE_ALL_RED_1;
  }
}

void updateTrafficPhase() {
  unsigned long now = millis();

  if (now - lastPhaseChange >= getPhaseDuration(currentPhase)) {
    currentPhase = getNextPhase(currentPhase);
    applyPhase(currentPhase);
    lastPhaseChange = now;
  }
}

// ============================================================
// Sensor handling
// ============================================================

void readSensor() {
  unsigned long now = millis();
  rawSensorState = digitalRead(SENSOR_PIN) == LOW; // adjust if needed

  if (rawSensorState != lastRawSensorState) {
    lastSensorChange = now;
    lastRawSensorState = rawSensorState;
  }

  if ((now - lastSensorChange) >= DEBOUNCE_TIME) {
    if (stableSensorState != rawSensorState) {
      stableSensorState = rawSensorState;

      if (stableSensorState) {
        sensorActiveSince = now;
      } else {
        sensorInactiveSince = now;
      }
    }
  }
}

bool phaseIsRedForTestedDirection() {
  return currentPhase == PHASE_ALL_RED_1 || currentPhase == PHASE_ALL_RED_2 ||
         currentPhase == PHASE_ROAD34_GREEN || currentPhase == PHASE_ROAD34_YELLOW;
}

bool phaseIsGreenForTestedDirection() {
  return currentPhase == PHASE_ROAD12_GREEN;
}

bool phaseIsYellowOrTransitionForTestedDirection() {
  return currentPhase == PHASE_ROAD12_YELLOW ||
         currentPhase == PHASE_ALL_RED_1 ||
         currentPhase == PHASE_ALL_RED_2;
}

void interpretSensorWithCurrentPhase() {
  unsigned long now = millis();

  if (!stableSensorState) {
    if ((now - sensorInactiveSince) >= SENSOR_CLEAR_TIMEOUT) {
      currentInterpretedState = NO_VEHICLE;
    }
    return;
  }

  unsigned long activeDuration = now - sensorActiveSince;

  if (phaseIsRedForTestedDirection()) {
    if (activeDuration >= WAITING_THRESHOLD) {
      currentInterpretedState = WAITING_VEHICLE;
    }
  } else if (phaseIsGreenForTestedDirection()) {
    if (activeDuration < WAITING_THRESHOLD) {
      currentInterpretedState = PASSING_VEHICLE;
    }
  } else if (phaseIsYellowOrTransitionForTestedDirection()) {
    currentInterpretedState = UNCLEAR_INPUT;
  } else {
    currentInterpretedState = UNCLEAR_INPUT;
  }
}

// ============================================================
// Backend-ready output
// ============================================================

const char* phaseName(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:     return "ALL_RED_1";
    case PHASE_ROAD12_GREEN:  return "ROAD12_GREEN";
    case PHASE_ROAD12_YELLOW: return "ROAD12_YELLOW";
    case PHASE_ALL_RED_2:     return "ALL_RED_2";
    case PHASE_ROAD34_GREEN:  return "ROAD34_GREEN";
    case PHASE_ROAD34_YELLOW: return "ROAD34_YELLOW";
    default:                  return "UNKNOWN";
  }
}

const char* stateName(InterpretedState state) {
  switch (state) {
    case NO_VEHICLE:      return "no_vehicle";
    case WAITING_VEHICLE: return "waiting_vehicle";
    case PASSING_VEHICLE: return "passing_vehicle";
    case UNCLEAR_INPUT:   return "unclear_input";
    default:              return "unknown";
  }
}

bool stateIsValid(InterpretedState state) {
  return state != UNCLEAR_INPUT;
}

void sendBackendMessageIfNeeded() {
  if (currentInterpretedState != lastSentState) {
    Serial.print("{\"sensorId\":\"north_1\",\"direction\":\"north\",\"phase\":\"");
    Serial.print(phaseName(currentPhase));
    Serial.print("\",\"interpretedState\":\"");
    Serial.print(stateName(currentInterpretedState));
    Serial.print("\",\"timestampMs\":");
    Serial.print(millis());
    Serial.print(",\"valid\":");
    Serial.print(stateIsValid(currentInterpretedState) ? "true" : "false");
    Serial.println("}");

    lastSentState = currentInterpretedState;
  }
}

// ============================================================
// Setup
// ============================================================

void setup() {
  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT_PULLUP);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  mcpInit();

  applyPhase(currentPhase);
  lastPhaseChange = millis();
}

// ============================================================
// Main loop
// ============================================================

void loop() {
  updateTrafficPhase();
  readSensor();
  interpretSensorWithCurrentPhase();
  sendBackendMessageIfNeeded();
}
```
