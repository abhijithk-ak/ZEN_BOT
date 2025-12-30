#pragma once
#include <Arduino.h>
#include "ZEN_EmotionManager.h"

// Touch state machine
enum TouchState {
    TOUCH_IDLE,      // Not touching
    TOUCH_PRESSED,   // Currently touching
    TOUCH_RELEASED   // Just released (gesture processing)
};

// Sleep lifecycle state
enum SleepState {
    AWAKE,           // Normal operation
    ASLEEP           // In sleep mode
};

class ZEN_TouchEngine {
public:
    void begin(uint8_t pin);
    void update(ZEN_EmotionManager& emotions);

private:
    uint8_t _pin;

    // Touch gesture tracking
    TouchState _touchState = TOUCH_IDLE;
    unsigned long _touchStartMs = 0;
    int _tapCount = 0;
    unsigned long _lastTapMs = 0;

    // Sleep lifecycle
    SleepState _sleepState = AWAKE;
    unsigned long _lastInteractionMs = 0;
    unsigned long _sleepEnteredMs = 0;

    // Emotion lock (prevents interruption during strong emotions)
    unsigned long _emotionLockUntilMs = 0;

    // Helper methods
    int readLevel(int raw);
    void processTap(ZEN_EmotionManager& emotions);
    void processLongPress(ZEN_EmotionManager& emotions);
    void processMultiTap(ZEN_EmotionManager& emotions);
    void enterSleep(ZEN_EmotionManager& emotions);
    void exitSleep(ZEN_EmotionManager& emotions);
};
