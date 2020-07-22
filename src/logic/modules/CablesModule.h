#ifndef BOMBA_CABLESMODULE_H
#define BOMBA_CABLESMODULE_H

#include <wiring/WiringManager.h>

class CablesModule {
public:
    explicit CablesModule(WiringManager *wiringManager);

    void begin();

private:
    WiringManager *wiringManager;
};

#endif //BOMBA_CABLESMODULE_H
