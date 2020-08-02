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
    this->answersNeeded = 2 * count;

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
    Serial.print("Verify; module ");
    Serial.print(module);
    Serial.print(" answer ");
    Serial.println((String) answer);

    if (!answer.equals(getActAnswer(module))) {
        badAnswer();
    } else {
        goodAnswer(module);
    }
}

void StateManager::start() {
    this->started_at = millis();
    this->progress[0] = 0;
    this->progress[1] = 0;
    this->actAnswers[MODULE_KEYBOARD] = answers[MODULE_KEYBOARD].begin();
    this->actAnswers[MODULE_CABLES] = answers[MODULE_CABLES].begin();
    visualModule->reset();
    this->state = STATE_RUNNING;
    this->setRemainingTime(TIME_TO_DEFUSE);

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
    progress[module]++;

    uint totalProgress = (float) (progress[0] + progress[1]) * 100.0 / (float) answersNeeded;
    Serial.printf("%d answers needed, %d done, progress %d %%\n", answersNeeded, progress[0] + progress[1], totalProgress);

    if (progress[0] + progress[1] == answersNeeded) {
        defuse();
    } else {
        sendStatusUpdate();
        visualModule->updateProgress(totalProgress);
        ++actAnswers[module];
    }
}

int StateManager::getState() const {
    return this->state;
}

void StateManager::sendStatusUpdate() {
    StaticJsonDocument<1024> json;

    json["status"] = this->getState();
    json["remainingSecs"] = this->remainingSecs;
    json["progress0"] = this->progress[0];
    json["progress1"] = this->progress[1];

    this->jsonConnector->send(json);
}

void StateManager::receiveCommand(const JsonDocument &json) {
    auto command = json["command"].as<char *>();

    Serial.print("Command received: ");
    Serial.println(command);

    if (strcmp(command, "start") == 0) {
        start();
    } else if (strcmp(command, "setTime") == 0) {
        setRemainingTime(json["time"].as<uint>());
    } else if (strcmp(command, "defuse") == 0) {
        defuse();
    } else if (strcmp(command, "explode") == 0) {
        explode();
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



