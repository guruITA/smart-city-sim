#include <Wire.h>

const int I2C_SDA_PIN = 5;
const int I2C_SCL_PIN = 4;
const uint8_t MCP_ADDR = 0x20;

// MCP23017 registers
const uint8_t IODIRB = 0x01;
const uint8_t GPIOB  = 0x13;

void mcpWriteRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t mcpReadRegister(uint8_t reg) {
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(MCP_ADDR, (uint8_t)1);
  if (Wire.available()) {
    return Wire.read();
  }
  return 0xFF;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  delay(100);

  Serial.println("MCP -> ULN -> LED test start");

  // Quick I2C presence check
  Wire.beginTransmission(MCP_ADDR);
  uint8_t err = Wire.endTransmission();

  if (err != 0) {
    Serial.print("MCP23017 not found at 0x");
    Serial.println(MCP_ADDR, HEX);
    while (true) {
      delay(1000);
    }
  }

  Serial.println("MCP23017 detected at 0x20");

  // GPB0 = output, all others = input
  // 1 = input, 0 = output
  mcpWriteRegister(IODIRB, 0b11111110);

  // Start with GPB0 low -> ULN off -> LED off
  mcpWriteRegister(GPIOB, 0b00000000);

  Serial.println("Blinking GPB0...");
}

void loop() {
  // GPB0 HIGH -> ULN input high -> ULN sinks -> LED ON
  mcpWriteRegister(GPIOB, 0b00000001);
  Serial.println("LED ON");
  delay(1000);

  // GPB0 LOW -> ULN off -> LED OFF
  mcpWriteRegister(GPIOB, 0b00000000);
  Serial.println("LED OFF");
  delay(1000);
}