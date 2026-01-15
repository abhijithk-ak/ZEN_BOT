#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Face.h>
#include "ZEN_EmotionManager.h"
#include "ZEN_EyeEmotionEngine.h"
#include "ZEN_MoodLight.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define EYE_SIZE 24
#define TOUCH_PIN 15        // GPIO 15 (T3) - Capacitive touch pin

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
extern Adafruit_GFX* gfx;

Face* face = nullptr;

ZEN_EmotionManager emotionManager;
ZEN_EyeEmotionEngine eyeEngine;
ZEN_MoodLight moodLight;

void setup() {
    Serial.begin(115200);
    delay(100);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED failed");
        while (1);
    }

    gfx = &display;
    delay(10);

    // ---- FACE ----
    face = new Face(SCREEN_WIDTH, SCREEN_HEIGHT, EYE_SIZE);
    face->EyeInterDistance = 10;
    face->RandomBlink = true;
    face->RandomLook = true;
    face->RandomBehavior = true;

    // ---- EYE EMOTION ENGINE ----
    eyeEngine.begin(face);

    // ---- PHASE 3.4 CORE MODULES ----
    moodLight.begin(4);        // WS2812 on GPIO 4
    emotionManager.begin();    // Initialize emotion state

    emotionManager.trigger(eEmotions::Happy, 1.0f);

    Serial.println("ZEN booted — Phase 3.4 Core Stable");
}

void loop() {
    emotionManager.update();              // 1️⃣ Emotion decay

    eyeEngine.update(emotionManager);     // 2️⃣ Apply emotions to eyes

    if (face) {
        face->Update();                   // 3️⃣ Render face
    }

    moodLight.update(emotionManager);     // 4️⃣ Apply emotions to mood light

    delay(16);
}
