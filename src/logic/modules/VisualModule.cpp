#include "VisualModule.h"

VisualModule::VisualModule(WiringManager *wiringManager) {
    this->wiringManager = wiringManager;
}

void VisualModule::begin() {
    // TODO init LED

    NetworkTasker.loopEvery(100, [this] {
        updateTimeDisplay();
    });
}

void VisualModule::updateTime(unsigned int remaining_secs) {
    this->remaining_secs = remaining_secs;
    Serial.print("Remaining time: ");
    Serial.print(remaining_secs / 60);
    Serial.print(":");
    Serial.println(remaining_secs % 60);
}

void VisualModule::updateLedRing() {
    // TODO display the progress - blue for cables, red for keyboard? total 24 LEDs
}

void VisualModule::updateTimeDisplay() {
    if (remaining_secs > 0) {
        char buff[5]{0};
        sprintf(buff, "%02d%02d", remaining_secs / 60, remaining_secs % 60);

        this->wiringManager->alphaNumWrite(buff);
    } else {
        this->wiringManager->alphaNumWrite("----");
    }
}

void VisualModule::showDefused() {
    // TODO show defused
}

// TODO add methods for displaying progress on LED ring - take % as input?
