#ifndef BOMBA_STATEMANAGER_H
#define BOMBA_STATEMANAGER_H

#include <wiring/WiringManager.h>
#include <networking/JsonConnector.h>

#define STATE_STARTED 0
#define STATE_RUNNING 1
#define STATE_DEFUSED 2
#define STATE_EXPLODED 2

class StateManager {
public:
    explicit StateManager(JsonConnector *jsonConnector, WiringManager *wiringManager);

    void setState(int state);

    int getState();

private:
    int state;
    JsonConnector *jsonConnector;
    WiringManager *wiringManager;
};


#endif //BOMBA_STATEMANAGER_H
