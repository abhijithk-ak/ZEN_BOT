#include "ZEN_FaceEngine.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

static Adafruit_SSD1306 display(
    SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1
);

void ZEN_FaceEngine::begin() {
    Wire.begin(21, 22);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("[FACE] OLED init failed");
        return;
    }

    display.clearDisplay();
    display.display();
    Serial.println("[FACE] OLED ready");
}

void ZEN_FaceEngine::update(const ZEN_EmotionManager& emotions) {
    blinkLogic();

    eEmotions emo = emotions.current();

    int eyeHeight = 18;
    int eyeWidth  = 14;
    int offsetY   = 0;

    if (emo == eEmotions::Sleepy) {
        eyeHeight = 8;
        offsetY = 6;
    }
    else if (emo == eEmotions::Angry) {
        eyeHeight = 10;
    }
    else if (emo == eEmotions::Happy) {
        eyeHeight = 20;
    }

    display.clearDisplay();
    drawEyes(eyeHeight, eyeWidth, offsetY);
    display.display();
}

void ZEN_FaceEngine::drawEyes(int h, int w, int yOffset) {
    int leftX  = 40;
    int rightX = 74;
    int y = 24 + yOffset;

    if (_blinking) h = 2;

    display.fillRoundRect(leftX,  y, w, h, 4, SSD1306_WHITE);
    display.fillRoundRect(rightX, y, w, h, 4, SSD1306_WHITE);
}

void ZEN_FaceEngine::blinkLogic() {
    unsigned long now = millis();

    if (!_blinking && now - _lastBlink > random(3000, 6000)) {
        _blinking = true;
        _lastBlink = now;
    }

    if (_blinking && now - _lastBlink > 120) {
        _blinking = false;
        _lastBlink = now;
    }
}
