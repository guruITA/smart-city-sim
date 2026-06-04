#include "OverrideController.h"
#include "NetworkController.h"
#include <HTTPClient.h>
#include <string.h>

const char* const OverrideController::_targets[OverrideController::TARGET_COUNT] = {
  "traffic", "barrier", "streetlight", "parking", "eink"
};

char OverrideController::_commands[OverrideController::TARGET_COUNT][OverrideController::MAX_CMD_LEN] = {{0}};

unsigned long OverrideController::_pollIntervalMs = 2000;
unsigned long OverrideController::_lastPollMs = 0;
unsigned long OverrideController::_lastSyncMs = 0;
bool OverrideController::_started = false;

void OverrideController::begin(unsigned long pollIntervalMs) {
  _pollIntervalMs = pollIntervalMs;
  _lastPollMs = 0;
  _started = true;
  for (int i = 0; i < TARGET_COUNT; ++i) {
    _commands[i][0] = '\0';
  }
  Serial.println("OverrideController started");
}

void OverrideController::update() {
  if (!_started) {
    return;
  }

  unsigned long now = millis();
  // _lastPollMs == 0 means we have never polled, so fire the first poll now.
  if (_lastPollMs != 0 && (now - _lastPollMs) < _pollIntervalMs) {
    return;
  }

  _lastPollMs = now;
  poll();
}

void OverrideController::poll() {
  if (!NetworkController::connected()) {
    Serial.println("[Override] skip poll: network not connected");
    return;
  }

  String url = NetworkController::buildUrl("api/v1/override/active");

  HTTPClient http;
  // Keep the poll snappy so a missing backend never stalls the main loop.
  http.setConnectTimeout(1500);
  http.setTimeout(1500);

  if (!http.begin(url)) {
    Serial.println("[Override] http.begin failed");
    return;
  }

  int httpCode = http.GET();
  if (httpCode != 200) {
    // On any failure keep the last known state instead of dropping overrides,
    // so a brief network blip never flips the whole city out of emergency.
    Serial.printf("[Override] poll HTTP %d, keeping last known state\n", httpCode);
    http.end();
    return;
  }

  String body = http.getString();
  http.end();

  for (int i = 0; i < TARGET_COUNT; ++i) {
    String cmd = extractCommandForTarget(body, _targets[i]);
    if (cmd != String(_commands[i])) {
      strncpy(_commands[i], cmd.c_str(), MAX_CMD_LEN - 1);
      _commands[i][MAX_CMD_LEN - 1] = '\0';
      Serial.printf("[Override] %s -> %s\n", _targets[i],
                    _commands[i][0] ? _commands[i] : "(cleared)");
    }
  }

  _lastSyncMs = millis();
}

// The /active response is a JSON array of override objects. The backend emits
// them with fields in order id, target, command, reason, ... so the command of
// an object is the first "command" key after that object's "target" key. We do
// a small manual scan instead of pulling in ArduinoJson, matching how the other
// tiles parse backend responses.
String OverrideController::extractCommandForTarget(const String& body, const char* target) {
  String needle = String("\"target\":\"") + target + "\"";
  int t = body.indexOf(needle);
  if (t < 0) {
    // Tolerate a space after the colon in case the backend pretty-prints.
    needle = String("\"target\": \"") + target + "\"";
    t = body.indexOf(needle);
  }
  if (t < 0) {
    return String("");
  }

  int c = body.indexOf("\"command\"", t);
  if (c < 0) {
    return String("");
  }

  int colon = body.indexOf(':', c);
  if (colon < 0) {
    return String("");
  }

  int q1 = body.indexOf('"', colon + 1);
  if (q1 < 0) {
    return String("");
  }

  int q2 = body.indexOf('"', q1 + 1);
  if (q2 < 0) {
    return String("");
  }

  return body.substring(q1 + 1, q2);
}

int OverrideController::indexOfTarget(const String& target) {
  for (int i = 0; i < TARGET_COUNT; ++i) {
    if (target == _targets[i]) {
      return i;
    }
  }
  return -1;
}

bool OverrideController::isCommand(const String& target, const char* command) {
  int i = indexOfTarget(target);
  if (i < 0) {
    return false;
  }
  return strcmp(_commands[i], command) == 0;
}

bool OverrideController::hasOverride(const String& target) {
  int i = indexOfTarget(target);
  if (i < 0) {
    return false;
  }
  return _commands[i][0] != '\0';
}

String OverrideController::commandFor(const String& target) {
  int i = indexOfTarget(target);
  if (i < 0) {
    return String("");
  }
  return String(_commands[i]);
}

unsigned long OverrideController::lastSyncMs() {
  return _lastSyncMs;
}
