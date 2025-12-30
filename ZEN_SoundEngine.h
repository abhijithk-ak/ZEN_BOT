#pragma once
#include <Arduino.h>
#include "ZEN_EmotionManager.h"

class ZEN_SoundEngine {
public:
    void begin(uint8_t pin);
    void update(const ZEN_EmotionManager& emotions);

private:
    enum SoundPattern {
        SP_NONE,
        SP_POSITIVE,
        SP_ALERT,
        SP_NEGATIVE,
        SP_LOW,
        SP_SUBTLE
    };

    void startPattern(SoundPattern p);
    void runPattern();
    void stop();

    uint8_t _pin;
    SoundPattern _active = SP_NONE;
    unsigned long _startMs = 0;
    eEmotions _lastEmotion = eEmotions::Normal;
};
