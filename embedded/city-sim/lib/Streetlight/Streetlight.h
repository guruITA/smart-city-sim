#ifndef STREETLIGHT_H
#define STREETLIGHT_H

#include <Arduino.h>

class StreetLight {

private:
  int _ldrPin;
  int _relayPin;
  int _threshold;

  int _interval;
  unsigned long _previousMillis;

public:
  StreetLight(int ldrPin, int relayPin, int threshold, int interval);

  void begin();
  void update();
};

#endif