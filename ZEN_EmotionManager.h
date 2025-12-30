#pragma once
#include "ZEN_EmotionBlender.h"

class ZEN_EmotionManager {
public:
    void begin();
    void update();

    void trigger(ZENEmotion emotion, float strength = 0.3f);
    void forceSleep(bool sleep);

    ZENEmotion current() const;
    bool isSleeping() const { return _sleeping; }

private:
    ZEN_EmotionBlender _blender;
    bool _sleeping = false;
};
