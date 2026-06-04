#ifndef OVERRIDE_CONTROLLER_H
#define OVERRIDE_CONTROLLER_H

#include <Arduino.h>

// Firmware side of the backend override contract.
//
// The back-end requirement says it must be possible to override the decisions
// of individual hubs from the backend, for example to set all traffic lights on
// a road to red for an emergency vehicle. The backend exposes the active
// overrides at GET /api/v1/override/active. This controller polls that endpoint
// on a timer and remembers the forced command per tile target. Each tile calls
// isCommand()/commandFor() at the top of its update() and obeys the forced
// command instead of its own sensor logic, until the override is cleared.
//
// One poll fetches every active override for the whole board, so the network
// cost is a single GET per interval no matter how many tiles run here.
class OverrideController {
public:
  // Tile targets this firmware understands. Matches EMERGENCY_COMMANDS in the
  // backend override router.
  static const int TARGET_COUNT = 5;

  // Start polling. Call once in setup() after the API base URL is set on
  // NetworkController. The first update() then polls immediately.
  static void begin(unsigned long pollIntervalMs = 2000);

  // Call every loop(). Polls the backend only when its own timer elapses, so it
  // is cheap to call on every iteration.
  static void update();

  // True when the backend currently forces exactly `command` on `target`.
  static bool isCommand(const String& target, const char* command);

  // True when any override is active for `target`.
  static bool hasOverride(const String& target);

  // The forced command for `target`, or "" when none is active.
  static String commandFor(const String& target);

  // millis() timestamp of the last successful poll, or 0 if never.
  static unsigned long lastSyncMs();

private:
  OverrideController() = delete;

  // Commands are short words (all_red, force_on, show_alert, close, full).
  static const int MAX_CMD_LEN = 24;

  static const char* const _targets[TARGET_COUNT];
  // Fixed buffers, not String, so a read from the e-ink FreeRTOS task cannot
  // race a String reallocation while poll() writes from the main loop.
  static char _commands[TARGET_COUNT][MAX_CMD_LEN];

  static unsigned long _pollIntervalMs;
  static unsigned long _lastPollMs;
  static unsigned long _lastSyncMs;
  static bool _started;

  static int indexOfTarget(const String& target);
  static void poll();
  static String extractCommandForTarget(const String& body, const char* target);
};

#endif
