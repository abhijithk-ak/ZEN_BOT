#pragma once
#include <Arduino.h>
#include "ZEN_EmotionManager.h"

class ZEN_TouchEngine {
public:
    void begin(uint8_t pin);
    void update(ZEN_EmotionManager& emotions);

private:
    uint8_t _pin;
    bool _touching = false;

    unsigned long _lastTapMs = 0;
    int _tapCount = 0;

    void singleTap(ZEN_EmotionManager& emotions);
    void tripleTap(ZEN_EmotionManager& emotions);
};
