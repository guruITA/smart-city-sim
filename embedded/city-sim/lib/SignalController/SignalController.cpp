#include "SignalController.h"

SignalController::SignalController(int led1, int led2, int buzzer, int servoPin)
  : _led1(led1), _led2(led2), _buzzer(buzzer), _servoPin(servoPin),
    _state(SIGNAL_IDLE), _toggle(false) {}

void SignalController::begin() {
  pinMode(_led1, OUTPUT);
  pinMode(_led2, OUTPUT);
  ledcAttach(_buzzer, 2000, 8);

  _servo.attach(_servoPin);
  _servo.write(0);
}

void SignalController::setState(SignalState state) {
  _state = state;
}

void SignalController::update() {
  switch (_state) {
    case SIGNAL_IDLE:
      digitalWrite(_led1, LOW);
      digitalWrite(_led2, LOW);
      ledcWriteTone(_buzzer, 0);
      _servo.write(0);
      break;

    case SIGNAL_WARNING:
      if (millis() - _blinkTimer > 300) {
        _blinkTimer = millis();
        _toggle = !_toggle;
        digitalWrite(_led1, _toggle);
        digitalWrite(_led2, !_toggle);
        ledcWriteTone(_buzzer, _toggle ? 800 : 1200);
      }
      break;

    case SIGNAL_ACTIVE:
      digitalWrite(_led1, HIGH);
      digitalWrite(_led2, HIGH);
      ledcWriteTone(_buzzer, 1000);
      _servo.write(90);
      break;
  }
}