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

    DefaultTasker.loopEvery(200, [this] {
//        const int key = this->wiringManager->keyboardRead(); // TODO
//        if (key > 0) {
////             ...
//        }
    });
}
