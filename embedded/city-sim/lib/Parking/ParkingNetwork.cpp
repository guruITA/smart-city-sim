#include "ParkingNetwork.h"

#include "NetworkController.h"

/**
 * @brief Sends an HTTP request to the backend.
 *
 * This function builds the full backend URL, sends the request through the
 * NetworkController, and checks whether the backend response was successful.
 *
 * @param method HTTP method used for the request, such as "POST" or "GET".
 * @param endpoint API endpoint that is added after the base URL.
 * @param body Request body that is sent to the backend.
 * @param httpCode Stores the HTTP response code returned by the backend.
 * @param responseBody Stores the response body returned by the backend.
 * @return true when the request was successful, otherwise false.
 */
bool ParkingNetwork::httpRequest(const String& method, const String& endpoint, const String& body,
                                 int& httpCode, String& responseBody) {

  // Build the full API URL from the base URL and endpoint.
  String url = NetworkController::buildUrl(endpoint);

  // Print the HTTP method and URL for debugging.
  Serial.printf("ParkingNetwork HTTP %s %s\n", method.c_str(), url.c_str());

  // Send the HTTP request through the shared network controller.
  bool ok = NetworkController::request(url, method, body, httpCode, responseBody);

  // Stop when the request could not be sent.
  if (!ok) {
    Serial.printf("ParkingNetwork::httpRequest failed, httpCode=%d\n", httpCode);
    return false;
  }

  // Stop when the backend returns an error code.
  if (httpCode < 200 || httpCode >= 300) {
    Serial.printf("ParkingNetwork backend returned HTTP %d\n", httpCode);
    Serial.println(responseBody);
    return false;
  }

  // The request was sent and the backend returned a success code.
  return true;
}

/**
 * @brief Sends the occupied or free state of one parking spot to the backend.
 *
 * This function sends the final parking state that was already determined
 * by the embedded parking logic. The backend only stores this status so the
 * frontend can show the correct parking state.
 *
 * @param spotNumber Number of the parking spot that is being updated.
 * @param isOccupied True when the spot is occupied, false when the spot is free.
 * @return true when the backend update was successful, otherwise false.
 */
bool ParkingNetwork::sendSpotUpdate(uint8_t spotNumber, bool isOccupied) {

  // Do not send data when WiFi is not connected.
  if (!NetworkController::connected()) {
    Serial.println("ParkingNetwork skipped: WiFi offline.");
    return false;
  }

  // Prepare variables for the backend response.
  int httpCode = -1;
  String responseBody;

  // Build the parking update endpoint with the spot number
  String endpoint = "/api/v1/parking/update/";
  endpoint += String(spotNumber);

  // Add the occupied or free state as a query parameter.
  endpoint += "?is_occupied=";
  endpoint += isOccupied ? "true" : "false";

  // Send the status update to the backend.
  bool ok = httpRequest("POST", endpoint, "", httpCode, responseBody);

  // Stop when the backend update failed.
  if (!ok) {
    Serial.println("ParkingNetwork sendSpotUpdate failed.");
    return false;
  }

  // Print the backend response for debugging.
  Serial.println("ParkingNetwork sendSpotUpdate succeeded.");
  Serial.print("Backend response: ");
  Serial.println(responseBody);

  // The backend update was successful.
  return true;
}