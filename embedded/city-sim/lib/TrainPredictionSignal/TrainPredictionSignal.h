#ifndef TRAIN_PREDICTION_SIGNAL_H
#define TRAIN_PREDICTION_SIGNAL_H

#include <Arduino.h>
#include <ESP32Servo.h>

class TrainPredictionSignal {
private:
  // Pins
  int _led1Pin;
  int _led2Pin;
  int _buzzerPin;
  int _servoPin;
  int _btnPin;

  // Distances
  int _aBDistance;
  int _bCDistance;

  // Other parameters
  unsigned int _safetyMargin;
  int _buzzerFreq;
  int _buzzerResolution;
  int _barrierOpenAngle;
  int _barrierClosedAngle;

  // State
  enum State {
    IDLE,
    MEASURING,
    WAITING
  };
  State _currentState;

  // Servo
  Servo _servo;
  int _currentBarrierPos;

  // Timing
  unsigned long _startMillis;
  unsigned long _predictedTime;

  // Button debouncing
  bool _buttonState;
  bool _lastReading;
  unsigned long _lastDebounceTime;
  const unsigned long _debounceDelay = 50;

  // LED blinking
  unsigned long _blinkTimer;
  const unsigned long _blinkInterval = 400;
  bool _ledToggle;

  // Train state
  int _trainId;

  // Methods
  void handleServo();
  void handleWarningLEDSAndSound();
  void handleButton();
  void applyOverrideClose();

  bool httpRequest(const String& method, const String& endpoint, const String& body, int& httpCode, String& responseBody);
  bool createTrain();
  bool updateTrainSecondSensor(float predictedSeconds);
  bool updateTrainCrossed();

public:
  TrainPredictionSignal(int led1Pin, int led2Pin, int buzzerPin, int servoPin, int btnPin,
                        int aBDistance = 200, int bCDistance = 1000, unsigned int safetyMargin = 5000,
                        int buzzerFreq = 1000, int buzzerResolution = 8,
                        int barrierOpenAngle = 0, int barrierClosedAngle = 90);

  void begin();
  void update();
};

#endif
