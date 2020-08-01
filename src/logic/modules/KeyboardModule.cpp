#include <logic/StateManager.h>
#include "KeyboardModule.h"

#define KEY_PAUSE_THRESHOLD 150

KeyboardModule::KeyboardModule(StateManager *stateManager, WiringManager *wiringManager) {
    this->wiringManager = wiringManager;
    this->stateManager = stateManager;
}

void KeyboardModule::begin() {
    clean();

    DefaultTasker.loopEvery(10, [this] {
        handleKeyPress();
    });
}

void KeyboardModule::handleKeyPress() {
    char pressed = wiringManager->keyboardRead();

    if (pressed == 0) return;

    if (pressed == lastKey && millis() - lastKeyTime < KEY_PAUSE_THRESHOLD) {
        // Skipping pressed key - too fast
        return;
    }

    Serial.print("Pressed key: ");
    Serial.println(pressed);

    lastKey = pressed;
    lastKeyTime = millis();

    switch (pressed) {
        case '*':
            clean();
            break;
        case '#':
            stateManager->verify(MODULE_KEYBOARD, pressedKeyBuffer);
            clean();
            break;
        default:
            append(pressed);
    }
}

void KeyboardModule::clean() {
    this->pressedKeyBuffer = "";
}

void KeyboardModule::append(char c) {
    this->pressedKeyBuffer.concat(c);
}
