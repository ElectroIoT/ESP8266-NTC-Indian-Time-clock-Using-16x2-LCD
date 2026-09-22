// ESP8266 NTP Clock with 16x2 I2C LCD
// Primary version - open this folder in Arduino IDE.
// A PlatformIO version also exists in /src/main.cpp for those who prefer it.
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <time.h>

// ================= WiFi =================
const char* ssid     = "your_SSID";
const char* password = "your_PASSWORD";

// ================= I2C Pins =================
// Wired as: SDA -> D6 (GPIO12), SCL -> D5 (GPIO14)
#define SDA_PIN D6
#define SCL_PIN D5

// ================= LCD =================
LiquidCrystal_I2C lcd(0x27, 16, 2); // try 0x3F if the display stays blank

// ================= Time Format =================
// 1 = 24-hour clock (13:45:02), 0 = 12-hour clock with AM/PM
#define USE_24HR_FORMAT 0

// ================= Timezone =================
// Pick ONE offset below (seconds from UTC) and set the matching DST offset.
// Region                      UTC Offset   TIMEZONE_OFFSET_SEC   DST_OFFSET_SEC
// India (IST)                 UTC+5:30     19800                 0 (no DST)
// UK (GMT/BST)                UTC+0        0                     3600 (Mar-Oct)
// Central Europe (CET/CEST)   UTC+1        3600                  3600 (Mar-Oct)
// USA Eastern (EST/EDT)       UTC-5        -18000                3600 (Mar-Nov)
// USA Central (CST/CDT)       UTC-6        -21600                3600 (Mar-Nov)
// USA Pacific (PST/PDT)       UTC-8        -28800                3600 (Mar-Nov)
// UAE (GST)                   UTC+4        14400                 0 (no DST)
// Singapore (SGT)             UTC+8        28800                 0 (no DST)
// Japan (JST)                 UTC+9        32400                 0 (no DST)
// Australia East (AEST/AEDT)  UTC+10       36000                 3600 (Oct-Apr)
#define TIMEZONE_OFFSET_SEC 19800
#define DST_OFFSET_SEC      0

const char* weekDays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

// Custom char index 0 is avoided: its byte value (0x00) doubles as the
// C-string null terminator, which would make lcd.print()/lcd.printf() stop early.
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
    WiFi.begin(ssid, password);

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
    configTime(TIMEZONE_OFFSET_SEC, DST_OFFSET_SEC, "pool.ntp.org", "in.pool.ntp.org");

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

#if USE_24HR_FORMAT
    lcd.setCursor(0, 0);
    lcd.printf("\x01 %02d:%02d:%02d    ", t->tm_hour, t->tm_min, t->tm_sec);
#else
    const char* ampm = (t->tm_hour >= 12) ? "PM" : "AM";
    int hour12 = t->tm_hour % 12;
    if (hour12 == 0) hour12 = 12;
    lcd.setCursor(0, 0);
    lcd.printf("\x01 %02d:%02d:%02d %s", hour12, t->tm_min, t->tm_sec, ampm);
#endif

    lcd.setCursor(0, 1);
    lcd.printf("\x02 %02d/%02d/%04d %s", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, weekDays[t->tm_wday]);

    delay(1000);
}
