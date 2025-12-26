#include "ZEN_MoodLight.h"

void ZEN_MoodLight::begin(uint8_t pin) {
    _led.setPin(pin);
    _led.begin();
    _led.clear();
    _led.show();

    Serial.println("[MOOD] WS2812 ready");
}

void ZEN_MoodLight::update(const ZEN_EmotionManager& emotions) {
    eEmotions emo = emotions.current();

    // Emotion → Color mapping (ZEN emotional palette)
    switch (emo) {

        case eEmotions::Happy:
        case eEmotions::Glee:
            setTargetColor(255, 180, 60);   // warm joy yellow
            break;

        case eEmotions::Awe:
            setTargetColor(180, 160, 255);  // soft lavender
            break;

        case eEmotions::Sad:
        case eEmotions::Worried:
            setTargetColor(60, 80, 140);    // emotional blue
            break;

        case eEmotions::Focused:
            setTargetColor(80, 140, 120);   // calm teal
            break;

        case eEmotions::Angry:
        case eEmotions::Annoyed:
        case eEmotions::Frustrated:
        case eEmotions::Furious:
            setTargetColor(255, 50, 40);    // restrained red
            break;

        case eEmotions::Scared:
        case eEmotions::Suspicious:
            setTargetColor(140, 40, 160);   // uneasy purple
            break;

        case eEmotions::Surprised:
            setTargetColor(200, 200, 255);  // pale white-blue
            break;

        case eEmotions::Skeptic:
        case eEmotions::Unimpressed:
        case eEmotions::Squint:
            setTargetColor(90, 90, 90);     // neutral gray
            break;

        case eEmotions::Sleepy:
            setTargetColor(40, 60, 120);    // dim sleepy blue
            break;

        case eEmotions::Normal:
        default:
            setTargetColor(40, 120, 60);    // calm green
            break;
    }

    breathe();
}

void ZEN_MoodLight::setTargetColor(uint8_t r, uint8_t g, uint8_t b) {
    _tgtR = r;
    _tgtG = g;
    _tgtB = b;
}

void ZEN_MoodLight::breathe() {
    unsigned long now = millis();
    if (now - _lastUpdate < 20) return;
    _lastUpdate = now;

    // Smooth color interpolation
    _curR += (_tgtR - _curR) * 0.05f;
    _curG += (_tgtG - _curG) * 0.05f;
    _curB += (_tgtB - _curB) * 0.05f;

    // Breathing brightness (very subtle)
    _breathPhase += 0.02f;
    if (_breathPhase > TWO_PI) _breathPhase = 0;

    float breath = (sin(_breathPhase) + 1.0f) * 0.5f;
    uint8_t scale = 35 + breath * 55;

    _led.setPixelColor(
        0,
        (_curR * scale) / 255,
        (_curG * scale) / 255,
        (_curB * scale) / 255
    );
    _led.show();
}
