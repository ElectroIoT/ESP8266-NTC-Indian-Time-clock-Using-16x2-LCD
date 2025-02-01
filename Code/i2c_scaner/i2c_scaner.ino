#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>  // TFT library for ST7789
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// WiFi credentials
const char* ssid = "FTTD";
const char* password = "M@$ter@2050";

// NTP setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);  // IST (UTC+5:30)

// TFT Display
TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected!");
    
    timeClient.begin();
    
    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK);
}

void loop() {
    timeClient.update();
    String currentTime = timeClient.getFormattedTime();
    
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(40, 50);
    tft.print(currentTime);
    
    delay(1000);
}
