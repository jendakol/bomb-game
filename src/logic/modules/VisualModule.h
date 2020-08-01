#ifndef BOMBA_VISUALMODULE_H
#define BOMBA_VISUALMODULE_H

#include <Tasker.h>
#include <wiring/WiringManager.h>

class VisualModule {
public:
    explicit VisualModule(WiringManager *wiringManager);

    void begin();

    void updateTime(unsigned int remainingSecs);

    void updateProgress(unsigned int value);

    void showDefused();

    void showExploded();

private:
    WiringManager *wiringManager;
    unsigned int remainingSecs = 0;
    unsigned int progress = 0;
    bool ended = false;

    void updateLedRing();

    void updateTimeDisplay();
};


#endif //BOMBA_VISUALMODULE_H
