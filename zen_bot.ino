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

    face = new Face(SCREEN_WIDTH, SCREEN_HEIGHT, EYE_SIZE);
    face->EyeInterDistance = 10;       // Subtle spacing between eyes
    face->RandomBlink = true;         // Enable organic blinking
    face->RandomLook = true;          // Enable eye movement
    face->RandomBehavior = true;      // Enable idle behaviors

    Serial.println("ZEN booted — Face engine online");
}

void loop() {
    if (face) {
        face->Update();
    }
    delay(16);  // ~60 FPS
}
