#include <logic/StateManager.h>
#include "CablesModule.h"

#define INTERVAL 50

CablesModule::CablesModule(StateManager *stateManager, WiringManager *wiringManager) {
    this->wiringManager = wiringManager;
    this->stateManager = stateManager;
}

void CablesModule::begin() {
    wiringManager->registerPcf(CHANNEL_CABLES, 1, ADDR_CABLES);
    wiringManager->pcfWrite8(CHANNEL_CABLES, 1, 0xff);

    DefaultTasker.loopEvery(INTERVAL, [this] {
        if (stateManager->getState() == STATE_RUNNING)
        checkCableConnections();
    });
}

void CablesModule::checkCableConnections() {
    const byte value = wiringManager->pcfRead8(CHANNEL_CABLES, 1) & 0x1f; // mask out unused bits

    int col = 0;
    for (; col < 5; col++) {
        auto x = value - (0x10 >> col);
        if (x == 0x00) break;
    }
    col += 1;

//    Serial.print(value, BIN);
//    Serial.print(" ");
//    Serial.println(col);

    if (col != this->lastState && (millis() - this->lastStateTime) < 3 * INTERVAL) {
//        Serial.print("Skip, too fast change ");
//        Serial.println(value, BIN);
        return;
    }

    if (col == NO_CABLE && value == 0x1f) {
        // like multiple cables but it may be a HW a bug
        return;
    }

    if (col == NO_CABLE && value != 0x00) {
        Serial.println("Multiple cables!");
        Serial.print("0x");
        Serial.print(value, BIN);
        Serial.print(" ");
        Serial.println(col);
        this->stateManager->explode();
        return;
    }

//    Serial.printf("Col %d, last %d\n", col, lastState);

    if (col == NO_CABLE) {
//        if (col == this->lastState) {
//            Serial.println("Keep NO_CABLE");
//        } else {
//            Serial.println("Return to NO_CABLE");
//        }
    } else {
        if (col == this->lastState) {
//            Serial.printf("Keep answer %d\n", col);
        } else {
            // answered
            Serial.printf("Cable disconnected: %d\n", col);
            this->stateManager->verify(MODULE_CABLES, *new String((char) (48 + col)));
        }
    }

    this->lastState = col;
    this->lastStateTime = millis();
}
