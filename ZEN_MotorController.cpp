#include "ZEN_MotorController.h"

#define AIN1 32
#define AIN2 33
#define BIN1 26
#define BIN2 27

#define PWM_MIN 40
#define PWM_MAX 140

void ZEN_MotorController::begin() {
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    stop();
    Serial.println("[MOTOR] Ready");
}

void ZEN_MotorController::update(const ZEN_EmotionManager& emotions) {
    eEmotions emo = emotions.current();
    unsigned long now = millis();

    if (now - _lastAction < 2000) return;
    _lastAction = now;

    switch (emo) {
        case eEmotions::Happy:
        case eEmotions::Glee:
            behaviorHappy();
            break;

        case eEmotions::Focused:
            behaviorFocused();
            break;

        case eEmotions::Annoyed:
        case eEmotions::Frustrated:
            behaviorAnnoyed();
            break;

        case eEmotions::Sleepy:
            behaviorSleepy();
            break;

        default:
            stop();
            break;
    }
}

void ZEN_MotorController::behaviorHappy() {
    drive(PWM_MAX, PWM_MAX);
    delay(80);
    stop();
}

void ZEN_MotorController::behaviorFocused() {
    drive(PWM_MIN + 20, PWM_MIN + 20);
    delay(50);
    stop();
}

void ZEN_MotorController::behaviorAnnoyed() {
    drive(PWM_MAX, -PWM_MAX);
    delay(60);
    stop();
}

void ZEN_MotorController::behaviorSleepy() {
    drive(PWM_MIN, -PWM_MIN);
    delay(120);
    stop();
}

void ZEN_MotorController::drive(int left, int right) {
    analogWrite(AIN1, left > 0 ? left : 0);
    analogWrite(AIN2, left < 0 ? -left : 0);
    analogWrite(BIN1, right > 0 ? right : 0);
    analogWrite(BIN2, right < 0 ? -right : 0);
}

void ZEN_MotorController::stop() {
    analogWrite(AIN1, 0);
    analogWrite(AIN2, 0);
    analogWrite(BIN1, 0);
    analogWrite(BIN2, 0);
}
