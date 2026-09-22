// ESP8266 NTP Clock with 16x2 I2C LCD (PlatformIO version)
// The Arduino IDE version is primary - see Code/Clock_Close/code/code.ino.
// Kept feature-identical for those who prefer PlatformIO.
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <time.h>
#include "config.h"

LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

const char *weekDays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

byte clockIcon[8] = {
    0b00000,
    0b01110,
    0b10101,
    0b10111,
    0b10001,
    0b01110,
    0b00000,
    0b00000};

byte calendarIcon[8] = {
    0b01110,
    0b10001,
    0b11111,
    0b10101,
    0b10101,
    0b10001,
    0b11111,
    0b00000};

void connectWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Connecting...");

    uint8_t dots = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        lcd.setCursor(13, 0);
        lcd.print(dots % 2 == 0 ? "." : " ");
        dots++;
        Serial.print(".");
    }

    Serial.println("\nWiFi connected: " + WiFi.localIP().toString());
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" WiFi Connected");
    delay(1000);
    lcd.clear();
}

void syncTime() {
    configTime(TIMEZONE_OFFSET_SEC, DST_OFFSET_SEC, NTP_SERVER_1, NTP_SERVER_2);

    lcd.setCursor(0, 0);
    lcd.print(" Syncing time...");

    // Epoch stays small until SNTP actually sets the clock; wait for a sane value.
    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2) {
        delay(200);
        now = time(nullptr);
    }

    Serial.println("Time synced");
    lcd.clear();
}

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);

    lcd.init();
    lcd.backlight();
    // Custom char index 0 is avoided: its byte value (0x00) doubles as the
    // C-string null terminator, which would make lcd.print() see an empty string.
    lcd.createChar(1, clockIcon);
    lcd.createChar(2, calendarIcon);

    connectWiFi();
    syncTime();
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi();
    }

    time_t now = time(nullptr);
    struct tm *t = localtime(&now);

    char line1[20];
#if USE_24HR_FORMAT
    snprintf(line1, sizeof(line1), "\x01 %02d:%02d:%02d    ", t->tm_hour, t->tm_min, t->tm_sec);
#else
    const char *ampm = (t->tm_hour >= 12) ? "PM" : "AM";
    int hour12 = t->tm_hour % 12;
    if (hour12 == 0) hour12 = 12;
    snprintf(line1, sizeof(line1), "\x01 %02d:%02d:%02d %s", hour12, t->tm_min, t->tm_sec, ampm);
#endif
    lcd.setCursor(0, 0);
    lcd.print(line1);

    char line2[20];
    snprintf(line2, sizeof(line2), "\x02 %02d/%02d/%04d %s",
             t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, weekDays[t->tm_wday]);
    lcd.setCursor(0, 1);
    lcd.print(line2);

    delay(1000);
}
