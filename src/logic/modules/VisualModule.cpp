#include "VisualModule.h"

VisualModule::VisualModule(WiringManager *wiringManager) {
    this->wiringManager = wiringManager;
}

void VisualModule::begin(unsigned int remaining_secs) {
    // TODO init LED

    DefaultTasker.loopEvery(300, [this] {
       updateTimeDisplay();
    });
}

void VisualModule::updateTime(unsigned int remaining_secs) {
    this->remaining_secs = remaining_secs;
}

void VisualModule::updateLedRing() {
    // TODO display the progress - blue for cables, red for keyboard? total 24 LEDs
}

void VisualModule::updateTimeDisplay() {
    // TODO display the time
}

// TODO add methods for displaying progress on LED ring - take % as input?
