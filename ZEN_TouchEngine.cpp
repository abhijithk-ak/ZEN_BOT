#include "ZEN_TouchEngine.h"

#define TOUCH_THRESHOLD 45
#define TAP_WINDOW 800

static const eEmotions cycleEmotions[] = {
    eEmotions::Normal,
    eEmotions::Glee,
    eEmotions::Happy,
    eEmotions::Awe,
    eEmotions::Surprised,
    eEmotions::Focused,
    eEmotions::Sleepy,
    eEmotions::Sad,
    eEmotions::Worried,
    eEmotions::Skeptic,
    eEmotions::Unimpressed,
    eEmotions::Suspicious,
    eEmotions::Squint,
    eEmotions::Scared
};

static const eEmotions angryEmotions[] = {
    eEmotions::Annoyed,
    eEmotions::Angry,
    eEmotions::Frustrated,
    eEmotions::Furious
};

void ZEN_TouchEngine::begin(uint8_t pin) {
    _pin = pin;
    Serial.println("[TOUCH] Simple tap engine ready");
}

void ZEN_TouchEngine::update(ZEN_EmotionManager& emotions) {
    int raw = touchRead(_pin);
    bool pressed = raw < TOUCH_THRESHOLD;
    unsigned long now = millis();

    if (pressed && !_touching) {
        _touching = true;
    }

    if (!pressed && _touching) {
        _touching = false;

        if (now - _lastTapMs < TAP_WINDOW) {
            _tapCount++;
        } else {
            _tapCount = 1;
        }

        _lastTapMs = now;

        if (_tapCount >= 3) {
            tripleTap(emotions);
            _tapCount = 0;
        } else {
            singleTap(emotions);
        }
    }
}

void ZEN_TouchEngine::singleTap(ZEN_EmotionManager& emotions) {
    static int index = 0;
    emotions.trigger(cycleEmotions[index], 1.0f);
    Serial.printf("[TOUCH] TAP → emotion %d\n", cycleEmotions[index]);
    index = (index + 1) % (sizeof(cycleEmotions) / sizeof(eEmotions));
}

void ZEN_TouchEngine::tripleTap(ZEN_EmotionManager& emotions) {
    static int index = 0;
    emotions.trigger(angryEmotions[index], 1.0f);
    Serial.printf("[TOUCH] TRIPLE TAP → emotion %d\n", angryEmotions[index]);
    index = (index + 1) % (sizeof(angryEmotions) / sizeof(eEmotions));
}
