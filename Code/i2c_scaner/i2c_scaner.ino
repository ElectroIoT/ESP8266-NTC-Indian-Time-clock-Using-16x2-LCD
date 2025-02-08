#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <U8g2lib.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Set up the OLED display (Change the model and pin numbers if necessary)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// NTP settings
WiFiUDP udp;
NTPClient timeClient(udp, "in.pool.ntp.org", 19800, 60000);  // UTC +5:30 for IST

// Days of the week array
const char* daysOfWeek[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize NTP client
  timeClient.begin();

  // Initialize OLED display
  u8g2.begin();
}

void loop() {
  timeClient.update();

  // Get the current epoch time (in seconds since 1970)
  unsigned long epochTime = timeClient.getEpochTime();
  
  // Convert epoch time to a tm struct (local time)
  struct tm *ptm = gmtime((time_t *)&epochTime);

  // Adjust for IST (Indian Standard Time, UTC +5:30)
  ptm->tm_hour += 5;   // Add 5 hours
  ptm->tm_min += 30;   // Add 30 minutes
  
  // Normalize time after adjustment
  if (ptm->tm_min >= 60) {
    ptm->tm_min -= 60;
    ptm->tm_hour += 1;
  }
  if (ptm->tm_hour >= 24) {
    ptm->tm_hour -= 24;
    ptm->tm_mday += 1;
  }
  
  // Get time components
  int hours = ptm->tm_hour;
  int minutes = ptm->tm_min;
  int seconds = ptm->tm_sec;
  int day = ptm->tm_mday;
  int month = ptm->tm_mon + 1;  // Months are 0-11, so add 1
  int year = ptm->tm_year + 1900;  // Year since 1900

  // Get AM/PM
  String period = (hours >= 12) ? "PM" : "AM";
  if (hours > 12) hours -= 12; // Convert to 12-hour format
  if (hours == 0) hours = 12;  // Midnight hour is 12

  // Get day of the week
  String weekday = daysOfWeek[ptm->tm_wday];

  // Clear the display
  u8g2.clearBuffer();

  // Display time, date, and weekday
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 10);
  u8g2.print("Time: ");
  u8g2.print(hours);
  u8g2.print(":");
  if (minutes < 10) u8g2.print("0");
  u8g2.print(minutes);
  u8g2.print(":");
  if (seconds < 10) u8g2.print("0");
  u8g2.print(seconds);
  u8g2.print(" ");
  u8g2.print(period);

  u8g2.setCursor(0, 30);
  u8g2.print("Date: ");
  u8g2.print(day);
  u8g2.print("/");
  u8g2.print(month);
  u8g2.print("/");
  u8g2.print(year);

  u8g2.setCursor(0, 50);
  u8g2.print("Weekday: ");
  u8g2.print(weekday);

  // Send data to OLED
  u8g2.sendBuffer();
  
  delay(1000);  // Update every second
}
