#include <Wire.h>

#define BMP180_ADDRESS 0x77

#define BMP180_REG_CONTROL 0xF4
#define BMP180_REG_RESULT 0xF6

#define BMP180_COMMAND_TEMPERATURE 0x2E
#define BMP180_COMMAND_PRESSURE0 0x34

int16_t readRawTemperature() {
  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(BMP180_REG_CONTROL);
  Wire.write(BMP180_COMMAND_TEMPERATURE);
  Wire.endTransmission();

  delay(5); // Wait for the measurement

  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(BMP180_REG_RESULT);
  Wire.endTransmission();

  Wire.requestFrom(BMP180_ADDRESS, 2);
  int16_t raw = (Wire.read() << 8) | Wire.read();
  return raw;
}

int32_t readRawPressure() {
  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(BMP180_REG_CONTROL);
  Wire.write(BMP180_COMMAND_PRESSURE0);
  Wire.endTransmission();

  delay(8); // Wait for the measurement

  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(BMP180_REG_RESULT);
  Wire.endTransmission();

  Wire.requestFrom(BMP180_ADDRESS, 3);
  int32_t raw = ((Wire.read() << 16) | (Wire.read() << 8) | Wire.read()) >> (8 - 0);
  return raw;
}

void setup() {
  Wire.begin(42, 41);
  Serial.begin(115200);
  Serial.println("BMP180 I2C Example");
}

void loop() {
  int16_t rawTemperature = readRawTemperature();
  int32_t rawPressure = readRawPressure();

  Serial.print("Raw Temperature: ");
  Serial.println(rawTemperature);
  Serial.print("Raw Pressure: ");
  Serial.println(rawPressure);

  delay(2000);
}