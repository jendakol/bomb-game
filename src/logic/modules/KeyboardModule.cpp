#include <logic/StateManager.h>
#include "KeyboardModule.h"

#define KEY_PAUSE_THRESHOLD 30

KeyboardModule::KeyboardModule(StateManager *stateManager, WiringManager *wiringManager) {
    this->wiringManager = wiringManager;
    this->stateManager = stateManager;
}

void KeyboardModule::begin() {
    clean();

    DefaultTasker.loopEvery(10, [this] {
        if (stateManager->getState() == STATE_RUNNING)
            handleKeyPress();
    });
}

void KeyboardModule::handleKeyPress() {
    char current = wiringManager->keyboardRead();
    char wasPressed = 0;

    if (current != lastKey && millis() - lastKeyTime < KEY_PAUSE_THRESHOLD) {
        // Skipping current key - may be shimmering
        return;
    }

    if (current != 0) {
        // pressed key!
        lastKey = current;
        lastKeyTime = millis();
    } else {
        // released or nothing
        if (lastKey != 0) {
            wasPressed = lastKey;
            lastKey = 0;
        }
    }

    if (wasPressed == 0) return;

    switch (wasPressed) {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
            // ignore
            break;
        case '*':
            Serial.println("Cleaning keyboard buffer");
            clean();
            break;
        case '#':
            Serial.println("Pressed #, verifying the answer");
            stateManager->verify(MODULE_KEYBOARD, pressedKeyBuffer);
            clean();
            break;
        default:
            Serial.print("Pressed key: ");
            Serial.println(wasPressed);

            append(wasPressed);
    }
}

void KeyboardModule::clean() {
    this->pressedKeyBuffer = "";
}

void KeyboardModule::append(char c) {
    this->pressedKeyBuffer.concat(c);
}
