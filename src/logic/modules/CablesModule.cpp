#include <logic/StateManager.h>
#include "CablesModule.h"

CablesModule::CablesModule(StateManager *stateManager, WiringManager *wiringManager) {
    this->wiringManager = wiringManager;
    this->stateManager = stateManager;
}

void CablesModule::begin() {
    // TODO monitor state of cables, report change to StateManager
}
