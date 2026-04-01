#ifndef SIGNAL_CONTROLLER_H
#define SIGNAL_CONTROLLER_H

#include <Arduino.h>
#include <ESP32Servo.h>

enum SignalState {
  SIGNAL_IDLE,
  SIGNAL_WARNING,
  SIGNAL_ACTIVE
};

class SignalController {
public:
  SignalController(int led1, int led2, int buzzer, int servoPin);

  void begin();
  void setState(SignalState state);
  void update();

private:
  int _led1, _led2, _buzzer, _servoPin;
  Servo _servo;

  SignalState _state;

  unsigned long _blinkTimer;
  bool _toggle;
};

#endif