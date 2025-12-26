#include "ZEN_EmotionBlender.h"
#include <Arduino.h>

void ZEN_EmotionBlender::set(ZENEmotion emotion, float intensity) {
    _weights[emotion] = constrain(intensity,
                                  ZENEmotionConst::MIN_INTENSITY,
                                  ZENEmotionConst::MAX_INTENSITY);
}

void ZEN_EmotionBlender::add(ZENEmotion emotion, float delta) {
    float current = _weights[emotion];
    set(emotion, current + delta);
}

void ZEN_EmotionBlender::decay(unsigned long now) {
    if (_lastUpdate == 0) {
        _lastUpdate = now;
        return;
    }

    float dt = (now - _lastUpdate) / 1000.0f;
    _lastUpdate = now;

    for (auto &e : _weights) {
        e.second -= dt * 0.05f;   // smooth decay
        if (e.second < 0.0f) e.second = 0.0f;
    }
}

ZENEmotion ZEN_EmotionBlender::dominantEmotion() const {
    ZENEmotion dominant = eEmotions::Normal;
    float maxVal = 0.0f;

    for (auto &e : _weights) {
        if (e.second > maxVal) {
            maxVal = e.second;
            dominant = e.first;
        }
    }
    return dominant;
}

float ZEN_EmotionBlender::getIntensity(ZENEmotion emotion) const {
    auto it = _weights.find(emotion);
    return (it != _weights.end()) ? it->second : 0.0f;
}
