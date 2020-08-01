#include "StateManager.h"

StateManager::StateManager(JsonConnector *jsonConnector, WiringManager *wiringManager) {
    this->jsonConnector = jsonConnector;
    this->visualModule = new VisualModule(wiringManager);

    this->state = STATE_STARTED;

    wiringManager->alphaNumWrite("----");
}

void StateManager::begin() {
    this->visualModule->begin();

    //load files
    DynamicJsonDocument answersJson(512);
    DynamicJsonDocument puzzlesJson(512);
    DeserializationError err;
    File answersFile = SPIFFS.open(ANSWERS_PATH, FILE_READ);
    err = deserializeJson(answersJson, answersFile);
    if (err) {
        //TODO
    }

    const std::vector<String> &keyboardAnswers = this->loadJsonItem(&answersJson, MODULE_KEYBOARD);
    const std::vector<String> &cablesAnswers = this->loadJsonItem(&answersJson, MODULE_CABLES);

    unsigned int count = keyboardAnswers.size();
    if (count != cablesAnswers.size()) {
        Serial.println("Loaded answers for keyboard and cables module are not of the same size!");
        return;
    }
    // krát dva to mělo bejt!!!
    this->answersNeeded = count;

    this->answers.insert(std::make_pair(MODULE_KEYBOARD, keyboardAnswers));
    this->answers.insert(std::make_pair(MODULE_CABLES, cablesAnswers));

    this->actAnswers.insert(std::make_pair(MODULE_KEYBOARD, answers[MODULE_KEYBOARD].begin()));
    this->actAnswers.insert(std::make_pair(MODULE_CABLES, answers[MODULE_CABLES].begin()));

    answersFile.close();

    DefaultTasker.loopEvery(1000, [this] {
        if (state == STATE_RUNNING) {
            uint rem = this->shortenRemainingTime(1);
            if (rem == 0) this->explode();
        }
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
    this->state = STATE_DEFUSED;
    this->started_at = 0;
    this->sendStatusUpdate();

    this->setRemainingTime(0);
    this->visualModule->showDefused();
    Serial.println("DEFUSED");
}

void StateManager::explode() {
    this->state = STATE_EXPLODED;
    this->started_at = 0;
    this->sendStatusUpdate();

    this->setRemainingTime(0);
    this->visualModule->showExploded();
    Serial.println("EXPLODED");
}

void StateManager::verify(int module, const String &answer) {
    Serial.print("Verify; module ");
    Serial.print(module);
    Serial.print(" answer ");
    Serial.println((String) answer);

    if (!answer.equals(getActAnswer(module))) {
        Serial.println("Bad answer!");
        // This may cause "explosion":
        shortenRemainingTime(BAD_ANSWER_PENALIZATION);
    } else {
        Serial.println("Good answer!");
        progress[module]++;
        uint totalProgress = (float) (progress[0] + progress[1]) * 100.0 / (float) this->answersNeeded;
        Serial.printf("%d answers needed, %d done\n", this->answersNeeded, progress[0] + progress[1]);

        if (progress[0] + progress[1] == this->answersNeeded) {
            this->defuse();
        } else {
            this->sendStatusUpdate();
            this->visualModule->updateProgress(totalProgress);
            ++this->actAnswers[module];
        }
    }
}

void StateManager::sendStatusUpdate() {
    // TODO send json status update
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

unsigned int StateManager::shortenRemainingTime(int delta) {
    if (this->remainingSecs < delta) {
        this->explode();
    } else {
        std::lock_guard<std::mutex> lg(mutex_time);

        this->remainingSecs -= delta;
        this->visualModule->updateTime(this->remainingSecs);

        this->sendStatusUpdate();
    }

    return this->remainingSecs;
}

void StateManager::setRemainingTime(unsigned int value) {
    std::lock_guard<std::mutex> lg(mutex_time);

    this->remainingSecs = value;
    this->visualModule->updateTime(this->remainingSecs);
    this->sendStatusUpdate();
}



