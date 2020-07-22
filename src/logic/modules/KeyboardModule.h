#ifndef BOMBA_KEYBOARDMODULE_H
#define BOMBA_KEYBOARDMODULE_H

#include <wiring/WiringManager.h>

class KeyboardModule {
public:
    explicit KeyboardModule(WiringManager *wiringManager);

    void begin();

private:
    WiringManager *wiringManager;
};

#endif //BOMBA_KEYBOARDMODULE_H
