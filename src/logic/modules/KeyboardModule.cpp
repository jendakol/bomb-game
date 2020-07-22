#include "KeyboardModule.h"

KeyboardModule::KeyboardModule(WiringManager *wiringManager) {
    this->wiringManager = wiringManager;
}

void KeyboardModule::begin() {
    const int key = this->wiringManager->keyboardRead(); // TODO
    if (key > 0) {
        // ...
    }
}
