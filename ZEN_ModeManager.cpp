#include "ZEN_ModeManager.h"

void ZEN_ModeManager::begin() {
    _current = MODE_EMO;
    _previous = MODE_EMO;
    Serial.println("[MODE] Initialized → EMO");
}

void ZEN_ModeManager::update() {
    // Phase 4.1: nothing periodic yet
    // Later phases will hook Bluetooth / timers here
}

void ZEN_ModeManager::set(ZEN_Mode m) {
    if (m == _current) return;

    _previous = _current;
    _current = m;

    Serial.print("[MODE] ");
    Serial.print(_previous);
    Serial.print(" → ");
    Serial.println(_current);
}

ZEN_Mode ZEN_ModeManager::current() const {
    return _current;
}

bool ZEN_ModeManager::is(ZEN_Mode m) const {
    return _current == m;
}
