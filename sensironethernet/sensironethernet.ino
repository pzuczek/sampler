// #include <Arduino.h>
// #include <SensirionI2cSfx6xxx.h>
// #include <Wire.h>
// #include <SPI.h>       
// #include <Ethernet.h>  

// byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// const char* host = "api.thingspeak.com";

// String apiKey = "E7YQAHUJXI7GUUJ2";

// EthernetClient client;

// unsigned long lastConnectionTime = 0;
// const long postingInterval = 20000; 

// #ifdef NO_ERROR
// #undef NO_ERROR
// #endif
// #define NO_ERROR 0

// SensirionI2cSfx6xxx sensor;

// static char errorMessage[64];
// static int16_t error;

// float globalFlow = 0.0; 

// // Deklaracje funkcji
// void connectToEthernet();
// void sendFlowToThingSpeak(float flowValue);
// void PrintUint64(uint64_t& value);

// void PrintUint64(uint64_t& value) {
//     Serial.print("0x");
//     Serial.print((uint32_t)(value >> 32), HEX);
//     Serial.print((uint32_t)(value & 0xFFFFFFFF), HEX);
// }

// // --- SETUP ---
// void setup() {
//     Serial.begin(115200);
//     while (!Serial) {
//         delay(100);
//     }

//     connectToEthernet();

//     Wire.begin();
//     sensor.begin(Wire, SFC6000_I2C_ADDR_24); 

//     uint32_t productIdentifier = 0;
//     uint64_t serialNumber = 0;
//     error = sensor.readProductIdentifier(productIdentifier, serialNumber);
//     if (error != NO_ERROR) {
//         Serial.print("Error trying to execute readProductIdentifier(): ");
//         errorToString(error, errorMessage, sizeof errorMessage);
//         Serial.println(errorMessage);
//     } else {
//         Serial.println("--- Inicjalizacja Sensirion ---");
//         Serial.print("productIdentifier: ");
//         Serial.print(productIdentifier);
//         Serial.print("\t");
//         Serial.print("serialNumber: ");
//         PrintUint64(serialNumber);
//         Serial.println();
//     }
    
//     sensor.stopContinuousMeasurement();
//     error = sensor.startO2ContinuousMeasurement(); 
//     if (error != NO_ERROR) {
//         Serial.print("Error trying to execute startO2ContinuousMeasurement(): ");
//         errorToString(error, errorMessage, sizeof errorMessage);
//         Serial.println(errorMessage);
//         return; 
//     }
//     Serial.println("Ciągły pomiar rozpoczęty.");
// }


// void loop() {
//     Ethernet.maintain();

//     // 1. Odczyt danych z czujnika
//     error = sensor.readFlow(globalFlow);
//     if (error != NO_ERROR) {
//         Serial.print("Błąd odczytu przepływu: ");
//         errorToString(error, errorMessage, sizeof errorMessage);
//         Serial.println(errorMessage);
//     } else {
//         Serial.print("Aktualny przepływ (aFlow): ");
//         Serial.print(globalFlow, 4);
//         Serial.println();
//     }

//     if (millis() - lastConnectionTime > postingInterval) {
//         if (Ethernet.linkStatus() == LinkON) {
//             sendFlowToThingSpeak(globalFlow);
//         } else {
//             Serial.println("Błąd: Kabel Ethernet odłączony.");
//         }
//         lastConnectionTime = millis(); 
//     }
    

//     delay(500); 
// }



// void connectToEthernet() {
//     Serial.println("\nInicjalizacja Ethernetu (pobieranie IP przez DHCP)...");

//     if (Ethernet.begin(mac) == 0) {
//         Serial.println("Błąd konfiguracji Ethernet przy użyciu DHCP!");
        
//         if (Ethernet.hardwareStatus() == EthernetNoHardware) {
//             Serial.println("Nie znaleziono modułu Ethernet. Sprawdź okablowanie SPI.");
//         } 
//         if (Ethernet.linkStatus() == LinkOFF) {
//             Serial.println("Kabel sieciowy nie jest podłączony.");
//         }
//     } else {
//         Serial.println("Pomyślnie połączono z siecią Ethernet!");
//         Serial.print("Przypisany adres IP: ");
//         Serial.println(Ethernet.localIP());
//     }
    
//     delay(1000);
// }


// void sendFlowToThingSpeak(float flowValue) {
//     Serial.print("\n--- Wysyłam dane przepływu na ThingSpeak ---");
//     Serial.print(" (Flow: ");
//     Serial.print(flowValue, 4);
//     Serial.println(")");

//     String getUri = "/update?api_key=";
//     getUri += apiKey;
//     getUri += "&field1=";
//     getUri += String(flowValue, 4); 

//     Serial.print("Pełne żądanie (URI): ");
//     Serial.println(getUri);

//     if (client.connect(host, 80)) { 
//         client.print("GET ");
//         client.print(getUri);
//         client.println(" HTTP/1.1");
//         client.print("Host: ");
//         client.println(host);
//         client.println("Connection: close");
//         client.println(); 
        
//         Serial.println("Dane wysłane. Odpowiedź serwera:");
        
//         unsigned long timeout = millis();
//         while (client.available() == 0) { 
//             if (millis() - timeout > 5000) {
//                 Serial.println("Timeout przy oczekiwaniu na odpowiedź!");
//                 client.stop();
//                 return;
//             }
//         }
        
//         while (client.available()) {
//             String line = client.readStringUntil('\n');
//             Serial.print(line);
//         }
//         Serial.println();
        
//     } else {
//         Serial.println("--- BŁĄD: Nie udało się połączyć z ThingSpeak ---");
//     }
    
//     client.stop(); 
// }