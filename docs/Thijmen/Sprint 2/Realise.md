# Realise Document

## Table of Contents

1. [Goal of This Realisation](#1-goal-of-this-realisation)  
2. [Where the Code Was Realised](#2-where-the-code-was-realised)  
3. [City-Sim Integration Realisation](#3-city-sim-integration-realisation)  
4. [Train Signal Behavior Realisation](#4-train-signal-behavior-realisation)  
  4.1 [IDLE](#41-idle)  
  4.2 [MEASURING](#42-measuring)  
  4.3 [WAITING](#43-waiting)  
5. [Backend Communication Realisation](#5-backend-communication-realisation)  
  5.1 [Implemented Train API Flow](#51-implemented-train-api-flow)  
  5.2 [Reliability Behavior Realised](#52-reliability-behavior-realised)  
6. [Reusability and Mergeability Outcome](#6-reusability-and-mergeability-outcome)  
7. [Reflection and Next Improvements](#7-reflection-and-next-improvements)  

## 1. Goal of This Realisation

This realisation phase turns the Sprint 2 design into working code inside the shared city simulation firmware.  

The focus is on:  
Integrating the train prediction crossing logic into the shared `embedded/city-sim` project  
Making the implementation reusable through a class-based library  
Connecting train events to the backend API while keeping local simulation behavior active if the network is unavailable  

---

## 2. Where the Code Was Realised

Main integration entry point:  
`embedded/city-sim/city-sim.ino`  

Train prediction signal library:  
`embedded/city-sim/lib/TrainPredictionSignal/TrainPredictionSignal.h`  
`embedded/city-sim/lib/TrainPredictionSignal/TrainPredictionSignal.cpp`  
`embedded/city-sim/lib/TrainPredictionSignal/TrainPredictionSignalButton.cpp`  
`embedded/city-sim/lib/TrainPredictionSignal/TrainPredictionSignalOutputs.cpp`  
`embedded/city-sim/lib/TrainPredictionSignal/TrainPredictionSignalNetwork.cpp`  

Network abstraction used by the train signal and other tiles:  
`embedded/city-sim/lib/NetworkController/NetworkController.h`  
`embedded/city-sim/lib/NetworkController/NetworkController.cpp`  

Reference prototype used earlier:  
`embedded/Thijmen/train-prediction-signal/train-prediction-signal.ino`  

---

## 3. City-Sim Integration Realisation

In `city-sim.ino`, the train crossing is instantiated as:

```cpp
TrainPredictionSignal trainSignal(37, 36, 42, 18, 45, 200, 1000, 5000, 1000, 8, 0, 90);
```

This constructor call realises all key parameters as configurable values:

- Hardware pins (LED1, LED2, buzzer, servo, button)  
- Distance model (`A_B = 200`, `B_C = 1000`)  
- Safety margin (`5000` ms)  
- Buzzer and barrier settings  

The main loop integrates this tile with the rest of the city simulation:

```cpp
void loop() {
  lamp.update();
  trainSignal.update();
  speedCamera.update();
}
```

This confirms the train signal runs as a reusable component alongside other tiles, instead of as a standalone sketch.

---

## 4. Train Signal Behavior Realisation

The realised implementation uses a finite state machine with three states:  

- `IDLE`    
- `MEASURING`    
- `WAITING`  

### 4.1 IDLE  
- Barrier open  
- LEDs and buzzer off  
- Wait for first button press (simulated first sensor)  

### 4.2 MEASURING  
- Time between first and second press is measured  
- Predicted arrival is calculated with:  

```text
predictedTime = measuredTimeAB * (B_C / A_B)
```

### 4.3 WAITING  

- System waits for predicted arrival time  
- During the final safety window (`predictedTime - safetyMargin`):  
  - LEDs alternate every 400 ms  
  - Buzzer alternates tones (800/1200 Hz)  
- At predicted arrival time:  
  - Servo barrier moves from open angle to closed angle  
- Third press marks train crossed and resets to `IDLE`  

This logic is implemented primarily in:  
`TrainPredictionSignalButton.cpp` (input, state transitions, prediction)  
  `TrainPredictionSignalOutputs.cpp` (servo, lights, buzzer)  

---

## 5. Backend Communication Realisation

Backend communication is realised through `NetworkController` and used by `TrainPredictionSignalNetwork.cpp`.

### 5.1 Implemented Train API Flow
  
1. First sensor detected (`IDLE -> MEASURING`):  
- `POST /api/v1/railroadcrossing/train/first`  
- Response `id` is parsed and stored as `_trainId`  

2. Second sensor detected (`MEASURING -> WAITING`):  
- `PUT /api/v1/railroadcrossing/train/{id}/second`  
- Body includes:  

```json
{
  "predicted_arrival_seconds": 4.25
}
```

3. Train crossed (`WAITING -> IDLE`):  
- `PUT /api/v1/railroadcrossing/train/{id}/crossed`  

### 5.2 Reliability Behavior Realised

- If WiFi is not connected, requests fail gracefully and simulation still continues locally.  
- If train creation fails, the local state machine still runs.  
- URL composition is centralized in `NetworkController::buildUrl()`.      

This keeps the crossing behavior functional even during backend or network issues.

---

## 6. Reusability and Mergeability Outcome

Compared to the original standalone prototype, the realised solution improves integration quality by:

- Wrapping train crossing logic in a configurable class (`TrainPredictionSignal`)  
- Splitting implementation into focused files (button, outputs, network)  
- Using a shared network layer (`NetworkController`) instead of direct networking in the main sketch  
- Enabling clean initialization via `begin()` and periodic updates via `update()`  

This made it possible to merge train crossing with streetlight and speed camera logic in one shared firmware.

---

## 7. Reflection and Next Improvements  

What is successfully realised:  

- Working FSM-based train crossing behavior  
- Predictive timing and safety margin warnings  
- Servo barrier control  
- Backend event updates for first sensor, second sensor prediction, and crossed event  
- Integration into the shared city simulation project structure  

Potential improvements for future sprints:  

- Replace manual JSON parsing with a proper JSON parser for robustness  
- Add retry/backoff strategy for failed HTTP requests  
- Add support for real hardware sensors instead of button simulation  
- Add unit/integration tests for state transitions and API flow  
