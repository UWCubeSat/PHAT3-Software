// Copyright 2025 by Yehoshua Luna.
#include <Wire.h>

float angle_x = 0.0;
float angle_y = 0.0;
float angle_z = 0.0;

float gyro_x = 0.0;
float gyro_y = 0.0;
float gyro_z = 0.0;

float accel_x = 0.0;
float accel_y = 0.0;
float accel_z = 0.0;

float elapsed_time = 0.0;
long current_time = 0;
long previous_time = 0;

void setup() {
  Wire.begin(42, 41);
  Serial.begin(115200);
  configureMPU6050();

  if (!checkMPU6050Status()) {
    Serial.println("MPU6050 not connected!");
  }
}

void loop() {
  previous_time = current_time;
  current_time = millis();
  elapsed_time = (current_time - previous_time)/1000.0; // Divides by 1000 to get seconds.

  getMPU6050AccelData(accel_x, accel_y, accel_z);
  getMPU6050GyroData(gyro_x, gyro_y, gyro_z);

  angle_x += gyro_x * elapsed_time;
  angle_y += gyro_y * elapsed_time;
  angle_z += gyro_z * elapsed_time;

  Serial.print("Gyro X: ");
  Serial.print(angle_x);
  Serial.print(", Gyro Y: ");
  Serial.print(angle_y);
  Serial.print(", Gyro Z: ");
  Serial.println(angle_y);

  Serial.print("Accel X: ");
  Serial.print(accel_x);
  Serial.print(", Accel Y: ");
  Serial.print(accel_y);
  Serial.print(", Accel Z: ");
  Serial.println(accel_z);

  delay(250);
}

bool getMPU6050AccelData(float &accelX, float &accelY, float &accelZ) {
  Wire.beginTransmission(0x68);
  Wire.write(0x3B); // First accelerometer register is at 0x3B.
  
  if (Wire.endTransmission() != 0) {
    return false;
  }

  Wire.requestFrom(0x68, 6, true); // Reads 6 registers total, since each axis value is stored in 2 registers.

  if (Wire.available() != 6) {
    return false;
  }

  // We divide by 8192.0 to give a ±4g range.
  accelX = (Wire.read() << 8 | Wire.read())/8192.0;
  accelY = (Wire.read() << 8 | Wire.read())/8192.0;
  accelZ = (Wire.read() << 8 | Wire.read())/8192.0;

  return true;
}

bool getMPU6050GyroData(float &gyroX, float &gyroY, float &gyroZ) {
  Wire.beginTransmission(0x68);
  Wire.write(0x43); // First gyro register is at 0x43.
  
  if (Wire.endTransmission() != 0) {
    return false;
  }

  Wire.requestFrom(0x68, 6, true); // Reads 6 registers total, since each axis value is stored in 2 registers.

  if (Wire.available() != 6) {
    return false;
  }

  // We divide by 65.5 to give a ±500°/s range.
  gyroX = (Wire.read() << 8 | Wire.read())/65.5;
  gyroY = (Wire.read() << 8 | Wire.read())/65.5;
  gyroZ = (Wire.read() << 8 | Wire.read())/65.5;

  return true;
}

bool checkMPU6050Status() {
  Wire.beginTransmission(0x68);
  Wire.write(0x75); // Register containing the self-identifcation address.
  Wire.endTransmission();
  
  Wire.requestFrom(0x68, 1);

  if (Wire.available() & Wire.read() == 0x68) {
    return true;
  } else {
    return false;
  }
}

void configureMPU6050() {
  Wire.begin(42, 41);

  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00); // Makes a reset by placing a zero into the 6B register.
  Wire.endTransmission(true);

  Wire.beginTransmission(0x68);
  Wire.write(0x1B); // Talks to the GYRO_CONFIG register (1B hex).
  Wire.write(0x08); // Sets gyroscope sensitivity to a ±500°/s range.
  Wire.endTransmission(true);

  Wire.beginTransmission(0x68);
  Wire.write(0x1C); // Talks to ACCEL_CONFIG register (1C hex).
  Wire.write(0x08); // Sets the accelerometer sensitivity to ±4g/s.
  Wire.endTransmission(true);
}
