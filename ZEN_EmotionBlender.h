#pragma once
#include <map>
#include "ZEN_Emotion.h"

class ZEN_EmotionBlender {
public:
    void set(ZENEmotion emotion, float intensity);
    void add(ZENEmotion emotion, float delta);
    void decay(unsigned long now);
    ZENEmotion dominantEmotion() const;
    float getIntensity(ZENEmotion emotion) const;

private:
    std::map<ZENEmotion, float> _weights;
    unsigned long _lastUpdate = 0;
};
