#include "ZEN_SoundEngine.h"
#include <driver/dac.h>

#define SOUND_COOLDOWN 2500  // ms
#define VOLUME 50          // 0–255 (keep LOW for safety)

void ZEN_SoundEngine::begin(uint8_t dacPin) {
    _dacPin = dacPin;
    dac_output_enable((dac_channel_t)(dacPin == 25 ? DAC_CHANNEL_1 : DAC_CHANNEL_2));
    Serial.println("[SOUND] Engine ready");
}

void ZEN_SoundEngine::update(const ZEN_EmotionManager& emotions) {
    unsigned long now = millis();
    if (now - _lastSound < SOUND_COOLDOWN) return;

    eEmotions emo = emotions.current();
    emotionSound(emo);
    _lastSound = now;
}

void ZEN_SoundEngine::emotionSound(eEmotions emo) {
    switch (emo) {
        case eEmotions::Happy:
        case eEmotions::Glee:
            playTone(880, 80);
            playTone(1200, 60);
            break;

        case eEmotions::Sleepy:
            playTone(220, 150);
            break;

        case eEmotions::Annoyed:
        case eEmotions::Frustrated:
            playTone(180, 120);
            break;

        case eEmotions::Surprised:
            playTone(1400, 50);
            break;

        case eEmotions::Focused:
            playTone(600, 60);
            break;

        default:
            break;
    }
}

void ZEN_SoundEngine::playTone(float freq, uint16_t ms) {
    const uint32_t period = 1000000 / freq;
    uint32_t cycles = (ms * 1000) / period;

    for (uint32_t i = 0; i < cycles; i++) {
        dac_output_voltage((dac_channel_t)(_dacPin == 25 ? DAC_CHANNEL_1 : DAC_CHANNEL_2), VOLUME);
        delayMicroseconds(period / 2);
        dac_output_voltage((dac_channel_t)(_dacPin == 25 ? DAC_CHANNEL_1 : DAC_CHANNEL_2), 0);
        delayMicroseconds(period / 2);
    }
}
