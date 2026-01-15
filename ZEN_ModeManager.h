#pragma once
#include <Arduino.h>

enum ZEN_Mode {
    MODE_EMO = 0,
    MODE_ASSISTANT,
    MODE_CLOCK,
    MODE_FOLLOW,
    MODE_CONTROLLED,
    MODE_GAME
};

class ZEN_ModeManager {
public:
    void begin();
    void update();

    void set(ZEN_Mode m);
    ZEN_Mode current() const;
    bool is(ZEN_Mode m) const;

private:
    ZEN_Mode _current = MODE_EMO;
    ZEN_Mode _previous = MODE_EMO;
};
