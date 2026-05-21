#ifndef PARKINGNETWORK_H
#define PARKINGNETWORK_H

#include <Arduino.h>

/**
 * @brief Class that handles backend communication for the parking system.
 */
class ParkingNetwork {
public:
  static bool sendSpotUpdate(uint8_t spotNumber, bool isOccupied);

private:
  static bool httpRequest(const String& method, const String& endpoint, const String& body,
                          int& httpCode, String& responseBody);
};

#endif