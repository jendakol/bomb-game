#ifndef BOMBA_KEYBOARDMODULE_H
#define BOMBA_KEYBOARDMODULE_H

#include <wiring/WiringManager.h>

class KeyboardModule {
public:
    explicit KeyboardModule(StateManager *stateManager, WiringManager *wiringManager);

    void begin();

private:
    StateManager *stateManager;
    WiringManager *wiringManager;
};

#endif //BOMBA_KEYBOARDMODULE_H
