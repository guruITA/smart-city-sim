#include <Wire.h>

/*
  Traffic light controller
  ESP32-S3 -> MCP23017 -> 2x ULN2803 -> 4 traffic lights

  Non-blocking version using millis()
*/

// ============================================================
// I2C settings
// ============================================================

const int I2C_SDA_PIN = 5;
const int I2C_SCL_PIN = 4;

const uint8_t MCP_ADDR = 0x20;

// ============================================================
// MCP23017 registers
// ============================================================

const uint8_t IODIRA = 0x00;
const uint8_t IODIRB = 0x01;
const uint8_t GPIOA  = 0x12;
const uint8_t GPIOB  = 0x13;

// ============================================================
// Output mapping
// ============================================================

// Port B -> bottom ULN2803
const uint8_t TL1_RED_B    = 0;
const uint8_t TL1_YELLOW_B = 1;
const uint8_t TL1_GREEN_B  = 2;
const uint8_t TL2_RED_B    = 3;
const uint8_t TL2_YELLOW_B = 4;
const uint8_t TL2_GREEN_B  = 5;

// Port A -> top ULN2803
const uint8_t TL3_YELLOW_A = 6;
const uint8_t TL3_RED_A    = 5;
const uint8_t TL3_GREEN_A  = 4;
const uint8_t TL4_YELLOW_A = 3;
const uint8_t TL4_RED_A    = 2;
const uint8_t TL4_GREEN_A  = 1;

// ============================================================
// Timing values in milliseconds
// ============================================================

const unsigned long ALL_RED_TIME = 1000;
const unsigned long GREEN_TIME   = 5000;
const unsigned long YELLOW_TIME  = 2000;

// ============================================================
// Phase constants
// ============================================================

const uint8_t PHASE_ALL_RED_1     = 0;
const uint8_t PHASE_ROAD12_GREEN  = 1;
const uint8_t PHASE_ROAD12_YELLOW = 2;
const uint8_t PHASE_ALL_RED_2     = 3;
const uint8_t PHASE_ROAD34_GREEN  = 4;
const uint8_t PHASE_ROAD34_YELLOW = 5;

// ============================================================
// State variables
// ============================================================

uint8_t currentPhase = PHASE_ALL_RED_1;
unsigned long lastPhaseChange = 0;

// ============================================================
// Helper functions
// ============================================================

uint8_t bitMask(uint8_t bit) {
  return (1 << bit);
}

void mcpWriteRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void mcpWriteBoth(uint8_t portAValue, uint8_t portBValue) {
  mcpWriteRegister(GPIOA, portAValue);
  mcpWriteRegister(GPIOB, portBValue);
}

void mcpInit() {
  mcpWriteRegister(IODIRA, 0x00);
  mcpWriteRegister(IODIRB, 0x00);
  mcpWriteBoth(0x00, 0x00);
}

// ============================================================
// Phase output functions
// ============================================================

void allRed() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_RED_B);
  b |= bitMask(TL2_RED_B);

  a |= bitMask(TL3_RED_A);
  a |= bitMask(TL4_RED_A);

  mcpWriteBoth(a, b);
}

void road12Green_road34Red() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_GREEN_B);
  b |= bitMask(TL2_GREEN_B);

  a |= bitMask(TL3_RED_A);
  a |= bitMask(TL4_RED_A);

  mcpWriteBoth(a, b);
}

void road12Yellow_road34Red() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_YELLOW_B);
  b |= bitMask(TL2_YELLOW_B);

  a |= bitMask(TL3_RED_A);
  a |= bitMask(TL4_RED_A);

  mcpWriteBoth(a, b);
}

void road12Red_road34Green() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_RED_B);
  b |= bitMask(TL2_RED_B);

  a |= bitMask(TL3_GREEN_A);
  a |= bitMask(TL4_GREEN_A);

  mcpWriteBoth(a, b);
}

void road12Red_road34Yellow() {
  uint8_t a = 0;
  uint8_t b = 0;

  b |= bitMask(TL1_RED_B);
  b |= bitMask(TL2_RED_B);

  a |= bitMask(TL3_YELLOW_A);
  a |= bitMask(TL4_YELLOW_A);

  mcpWriteBoth(a, b);
}

// ============================================================
// State machine functions
// ============================================================

void applyPhase(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
      allRed();
      Serial.println("Phase: All Red");
      break;

    case PHASE_ROAD12_GREEN:
      road12Green_road34Red();
      Serial.println("Phase: Road 1+2 Green | Road 3+4 Red");
      break;

    case PHASE_ROAD12_YELLOW:
      road12Yellow_road34Red();
      Serial.println("Phase: Road 1+2 Yellow | Road 3+4 Red");
      break;

    case PHASE_ALL_RED_2:
      allRed();
      Serial.println("Phase: All Red");
      break;

    case PHASE_ROAD34_GREEN:
      road12Red_road34Green();
      Serial.println("Phase: Road 1+2 Red | Road 3+4 Green");
      break;

    case PHASE_ROAD34_YELLOW:
      road12Red_road34Yellow();
      Serial.println("Phase: Road 1+2 Red | Road 3+4 Yellow");
      break;
  }
}

unsigned long getPhaseDuration(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
    case PHASE_ALL_RED_2:
      return ALL_RED_TIME;

    case PHASE_ROAD12_GREEN:
    case PHASE_ROAD34_GREEN:
      return GREEN_TIME;

    case PHASE_ROAD12_YELLOW:
    case PHASE_ROAD34_YELLOW:
      return YELLOW_TIME;

    default:
      return ALL_RED_TIME;
  }
}

uint8_t getNextPhase(uint8_t phase) {
  switch (phase) {
    case PHASE_ALL_RED_1:
      return PHASE_ROAD12_GREEN;

    case PHASE_ROAD12_GREEN:
      return PHASE_ROAD12_YELLOW;

    case PHASE_ROAD12_YELLOW:
      return PHASE_ALL_RED_2;

    case PHASE_ALL_RED_2:
      return PHASE_ROAD34_GREEN;

    case PHASE_ROAD34_GREEN:
      return PHASE_ROAD34_YELLOW;

    case PHASE_ROAD34_YELLOW:
      return PHASE_ALL_RED_1;

    default:
      return PHASE_ALL_RED_1;
  }
}

// ============================================================
// Setup
// ============================================================

void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  mcpInit();

  applyPhase(currentPhase);
  lastPhaseChange = millis();

  Serial.println("Breadboard traffic light controller started");
  Serial.println("Non-blocking timing with millis()");
}

// ============================================================
// Main loop
// ============================================================

void loop() {
  unsigned long now = millis();

  if (now - lastPhaseChange >= getPhaseDuration(currentPhase)) {
    currentPhase = getNextPhase(currentPhase);
    applyPhase(currentPhase);
    lastPhaseChange = now;
  }
}