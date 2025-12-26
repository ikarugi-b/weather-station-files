#pragma once
#include <Arduino.h>
#include <SD.h>
#include <vector>
#include "LGFX_Config.h"

void loadImageList();
void showNextImage(LGFX &tft);
void showCurrentImage(LGFX &tft);
int  imageCount();
