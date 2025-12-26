#pragma once
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Bus_SPI _bus;
  lgfx::Panel_ST7735S _panel;

public:
  LGFX(void) {
    { // --- SPI Bus ---
      auto cfg = _bus.config();
      cfg.spi_host   = SPI2_HOST;
      cfg.spi_mode   = 0;
      cfg.freq_write = 20000000;
      cfg.freq_read  = 8000000;
      cfg.pin_sclk   = 7;
      cfg.pin_mosi   = 9;
      cfg.pin_miso   = 8;
      cfg.pin_dc     = 5;
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }

    { // --- Panel Parameters ---
      auto cfg = _panel.config();
      cfg.pin_cs        = 6;
      cfg.pin_rst       = 4;
      cfg.pin_busy      = -1;
      cfg.memory_width  = 132;
      cfg.memory_height = 162;
      cfg.panel_width   = 128;
      cfg.panel_height  = 160;
      cfg.offset_x      = 0;
      cfg.offset_y      = 0;
      cfg.invert        = false;
      cfg.rgb_order     = true; // RGB
      cfg.bus_shared    = false;
      _panel.config(cfg);
    }
    setPanel(&_panel);
  }
};

// Helper function to draw centered text
inline void drawCenteredText(LGFX &tft, const char *msg, int y, const lgfx::IFont *font) {
  tft.setFont(font);
  int16_t w = tft.textWidth(msg);
  tft.drawString(msg, (tft.width() - w) / 2, y);
}
