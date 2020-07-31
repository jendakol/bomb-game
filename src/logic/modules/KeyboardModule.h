#ifndef BOMBA_KEYBOARDMODULE_H
#define BOMBA_KEYBOARDMODULE_H

#include <wiring/WiringManager.h>

class KeyboardModule {
public:
    explicit KeyboardModule(StateManager *stateManager, WiringManager *wiringManager);

    void begin();

private:

    void clean();

    void append(char c);

    StateManager *stateManager;
    WiringManager *wiringManager;
    String pressedKeyBuffer;
};

#endif //BOMBA_KEYBOARDMODULE_H
