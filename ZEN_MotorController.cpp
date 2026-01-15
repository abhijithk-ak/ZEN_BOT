#include "ZEN_MotorController.h"

#define AIN1 32
#define AIN2 33
#define BIN1 26
#define BIN2 27

#define PWM_FREQ 1000
#define PWM_RES 8

#define CH_A1 0
#define CH_A2 1
#define CH_B1 2
#define CH_B2 3

void ZEN_MotorController::begin() {
    ledcSetup(CH_A1, PWM_FREQ, PWM_RES);
    ledcSetup(CH_A2, PWM_FREQ, PWM_RES);
    ledcSetup(CH_B1, PWM_FREQ, PWM_RES);
    ledcSetup(CH_B2, PWM_FREQ, PWM_RES);

    ledcAttachPin(AIN1, CH_A1);
    ledcAttachPin(AIN2, CH_A2);
    ledcAttachPin(BIN1, CH_B1);
    ledcAttachPin(BIN2, CH_B2);

    stopAll();
    Serial.println("[MOTOR] Emotion engine ready");
}

void ZEN_MotorController::update(const ZEN_EmotionManager& emotions,
                                 const ZEN_ModeManager& mode) {
    // Only in EMO mode
    if (!mode.is(MODE_EMO)) {
        stopAll();
        _active = MP_NONE;
        return;
    }

    eEmotions current = emotions.current();

    // Trigger ONLY on emotion change
    if (current != _lastEmotion) {
        _lastEmotion = current;

        switch (current) {
            case eEmotions::Happy:
                startPattern(MP_FORWARD);
                break;

            case eEmotions::Glee:
                startPattern(MP_DOUBLE);
                break;

            case eEmotions::Surprised:
            case eEmotions::Focused:
            case eEmotions::Skeptic:
            case eEmotions::Suspicious:
                startPattern(MP_TWITCH);
                break;

            case eEmotions::Angry:
            case eEmotions::Furious:
                startPattern(MP_JERK);
                break;

            case eEmotions::Sad:
            case eEmotions::Worried:
            case eEmotions::Unimpressed:
                startPattern(MP_FORWARD);
                break;

            default:
                // Normal, Sleepy, Awe, Scared → no motor
                break;
        }
    }

    runPattern();
}

void ZEN_MotorController::startPattern(MotorPattern p) {
    _active = p;
    _startMs = millis();
}

void ZEN_MotorController::runPattern() {
    unsigned long t = millis() - _startMs;

    switch (_active) {

        case MP_FORWARD:
            if (t < 80) drive(100, 100);
            else stopAll();
            if (t >= 120) _active = MP_NONE;
            break;

        case MP_DOUBLE:
            if (t < 60) drive(100, 100);
            else if (t < 120) stopAll();
            else if (t < 180) drive(100, 100);
            else { stopAll(); _active = MP_NONE; }
            break;

        case MP_TWITCH:
            if (t < 40) drive(80, -80);
            else { stopAll(); _active = MP_NONE; }
            break;

        case MP_JERK:
            if (t < 70) drive(120, -120);
            else { stopAll(); _active = MP_NONE; }
            break;

        default:
            break;
    }
}

void ZEN_MotorController::drive(int left, int right) {
    left = constrain(left, -255, 255);
    right = constrain(right, -255, 255);

    ledcWrite(CH_A1, left > 0 ? left : 0);
    ledcWrite(CH_A2, left < 0 ? -left : 0);
    ledcWrite(CH_B1, right > 0 ? right : 0);
    ledcWrite(CH_B2, right < 0 ? -right : 0);
}

void ZEN_MotorController::stopAll() {
    ledcWrite(CH_A1, 0);
    ledcWrite(CH_A2, 0);
    ledcWrite(CH_B1, 0);
    ledcWrite(CH_B2, 0);
}

// ============ MANUAL CONTROL (Bluetooth / RC) ============

void ZEN_MotorController::manualForward() {
    ledcWrite(CH_A1, 120); ledcWrite(CH_A2, 0);
    ledcWrite(CH_B1, 120); ledcWrite(CH_B2, 0);
}

void ZEN_MotorController::manualBack() {
    ledcWrite(CH_A1, 0); ledcWrite(CH_A2, 120);
    ledcWrite(CH_B1, 0); ledcWrite(CH_B2, 120);
}

void ZEN_MotorController::manualLeft() {
    ledcWrite(CH_A1, 0); ledcWrite(CH_A2, 120);
    ledcWrite(CH_B1, 120); ledcWrite(CH_B2, 0);
}

void ZEN_MotorController::manualRight() {
    ledcWrite(CH_A1, 120); ledcWrite(CH_A2, 0);
    ledcWrite(CH_B1, 0); ledcWrite(CH_B2, 120);
}

void ZEN_MotorController::manualStop() {
    stopAll();
}
