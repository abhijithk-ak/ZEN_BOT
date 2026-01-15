#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Face.h>
#include "ZEN_EmotionManager.h"
#include "ZEN_EyeEmotionEngine.h"
#include "ZEN_MoodLight.h"
#include "ZEN_MotorController.h"
#include "ZEN_SoundEngine.h"
#include "ZEN_TouchEngine.h"
#include "ZEN_ModeManager.h"
#include "ZEN_Bluetooth.h"

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
ZEN_MotorController motors;
ZEN_SoundEngine sound;
ZEN_TouchEngine touch;
ZEN_ModeManager modeManager;
ZEN_Bluetooth bt;

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

    // ---- RE-ENABLE PHASE 2 MODULES ----
    moodLight.begin(4);     // WS2812 on GPIO 4
    sound.begin(25);        // Piezo speaker on GPIO 25
    motors.begin();         // DRV8833 pins already defined internally
    emotionManager.begin(); // Initialize emotion state
    touch.begin(TOUCH_PIN); // Capacitive touch on GPIO 15 (T3)

    // ---- PHASE 4.1 MODE MANAGER ----
    modeManager.begin();    // Initialize mode state

    // ---- PHASE 4.3 BLUETOOTH ----
    bt.begin("ZEN-BOT");    // Start Bluetooth with name

    emotionManager.trigger(eEmotions::Happy, 1.0f);

    Serial.println("ZEN booted — Phase 4.3 Lifecycle online");
}

void loop() {
    // ---- PHASE 4.3: BLUETOOTH COMMAND HANDLING ----
    if (bt.available()) {
        char cmd = bt.read();
        Serial.printf("[BT] CMD: %c\n", cmd);

        switch (cmd) {
            // Mode switching
            case '0': modeManager.set(MODE_EMO); break;
            case '1': modeManager.set(MODE_CONTROLLED); break;
            case '2': modeManager.set(MODE_ASSISTANT); break;

            // Manual motor control (only in CONTROLLED mode)
            case 'F': if (modeManager.is(MODE_CONTROLLED)) motors.manualForward(); break;
            case 'B': if (modeManager.is(MODE_CONTROLLED)) motors.manualBack(); break;
            case 'L': if (modeManager.is(MODE_CONTROLLED)) motors.manualLeft(); break;
            case 'R': if (modeManager.is(MODE_CONTROLLED)) motors.manualRight(); break;
            case 'S': motors.manualStop(); break;

            // Emotion triggers (works in any mode)
            case 'H': emotionManager.trigger(eEmotions::Happy, 1.0f); break;
            case 'A': emotionManager.trigger(eEmotions::Angry, 1.0f); break;
            case 'Z': emotionManager.trigger(eEmotions::Sleepy, 1.0f); break;
        }
    }

    modeManager.update();                 // 0️⃣ Mode authority
    emotionManager.update();              // 1️⃣ Emotion decay

    if (modeManager.is(MODE_EMO)) {
        touch.update(emotionManager);     // 2️⃣ Touch only in EMO mode
    }

    eyeEngine.update(emotionManager);     // 3️⃣ Apply emotions to eyes

    if (face) {
        face->Update();                   // 4️⃣ Render face
    }

    moodLight.update(emotionManager);     // 5️⃣ Apply emotions to mood light
    motors.update(emotionManager, modeManager);  // 6️⃣ Apply emotions to motors
    sound.update(emotionManager);         // 7️⃣ Apply emotions to sound

    delay(16);
}
