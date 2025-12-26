#pragma once
#include <Arduino.h>
#include "ZEN_EmotionManager.h"

class ZEN_SoundEngine {
public:
    void begin(uint8_t dacPin);
    void update(const ZEN_EmotionManager& emotions);

private:
    void playTone(float freq, uint16_t ms);
    void emotionSound(eEmotions emo);

    uint8_t _dacPin;
    unsigned long _lastSound = 0;
};
