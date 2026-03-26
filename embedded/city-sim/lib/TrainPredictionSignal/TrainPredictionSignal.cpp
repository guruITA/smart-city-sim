#include "TrainPredictionSignal.h"

TrainPredictionSignal::TrainPredictionSignal(int led1Pin, int led2Pin, int buzzerPin, int servoPin, int btnPin,
                                             int aBDistance, int bCDistance, unsigned int safetyMargin,
                                             int buzzerFreq, int buzzerResolution,
                                             int barrierOpenAngle, int barrierClosedAngle)
  : _led1Pin(led1Pin), _led2Pin(led2Pin), _buzzerPin(buzzerPin), _servoPin(servoPin), _btnPin(btnPin),
    _aBDistance(aBDistance), _bCDistance(bCDistance), _safetyMargin(safetyMargin),
    _buzzerFreq(buzzerFreq), _buzzerResolution(buzzerResolution),
    _barrierOpenAngle(barrierOpenAngle), _barrierClosedAngle(barrierClosedAngle),
    _currentState(IDLE), _currentBarrierPos(_barrierOpenAngle),
    _startMillis(0), _predictedTime(0),
    _buttonState(HIGH), _lastReading(HIGH), _lastDebounceTime(0),
    _blinkTimer(0), _ledToggle(false) {}

void TrainPredictionSignal::begin() {
  Serial.begin(115200);
  pinMode(_led1Pin, OUTPUT);
  pinMode(_led2Pin, OUTPUT);
  pinMode(_btnPin, INPUT_PULLUP);
  ledcAttach(_buzzerPin, _buzzerFreq, _buzzerResolution);
  _servo.attach(_servoPin, 500, 2400);
  _servo.write(_barrierOpenAngle);
  Serial.println("Train Prediction Signal Setup complete");
}

void TrainPredictionSignal::update() {
  handleButton();
  handleWarningLEDSAndSound();
  handleServo();
}

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

void TrainPredictionSignal::handleButton() {
  bool reading = digitalRead(_btnPin);

  if (reading != _lastReading) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    if (reading != _buttonState) {
      _buttonState = reading;

      if (_buttonState == LOW) {
        switch (_currentState) {
          case IDLE:
            _startMillis = millis();
            _currentState = MEASURING;
            break;

          case MEASURING:
            _predictedTime = (millis() - _startMillis) * (_bCDistance / _aBDistance);
            _startMillis = millis();
            _currentState = WAITING;
            break;

          case WAITING:
            _currentState = IDLE;
            digitalWrite(_led1Pin, LOW);
            digitalWrite(_led2Pin, LOW);
            break;
        }
      }
    }
  }

  _lastReading = reading;
}