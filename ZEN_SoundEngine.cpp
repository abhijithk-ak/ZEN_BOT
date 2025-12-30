#include "ZEN_SoundEngine.h"

void ZEN_SoundEngine::begin(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
    silence();
    Serial.println("[SOUND] Phase 3.2 ready");
}

void ZEN_SoundEngine::setMuted(bool m) {
    _muted = m;
    if (_muted) noTone(_pin);
}

void ZEN_SoundEngine::update(const ZEN_EmotionManager& emotions) {
    if (_muted) return;  // Don't play sounds while muted
    
    eEmotions current = emotions.current();

    if (current != _lastEmotion) {
        _lastEmotion = current;

        switch (current) {
            case eEmotions::Happy:
            case eEmotions::Glee:
            case eEmotions::Awe:
                startPattern(SP_POSITIVE);
                break;

            case eEmotions::Surprised:
            case eEmotions::Scared:
                startPattern(SP_ALERT);
                break;

            case eEmotions::Angry:
            case eEmotions::Furious:
            case eEmotions::Annoyed:
            case eEmotions::Frustrated:
                startPattern(SP_NEGATIVE);
                break;

            case eEmotions::Focused:
            case eEmotions::Skeptic:
            case eEmotions::Suspicious:
            case eEmotions::Squint:
                startPattern(SP_COGNITIVE);
                break;

            case eEmotions::Sad:
            case eEmotions::Worried:
            case eEmotions::Unimpressed:
            case eEmotions::Sleepy:
                startPattern(SP_SLEEP);
                break;

            default:
                silence();
                break;
        }
    }

    runPattern();
}

void ZEN_SoundEngine::startPattern(SoundPattern p) {
    _active = p;
    _startMs = millis();
}

void ZEN_SoundEngine::runPattern() {
    if (_active == SP_NONE) return;

    unsigned long t = millis() - _startMs;

    switch (_active) {

        case SP_POSITIVE:
            if (t < 60) tone(_pin, 1200);
            else if (t < 100) noTone(_pin);
            else if (t < 160) tone(_pin, 1400);
            else silence();
            break;

        case SP_ALERT:
            if (t < 80) tone(_pin, 1600);
            else silence();
            break;

        case SP_NEGATIVE:
            if (t < 100) tone(_pin, 400);
            else silence();
            break;

        case SP_COGNITIVE:
            if (t < 50) tone(_pin, 900);
            else silence();
            break;

        case SP_SLEEP:
            if (t < 200) tone(_pin, 250);
            else silence();
            break;

        default:
            silence();
            break;
    }
}

void ZEN_SoundEngine::silence() {
    noTone(_pin);
    _active = SP_NONE;
}
