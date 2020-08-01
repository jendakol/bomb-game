#include "VisualModule.h"

VisualModule::VisualModule(WiringManager *wiringManager) {
    this->wiringManager = wiringManager;
}

void VisualModule::begin() {
    NetworkTasker.loopEvery(100, [this] {
        if (!ended) {
            updateTimeDisplay();
            updateLedRing();
        }
    });
}

void VisualModule::updateTime(uint remainingSecs) {
    this->remainingSecs = remainingSecs;
    this->ended = false;
//    Serial.print("Remaining time: ");
//    Serial.print(remainingSecs / 60);
//    Serial.print(":");
//    Serial.println(remainingSecs % 60);
}

void VisualModule::updateProgress(uint value) {
    this->progress = value;
}

void VisualModule::updateLedRing() {
    // TODO show progress animation?

    if (progress > 0) {
        uint leds = (float) LEDS_RING_COUNT * ((float) progress / 100.0);
        this->wiringManager->strip->ClearTo(RING_COLOR_BLUE, 0, leds - 1);
        this->wiringManager->strip->Show();
    }
}

void VisualModule::updateTimeDisplay() {
    if (remainingSecs > 0) {
        char buff[5]{0};
        sprintf(buff, "%02d%02d", remainingSecs / 60, remainingSecs % 60);

        this->wiringManager->alphaNumWrite(buff);
    }
}

void VisualModule::showDefused() {
    this->ended = true;
    this->remainingSecs = 0;

    this->wiringManager->alphaNumWrite("--OK");
    this->wiringManager->strip->ClearTo(RING_COLOR_GREEN);
    this->wiringManager->strip->Show();
}

void VisualModule::showExploded() {
    this->ended = true;
    this->remainingSecs = 0;

    this->wiringManager->alphaNumWrite("XXXX");
    this->wiringManager->strip->ClearTo(RING_COLOR_RED);
    this->wiringManager->strip->Show();
}
