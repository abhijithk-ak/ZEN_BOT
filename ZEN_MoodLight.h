#pragma once
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "ZEN_EmotionManager.h"

class ZEN_MoodLight {
public:
    void begin(uint8_t pin);
    void update(const ZEN_EmotionManager& emotions);

private:
    void setTargetColor(uint8_t r, uint8_t g, uint8_t b);
    void breathe();

    Adafruit_NeoPixel _led = Adafruit_NeoPixel(1, 4, NEO_GRB + NEO_KHZ800);

    uint8_t _curR = 0, _curG = 0, _curB = 0;
    uint8_t _tgtR = 0, _tgtG = 0, _tgtB = 0;

    unsigned long _lastUpdate = 0;
    float _breathPhase = 0.0f;
};
