#include "ZEN_TouchEngine.h"

// Touch thresholds (capacitive values)
#define TOUCH_THRESHOLD  45      // Touch detected if < this
#define HARD_THRESHOLD   15      // Hard press if < this

// Gesture timings (milliseconds)
#define TAP_DURATION_MAX       300   // Tap must be < 300ms
#define MULTITAP_WINDOW        2000  // 3+ taps within 2 seconds = strong emotion
#define LONG_PRESS_TIME        10000 // >= 10 seconds = sleep

// Lifecycle timings (testing durations)
#define AUTO_SLEEP_INACTIVITY  60000 // Sleep after 60s no interaction
#define AUTO_WAKE_DURATION     60000 // Auto-wake after 60s asleep
#define EMOTION_LOCK_TIME      10000 // Lock emotions for 10s after multi-tap

// Sleep constraints
#define MIN_SLEEP_DURATION     10000 // Must sleep at least 10 seconds

void ZEN_TouchEngine::begin(uint8_t pin) {
    _pin = pin;
    _lastInteractionMs = millis();
    _touchState = TOUCH_IDLE;
    _sleepState = AWAKE;
    Serial.println("[TOUCH] Lifecycle engine ready");
}

int ZEN_TouchEngine::readLevel(int raw) {
    if (raw <= HARD_THRESHOLD) return 2;      // Hard press
    if (raw <= TOUCH_THRESHOLD) return 1;      // Light touch
    return 0;                                  // Not touching
}

void ZEN_TouchEngine::enterSleep(ZEN_EmotionManager& emotions) {
    if (_sleepState == ASLEEP) return;  // Already asleep
    
    _sleepState = ASLEEP;
    _sleepEnteredMs = millis();
    emotions.trigger(eEmotions::Sleepy, 1.0f);
    Serial.println("[ZEN] → SLEEP");
}

void ZEN_TouchEngine::exitSleep(ZEN_EmotionManager& emotions) {
    if (_sleepState == AWAKE) return;  // Already awake
    
    _sleepState = AWAKE;
    _lastInteractionMs = millis();
    emotions.trigger(eEmotions::Normal, 0.8f);
    _tapCount = 0;  // Reset tap counter on wake
    Serial.println("[ZEN] → AWAKE");
}

void ZEN_TouchEngine::processTap(ZEN_EmotionManager& emotions) {
    static const eEmotions lightEmotions[] = {
        eEmotions::Happy,
        eEmotions::Glee,
        eEmotions::Surprised,
        eEmotions::Awe
    };
    static int lightIndex = 0;
    
    emotions.trigger(lightEmotions[lightIndex], 0.8f);
    lightIndex = (lightIndex + 1) % 4;
    
    Serial.printf("[ZEN] TAP → %d\n", lightIndex);
}

void ZEN_TouchEngine::processLongPress(ZEN_EmotionManager& emotions) {
    enterSleep(emotions);
    Serial.println("[ZEN] LONG PRESS → Sleep");
}

void ZEN_TouchEngine::processMultiTap(ZEN_EmotionManager& emotions) {
    static const eEmotions strongEmotions[] = {
        eEmotions::Annoyed,
        eEmotions::Angry,
        eEmotions::Furious
    };
    static int strongIndex = 0;
    
    emotions.trigger(strongEmotions[strongIndex], 1.0f);
    strongIndex = (strongIndex + 1) % 3;
    
    // LOCK emotion for 10 seconds
    _emotionLockUntilMs = millis() + EMOTION_LOCK_TIME;
    
    Serial.printf("[ZEN] MULTI-TAP (lock 10s) → %d\n", strongIndex);
}

void ZEN_TouchEngine::update(ZEN_EmotionManager& emotions) {
    unsigned long now = millis();
    int raw = touchRead(_pin);
    int level = readLevel(raw);

    // ========================================
    // 🔒 EMOTION LOCK CHECK
    // ========================================
    if (now < _emotionLockUntilMs) {
        // Locked: ignore all touch input except for auto-timers
        // Continue to lifecycle checks only
    } else if (_emotionLockUntilMs > 0) {
        // Lock just expired
        _emotionLockUntilMs = 0;
        Serial.println("[ZEN] Emotion lock released");
    }

    // ========================================
    // 😴 AUTO SLEEP (no interaction for 60s)
    // ========================================
    if (_sleepState == AWAKE && (now - _lastInteractionMs) > AUTO_SLEEP_INACTIVITY) {
        enterSleep(emotions);
        return;
    }

    // ========================================
    // 😴 AUTO WAKE (asleep for 60s)
    // ========================================
    if (_sleepState == ASLEEP && (now - _sleepEnteredMs) > AUTO_WAKE_DURATION) {
        exitSleep(emotions);
        return;
    }

    // ========================================
    // ✋ TOUCH STATE MACHINE
    // ========================================

    // PRESS: Not touching → touching
    if (level > 0 && _touchState == TOUCH_IDLE) {
        _touchState = TOUCH_PRESSED;
        _touchStartMs = now;
        Serial.printf("[ZEN] PRESS (raw=%d)\n", raw);
        return;
    }

    // HOLD: Still touching (check for long press timeout)
    if (level > 0 && _touchState == TOUCH_PRESSED) {
        unsigned long held = now - _touchStartMs;
        
        // If emotion is locked, ignore
        if (now < _emotionLockUntilMs) {
            return;
        }
        
        // Long press threshold for sleep
        if (held >= LONG_PRESS_TIME && _sleepState == AWAKE) {
            _touchState = TOUCH_RELEASED;
            processLongPress(emotions);
            _touchState = TOUCH_IDLE;
            return;
        }
        
        return;  // Still holding, wait for release
    }

    // RELEASE: Touching → not touching (process gesture)
    if (level == 0 && _touchState == TOUCH_PRESSED) {
        _touchState = TOUCH_RELEASED;
        unsigned long held = now - _touchStartMs;

        // ---- WAKE from sleep ----
        if (_sleepState == ASLEEP && held < TAP_DURATION_MAX) {
            unsigned long sleepDuration = now - _sleepEnteredMs;
            if (sleepDuration >= MIN_SLEEP_DURATION) {
                exitSleep(emotions);
                Serial.println("[ZEN] Woken by tap");
                _touchState = TOUCH_IDLE;
                return;
            }
            // Too early to wake, ignore
            _touchState = TOUCH_IDLE;
            return;
        }

        // ---- Process if awake ----
        if (_sleepState == AWAKE) {
            _lastInteractionMs = now;

            // If locked, ignore all input
            if (now < _emotionLockUntilMs) {
                Serial.println("[ZEN] Touch ignored (locked)");
                _touchState = TOUCH_IDLE;
                return;
            }

            // ---- TAP LOGIC ----
            if (held < TAP_DURATION_MAX) {
                // Check if this is part of multi-tap sequence
                if ((now - _lastTapMs) < MULTITAP_WINDOW) {
                    _tapCount++;
                } else {
                    _tapCount = 1;
                }
                _lastTapMs = now;

                // Multi-tap: 3 or more taps
                if (_tapCount >= 3) {
                    processMultiTap(emotions);
                    _tapCount = 0;
                    _touchState = TOUCH_IDLE;
                    return;
                }

                // Single/double tap (treat as single light emotion)
                if (_tapCount == 1) {
                    processTap(emotions);
                    _touchState = TOUCH_IDLE;
                    return;
                }

                _touchState = TOUCH_IDLE;
                return;
            }

            // ---- LONG PRESS already handled above ----
        }

        _touchState = TOUCH_IDLE;
        return;
    }

    // IDLE: Not touching, no state change needed
}
