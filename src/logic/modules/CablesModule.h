#ifndef BOMBA_CABLESMODULE_H
#define BOMBA_CABLESMODULE_H

#include <wiring/WiringManager.h>

#define NO_CABLE 8

class CablesModule {
public:
    explicit CablesModule(StateManager *stateManager,WiringManager *wiringManager);

    void begin();

private:
    StateManager *stateManager;
    WiringManager *wiringManager;
    uint lastState = NO_CABLE;
    ulong lastStateTime = 0;

    void checkCableConnections();
};

#endif //BOMBA_CABLESMODULE_H
