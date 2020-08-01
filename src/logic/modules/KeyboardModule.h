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
    char lastKey = 0;
    unsigned long lastKeyTime = 0;

    void handleKeyPress();
};

#endif //BOMBA_KEYBOARDMODULE_H
