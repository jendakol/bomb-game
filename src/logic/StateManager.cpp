#include "StateManager.h"

StateManager::StateManager(JsonConnector *jsonConnector, WiringManager *wiringManager) {
    this->jsonConnector = jsonConnector;
    this->wiringManager = wiringManager;

    this->state = STATE_STARTED;
}

void StateManager::setState(int state) {
    this->state = state;

    // TODO react
}

int StateManager::getState() {
    return this->state;
}
