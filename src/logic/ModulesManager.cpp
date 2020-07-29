#include "ModulesManager.h"

ModulesManager::ModulesManager(StateManager *stateManager, WiringManager *wiringManager) {
    this->keyboardModule = new KeyboardModule(stateManager, wiringManager);
    this->cablesModule = new CablesModule(stateManager, wiringManager);
}

void ModulesManager::begin() {
    this->keyboardModule->begin();
    this->cablesModule->begin();
}
