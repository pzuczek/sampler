#include <Arduino.h>
#include <SensirionI2cSfx6xxx.h>
#include <Wire.h>

#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

SensirionI2cSfx6xxx sensor;
static char errorMessage[64];
static int16_t error;
float globalFlow = 0.0; 

void PrintUint64(uint64_t& value);

void PrintUint64(uint64_t& value) {
    Serial.print("0x");
    Serial.print((uint32_t)(value >> 32), HEX);
    Serial.print((uint32_t)(value & 0xFFFFFFFF), HEX);
}

void setup(){
  Serial.begin(115200);
  while(!Serial) delay(100);
  Wire.begin();
  sensor.begin(Wire, SFC6000_I2C_ADDR_24); 

  uint32_t productIdentifier = 0;
  uint64_t serialNumber = 0;
  error = sensor.readProductIdentifier(productIdentifier, serialNumber);
  if (error != NO_ERROR) {
    Serial.print("Error trying to execute readProductIdentifier(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
  } else {
    Serial.println("--- Inicjalizacja Sensirion ---");
    Serial.print("productIdentifier: ");
    Serial.print(productIdentifier);
    Serial.print("\t");
    Serial.print("serialNumber: ");
    PrintUint64(serialNumber);
    Serial.println();
  }
  
  sensor.stopContinuousMeasurement();
  error = sensor.startO2ContinuousMeasurement(); 
  if (error != NO_ERROR) {
      Serial.print("Error trying to execute startO2ContinuousMeasurement(): ");
      errorToString(error, errorMessage, sizeof errorMessage);
      Serial.println(errorMessage);
      return; 
  }
  Serial.println("Ciągły pomiar rozpoczęty.");
}

void loop(){
  // Serial.println("zamn");
    // 1. Odczyt danych z czujnika
  error = sensor.readFlow(globalFlow);
  if (error != NO_ERROR) {
      Serial.print("Błąd odczytu przepływu: ");
      errorToString(error, errorMessage, sizeof errorMessage);
      Serial.println(errorMessage);
  } else {
      Serial.print("Aktualny przepływ (aFlow): ");
      Serial.print(globalFlow, 4);
      Serial.println();
  }
  delay(150);
}
