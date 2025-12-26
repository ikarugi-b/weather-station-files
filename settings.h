#pragma once

// ---- Wi-Fi ----
// If left empty, Wi-Fi will be skipped (offline weather mode).
constexpr char SSID[]     = "SSID";
constexpr char PASSWORD[] = "PASSWORD";

// ---- Pins ----
constexpr uint8_t BUTTON_PIN = 1; // INPUT_PULLUP
constexpr uint8_t LED_PIN    = 3; // TFT Backlight
constexpr uint8_t SD_CS_PIN  = 10;

// ---- Intervals ----
constexpr uint32_t SLIDE_INTERVAL_MS  = 10UL * 1000UL;  // 10 sec
constexpr uint32_t WEATHER_VIEW_MS    = 30UL * 60UL * 1000UL;    // 30 min

// ---- Geolocation (your location name) ----
constexpr double LATITUDE  = 45.493525;
constexpr double LONGITUDE = 15.565131;
