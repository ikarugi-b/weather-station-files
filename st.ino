#include <WiFi.h>
#include <Wire.h>
#include "LGFX_Config.h"
#include "weather.h"
#include "settings.h"
#include "utils.h"
#include <Adafruit_SHT31.h>

LGFX tft;
Adafruit_SHT31 sht31 = Adafruit_SHT31();
Button btn(BUTTON_PIN);

enum Mode { OUTDOOR, INDOOR };
Mode mode = OUTDOOR;

unsigned long lastAction = 0;
float currentBrightness = 0.5;

void setBrightness(float level) {
  level = constrain(level, 0.0, 1.0);
  analogWrite(LED_PIN, int(255 * level));
  currentBrightness = level;
}

void connectWiFi() {
  if (strlen(SSID) == 0) return;
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  delay(100);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Failed");
  }
}

void drawIndoor() {
  float temp = sht31.readTemperature();
  float hum  = sht31.readHumidity();

  Serial.printf("Indoor: %.1f C, %.0f %%\n", temp, hum);

  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(textdatum_t::middle_center);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFont(&fonts::Font4);
  tft.drawString(String(temp, 1) + " c", tft.width()/2, 60);
  tft.drawString(String((int)roundf(hum)) + " %", tft.width()/2, 110);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  analogWriteResolution(LED_PIN, 8);
  setBrightness(0.5);
  
  btn.begin();

  Wire.begin(2, 0);
  sht31.begin(0x44);

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(textdatum_t::middle_center);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFont(&fonts::Font2);
  tft.drawString("Connecting WiFi...", tft.width()/2, tft.height()/2);

  connectWiFi();

  tft.fillScreen(TFT_BLACK);
  if (WiFi.status() == WL_CONNECTED) {
    renderWeather(tft, fetchWeather());
  } else {
    tft.drawString("WiFi Failed", tft.width()/2, tft.height()/2);
    delay(2000);
    drawIndoor();
  }

  lastAction = millis();
}

void loop() {
  btn.update();

  if (btn.fell()) {
    mode = (mode == OUTDOOR) ? INDOOR : OUTDOOR;
    lastAction = millis();
    setBrightness(0.5);
    tft.fillScreen(TFT_BLACK);
    if (mode == OUTDOOR) {
      if (WiFi.status() != WL_CONNECTED) connectWiFi();
      renderWeather(tft, fetchWeather());
    } else {
      drawIndoor();
    }
  }

  unsigned long idle = millis() - lastAction;

  if (idle > 5000 && currentBrightness > 0.05f) {
    setBrightness(0.02f);
  }

  if (idle > 20000) {
    Serial.println("Entering Deep Sleep...");
    setBrightness(0.0f);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    delay(100);
    esp_deep_sleep_enable_gpio_wakeup(1ULL << BUTTON_PIN, ESP_GPIO_WAKEUP_GPIO_LOW);
    esp_deep_sleep_start();
  }

  delay(10);
}
