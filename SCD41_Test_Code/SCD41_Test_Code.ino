#include <Wire.h>
#include <SparkFun_SCD4x_Arduino_Library.h> // Make sure to install this library

SCD4x airSensor;

void setup() {
  Serial.begin(115200); // Initialize serial communication
  Wire.begin(42, 41);

  if (airSensor.begin() == false) { // Initialize the SCD41 sensor
    Serial.println("SCD41 not detected. Please check wiring.");
    while (1);
  }

  Serial.println("SCD41 initialized successfully.");
}




void loop() {
  if (airSensor.readMeasurement()) { // Read sensor data
    Serial.print("CO2 (ppm): ");
    Serial.println(airSensor.getCO2());

    Serial.print("Temperature (°C): ");
    Serial.println(airSensor.getTemperature());

    Serial.print("Humidity (%RH): ");
    Serial.println(airSensor.getHumidity());
  } else {
    Serial.println("Sensor data not available. Check connection.");
  }

  delay(4000); // Wait 1 second before the next measurement
}
