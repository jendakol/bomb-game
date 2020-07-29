#ifndef BOMBA_CABLESMODULE_H
#define BOMBA_CABLESMODULE_H

#include <wiring/WiringManager.h>

class CablesModule {
public:
    explicit CablesModule(StateManager *stateManager,WiringManager *wiringManager);

    void begin();

private:
    StateManager *stateManager;
    WiringManager *wiringManager;
};

#endif //BOMBA_CABLESMODULE_H
