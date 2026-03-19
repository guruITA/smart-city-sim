#include <Arduino.h>

const int RED_PIN = 4;
const int YELLOW_PIN = 5;
const int GREEN_PIN = 6;


void allOff() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void redOn() {
  allOff();
  digitalWrite(RED_PIN, HIGH);
  Serial.println("RED");
  delay(5000);
}

void yellowOn() {
  allOff();
  digitalWrite(YELLOW_PIN, HIGH);
  Serial.println("YELLOW");
  delay(2000);
}

void greenOn() {
  allOff();
  digitalWrite(GREEN_PIN, HIGH);
  Serial.println("GREEN");
  delay(5000);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  //register pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // Safe startup: red first
  digitalWrite(LED_BUILTIN, LOW);
  redOn();
}

void loop() {
  redOn();
  greenOn();
  yellowOn();
}