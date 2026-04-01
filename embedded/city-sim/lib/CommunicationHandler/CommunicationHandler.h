#ifndef COMMUNICATION_CONTROLLER_H
#define COMMUNICATION_CONTROLLER_H

#include <Arduino.h>

class CommunicationHandler {
public:
  bool createTrain(int& trainId);
  bool sendPrediction(int trainId, float seconds);
  bool sendCrossed(int trainId);

private:
  bool request(const String& method, const String& endpoint, const String& body, String& response);
};

#endif