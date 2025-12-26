Quick Start Guide: How to Flash
Follow these steps to get your Weather Station up and running on an ESP32-C3.

1. Prepare Arduino IDE
Install the latest Arduino IDE.
Go to Settings -> Additional Boards Manager URLs and add: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
In Boards Manager, search for esp32 and install version 3.0.x (latest).
2. Install Required Libraries
Open the Library Manager (Ctrl+Shift+I) and install:

LovyanGFX (Fast display driver).
ArduinoJson (Version 6 or 7).
Adafruit SHT31 Library (For indoor sensor).
3. Configuration
Open 
settings.h
 and update the following:

Wi-Fi: Provide your SSID and PASSWORD.
Location: Enter your local LATITUDE and LONGITUDE.
4. Board Settings (Crucial!)
In the Tools menu, select:

Board: ESP32C3 Dev Module (or your specific board model).
USB CDC On Boot: Enabled (Required to see Serial logs).
Flash Mode: DIO (Standard for most C3 boards).
Port: Select the COM port of your connected device.
5. Upload
Connect your device via USB.
Click the Upload arrow button.
Once "Done uploading" appears, open the Serial Monitor (set to 115200 baud) to see the startup logs.

Controls
Single Click: Toggle between Outdoor (Internet) and Indoor (SHT31) sensor data.
Deep Sleep: The device will automatically enter deep sleep after 20 seconds of inactivity to save power. Press the button to wake it up!
