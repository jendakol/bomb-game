#include "ModulesManager.h"

ModulesManager::ModulesManager(StateManager *stateManager, WiringManager *wiringManager) {
    this->stateManager = stateManager;

    this->keyboardModule = new KeyboardModule(wiringManager);
    this->cablesModule = new CablesModule(wiringManager);
}

void ModulesManager::begin() {
    this->keyboardModule->begin();
    this->cablesModule->begin();
}
