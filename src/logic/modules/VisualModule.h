#ifndef BOMBA_VISUALMODULE_H
#define BOMBA_VISUALMODULE_H

#include <Tasker.h>
#include <wiring/WiringManager.h>

class VisualModule {
public:
    explicit VisualModule(WiringManager *wiringManager);

    void begin();

    void updateTime(uint remainingSecs);

    void updateProgress(uint value);

    void showDefused();

    void showExploded();

private:
    WiringManager *wiringManager;
    uint remainingSecs = 0;
    uint progress = 0;
    bool ended = false;

    void updateLedRing();

    void updateTimeDisplay();
};


#endif //BOMBA_VISUALMODULE_H
