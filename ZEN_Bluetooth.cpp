#include "ZEN_Bluetooth.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void ZEN_Bluetooth::begin(const char* name) {
    SerialBT.begin(name);
    _ready = true;
    Serial.println("[BT] Bluetooth ready");
}

bool ZEN_Bluetooth::available() {
    if (!_ready) return false;
    return SerialBT.available();
}

char ZEN_Bluetooth::read() {
    return SerialBT.read();
}
