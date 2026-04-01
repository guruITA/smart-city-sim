#include "TrainDetector.h"

TrainDetector::TrainDetector(int buttonPin, int aBDistance, int bCDistance)
  : _btnPin(buttonPin),
    _aBDistance(aBDistance),
    _bCDistance(bCDistance),
    _state(IDLE),
    _buttonState(HIGH),
    _lastReading(HIGH),
    _firstEvent(false),
    _secondEvent(false),
    _thirdEvent(false) {}

void TrainDetector::begin() {
  pinMode(_btnPin, INPUT_PULLUP);
}

void TrainDetector::update() {
  _firstEvent = _secondEvent = _thirdEvent = false;

  bool reading = digitalRead(_btnPin);

  if (reading != _lastReading) {
    _lastDebounceTime = millis();
  }

  if ((millis() - _lastDebounceTime) > _debounceDelay) {
    if (reading != _buttonState) {
      _buttonState = reading;

      if (_buttonState == LOW) {
        switch (_state) {
          case IDLE:
            _startMillis = millis();
            _state = MEASURING;
            _firstEvent = true;
            break;

          case MEASURING:
            _predictedTime = (millis() - _startMillis) *
              ((float)_bCDistance / (float)_aBDistance);
            _startMillis = millis();
            _state = WAITING;
            _secondEvent = true;
            break;

          case WAITING:
            _state = IDLE;
            _thirdEvent = true;
            break;
        }
      }
    }
  }

  _lastReading = reading;
}

bool TrainDetector::firstTriggered() { return _firstEvent; }
bool TrainDetector::secondTriggered() { return _secondEvent; }
bool TrainDetector::trainPassed() { return _thirdEvent; }

float TrainDetector::getPredictedTime() {
  return _predictedTime / 1000.0;
}