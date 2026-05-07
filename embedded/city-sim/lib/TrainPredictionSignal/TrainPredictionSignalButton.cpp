#include "TrainPredictionSignal.h"

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
            _trainId = -1;
            if (createTrain()) {
              Serial.println("Train created in backend");
            } else {
              Serial.println("Train not created; continue local simulation");
            }
            _startMillis = millis();
            _currentState = MEASURING;
            break;

          case MEASURING:
            _predictedTime = (millis() - _startMillis) * ((float)_bCDistance / (float)_aBDistance);
            if (updateTrainSecondSensor(_predictedTime / 1000.0)) {
              Serial.println("Updated second sensor + predicted arrival to backend");
            } else {
              Serial.println("Failed to update second sensor in backend");
            }
            _startMillis = millis();
            _currentState = WAITING;
            break;

          case WAITING:
            if (updateTrainCrossed()) {
              _trainId = -1;
            }
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
