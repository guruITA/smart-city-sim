#ifndef TRAIN_DETECTOR_H
#define TRAIN_DETECTOR_H


#include <Arduino.h>

class TrainDetector {
public:
  TrainDetector(int buttonPin, int aBDistance, int bCDistance);

  void begin();
  void update();

  bool firstTriggered();
  bool secondTriggered();
  bool trainPassed();

  float getPredictedTime();

private:
  int _btnPin;
  int _aBDistance;
  int _bCDistance;

  unsigned long _startMillis;
  unsigned long _predictedTime;

  bool _buttonState;
  bool _lastReading;
  unsigned long _lastDebounceTime;
  const unsigned long _debounceDelay = 50;

  enum State { IDLE, MEASURING, WAITING };
  State _state;

  bool _firstEvent;
  bool _secondEvent;
  bool _thirdEvent;
};

#endif