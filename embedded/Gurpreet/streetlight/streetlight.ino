#define LDR_PIN 4
#define RELAY_PIN 5

int lightLevel = 0;
int threshold = 500;

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  Serial.println("Automatic Street Light System");
}

void loop() {
  lightLevel = analogRead(LDR_PIN); 

  Serial.print("Light Level: ");
  Serial.println(lightLevel);
  
  if (lightLevel < threshold) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("It's dark! turning light on the light...");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("It's bright! Turning off the light...");
  }
  
  delay(1000); 
}
