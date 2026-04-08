void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("HELLO FROM ESP32-S3");
}

void loop() {
  Serial.println(millis());
  delay(1000);
}