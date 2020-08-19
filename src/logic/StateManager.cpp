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
    DeserializationError err;
    File answersFile = SPIFFS.open(ANSWERS_PATH, FILE_READ);
    err = deserializeJson(answersJson, answersFile);
    if (err) {
        //TODO
        Serial.println("ERR loading answers JSON!");
    }

    const std::vector<String> &keyboardAnswers = this->loadJsonItem(&answersJson, MODULE_KEYBOARD);
    const std::vector<String> &cablesAnswers = this->loadJsonItem(&answersJson, MODULE_CABLES);

    this->answersNeeded = keyboardAnswers.size() + cablesAnswers.size();

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

    jsonConnector->subscribe([this](const JsonDocument &json) {
        receiveCommand(json);
    });
}

void StateManager::verify(int module, const String &answer) {
    Serial.printf("Verify: module %d, answer '%s'\n", module, answer.c_str());

    if (!answer.equals(getActAnswer(module))) {
        badAnswer();
    } else {
        goodAnswer(module);
    }
}

void StateManager::start() {
    { // to limit mutex lock validity
        std::lock_guard<std::mutex> lg(mutex_progress);

        this->started_at = millis();
        this->totalAnswered = 0;

        this->actAnswers[MODULE_KEYBOARD] = answers[MODULE_KEYBOARD].begin();
        this->actAnswers[MODULE_CABLES] = answers[MODULE_CABLES].begin();
    }
    visualModule->reset();
    this->state = STATE_RUNNING;

    this->sendStatusUpdate();
}

void StateManager::defuse() {
    this->state = STATE_DEFUSED;
    this->started_at = 0;
    this->setRemainingTime(0);

    this->sendStatusUpdate();
    this->visualModule->showDefused();
    Serial.println("DEFUSED");
}

void StateManager::explode() {
    this->state = STATE_EXPLODED;
    this->started_at = 0;
    this->setRemainingTime(0);

    this->sendStatusUpdate();
    this->visualModule->showExploded();
    Serial.println("EXPLODED");
}

void StateManager::badAnswer() {
    Serial.println("Bad answer!");
    // This may cause "explosion":
    shortenRemainingTime(BAD_ANSWER_PENALIZATION);
}

void StateManager::goodAnswer(int module) {
    Serial.println("Good answer!");

    std::lock_guard<std::mutex> lg(mutex_progress);

    ++totalAnswered;
    ++actAnswers[module];

    evaluateActProgress();
}

int StateManager::getState() const {
    return this->state;
}

void StateManager::sendStatusUpdate() {
    StaticJsonDocument<1024> json;

    json["status"] = this->getState();
    json["remainingSecs"] = this->remainingSecs;

    for (auto &answer : answers) {
        String progressName = "progress" + (String) answer.first;
        json[progressName] = actAnswers[answer.first] - answer.second.begin();
    }

    this->jsonConnector->send(json);
}

void StateManager::receiveCommand(const JsonDocument &json) {
    auto command = json["command"].as<char *>();

    Serial.print("Command received: ");
    Serial.println(command);

    if (strcmp(command, "start") == 0) {
        setRemainingTime(json["time"].as<uint>());
        start();
    } else if (strcmp(command, "setTime") == 0) {
        setRemainingTime(json["time"].as<uint>());
    } else if (strcmp(command, "defuse") == 0) {
        defuse();
    } else if (strcmp(command, "explode") == 0) {
        explode();
    } else if (strcmp(command, "progress") == 0) {
        int value = json["value"].as<int>();
        int moduleId = json["module"].as<int>();

        Serial.printf("Manual progress update: %s -> %d\n", MODULE_NAME_MAPPING[moduleId].c_str(), value);

        { // to limit mutex lock validity
            std::lock_guard<std::mutex> lg(mutex_progress);

            totalAnswered -= getCountOfAnswered(moduleId);
            actAnswers[moduleId] = answers[moduleId].begin() + value;
            totalAnswered += getCountOfAnswered(moduleId);
        }

        evaluateActProgress();
    }

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

int StateManager::getCountOfAnswered(int module) {
    return actAnswers[module] - answers[module].begin();
}

void StateManager::evaluateActProgress() {
    uint totalProgress = (float) (totalAnswered) * 100.0 / (float) answersNeeded;
    Serial.printf("%d answers needed, %d done, progress %d %%\n", answersNeeded, totalAnswered,
                  totalProgress);

    if (totalAnswered == answersNeeded) {
        defuse();
    } else {
        sendStatusUpdate();
        visualModule->updateProgress(totalProgress);
    }
}



