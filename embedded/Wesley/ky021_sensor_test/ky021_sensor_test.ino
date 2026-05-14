/*
  File: ky021_sensor_test.ino

  Purpose:
  Standalone KY-021 sensor test for the smart traffic-light project.

  What this test proves:
  - The ESP32-S3 can read the KY-021 signal on GPIO 6.
  - The raw input changes when the magnet/car is placed at the detector.
  - The debounced/stable input changes only after a short debounce period.
  - Stable active detection can be observed with timestamps and durations.

  Wiring:
  ESP32-S3 3V3   -> KY-021 VCC
  ESP32-S3 GND   -> KY-021 GND
  ESP32-S3 GPIO6 -> KY-021 signal

  Serial monitor:
  115200 baud
*/

#include <Arduino.h>

// ============================================================
// Sensor settings
// ============================================================

const int SENSOR_PIN = 6;

// Most KY-021 reed-switch modules behave as active LOW when used with INPUT_PULLUP.
// If your serial monitor shows the opposite behaviour, change this to HIGH.
const int SENSOR_ACTIVE_LEVEL = LOW;

// Timing
const unsigned long DEBOUNCE_TIME = 50;
const unsigned long STATUS_PRINT_INTERVAL = 4000;
const unsigned long STABLE_WAITING_PROOF_TIME = 500;

// ============================================================
// Sensor state variables
// ============================================================

int rawPinLevel = HIGH;
bool rawSensorActive = false;
bool lastRawSensorActive = false;

bool stableSensorActive = false;
bool lastStableSensorActive = false;

unsigned long lastRawChangeTime = 0;
unsigned long stableActiveSince = 0;
unsigned long stableInactiveSince = 0;
unsigned long lastStatusPrint = 0;

bool testSawActive = false;
bool testSawInactiveAfterActive = false;
bool testSawStableLongEnough = false;

unsigned int stableActiveEvents = 0;
unsigned int stableInactiveEvents = 0;

// ============================================================
// Helper functions
// ============================================================

const char* activeText(bool active) {
  return active ? "ACTIVE / vehicle detected" : "INACTIVE / no vehicle";
}

const char* passFailText(bool passed) {
  return passed ? "PASSED" : "not passed yet";
}

void printHeader() {
  Serial.println();
  Serial.println("============================================================");
  Serial.println("KY-021 SENSOR TEST START");
  Serial.println("============================================================");
  Serial.println("Goal:");
  Serial.println("- Test the KY-021 separately before combining it with traffic-light logic.");
  Serial.println("- Show raw input, debounced input, and test results in the serial monitor.");
  Serial.println();
  Serial.println("Wiring used:");
  Serial.println("ESP32-S3 3V3   -> KY-021 VCC");
  Serial.println("ESP32-S3 GND   -> KY-021 GND");
  Serial.println("ESP32-S3 GPIO6 -> KY-021 signal");
  Serial.println();
  Serial.println("Testing steps:");
  Serial.println("1. Leave the magnet/car away from the KY-021.");
  Serial.println("2. Place the magnet/car above the sensor.");
  Serial.println("3. Keep it there for at least 0.5 seconds.");
  Serial.println("4. Remove the magnet/car again.");
  Serial.println();
  Serial.println("Expected result:");
  Serial.println("- The stable state changes to ACTIVE when the magnet/car is detected.");
  Serial.println("- The stable state changes back to INACTIVE when it is removed.");
  Serial.println("============================================================");
  Serial.println();
}

void printResultSummary() {
  Serial.println();
  Serial.println("---------------- SENSOR TEST RESULT SUMMARY ----------------");
  Serial.print("Test 1 - Sensor becomes active: ");
  Serial.println(passFailText(testSawActive));

  Serial.print("Test 2 - Sensor becomes inactive after active detection: ");
  Serial.println(passFailText(testSawInactiveAfterActive));

  Serial.print("Test 3 - Sensor stays active for at least 500 ms: ");
  Serial.println(passFailText(testSawStableLongEnough));

  Serial.print("Stable ACTIVE events counted: ");
  Serial.println(stableActiveEvents);

  Serial.print("Stable INACTIVE events counted: ");
  Serial.println(stableInactiveEvents);

  Serial.println("------------------------------------------------------------");
  Serial.println();
}

void printLiveStatus(unsigned long now) {
  Serial.println();
  Serial.println("---------------- KY-021 LIVE STATUS ----------------");
  Serial.print("timeMs: ");
  Serial.println(now);

  Serial.print("raw GPIO level: ");
  Serial.println(rawPinLevel == HIGH ? "HIGH" : "LOW");

  Serial.print("raw sensor state: ");
  Serial.println(activeText(rawSensorActive));

  Serial.print("stable sensor state: ");
  Serial.println(activeText(stableSensorActive));

  if (stableSensorActive) {
    Serial.print("stable active duration ms: ");
    Serial.println(now - stableActiveSince);
  } else {
    Serial.print("stable inactive duration ms: ");
    Serial.println(now - stableInactiveSince);
  }

  Serial.println("-----------------------------------------------------");
}

void updateSensor(unsigned long now) {
  rawPinLevel = digitalRead(SENSOR_PIN);
  rawSensorActive = (rawPinLevel == SENSOR_ACTIVE_LEVEL);

  // Step 1: detect raw changes immediately.
  if (rawSensorActive != lastRawSensorActive) {
    lastRawChangeTime = now;
    lastRawSensorActive = rawSensorActive;

    Serial.println();
    Serial.println("[RAW CHANGE DETECTED]");
    Serial.print("timeMs: ");
    Serial.println(now);
    Serial.print("raw sensor state is now: ");
    Serial.println(activeText(rawSensorActive));
    Serial.println("Waiting for debounce time before accepting this as stable...");
  }

  // Step 2: accept the new state only after it stayed unchanged long enough.
  if ((now - lastRawChangeTime) >= DEBOUNCE_TIME) {
    if (stableSensorActive != rawSensorActive) {
      stableSensorActive = rawSensorActive;

      Serial.println();
      Serial.println("[STABLE SENSOR STATE CHANGED]");
      Serial.print("timeMs: ");
      Serial.println(now);
      Serial.print("stable sensor state is now: ");
      Serial.println(activeText(stableSensorActive));

      if (stableSensorActive) {
        stableActiveSince = now;
        stableActiveEvents++;
        testSawActive = true;

        Serial.println("Result:");
        Serial.println("- The KY-021 detected the magnet/car.");
        Serial.println("- Sensor input test step ACTIVE is PASSED.");
      } else {
        stableInactiveSince = now;
        stableInactiveEvents++;

        if (testSawActive) {
          testSawInactiveAfterActive = true;
        }

        Serial.println("Result:");
        Serial.println("- The KY-021 returned to no-vehicle state.");
        Serial.println("- Sensor input test step INACTIVE is PASSED after removal.");
      }

      printResultSummary();
    }
  }
}

void updateLongStableProof(unsigned long now) {
  if (stableSensorActive && !testSawStableLongEnough) {
    if ((now - stableActiveSince) >= STABLE_WAITING_PROOF_TIME) {
      testSawStableLongEnough = true;

      Serial.println();
      Serial.println("[STABLE ACTIVE PROOF]");
      Serial.print("The sensor stayed ACTIVE for at least ");
      Serial.print(STABLE_WAITING_PROOF_TIME);
      Serial.println(" ms.");
      Serial.println("Result:");
      Serial.println("- Stable vehicle presence can be detected.");
      Serial.println("- This is suitable for later waiting-car interpretation.");
      printResultSummary();
    }
  }
}

// ============================================================
// Setup
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(SENSOR_PIN, INPUT_PULLUP);

  unsigned long now = millis();

  rawPinLevel = digitalRead(SENSOR_PIN);
  rawSensorActive = (rawPinLevel == SENSOR_ACTIVE_LEVEL);
  lastRawSensorActive = rawSensorActive;

  stableSensorActive = rawSensorActive;
  lastStableSensorActive = stableSensorActive;

  if (stableSensorActive) {
    stableActiveSince = now;
  } else {
    stableInactiveSince = now;
  }

  printHeader();

  Serial.println("[INITIAL SENSOR READ]");
  Serial.print("raw GPIO level: ");
  Serial.println(rawPinLevel == HIGH ? "HIGH" : "LOW");
  Serial.print("initial stable sensor state: ");
  Serial.println(activeText(stableSensorActive));
  Serial.println();

  if (stableSensorActive) {
    Serial.println("Note: The sensor starts ACTIVE. Move the magnet/car away if this is not intended.");
  } else {
    Serial.println("Note: The sensor starts INACTIVE. Place the magnet/car above the KY-021 to test detection.");
  }
}

// ============================================================
// Main loop
// ============================================================

void loop() {
  unsigned long now = millis();

  updateSensor(now);
  updateLongStableProof(now);

  if ((now - lastStatusPrint) >= STATUS_PRINT_INTERVAL) {
    lastStatusPrint = now;
    printLiveStatus(now);
  }
}
