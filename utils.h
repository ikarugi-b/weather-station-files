#pragma once
#include <Arduino.h>

// Simple debounce and falling edge detector for INPUT_PULLUP
class Button {
public:
  Button(uint8_t pin, uint16_t debounceMs = 30)
    : _pin(pin), _debounce(debounceMs) {}

  void begin() {
    pinMode(_pin, INPUT_PULLUP);
    _lastRead = digitalRead(_pin);
    _stable = _lastRead;
    _lastChange = millis();
    _fell = false;
  }

  void update() {
    int r = digitalRead(_pin);
    uint32_t now = millis();
    if (r != _lastRead) {
      _lastChange = now;
      _lastRead = r;
    }
    if ((now - _lastChange) > _debounce && r != _stable) {
      // Stable change detected
      _fell = (_stable == HIGH && r == LOW);
      _stable = r;
    } else {
      _fell = false;
    }
  }

  bool fell() const { return _fell; }

private:
  uint8_t _pin;
  uint16_t _debounce;
  int _lastRead{HIGH};
  int _stable{HIGH};
  uint32_t _lastChange{0};
  bool _fell{false};
};
