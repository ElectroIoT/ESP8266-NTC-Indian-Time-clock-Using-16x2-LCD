#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set up I2C LCD (16x2, Address: 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x3F, 16, 2); 

void setup() {
    Serial.begin(115200);
    lcd.init();      // Initialize the LCD
    lcd.backlight(); // Turn on backlight

    // Display Test Messages
    lcd.setCursor(0, 0);
    lcd.print("Hello, World!");
    delay(2000);

    lcd.setCursor(0, 1);
    lcd.print("LCD Test Success!");
    delay(2000);

    lcd.clear();
}

void loop() {
    lcd.setCursor(0, 0);
    lcd.print("Counting: ");
    
    for (int i = 0; i <= 10; i++) {
        lcd.setCursor(10, 0);
        lcd.print(i);
        delay(1000);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Test Complete!");
    while (true); // Stop execution
}
