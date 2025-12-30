#include "ZEN_EyeEmotionEngine.h"

void ZEN_EyeEmotionEngine::begin(Face* face) {
    _face = face;
}

void ZEN_EyeEmotionEngine::drawSleepZZZ() {
    if (!_face) return;

    unsigned long now = millis();
    int phase = (now - _sleepAnimMs) / 400;  // Cycle every 400ms

    // Draw ZZZ that grows (small z to large z floating up)
    // This is simple text animation on the OLED
    extern Adafruit_GFX* gfx;
    if (!gfx) return;

    int z_size = 1 + (phase % 3);  // Sizes: 1, 2, 3
    int y_pos = 45 - (phase % 3) * 5;  // Float upward

    gfx->setTextSize(z_size);
    gfx->setTextColor(SSD1306_WHITE);
    gfx->setCursor(105, y_pos);
    gfx->print("Z");
}

void ZEN_EyeEmotionEngine::update(const ZEN_EmotionManager& emotions) {
    if (!_face) return;

    eEmotions current = emotions.current();
    bool sleeping = emotions.isSleeping();

    // ---- SLEEP MODE: Disable blinking, show ZZZ ----
    if (sleeping) {
        if (current != eEmotions::Sleepy) {
            _lastEmotion = eEmotions::Sleepy;
            _face->Expression.GoTo_Sleepy();
            _face->RandomBlink = false;   // No random blinking
            _face->RandomLook = false;
            _face->RandomBehavior = false;
            _sleepAnimMs = millis();
        }
        drawSleepZZZ();
        return;
    }

    // ---- AWAKE MODE: Normal emotion processing ----
    if (current == _lastEmotion) return;

    _lastEmotion = current;
    applyEmotion(current);
}

void ZEN_EyeEmotionEngine::applyEmotion(eEmotions e) {
    // Reset autonomous systems (keep organic feel)
    _face->RandomBlink = true;
    _face->RandomLook = true;
    _face->RandomBehavior = true;

    switch (e) {

        case eEmotions::Normal:
            _face->Expression.GoTo_Normal();
            break;

        case eEmotions::Angry:
            _face->Expression.GoTo_Angry();
            break;

        case eEmotions::Glee:
            _face->Expression.GoTo_Glee();
            break;

        case eEmotions::Happy:
            _face->Expression.GoTo_Happy();
            break;

        case eEmotions::Sad:
            _face->Expression.GoTo_Sad();
            break;

        case eEmotions::Worried:
            _face->Expression.GoTo_Worried();
            break;

        case eEmotions::Focused:
            _face->Expression.GoTo_Focused();
            break;

        case eEmotions::Annoyed:
            _face->Expression.GoTo_Annoyed();
            break;

        case eEmotions::Surprised:
            _face->Expression.GoTo_Surprised();
            break;

        case eEmotions::Skeptic:
            _face->Expression.GoTo_Skeptic();
            break;

        case eEmotions::Frustrated:
            _face->Expression.GoTo_Frustrated();
            break;

        case eEmotions::Unimpressed:
            _face->Expression.GoTo_Unimpressed();
            break;

        case eEmotions::Sleepy:
            _face->Expression.GoTo_Sleepy();
            break;

        case eEmotions::Suspicious:
            _face->Expression.GoTo_Suspicious();
            break;

        case eEmotions::Squint:
            _face->Expression.GoTo_Squint();
            break;

        case eEmotions::Furious:
            _face->Expression.GoTo_Furious();
            break;

        case eEmotions::Scared:
            _face->Expression.GoTo_Scared();
            break;

        case eEmotions::Awe:
            _face->Expression.GoTo_Awe();
            break;

        default:
            _face->Expression.GoTo_Normal();
            break;
    }
}
