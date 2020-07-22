#ifndef BOMBA_MODULESMANAGER_H
#define BOMBA_MODULESMANAGER_H

#include <logic/StateManager.h>
#include <wiring/WiringManager.h>

#include "modules/CablesModule.h"
#include "modules/KeyboardModule.h"

class ModulesManager {
public:
    explicit ModulesManager(StateManager *stateManager, WiringManager *wiringManager);

    void begin();

private:
    StateManager *stateManager;
    KeyboardModule *keyboardModule;
    CablesModule *cablesModule;
};

#endif //BOMBA_MODULESMANAGER_H
