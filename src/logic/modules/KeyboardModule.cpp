#include <logic/StateManager.h>
#include "KeyboardModule.h"

KeyboardModule::KeyboardModule(StateManager *stateManager, WiringManager *wiringManager) {
    this->wiringManager = wiringManager;
    this->stateManager = stateManager;
}

void KeyboardModule::begin() {
    clean();

    DefaultTasker.loopEvery(50, [this] {
        char pressed = this->wiringManager->keyboardRead();

        // TODO protection against multiple-catches of single press

        switch (pressed) {
            case '\0':
                return;
            case '*':
                clean();
                break;
            case '#':
                this->stateManager->verify(MODULE_KEYBOARD, pressedKeyBuffer);
                clean();
                break;
            default:
                append(pressed);
        }
    });
}

void KeyboardModule::clean() {
    this->pressedKeyBuffer = "";
}

void KeyboardModule::append(char c) {
    this->pressedKeyBuffer.concat(c);
}
