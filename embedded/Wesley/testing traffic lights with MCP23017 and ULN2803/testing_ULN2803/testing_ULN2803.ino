const int IN_PIN = 6;

void setup() {
  pinMode(IN_PIN, OUTPUT);
}

void loop() {
  digitalWrite(IN_PIN, HIGH);  // ULN input on -> output sinks -> LED on
  delay(500);
  digitalWrite(IN_PIN, LOW);   // LED off
  delay(500);
}