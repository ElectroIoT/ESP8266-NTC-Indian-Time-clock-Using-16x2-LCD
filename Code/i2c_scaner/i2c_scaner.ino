#include <Wire.h>

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for serial monitor to open
    Serial.println("\nI2C Scanner Initialized...");
    
    Wire.begin();
}

void loop() {
    Serial.println("Scanning...");
    byte count = 0;
    
    for (byte address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            Serial.print("Found I2C device at address 0x");
            Serial.println(address, HEX);
            count++;
            delay(10);
        }
    }

    if (count == 0) {
        Serial.println("No I2C devices found!");
    } else {
        Serial.println("Scan Complete.");
    }
    
    delay(5000); // Scan every 5 seconds
}
