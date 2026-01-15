#pragma once
#include <Arduino.h>

class ZEN_Bluetooth {
public:
    void begin(const char* name);
    bool available();
    char read();

private:
    bool _ready = false;
};
