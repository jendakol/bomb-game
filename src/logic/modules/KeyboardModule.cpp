#include <logic/StateManager.h>
#include "KeyboardModule.h"

KeyboardModule::KeyboardModule(StateManager *stateManager,WiringManager *wiringManager) {
    this->wiringManager = wiringManager;
    this->stateManager = stateManager;
}

void KeyboardModule::begin() {
    // TODO:
    /*  Read keys, buffer them, support resetting answering with `*`, confirmation with `#`.
        When confirmed, call this->stateManager->verify.
     */

    clean();

    DefaultTasker.loopEvery(200, [this] {
        char pressed = this->wiringManager->keyboardRead();
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

void KeyboardModule::append(uint8_t c) {
    this->pressedKeyBuffer.concat(c);
}
