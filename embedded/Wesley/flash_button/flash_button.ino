#include <Arduino.h>
#include <FastLED.h>

#define LED_FASTLED 48 
#define BOOTBUTTON 0

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BOOTBUTTON, INPUT_PULLUP);
}
  
void loop() {
  bool currentButton = digitalRead(BOOTBUTTON);

  if (currentButton == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Light on");
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}  