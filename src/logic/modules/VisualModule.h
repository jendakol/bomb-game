#ifndef BOMBA_VISUALMODULE_H
#define BOMBA_VISUALMODULE_H

#include <Tasker.h>
#include <wiring/WiringManager.h>

class VisualModule {
public:
    explicit VisualModule(WiringManager *wiringManager);

    void begin();

    void updateTime(unsigned int remaining_secs);

    void showDefused();

private:
    WiringManager *wiringManager;
    unsigned int remaining_secs = 0;

    void updateLedRing();

    void updateTimeDisplay();
};


#endif //BOMBA_VISUALMODULE_H
