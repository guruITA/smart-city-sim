// Pedestrian lights side A
const int pedA_Green = 5;
const int pedA_Red   = 6;

// Pedestrian lights side B
const int pedB_Green = 1;
const int pedB_Red   = 2;

// Buttons
const int buttonA = 12;
const int buttonB = 13;

bool request = false;
bool systemBusy = false;

unsigned long lastPressTime = 0;
const int debounceDelay = 200;

void setup() {
  pinMode(pedA_Green, OUTPUT);
  pinMode(pedA_Red, OUTPUT);
  pinMode(pedB_Green, OUTPUT);
  pinMode(pedB_Red, OUTPUT);

  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);

  // Default RED
  digitalWrite(pedA_Red, HIGH);
  digitalWrite(pedB_Red, HIGH);
}

void loop() {

  // Debounce check
  if ((millis() - lastPressTime) > debounceDelay) {

    if ((digitalRead(buttonA) == LOW || digitalRead(buttonB) == LOW) && !systemBusy) {
      request = true;
      lastPressTime = millis();
    }
  }

  if (request) {
    systemBusy = true;
    runCrossing();
    request = false;
    systemBusy = false;
  }
}

void runCrossing() {

  // WAIT TIME (cars stop)
  delay(3000);

  // GREEN
  digitalWrite(pedA_Red, LOW);
  digitalWrite(pedB_Red, LOW);

  digitalWrite(pedA_Green, HIGH);
  digitalWrite(pedB_Green, HIGH);

  delay(7000); // longer crossing time

  // BACK TO RED
  digitalWrite(pedA_Green, LOW);
  digitalWrite(pedB_Green, LOW);

  digitalWrite(pedA_Red, HIGH);
  digitalWrite(pedB_Red, HIGH);

  // COOLDOWN (prevent spam)
  delay(3000);
}