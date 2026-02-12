// First ESP32 sketch: blink the built-in LED
// Arduino Docs: Home/Referances/Language Reference (https://docs.arduino.cc/language-reference/)

#define const int LED_PIN = LED_BUILTIN;  // Built-in LED on many ESP32-S3 DevKit boards

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // LED on
  delay(500);
  digitalWrite(LED_PIN, LOW);  // LED off
  delay(500);
}