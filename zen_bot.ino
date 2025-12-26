#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Face.h>
#include "ZEN_EmotionManager.h"
#include "ZEN_MoodLight.h"
#include "ZEN_MotorController.h"
#include "ZEN_SoundEngine.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define EYE_SIZE 24

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
extern Adafruit_GFX* gfx;

Face* face = nullptr;

ZEN_EmotionManager emotionManager;
ZEN_MoodLight moodLight;
ZEN_MotorController motors;
ZEN_SoundEngine sound;

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

    // ---- RE-ENABLE PHASE 2 MODULES ----
    moodLight.begin(4);     // WS2812 on GPIO 4
    sound.begin(25);        // PAM8403/DAC on GPIO 25
    motors.begin();         // DRV8833 pins already defined internally
    emotionManager.begin(); // Initialize emotion state

    emotionManager.trigger(eEmotions::Happy, 1.0f);

    Serial.println("ZEN booted — Phase 2.4 online");
}

void loop() {
    if (face) {
        face->Update();
    }

    emotionManager.update();              // Update emotion state

    moodLight.update(emotionManager);     // Apply emotions to mood light
    motors.update(emotionManager);        // Apply emotions to motors
    sound.update(emotionManager);         // Apply emotions to sound

    delay(16);
}
