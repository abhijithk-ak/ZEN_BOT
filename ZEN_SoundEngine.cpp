#include "ZEN_SoundEngine.h"

void ZEN_SoundEngine::begin(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
    noTone(_pin);
}

void ZEN_SoundEngine::update(const ZEN_EmotionManager& emotions) {
    eEmotions current = emotions.current();
    if (current != _lastEmotion) {

        switch (current) {

            // POSITIVE
            case eEmotions::Happy:
            case eEmotions::Glee:
            case eEmotions::Awe:
                startPattern(SP_POSITIVE);
                break;

            // ALERT
            case eEmotions::Surprised:
            case eEmotions::Scared:
                startPattern(SP_ALERT);
                break;

            // NEGATIVE
            case eEmotions::Angry:
            case eEmotions::Furious:
            case eEmotions::Frustrated:
            case eEmotions::Annoyed:
                startPattern(SP_NEGATIVE);
                break;

            // LOW
            case eEmotions::Sad:
            case eEmotions::Worried:
            case eEmotions::Unimpressed:
            case eEmotions::Sleepy:
                startPattern(SP_LOW);
                break;

            // SUBTLE / NONE
            default:
                startPattern(SP_SUBTLE);
                break;
        }

        _lastEmotion = current;
    }

    runPattern();
}

void ZEN_SoundEngine::startPattern(SoundPattern p) {
    stop();
    _active = p;
    _startMs = millis();
}

void ZEN_SoundEngine::runPattern() {
    unsigned long elapsed = millis() - _startMs;

    switch (_active) {

        case SP_POSITIVE:
            if (elapsed < 80) tone(_pin, 1200);
            else if (elapsed < 120) noTone(_pin);
            else if (elapsed < 200) tone(_pin, 1200);
            else stop();
            break;

        case SP_ALERT:
            if (elapsed < 70) tone(_pin, 1500);
            else stop();
            break;

        case SP_NEGATIVE:
            if (elapsed < 100) tone(_pin, 400);
            else stop();
            break;

        case SP_LOW:
            if (elapsed < 180) tone(_pin, 250);
            else stop();
            break;

        case SP_SUBTLE:
            if (elapsed < 40) tone(_pin, 800);
            else stop();
            break;

        default:
            break;
    }
}

void ZEN_SoundEngine::stop() {
    noTone(_pin);
    _active = SP_NONE;
}
