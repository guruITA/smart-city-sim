#include <Arduino.h>

#define LED LED_BUILTIN
#define BTN 0
#define A_B_DISTANCE 200
#define B_C_DISTANCE 1000

enum State {
  IDLE,
  MEASURING,
  WAITING
};

State currentState = IDLE;

const unsigned int safetyMargin = 5000;

unsigned long startMillis;
unsigned long predictedTime;

bool buttonState = HIGH;
bool lastReading = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void handleButton() {
  bool reading = digitalRead(BTN);

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {

        switch (currentState) {

          case IDLE:
            startMillis = millis();
            currentState = MEASURING;
            Serial.println("State: MEASURING");
            break;

          case MEASURING:
            predictedTime = (millis() - startMillis) * (B_C_DISTANCE / A_B_DISTANCE);
            startMillis = millis();
            currentState = WAITING;
            Serial.println("State: WAITING");
            Serial.print("Predicted time: ");
            Serial.print(predictedTime / 1000.0, 2);
            Serial.println(" seconds");
            break;

          case WAITING:
            currentState = IDLE;
            digitalWrite(LED, LOW);
            Serial.println("State: IDLE");
            break;
        }
      }
    }
  }

  lastReading = reading;
}


void handleState() {
  switch (currentState) {

    case IDLE:
      digitalWrite(LED, LOW);
      break;

    case MEASURING:
      break;

    case WAITING:
      if (millis() - startMillis >= predictedTime) {
        digitalWrite(LED, HIGH);
      } else {
        digitalWrite(LED, LOW);
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
}

void loop() {
  handleButton();
  handleState();
}
