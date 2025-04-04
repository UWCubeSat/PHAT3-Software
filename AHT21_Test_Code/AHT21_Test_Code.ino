#include <Wire.h>
#define AHT21_ADDRESS 0x38
#define AHT21_MEASURE_CMD 0xAC
#define AHT21_STATUS_CMD 0x71

bool initAHT21() {
  Wire.beginTransmission(AHT21_ADDRESS);
  Wire.write(0xBE);
  Wire.write(0x08);
  Wire.write(0x00);

  if (Wire.endTransmission() != 0) {
    return false;
  }

  delay(10);
  return checkAHT21Status();
}

bool checkAHT21Status() {
  Wire.beginTransmission(AHT21_ADDRESS);
  Wire.write(AHT21_STATUS_CMD);

  if (Wire.endTransmission() != 0) {
    return false;
  }
  Wire.requestFrom(AHT21_ADDRESS, 1);

  if (Wire.available() != 1) {
    return false;
  }

  uint8_t status = Wire.read();
  return (status & 0x18) == 0x18;
}

bool getAHT21Data(float &temperature, float &humidity) {  
    Wire.beginTransmission(AHT21_ADDRESS);  
    Wire.write(AHT21_MEASURE_CMD);  
    Wire.write(0x33);  
    Wire.write(0x00);  
    if (Wire.endTransmission() != 0) {   
       return false;  
    }    
    delay(80);
    Wire.requestFrom(AHT21_ADDRESS, 6);  
    if (Wire.available() != 6) {    
        return false;  
    }    
    uint8_t data[6]; 
    for (int i = 0; i < 6; i++) {    
        data[i] = Wire.read();  
    }    
    humidity = ((data[1] << 12) | (data[2] << 4) | (data[3] >> 4)) * 100.0 / 0x100000;  
    temperature = ((((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5]) * 200.0 / 0x100000) - 50;    
    return true;
}
void printAHT21Data() {  
    float temperature, humidity;  
    if (getAHT21Data(temperature, humidity)) {    
        Serial.print("Temperature: ");    
        Serial.print(temperature, 2);    
        Serial.print(" °C, Humidity: ");    
        Serial.print(humidity, 2);    
        Serial.println(" %");  } 
    else {    
        Serial.println("Failed to read AHT21 data");  
    }
}
void setup() {  
    Serial.begin(115200);  
    Wire.begin(42,41);  
    if (initAHT21()) {    
        Serial.println("AHT21 initialized");  } 
    else {    
        Serial.println("No");  
    }  
    if (checkAHT21Status()) {    
        Serial.println("AHT21 sensor found and initialized");  
    } 
    else {    
       Serial.println("AHT21 sensor not found or not initialized");  
    }
}
void loop() {  
    if (checkAHT21Status()) {    
        printAHT21Data();  } 
    else {    
        Serial.println("AHT21 sensor not responding");  
    }    
    delay(2000);
}
