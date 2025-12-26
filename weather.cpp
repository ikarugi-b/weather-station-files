#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "weather.h"
#include "settings.h"

WeatherData fetchWeather() {
  WeatherData data{false, NAN, NAN, NAN, ""};
  if (WiFi.status() != WL_CONNECTED) return data;

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  String url = "https://api.met.no/weatherapi/locationforecast/2.0/compact?"
               "lat=" + String(LATITUDE, 6) + "&lon=" + String(LONGITUDE, 6);
  
  if (http.begin(client, url)) {
    http.addHeader("Accept-Encoding", "identity"); // Disable compression for stability
    http.setUserAgent("ESP32C3-WeatherStation/1.0");
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      String payload = "";
      payload.reserve(20480);
      
      WiFiClient * stream = http.getStreamPtr();
      uint32_t lastData = millis();
      // Wait and read data while it's coming (3s timeout per packet)
      while (http.connected() && (millis() - lastData < 3000)) {
        while (stream->available()) {
          payload += (char)stream->read();
          lastData = millis();
        }
        delay(10);
      }

#if ARDUINOJSON_VERSION_MAJOR >= 7
      JsonDocument filter;
#else
      StaticJsonDocument<512> filter;
#endif
      filter["properties"]["timeseries"][0]["data"]["instant"]["details"] = true;
      filter["properties"]["timeseries"][0]["data"]["next_1_hours"]["summary"]["symbol_code"] = true;

#if ARDUINOJSON_VERSION_MAJOR >= 7
      JsonDocument doc;
#else
      DynamicJsonDocument doc(24576);
#endif
      
      DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));
      
      if (error == DeserializationError::Ok) {
        JsonObject ts0 = doc["properties"]["timeseries"][0];
        JsonObject inst = ts0["data"]["instant"]["details"];
        data.temperature = inst["air_temperature"] | NAN;
        data.humidity    = inst["relative_humidity"] | NAN;
        data.wind_speed  = inst["wind_speed"] | NAN;
        data.condition   = ts0["data"]["next_1_hours"]["summary"]["symbol_code"].as<String>();
        data.valid = true;
      }
    }
    http.end();
  }
  return data;
}

void renderWeather(LGFX &tft, const WeatherData &d) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(textdatum_t::middle_center);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  if (!d.valid) {
    tft.setFont(&fonts::Font2);
    tft.drawString("Weather Error", tft.width()/2, tft.height()/2);
    return;
  }

  String summary = d.condition;
  summary.replace("_", " ");
  summary.replace(" day", "");
  summary.replace(" night", "");
  summary.toUpperCase();

  tft.setFont(&fonts::Font2);
  tft.drawString(summary, tft.width()/2, 22);

  tft.setFont(&fonts::Font4);
  tft.drawString(String(d.temperature,1) + " c", tft.width()/2, 62);
  tft.drawString(String(d.wind_speed,1) + " m/s", tft.width()/2, 98);

  tft.setFont(&fonts::Font2);
  tft.drawString(String((int)roundf(d.humidity)) + " % RH", tft.width()/2, 137);
}
