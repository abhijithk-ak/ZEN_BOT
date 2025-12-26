# ZEN_BOT - Emotional AI Robot
## Version 1.1 - Phase 2.4 Complete

An ESP32-based emotional robot with integrated perception, expression, and behavior systems. ZEN_BOT responds to emotions with synchronized facial expressions, mood lighting, motor movements, and sound cues.

![Status](https://img.shields.io/badge/status-stable-brightgreen)
![Version](https://img.shields.io/badge/version-1.1-blue)
![License](https://img.shields.io/badge/license-MIT-green)

---

## 🎯 Features

### Core Systems
- **Emotion Management** - 19 emotion types with intelligent blending and decay
- **Face Engine** - Organic eye animations with blinking, looking, and behavioral patterns
- **Mood Light** - WS2812 NeoPixel that changes color based on current emotion
- **Motor Control** - Synchronized micro-movements responding to emotional state
- **Sound Engine** - Audio feedback and emotion-based sound cues via DAC

### Hardware Integration
- **ESP32** microcontroller (240 MHz dual-core)
- **OLED Display** - 128x64 SSD1306 for real-time eye rendering
- **WS2812 NeoPixel** - Mood lighting on GPIO 4
- **DRV8833 Motor Driver** - Dual brushed motor control
- **PAM8403 Audio Amplifier** - Audio output via DAC (GPIO 25)
- **I2C/Wire Protocol** - Display and sensor communication

---

## 📦 Project Structure

```
zen_bot/
├── zen_bot.ino              # Main sketch - system orchestration
├── ZEN_Emotion.h            # Emotion type definitions
├── ZEN_EmotionManager.*     # Emotion state & triggering system
├── ZEN_EmotionBlender.*     # Emotion blending & decay logic
├── ZEN_MoodLight.*          # WS2812 LED control & color mapping
├── ZEN_MotorController.*    # DRV8833 motor behavior
├── ZEN_SoundEngine.*        # PAM8403 audio playback
├── ZEN_FaceEngine.*         # OLED face drawing (legacy)
└── lib/                     # External libraries (via Arduino IDE)
    └── esp32-eyes/          # Face animation library
```

---

## 🚀 Quick Start

### Prerequisites
- **Arduino IDE** 2.x
- **ESP32 Board Support** (via Boards Manager)
- **Libraries:**
  - Adafruit GFX
  - Adafruit SSD1306
  - esp32-eyes (included in Arduino libraries)

### Installation
1. Clone this repository
2. Open `zen_bot.ino` in Arduino IDE
3. Select **Board:** ESP32 Dev Module
4. Select **Port:** USB port where ESP32 is connected
5. Click **Upload**

### Wiring
| Component | GPIO | Purpose |
|-----------|------|---------|
| OLED SDA | 21 | I2C Display |
| OLED SCL | 22 | I2C Display |
| WS2812 Data | 4 | Mood Light |
| Motor Left A | 32 | Motor Forward |
| Motor Left B | 33 | Motor Backward |
| Motor Right A | 26 | Motor Forward |
| Motor Right B | 27 | Motor Backward |
| Audio Out | 25 | DAC Amplifier |

---

## 💡 Usage Examples

### Trigger an Emotion
```cpp
// In loop() or event handler
emotionManager.trigger(eEmotions::Happy, 1.0f);
```

### Supported Emotions
Normal, Happy, Sad, Angry, Sleepy, Worried, Focused, Annoyed, Surprised, Skeptic, Frustrated, Unimpressed, Suspicious, Squint, Furious, Scared, Awe, Glee

### System Update Loop
```cpp
void loop() {
    // Update face animation
    if (face) {
        face->Update();
    }

    // Update emotion state (decay, blending)
    emotionManager.update();

    // Propagate emotions to hardware
    moodLight.update(emotionManager);
    motors.update(emotionManager);
    sound.update(emotionManager);

    delay(16);  // ~60 FPS
}
```

---

## 🔧 Architecture

### Emotion Propagation Flow
```
trigger(emotion)
    ↓
ZEN_EmotionBlender (mix, decay)
    ↓
emotionManager.current() → ZENEmotion
    ↓
[Parallel Distribution]
    ├→ Face renders based on emotion
    ├→ MoodLight applies color mapping
    ├→ Motors execute behavior patterns
    └→ Sound plays emotion cues
```

### Key Design Decisions
- **Separation of Concerns** - Each module is independent
- **Pull Architecture** - Modules pull emotion state on demand (not pushed)
- **Single Source of Truth** - ZEN_EmotionManager is the emotion authority
- **No Global Ownership Conflicts** - Clean module initialization order
- **Null Safety** - Nullptr guards in critical paths (Face engine)

---

## 🐛 Version History

### v1.1 (Current)
- **Fixed:** Emotion propagation to hardware modules
- **Fixed:** Face engine initialization and nullptr safety
- **Added:** DRV8833 motor synchronization
- **Added:** Audio feedback system
- **Status:** All systems integrated and stable

### v1.0
- Initial release
- Face engine with esp32-eyes
- Basic emotion management
- Status:** Face working, other modules disconnected

---

## 📝 Development Notes

### Debugging Emotion System
```cpp
Serial.print("Current Emotion: ");
Serial.println((int)emotionManager.current());

Serial.print("Intensity: ");
Serial.println(emotionManager.current());  // Check blender intensity
```

### Motor Control Pins
- **Left Motor:** GPIO 32 (A), GPIO 33 (B)
- **Right Motor:** GPIO 26 (A), GPIO 27 (B)
- PWM-ready pins for speed control

### Audio Output
- **DAC Pin:** GPIO 25 (8-bit @ 0-255)
- **Supports:** Tone generation, audio playback
- **Amplifier:** PAM8403 (requires power supply)

---

## 🤝 Contributing

To contribute improvements:
1. Create a feature branch
2. Commit with descriptive messages
3. Test thoroughly before pushing
4. Update this README with changes

---

## 📄 License

MIT License - See LICENSE file for details

---

## 👤 Author

**Abhijith K** (abhijithk-ak)  
GitHub: [@abhijithk-ak](https://github.com/abhijithk-ak)

---

## 🔗 Resources

- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [Arduino IDE](https://www.arduino.cc/en/software)
- [Adafruit Libraries](https://github.com/adafruit)
- [esp32-eyes Library](https://github.com/playfultechnology/esp32-eyes) - Face animation and eye rendering engine

---

**Last Updated:** December 27, 2025