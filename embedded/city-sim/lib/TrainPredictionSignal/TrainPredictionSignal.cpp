#include "TrainPredictionSignal.h"
#include "NetworkController.h"
#include <WiFi.h>
#include <HTTPClient.h>

TrainPredictionSignal::TrainPredictionSignal(int led1Pin, int led2Pin, int buzzerPin, int servoPin, int btnPin,
                                             int aBDistance, int bCDistance, unsigned int safetyMargin,
                                             int buzzerFreq, int buzzerResolution,
                                             int barrierOpenAngle, int barrierClosedAngle, const String& apiBaseUrl)
  : _led1Pin(led1Pin), _led2Pin(led2Pin), _buzzerPin(buzzerPin), _servoPin(servoPin), _btnPin(btnPin),
    _aBDistance(aBDistance), _bCDistance(bCDistance), _safetyMargin(safetyMargin),
    _buzzerFreq(buzzerFreq), _buzzerResolution(buzzerResolution),
    _barrierOpenAngle(barrierOpenAngle), _barrierClosedAngle(barrierClosedAngle),
    _currentState(IDLE), _currentBarrierPos(_barrierOpenAngle),
    _startMillis(0), _predictedTime(0),
    _buttonState(HIGH), _lastReading(HIGH), _lastDebounceTime(0),
    _blinkTimer(0), _ledToggle(false),
    _apiBaseUrl(apiBaseUrl), _trainId(-1) {}

void TrainPredictionSignal::begin() {
  Serial.begin(115200);
  pinMode(_led1Pin, OUTPUT);
  pinMode(_led2Pin, OUTPUT);
  pinMode(_btnPin, INPUT_PULLUP);
  ledcAttach(_buzzerPin, _buzzerFreq, _buzzerResolution);
  _servo.attach(_servoPin, 500, 2400);
  _servo.write(_barrierOpenAngle);
  _trainId = -1;
  Serial.printf("Train Prediction Signal Setup complete, apiBaseUrl=%s\n", _apiBaseUrl.c_str());
}

void TrainPredictionSignal::update() {
  handleButton();
  handleServo();
  handleWarningLEDSAndSound();
}

bool TrainPredictionSignal::httpRequest(const String& method, const String& endpoint, const String& body, int& httpCode, String& responseBody) {
  String url = _apiBaseUrl + endpoint;
  Serial.printf("TrainPredictionSignal HTTP %s %s\n", method.c_str(), url.c_str());
  bool ok = NetworkController::request(url, method, body, httpCode, responseBody);
  if (!ok) {
    Serial.printf("TrainPredictionSignal::httpRequest failed, httpCode=%d\n", httpCode);
    return false;
  }
  return true;
}

bool TrainPredictionSignal::createTrain() {
  int httpCode;
  String response;

  if (!httpRequest("POST", "/api/v1/railroadcrossing/train/first", "{}", httpCode, response)) {
    Serial.println("createTrain failed");
    return false;
  }

  int idIndex = response.indexOf("\"id\"");
  if (idIndex < 0) {
    Serial.println("createTrain: no id returned");
    return false;
  }

  int colon = response.indexOf(':', idIndex);
  if (colon < 0) {
    Serial.println("createTrain: malformed response");
    return false;
  }

  int comma = response.indexOf(',', colon);
  if (comma < 0) comma = response.indexOf('}', colon);
  if (comma < 0) {
    Serial.println("createTrain: malformed response 2");
    return false;
  }

  String idText = response.substring(colon + 1, comma);
  idText.trim();
  int id = idText.toInt();
  if (id <= 0) {
    Serial.println("createTrain: invalid id");
    return false;
  }

  _trainId = id;
  Serial.printf("createTrain succeeded -> trainId=%d\n", _trainId);
  return true;
}

bool TrainPredictionSignal::updateTrainSecondSensor(float predictedSeconds) {
  if (_trainId <= 0) {
    Serial.println("updateTrainSecondSensor skipped: no trainId");
    return false;
  }

  int httpCode;
  String response;
  String endpoint = "/api/v1/railroadcrossing/train/" + String(_trainId) + "/second";
  String body = "{\"predicted_arrival_seconds\": " + String(predictedSeconds, 2) + "}";

  if (!httpRequest("PUT", endpoint, body, httpCode, response)) {
    Serial.println("updateTrainSecondSensor failed");
    return false;
  }

  Serial.println("updateTrainSecondSensor succeeded");
  return true;
}

bool TrainPredictionSignal::updateTrainCrossed() {
  if (_trainId <= 0) {
    Serial.println("updateTrainCrossed skipped: no trainId");
    return false;
  }

  int httpCode;
  String response;
  String endpoint = "/api/v1/railroadcrossing/train/" + String(_trainId) + "/crossed";

  if (!httpRequest("PUT", endpoint, "{}", httpCode, response)) {
    Serial.println("updateTrainCrossed failed");
    return false;
  }

  Serial.printf("updateTrainCrossed succeeded for trainId=%d\n", _trainId);
  return true;
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