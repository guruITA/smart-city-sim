#include "TrainPredictionSignal.h"

void TrainPredictionSignal::handleServo() {
  if (_currentState != WAITING) {
    if (_currentBarrierPos != _barrierOpenAngle) {
      _servo.write(_barrierOpenAngle);
      _currentBarrierPos = _barrierOpenAngle;
    }
    return;
  }

  unsigned long elapsed = millis() - _startMillis;

  if (elapsed >= _predictedTime && _currentBarrierPos != _barrierClosedAngle) {
    _servo.write(_barrierClosedAngle);
    _currentBarrierPos = _barrierClosedAngle;
  }
}

void TrainPredictionSignal::handleWarningLEDSAndSound() {
  if (_currentState != WAITING) {
    digitalWrite(_led1Pin, LOW);
    digitalWrite(_led2Pin, LOW);
    ledcWriteTone(_buzzerPin, 0);
    return;
  }

  unsigned long elapsed = millis() - _startMillis;

  if (elapsed >= _predictedTime - _safetyMargin) {
    if (millis() - _blinkTimer >= _blinkInterval) {
      _blinkTimer = millis();
      _ledToggle = !_ledToggle;
      digitalWrite(_led1Pin, _ledToggle);
      digitalWrite(_led2Pin, !_ledToggle);

      if (_ledToggle) {
        ledcWriteTone(_buzzerPin, 800);
      } else {
        ledcWriteTone(_buzzerPin, 1200);
      }
    }
  } else {
    digitalWrite(_led1Pin, LOW);
    digitalWrite(_led2Pin, LOW);
    ledcWriteTone(_buzzerPin, 0);
  }
}
