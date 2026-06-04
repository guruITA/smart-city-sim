#include "TrainPredictionSignal.h"
#include "NetworkController.h"
#include "OverrideController.h"
#include <WiFi.h>
#include <HTTPClient.h>

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
    _blinkTimer(0), _ledToggle(false),
    _trainId(-1) {}

void TrainPredictionSignal::begin() {
  Serial.begin(115200);
  pinMode(_led1Pin, OUTPUT);
  pinMode(_led2Pin, OUTPUT);
  pinMode(_btnPin, INPUT_PULLUP);
  ledcAttach(_buzzerPin, _buzzerFreq, _buzzerResolution);
  _servo.attach(_servoPin, 500, 2400);
  _servo.write(_barrierOpenAngle);
  _trainId = -1;
  String currentApiBaseUrl = NetworkController::getApiBaseUrl();
  Serial.printf("Train Prediction Signal Setup complete, apiBaseUrl=%s\n", currentApiBaseUrl.c_str());
}

void TrainPredictionSignal::update() {
  // Backend override: lock the crossing to its safe state (barrier down, lights
  // and buzzer warning) for an emergency, ignoring the button and train logic
  // until the override is cleared.
  if (OverrideController::isCommand("barrier", "close")) {
    applyOverrideClose();
    return;
  }

  handleButton();
  handleServo();
  handleWarningLEDSAndSound();
}