#pragma once
#include <Face.h>
#include "ZEN_EmotionManager.h"

class ZEN_EyeEmotionEngine {
public:
    void begin(Face* face);
    void update(const ZEN_EmotionManager& emotions);

private:
    void applyEmotion(eEmotions emotion);

    Face* _face = nullptr;
    eEmotions _lastEmotion = eEmotions::Normal;
};
