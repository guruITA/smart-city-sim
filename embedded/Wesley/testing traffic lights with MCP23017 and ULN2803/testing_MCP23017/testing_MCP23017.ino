#include <Wire.h>

const int I2C_SDA_PIN = 5;
const int I2C_SCL_PIN = 4;
const uint8_t MCP_ADDR = 0x20;

const uint8_t IODIRB = 0x01;
const uint8_t GPIOB  = 0x13;

void mcpWriteRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(MCP_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // GPB0 output, others input
  mcpWriteRegister(IODIRB, 0b11111110);

  // GPB0 HIGH = off
  mcpWriteRegister(GPIOB, 0b00000001);
}

void loop() {
  // GPB0 LOW = LED on
  mcpWriteRegister(GPIOB, 0b00000000);
  delay(500);

  // GPB0 HIGH = LED off
  mcpWriteRegister(GPIOB, 0b00000001);
  delay(500);
}