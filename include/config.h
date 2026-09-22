#pragma once

// ================= WiFi =================
#define WIFI_SSID     "your_SSID"
#define WIFI_PASSWORD "your_PASSWORD"

// ================= I2C Pins =================
// Wired as: SDA -> D6 (GPIO12), SCL -> D5 (GPIO14)
#define SDA_PIN D6
#define SCL_PIN D5

// ================= LCD =================
#define LCD_I2C_ADDRESS 0x27   // run the I2C scanner sketch if the display stays blank; try 0x3F
#define LCD_COLUMNS     16
#define LCD_ROWS        2

// ================= Time Format =================
// 1 = 24-hour clock   (13:45:02)
// 0 = 12-hour clock   (01:45:02 PM)
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

// ================= NTP =================
#define NTP_SERVER_1 "pool.ntp.org"
#define NTP_SERVER_2 "in.pool.ntp.org"
