#pragma once
#include <Arduino.h>
#include "ZEN_EmotionManager.h"

class ZEN_MotorController {
public:
    void begin();
    void update(const ZEN_EmotionManager& emotions);

private:
    void drive(int left, int right);
    void stop();

    void behaviorHappy();
    void behaviorFocused();
    void behaviorAnnoyed();
    void behaviorSleepy();

    unsigned long _lastAction = 0;
    bool _active = false;
};
