#include "StateManager.h"

StateManager::StateManager(JsonConnector *jsonConnector, WiringManager *wiringManager) {
    this->jsonConnector = jsonConnector;
    this->visualModule = new VisualModule(wiringManager);

    this->state = STATE_STARTED;
}

void StateManager::begin() {
    this->visualModule->begin(TIME_TO_DEFUSE);

    // TODO Load answers to puzzles (puzzles/answers.json), save them.
    // TODO initialize inner counters of puzzles/answers and of progress (reported to visual module).
}

int StateManager::getState() {
    return this->state;
}

void StateManager::start() {
    this->started_at = millis();
    this->state = STATE_RUNNING;
    this->sendStatusUpdate();
}

void StateManager::verify(int module, String answer) {
    // TODO Report answer from KeyboardModule and CablesModule. Verify - it's either success or failure. Don't forget to
    // explode when there's too much of failures. Make constant (Constants.h) for the threshold. Accept info about
    // completeness of each module and don't forget to switch to DEFUSED state.

    this->visualizeStatus();
}

void StateManager::sendStatusUpdate() {
    // TODO send json status update
}

void StateManager::visualizeStatus() {
    // TODO send progress to VisualModule
}



