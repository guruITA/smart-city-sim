# Train prediction signal demo

## 1. Overview

This demo demonstrates a basic concept used in railway systems to predict a train’s arrival time and control a railway crossing barrier.

This demo is located in the folder ```embedded/Thijmen/train-prediction-signal```

Using:

- A push button (simulating detection points)
- A built-in LED (simulating the warning signal / barrier activation)
- A finite state machine (FSM)
- Time prediction based on measured speed

The system estimates when a train will arrive at a crossing and activates the barrier at the correct moment.

---

## 2. Real-World Inspiration: Railway Crossing Systems

In real railway systems:

1. A train passes detection point A  
2. The train passes detection point B  
3. The system calculates the train's speed  
4. Based on the remaining distance to the crossing (C), it predicts arrival time  
5. The barrier closes before the train arrives  

This demo simulates that logic in a simplified way.

---

## 3. Distance Model

The code uses:

```arduino
#define A_B_DISTANCE 200  
#define B_C_DISTANCE 1000 
``` 

Meaning:

- Distance A -> B = 200 units  
- Distance B -> C (railway crossing) = 1000 units  

If the train speed is constant:

Time_BC = Time_AB × (B_C / A_B)

Since:

1000 / 200 = 5

The arrival time at point C is:

Predicted Time = Measured Time × 5

This is the core mathematical principle behind arrival prediction.

---

## 4. Relation to a Barrier System

Step-by-step real-world analogy:

| Demo Action | Real Railway Equivalent |
|-------------|-------------------------|
| First button press | Train passes sensor A |
| Second button press | Train passes sensor B |
| Time measurement | Speed calculation |
| Predicted time | Estimated arrival at crossing |
| LED turns ON | Barrier closing / warning lights activate |

---

## 5. State Machine in Railway Terms

### IDLE
No train detected  
Barrier is open  

### MEASURING
Train traveling between sensor A and B  
System measures travel time  

### WAITING
Train is traveling toward crossing  
System waits predicted arrival time  

When time expires:
- LED turns ON  
- Barrier would close  

---

## 6. Why This Is a Basic Concept

Real railway crossings are much more advanced and include:

- Multiple detection systems  
- Axle counters  
- Redundant fail-safe electronics  
- Continuous monitoring  
- Safety certifications  

However, the core logic remains the same:

Measure speed -> Predict arrival -> Activate barrier in advance

---

## 7. Why Timing Is Critical

If the barrier closes:

- Too early -> unnecessary traffic delay  
- Too late -> dangerous situation  

Accurate prediction ensures:

- Safety  
- Efficiency  
- Proper timing  

---

## 8. Educational Value

This project demonstrates:

- Finite State Machines (FSM)  
- Real-time embedded timing  
- Proportional prediction mathematics  
- Event-driven system design  
- Basic industrial automation principles  

It provides a strong foundation for understanding:

- Railway automation  
- Industrial safety systems  
- Embedded predictive control  
