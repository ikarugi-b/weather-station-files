#pragma once
#include <Arduino.h>
#include "LGFX_Config.h"

struct WeatherData {
  bool valid;
  float temperature;
  float humidity;
  float wind_speed;
  String condition;
};

WeatherData fetchWeather();
void renderWeather(LGFX &tft, const WeatherData &data);
