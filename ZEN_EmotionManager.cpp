#include "ZEN_EmotionManager.h"
#include <Arduino.h>

void ZEN_EmotionManager::begin() {
    _blender.set(eEmotions::Normal, 0.5f);
}

void ZEN_EmotionManager::update() {
    unsigned long now = millis();

    if (_sleeping) {
        _blender.set(eEmotions::Sleepy, 1.0f);
        return;
    }

    _blender.decay(now);
}

void ZEN_EmotionManager::trigger(ZENEmotion emotion, float strength) {
    if (_sleeping && emotion != eEmotions::Sleepy) return;

    _blender.add(emotion, strength);
}

void ZEN_EmotionManager::forceSleep(bool sleep) {
    _sleeping = sleep;
}

ZENEmotion ZEN_EmotionManager::current() const {
    return _blender.dominantEmotion();
}
