#include "images.h"

static std::vector<String> imageList;
static int currentIndex = 0;

void loadImageList() {
  imageList.clear();
  File root = SD.open("/");
  if (!root) return;

  while (true) {
    File f = root.openNextFile();
    if (!f) break;
    String name = f.name();
    name.toLowerCase();
    if (name.endsWith(".bmp") || name.endsWith(".jpg"))
      imageList.push_back("/" + String(f.name()));
    f.close();
  }
  root.close();
}

int imageCount() {
  return imageList.size();
}

void drawImage(LGFX &tft, const String &path) {
  if (path.endsWith(".bmp"))
    tft.drawBmpFile(SD, path.c_str(), 0, 0);
  else if (path.endsWith(".jpg"))
    tft.drawJpgFile(SD, path.c_str(), 0, 0);
}

void showNextImage(LGFX &tft) {
  if (imageList.empty()) return;
  currentIndex = (currentIndex + 1) % imageList.size();
  drawImage(tft, imageList[currentIndex]);
}

void showCurrentImage(LGFX &tft) {
  if (imageList.empty()) return;
  drawImage(tft, imageList[currentIndex]);
}
