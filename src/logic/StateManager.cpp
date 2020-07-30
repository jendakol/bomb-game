#include "StateManager.h"

StateManager::StateManager(JsonConnector *jsonConnector, WiringManager *wiringManager) {
    this->jsonConnector = jsonConnector;
    this->visualModule = new VisualModule(wiringManager);

    this->state = STATE_STARTED;
}

void StateManager::begin() {
    this->visualModule->begin(TIME_TO_DEFUSE);

    // Load answers to puzzles (puzzles/answers.json), save them.
    // TODO initialize inner counters of puzzles/answers and of progress (reported to visual module).

    //load files
    DynamicJsonDocument answersJson(512);
    DynamicJsonDocument puzzlesJson(512);
    DeserializationError err;
    File answersFile = SPIFFS.open(ANSWERS_PATH, FILE_READ);
    File puzzlesFile = SPIFFS.open(PUZZLES_PATH, FILE_READ);
    err = deserializeJson(answersJson, answersFile);
    if (err) {
        //TODO
    }
    err = deserializeJson(puzzlesJson, puzzlesFile);
    if (err) {
        //TODO
    }

    this->answers.insert(std::make_pair(MODULE_KEYBOARD, this->loadJsonItem(&answersJson, MODULE_KEYBOARD)));
    this->answers.insert(std::make_pair(MODULE_CABLES, this->loadJsonItem(&answersJson, MODULE_CABLES)));

    this->actAnswers.insert(std::make_pair(MODULE_KEYBOARD, answers[MODULE_KEYBOARD].begin()));
    this->actAnswers.insert(std::make_pair(MODULE_CABLES, answers[MODULE_CABLES].begin()));

    answersFile.close();
    puzzlesFile.close();
}

int StateManager::getState() {
    return this->state;
}

void StateManager::start() {
    this->started_at = millis();
    this->state = STATE_RUNNING;
    this->sendStatusUpdate();
}

void StateManager::verify(int module, const String &answer) {
    // TODO Report answer from KeyboardModule and CablesModule. Verify - it's either success or failure. Don't forget to
    // explode when there's too much of failures. Make constant (Constants.h) for the threshold. Accept info about
    // completeness of each module and don't forget to switch to DEFUSED state.

    if (!answer.equals(*this->actAnswers[module])) {
        //TODO bad answer
    } else {
        //TODO correct answer
        ++this->actAnswers[module];
    }

    this->visualizeStatus();
}

void StateManager::sendStatusUpdate() {
    // TODO send json status update
}

void StateManager::visualizeStatus() {
    // TODO send progress to VisualModule
}

String StateManager::getActAnswer(int module) {
    return *this->actAnswers[module];
}

std::vector<String> StateManager::loadJsonItem(DynamicJsonDocument *doc, int module) {
    std::vector<String> result;
    for (JsonVariant v : (JsonArray) (*doc)[MODULE_NAME_MAPPING[module]]) {
        result.push_back(v.as<String>());
    }
    return result;
}



