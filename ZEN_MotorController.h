#pragma once
#include <Arduino.h>
#include "ZEN_EmotionManager.h"
#include "ZEN_ModeManager.h"

enum MotorPattern {
    MP_NONE,
    MP_FORWARD,
    MP_JERK,
    MP_TWITCH,
    MP_DOUBLE
};

class ZEN_MotorController {
public:
    void begin();
    void update(const ZEN_EmotionManager& emotions,
                const ZEN_ModeManager& mode);

    // Manual control (for Bluetooth/RC)
    void manualForward();
    void manualBack();
    void manualLeft();
    void manualRight();
    void manualStop();

private:
    void startPattern(MotorPattern p);
    void runPattern();
    void stopAll();
    void drive(int left, int right);

    MotorPattern _active = MP_NONE;
    unsigned long _startMs = 0;
    eEmotions _lastEmotion = eEmotions::Normal;
};
