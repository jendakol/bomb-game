#ifndef BOMBA_STATEMANAGER_H
#define BOMBA_STATEMANAGER_H

#include <wiring/WiringManager.h>
#include <networking/JsonConnector.h>
#include <logic/modules/VisualModule.h>

#define STATE_STARTED 0
#define STATE_RUNNING 1
#define STATE_DEFUSED 2
#define STATE_EXPLODED 2

#define MODULE_KEYBOARD 1
#define MODULE_CABLES 2

class StateManager {
public:
    explicit StateManager(JsonConnector *jsonConnector, WiringManager *wiringManager);

    void begin();

    int getState();

    void start();

    void verify(int module, String answer);

private:
    JsonConnector *jsonConnector;
    VisualModule *visualModule;
    int state;
    unsigned long started_at;

    void sendStatusUpdate();
    void visualizeStatus();
};


#endif //BOMBA_STATEMANAGER_H
