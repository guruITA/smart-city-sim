#ifndef SPEEDCAMERANETWORK_H
#define SPEEDCAMERANETWORK_H

#include <Arduino.h>

class SpeedCameraNetwork {
public:
  static bool sendMeasurement(float speedKmh, const String& direction, bool isViolation,
                              float speedLimitKmh);

private:
  static bool httpRequest(const String& method, const String& endpoint, const String& body,
                          int& httpCode, String& responseBody);
};

#endif