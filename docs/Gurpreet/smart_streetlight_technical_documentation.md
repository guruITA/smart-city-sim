# Smart Streetlight Technical Documentation

**Author:** Gurpreet Singh  
**Date:** 15-04-2026  
**Project:** City Sim Learning Group  
**Component:** Smart Streetlight  
**Platform:** ESP32-S3  
**Language:** C++ / Arduino  

## Table of Contents
- [1. Introduction](#1-introduction)
- [2. Relevant files](#2-relevant-files)
- [3. Purpose of the module](#3-purpose-of-the-module)
- [4. Configuration in Config.h](#4-configuration-in-configh)
- [5. StreetLight class design](#5-streetlight-class-design)
- [6. Streetlight logic in StreetLight.cpp](#6-streetlight-logic-in-streetlightcpp)
- [7. Integration in city-sim.ino](#7-integration-in-city-simino)
- [8. Runtime behaviour](#8-runtime-behaviour)
- [9. Design choices](#9-design-choices)
- [10. Important notes](#10-important-notes)
- [11. Conclusion](#11-conclusion)

## 1. Introduction

This document explains the technical working of the smart streetlight component in the City Sim project. The streetlight uses an LDR sensor to measure the ambient light level and a relay to switch the lamp on or off automatically.

The logic is separated into its own module so it can be maintained and reused more easily.

## 2. Relevant files

The streetlight logic is implemented across the following files:

- `Config.h`
- `Streetlight/StreetLight.h`
- `Streetlight/StreetLight.cpp`
- `city-sim.ino`

`Config.h` contains the configuration values such as pin numbers, threshold, and interval.  
`StreetLight.h` contains the class definition.  
`StreetLight.cpp` contains the implementation of the streetlight logic.  
`city-sim.ino` creates the object and calls the streetlight functions in `setup()` and `loop()`.

## 3. Purpose of the module

The purpose of the smart streetlight module is to automatically turn a lamp on when it becomes dark and turn it off when the environment is bright again.

This simulates a simple smart city lighting system that responds to sensor input without manual control.

## 4. Configuration in Config.h

The configuration values for the streetlight are defined in `Config.h`.

```cpp
namespace Config {

    namespace Streetlight {
        constexpr uint8_t LDR_PIN = 4;
        constexpr uint8_t RELAY_PIN = 5;

        constexpr int THRESHOLD = 650;
        constexpr unsigned long INTERVAL_MS = 1000;
    }

}
```

### Explanation

`LDR_PIN` is the analog input pin for the LDR sensor.  
`RELAY_PIN` is the digital output pin for the relay.  
`THRESHOLD` is the limit used to decide whether it is dark or bright.  
`INTERVAL_MS` defines how often the sensor value is checked.

These values are stored separately from the main logic so the code is easier to change and maintain.

## 5. StreetLight class design

The streetlight module is implemented using the `StreetLight` class in `StreetLight.h`.

```cpp
#ifndef STREETLIGHT_H
#define STREETLIGHT_H

#include <Arduino.h>

class StreetLight {

private:
  int _ldrPin;
  int _relayPin;
  int _threshold;

  int _interval;
  unsigned long _previousMillis;

public:
  StreetLight(int ldrPin, int relayPin, int threshold, int interval);

  void begin();
  void update();
};

#endif
```

### Explanation

The class stores:
- the LDR pin
- the relay pin
- the threshold value
- the update interval
- the last recorded time for non-blocking timing

The class has two main public functions:
- `begin()` for initialisation
- `update()` for repeated execution during runtime

## 6. Streetlight logic in StreetLight.cpp

The actual logic is implemented in `StreetLight.cpp`.

### Constructor

```cpp
StreetLight::StreetLight(int ldrPin, int relayPin, int threshold, int interval) {
  _ldrPin = ldrPin;
  _relayPin = relayPin;
  _threshold = threshold;
  _interval = interval; 
  _previousMillis = 0;
}
```

The constructor stores the configuration values inside the object.

### Initialisation

```cpp
void StreetLight::begin() {
  Serial.begin(115200);
  pinMode(_ldrPin, INPUT);
  pinMode(_relayPin, OUTPUT);
  Serial.println("Automatic Street Light System");
}
```

This function starts serial communication and configures the sensor and relay pins.

### Update logic

```cpp
void StreetLight::update() {
  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= _interval) {
    _previousMillis = currentMillis;

    int lightLevel = analogRead(_ldrPin);

    Serial.print("Light Level: ");
    Serial.println(lightLevel);

    if (lightLevel > _threshold) {
      digitalWrite(_relayPin, HIGH);
      Serial.println("It's dark! Turning light on...");
    } else {
      digitalWrite(_relayPin, LOW);
      Serial.println("It's bright! Turning light off...");
    }
  }
}
```

### Explanation of the logic

The `update()` function uses `millis()` to check whether the set interval has passed. If enough time has passed, the ESP32 reads the analog value from the LDR sensor.

The measured value is compared with the threshold:
- if the value is greater than the threshold, the system treats this as darkness and turns the relay on
- if the value is lower than or equal to the threshold, the system treats this as brightness and turns the relay off

The measured value and system decision are also printed to the Serial Monitor for debugging.

## 7. Integration in city-sim.ino

The streetlight module is used in the main program file `city-sim.ino`.

### Object creation

```cpp
StreetLight lamp(
  Config::Streetlight::LDR_PIN,
  Config::Streetlight::RELAY_PIN,
  Config::Streetlight::THRESHOLD,
  Config::Streetlight::INTERVAL_MS
);
```

This creates the `StreetLight` object using values from `Config.h`.

### Setup integration

```cpp
void setup() {
  lamp.begin();
}
```

This starts the streetlight module.

### Loop integration

```cpp
void loop() {
  lamp.update();
}
```

This keeps the streetlight logic active during runtime.

In the full project, other modules are also updated in the loop, but for the streetlight logic, only `lamp.update();` is relevant.

## 8. Runtime behaviour

During runtime, the streetlight checks the LDR sensor every 1000 milliseconds. The measured light value is printed to the Serial Monitor.

Expected behaviour:
- bright environment -> relay off -> light off
- dark environment -> relay on -> light on

This allows the prototype to behave like an automatic streetlight.

## 9. Design choices

The streetlight logic was placed in a separate class to keep the code modular and easy to understand.

The pin definitions and other adjustable values were placed in `Config.h` so that hardware settings can be changed without rewriting the logic.

The system uses `millis()` instead of `delay()`. This is important because `millis()` is non-blocking, which means the ESP32 can continue running other modules in the same loop.

## 10. Important notes

The analog behaviour of an LDR depends on the module and wiring. In this implementation, a value above the threshold is interpreted as darkness. This was determined from the behaviour of the current setup and should always be tested in practice.

The threshold value of `650` is a calibration value. This may need to be adjusted when the sensor is used in a different environment.

The relay module may also behave differently depending on whether it is active-high or active-low. In this code, the relay is activated with `HIGH`.

## 11. Conclusion

The smart streetlight module uses an LDR sensor and relay to automate a lamp based on ambient light. The configuration is stored in `Config.h`, the logic is implemented in the `StreetLight` class, and the module is integrated into the main program through `city-sim.ino`.

This creates a simple, reusable, and maintainable smart streetlight component for the City Sim project.
