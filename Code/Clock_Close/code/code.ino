#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// WiFi Credentials
const char* ssid     = "SSID";
const char* password = "Password";

// Set up I2C LCD (16x2, Address: 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// NTP Setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // UTC+5:30 for IST

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();

    // Connect to WiFi
    WiFi.begin(ssid, password);
    lcd.setCursor(0, 0);
    lcd.print("\x7E Connecting...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("\x7E WiFi Connected");
    delay(1000);
    lcd.clear();
    
    timeClient.begin();
}

void loop() {
    timeClient.update();
    
    int rawHour = timeClient.getHours();
    int minutes = timeClient.getMinutes();
    int seconds = timeClient.getSeconds();
    int day = timeClient.getDay();
    int totalDays = timeClient.getEpochTime() / 86400; // Calculate days from epoch
    int year = 1970 + totalDays / 365; // Approximate year
    int month = (totalDays % 365) / 30 + 1; // Approximate month
    int date = (totalDays % 365) % 30 + 1; // Approximate date

    // Convert to 12-hour format
    String ampm = (rawHour >= 12) ? "PM" : "AM";
    int hour = rawHour % 12;
    if (hour == 0) hour = 12;

    // Days of the week
    String weekDays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    String currentDay = weekDays[day];

    // Display Time with Clock Symbol
    lcd.setCursor(0, 0);
    lcd.printf("\x7E %02d:%02d:%02d %s", hour, minutes, seconds, ampm.c_str());
    
    // Display Date & Weekday with Calendar Symbol
    lcd.setCursor(0, 1);
    lcd.printf("\x7F %02d/%02d/%04d %s", date, month, year, currentDay.c_str());
    
    delay(1000);
}
