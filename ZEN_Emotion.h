#pragma once
#include <Face.h>

// Wrap esp32-eyes emotions safely
using ZENEmotion = eEmotions;

// Helper constants
namespace ZENEmotionConst {
    const float MAX_INTENSITY = 1.0f;
    const float MIN_INTENSITY = 0.0f;
}
