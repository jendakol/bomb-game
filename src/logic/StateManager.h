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

    int getState();

    void start();
    void defuse();

    void verify(int module, const String &answer);

    String getActAnswer(int module);

private:
    JsonConnector *jsonConnector;
    VisualModule *visualModule;
    int state;
    unsigned long started_at;
    unsigned int remaining_secs;
    std::mutex mutex_time;
    std::map<int, std::vector<String>> answers;
    std::map<int, std::vector<String>> puzzles;
    std::map<int, std::vector<String>::iterator> actAnswers;

    void sendStatusUpdate();

    void visualizeStatus();

    void setRemainingTime(unsigned int value);

    unsigned int updateRemainingTime(int delta);

    std::vector<String> loadJsonItem(DynamicJsonDocument *doc, int module);
};


#endif //BOMBA_STATEMANAGER_H
