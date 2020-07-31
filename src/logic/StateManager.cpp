#include "StateManager.h"

StateManager::StateManager(JsonConnector *jsonConnector, WiringManager *wiringManager) {
    this->jsonConnector = jsonConnector;
    this->visualModule = new VisualModule(wiringManager);

    this->state = STATE_STARTED;

    wiringManager->alphaNumWrite("----");
}

void StateManager::begin() {
    this->visualModule->begin();

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

    DefaultTasker.loopEvery(1000, [this] {
        if (state == STATE_RUNNING)
            this->updateRemainingTime(-1);

        // TODO explode
    });
}

int StateManager::getState() {
    return this->state;
}

void StateManager::start() {
    this->started_at = millis();
    this->state = STATE_RUNNING;
    this->sendStatusUpdate();

    this->setRemainingTime(TIME_TO_DEFUSE);
}

void StateManager::defuse() {
    this->started_at = 0;
    this->state = STATE_DEFUSED;
    this->sendStatusUpdate();

    this->setRemainingTime(0);
    this->visualModule->showDefused();
}

void StateManager::verify(int module, const String &answer) {
    // TODO Report answer from KeyboardModule and CablesModule. Verify - it's either success or failure. Don't forget to
    // explode when there's too much of failures. Make constant (Constants.h) for the threshold. Accept info about
    // completeness of each module and don't forget to switch to DEFUSED state.

    Serial.print("Verify: module ");
    Serial.print(module);
    Serial.print("Answer: ");
    Serial.println((String) answer);

    if (!answer.equals(getActAnswer(module))) {
        //TODO bad answer
        Serial.println("Bad answer");
        updateRemainingTime(-BAD_ANSWER_PENALIZATION);
    } else {
        //TODO correct answer
        Serial.println("Good answer");
        ++this->actAnswers[module];
        // TODO defuse, if everything done!
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

unsigned int StateManager::updateRemainingTime(int delta) {
    std::lock_guard<std::mutex> lg(mutex_time);

    this->remaining_secs += delta;
    this->visualModule->updateTime(this->remaining_secs);

    this->sendStatusUpdate();
    return this->remaining_secs;
}

void StateManager::setRemainingTime(unsigned int value) {
    std::lock_guard<std::mutex> lg(mutex_time);

    this->remaining_secs = value;
    this->visualModule->updateTime(this->remaining_secs);
    this->sendStatusUpdate();
}



