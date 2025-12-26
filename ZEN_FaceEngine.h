#pragma once
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ZEN_EmotionManager.h"

class ZEN_FaceEngine {
public:
    void begin();
    void update(const ZEN_EmotionManager& emotions);

private:
    void drawEyes(int eyeHeight, int eyeWidth, int offsetY);
    void blinkLogic();

    unsigned long _lastBlink = 0;
    bool _blinking = false;
};
