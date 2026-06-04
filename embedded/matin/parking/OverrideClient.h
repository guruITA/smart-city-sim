#ifndef OVERRIDE_CLIENT_H
#define OVERRIDE_CLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <string.h>

// Self-contained backend override poller for a single standalone tile sketch.
//
// The back-end requirement says it must be possible to override the decisions of
// individual hubs from the backend (for example all traffic lights red for an
// emergency vehicle). The backend exposes the active overrides at
// GET /api/v1/override/active?target=<tile>. This client polls that endpoint on
// a timer and remembers the forced command, so the sketch can obey it instead of
// its own sensor logic until the backend clears it.
//
// Drop this file next to your .ino and use it like:
//   OverrideClient tileOverride("http://<pi-ip>:<port>", "parking");
//   tileOverride.begin();            // in setup(), after WiFi is up
//   tileOverride.update();           // once per loop()
//   if (tileOverride.is("full")) {   // obey the forced command
//     ...
//   }
class OverrideClient {
public:
  OverrideClient(const String& apiBaseUrl, const String& target, unsigned long pollIntervalMs = 2000)
    : _baseUrl(apiBaseUrl), _target(target), _pollIntervalMs(pollIntervalMs),
      _lastPollMs(0), _started(false) {
    _command[0] = '\0';
  }

  void begin() {
    _started = true;
    _lastPollMs = 0;
  }

  // Polls only when its own timer elapses, so it is cheap to call every loop().
  void update() {
    if (!_started) {
      return;
    }
    unsigned long now = millis();
    if (_lastPollMs != 0 && (now - _lastPollMs) < _pollIntervalMs) {
      return;
    }
    _lastPollMs = now;
    poll();
  }

  // True when the backend currently forces exactly `command` on this tile.
  bool is(const char* command) const {
    return strcmp(_command, command) == 0;
  }

  // True when any override is active for this tile.
  bool active() const {
    return _command[0] != '\0';
  }

  // The forced command, or "" when none is active.
  String command() const {
    return String(_command);
  }

private:
  static const int MAX_CMD_LEN = 24;

  void poll() {
    if (WiFi.status() != WL_CONNECTED) {
      return;
    }

    HTTPClient http;
    // Keep the poll snappy so a missing backend never stalls the loop.
    http.setConnectTimeout(1500);
    http.setTimeout(1500);

    String url = _baseUrl + "/api/v1/override/active?target=" + _target;
    if (!http.begin(url)) {
      return;
    }

    int httpCode = http.GET();
    if (httpCode == 200) {
      String body = http.getString();

      // The backend returns the active overrides newest first, so read the first
      // command value. Reading the newest (not just any match in the array)
      // avoids obeying a stale override a later command already replaced.
      char next[MAX_CMD_LEN];
      next[0] = '\0';

      int c = body.indexOf("\"command\"");
      if (c >= 0) {
        int colon = body.indexOf(':', c);
        int q1 = (colon >= 0) ? body.indexOf('"', colon + 1) : -1;
        int q2 = (q1 >= 0) ? body.indexOf('"', q1 + 1) : -1;
        if (q1 >= 0 && q2 > q1) {
          String value = body.substring(q1 + 1, q2);
          strncpy(next, value.c_str(), MAX_CMD_LEN - 1);
          next[MAX_CMD_LEN - 1] = '\0';
        }
      }

      if (strcmp(next, _command) != 0) {
        strncpy(_command, next, MAX_CMD_LEN - 1);
        _command[MAX_CMD_LEN - 1] = '\0';
        Serial.printf("[Override] %s -> %s\n", _target.c_str(),
                      _command[0] ? _command : "(cleared)");
      }
    } else if (httpCode > 0) {
      Serial.printf("[Override] poll HTTP %d, keeping last known state\n", httpCode);
    }
    // On a connection error we keep the last known command on purpose, so a
    // brief network blip never drops the tile out of an emergency override.

    http.end();
  }

  String _baseUrl;
  String _target;
  unsigned long _pollIntervalMs;
  unsigned long _lastPollMs;
  bool _started;
  char _command[MAX_CMD_LEN];
};

#endif
