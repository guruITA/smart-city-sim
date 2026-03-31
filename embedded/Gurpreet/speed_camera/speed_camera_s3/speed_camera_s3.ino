#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define IR1_PIN 6
#define IR2_PIN 12
#define OLED_SDA_PIN 18
#define OLED_SCL_PIN 46
#define CAM_TRIGGER_PIN 7

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

#define IR_ACTIVE_STATE LOW

#define SENSOR_DISTANCE_M 0.10f
#define SPEED_LIMIT_KMH 1.0f

#define PASS_TIMEOUT_US 2000000UL
#define CAM_TRIGGER_PULSE_MS 100
#define MEASUREMENT_COOLDOWN_MS 500
#define RESULT_SCREEN_HOLD_MS 2000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
bool displayReady = false;

enum MeasureState { IDLE, WAIT_FOR_SECOND_SENSOR };

MeasureState measureState = IDLE;
int firstSensor = 0;
unsigned long tStartUs = 0;

bool lastIr1Active = false;
bool lastIr2Active = false;

float lastSpeedKmh = 0.0f;
bool lastTooFast = false;
String lastDirection = "-";
unsigned long lastEventMs = 0;
unsigned long lastMeasurementDoneMs = 0;

unsigned long lastUiRefresh = 0;

bool sensorActive(int pin) {
  return digitalRead(pin) == IR_ACTIVE_STATE;
}

void pulseCameraTrigger() {
  digitalWrite(CAM_TRIGGER_PIN, HIGH);
  delay(CAM_TRIGGER_PULSE_MS);
  digitalWrite(CAM_TRIGGER_PIN, LOW);
}

void drawBootScreen() {
  if (!displayReady) {
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Speed Camera S3 start...");
  display.println("IR1 = GPIO6");
  display.println("IR2 = GPIO12");
  display.println("OLED SDA/SCL = 18/46");
  display.println("CAM trigger = GPIO7");
  display.display();
}

void drawStatusScreen(bool ir1, bool ir2) {
  if (!displayReady) {
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println("Speed Camera S3");
  display.println("----------------");

  display.print("IR1: ");
  display.println(ir1 ? "ACTIVE" : "REST");

  display.print("IR2: ");
  display.println(ir2 ? "ACTIVE" : "REST");

  display.print("Last: ");
  display.print(lastSpeedKmh, 1);
  display.println(" km/u");

  display.print("Limit: ");
  display.print(SPEED_LIMIT_KMH, 1);
  display.println(" km/u");

  display.print("Direction: ");
  display.println(lastDirection);

  if (millis() - lastMeasurementDoneMs < MEASUREMENT_COOLDOWN_MS) {
    display.println("STATUS: cooldown");
  } else {
    display.println("STATUS: waiting");
  }

  display.display();
}

void drawMeasurementScreen(float speedKmh, bool tooFast, const String& direction,
                           unsigned long dtUs) {
  if (!displayReady) {
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println("MEASUREMENT READY");
  display.println("------------");

  display.print("Direction: ");
  display.println(direction);

  display.print("Time: ");
  display.print(dtUs / 1000.0f, 1);
  display.println(" ms");

  display.print("Speed: ");
  display.print(speedKmh, 1);
  display.println(" km/u");

  display.print("Limit: ");
  display.print(SPEED_LIMIT_KMH, 1);
  display.println(" km/u");

  if (tooFast) {
    display.println("Result: TOO FAST!");
  } else {
    display.println("Result: OK");
  }

  display.display();
}

void resetMeasurement() {
  measureState = IDLE;
  firstSensor = 0;
  tStartUs = 0;
}

void processMeasurement(int fromSensor, int toSensor, unsigned long dtUs) {
  if (dtUs == 0) {
    resetMeasurement();
    return;
  }

  float dtSeconds = dtUs / 1000000.0f;
  float speedMs = SENSOR_DISTANCE_M / dtSeconds;
  float speedKmh = speedMs * 3.6f;
  bool tooFast = speedKmh > SPEED_LIMIT_KMH;

  lastSpeedKmh = speedKmh;
  lastTooFast = tooFast;
  lastDirection = (fromSensor == 1 && toSensor == 2) ? "1->2" : "2->1";
  lastEventMs = millis();
  lastMeasurementDoneMs = millis();

  Serial.println();
  Serial.println("=== MEASUREMENT ===");
  Serial.print("Direction: ");
  Serial.println(lastDirection);
  Serial.print("Time (us): ");
  Serial.println(dtUs);
  Serial.print("Distance (m): ");
  Serial.println(SENSOR_DISTANCE_M, 3);
  Serial.print("Speed (km/u): ");
  Serial.println(speedKmh, 2);
  Serial.print("Too Fast: ");
  Serial.println(tooFast ? "YES" : "NO");

  drawMeasurementScreen(speedKmh, tooFast, lastDirection, dtUs);

  if (tooFast) {
    Serial.println("Camera trigger HIGH");
    pulseCameraTrigger();
  }

  resetMeasurement();
}

void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);

  pinMode(CAM_TRIGGER_PIN, OUTPUT);
  digitalWrite(CAM_TRIGGER_PIN, LOW);

  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);

  if (display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    displayReady = true;
    drawBootScreen();
    delay(1500);
    drawStatusScreen(false, false);
  } else {
    Serial.println("OLED not found.");
  }

  lastIr1Active = sensorActive(IR1_PIN);
  lastIr2Active = sensorActive(IR2_PIN);

  Serial.println("Starting ESP32-S3.");
  Serial.println("Waiting on IR-measurements...");
}

void loop() {
  bool ir1 = sensorActive(IR1_PIN);
  bool ir2 = sensorActive(IR2_PIN);

  bool edge1 = ir1 && !lastIr1Active;
  bool edge2 = ir2 && !lastIr2Active;

  bool inCooldown = (millis() - lastMeasurementDoneMs) < MEASUREMENT_COOLDOWN_MS;

  switch (measureState) {
  case IDLE:
    if (!inCooldown) {
      if (edge1 && !ir2) {
        firstSensor = 1;
        tStartUs = micros();
        measureState = WAIT_FOR_SECOND_SENSOR;
        Serial.println("Start on IR1");
      } else if (edge2 && !ir1) {
        firstSensor = 2;
        tStartUs = micros();
        measureState = WAIT_FOR_SECOND_SENSOR;
        Serial.println("Start on IR2");
      }
    }
    break;

  case WAIT_FOR_SECOND_SENSOR: {
    unsigned long elapsedUs = micros() - tStartUs;

    if (elapsedUs > PASS_TIMEOUT_US) {
      Serial.println("Timeout, measurement reset.");
      resetMeasurement();
    } else if (firstSensor == 1 && edge2) {
      processMeasurement(1, 2, elapsedUs);
    } else if (firstSensor == 2 && edge1) {
      processMeasurement(2, 1, elapsedUs);
    }
    break;
  }
  }

  if (millis() - lastUiRefresh > 150) {
    if ((millis() - lastEventMs) >= RESULT_SCREEN_HOLD_MS) {
      drawStatusScreen(ir1, ir2);
    }
    lastUiRefresh = millis();
  }

  lastIr1Active = ir1;
  lastIr2Active = ir2;
}