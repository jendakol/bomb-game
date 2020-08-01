#ifndef BOMBA_STATEMANAGER_H
#define BOMBA_STATEMANAGER_H

#include <wiring/WiringManager.h>
#include <networking/JsonConnector.h>
#include <logic/modules/VisualModule.h>

#define STATE_STARTED 0
#define STATE_RUNNING 1
#define STATE_DEFUSED 2
#define STATE_EXPLODED 2

class StateManager {
public:
    explicit StateManager(JsonConnector *jsonConnector, WiringManager *wiringManager);

    void begin();

    int getState() const;

    void start();

    void defuse();

    void explode();

    void verify(int module, const String &answer);

    String getActAnswer(int module);

    void badAnswer();

private:
    JsonConnector *jsonConnector;
    VisualModule *visualModule;
    int state;
    ulong started_at;
    uint remainingSecs, answersNeeded;
    uint progress[2]{0, 0};
    std::mutex mutex_time;
    std::map<int, std::vector<String>> answers;
    std::map<int, std::vector<String>::iterator> actAnswers;

    void sendStatusUpdate();

    void setRemainingTime(uint value);

    uint shortenRemainingTime(int delta);

    std::vector<String> loadJsonItem(DynamicJsonDocument *doc, int module);

};


#endif //BOMBA_STATEMANAGER_H
