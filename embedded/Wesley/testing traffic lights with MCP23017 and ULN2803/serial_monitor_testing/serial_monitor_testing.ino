void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Serial test start");
  Serial.println("If you can read this, the serial monitor works.");
}

void loop() {
  Serial.println("Serial monitor heartbeat");
  delay(1000);
}