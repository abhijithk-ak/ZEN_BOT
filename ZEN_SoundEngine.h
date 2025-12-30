#pragma once
#include <Arduino.h>
#include "ZEN_EmotionManager.h"

class ZEN_SoundEngine {
public:
    void begin(uint8_t pin);
    void update(const ZEN_EmotionManager& emotions);
    void setMuted(bool m);

private:
    enum SoundPattern {
        SP_NONE,
        SP_POSITIVE,
        SP_ALERT,
        SP_NEGATIVE,
        SP_COGNITIVE,
        SP_SLEEP
    };

    void startPattern(SoundPattern p);
    void runPattern();
    void silence();

    uint8_t _pin;
    bool _muted = false;
    SoundPattern _active = SP_NONE;
    SoundPattern _pending = SP_NONE;
    unsigned long _startMs = 0;
    eEmotions _lastEmotion = eEmotions::Normal;
};
