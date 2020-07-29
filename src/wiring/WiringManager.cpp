#include "WiringManager.h"

#include <Wire.h>
#include <PCF8574.h>

// TODO handle errors

WiringManager::WiringManager() {
    // initialize keyboard:
    const byte ROWS = 4;
    const byte COLS = 4;
    char keys[ROWS][COLS] = {
            {'1', '2', '3', 'A'},
            {'4', '5', '6', 'B'},
            {'7', '8', '9', 'C'},
            {'*', '0', '#', 'D'}
    };

    byte rowPins[ROWS] = {0, 1, 2, 3};
    byte colPins[COLS] = {4, 5, 6, 7};

    this->keypad = new Keypad_I2C(makeKeymap(keys), rowPins, colPins, ROWS, COLS, 0x20, 1);
}

void WiringManager::begin() {
    std::lock_guard<std::mutex> lg(mutex);

    Wire.begin();

    selectChannel(CHANNEL_ALPHANUM4);
    alphaNum4.begin(ADDR_ALPHANUM4);

    alphaNum4.writeDigitAscii(0, '-');
    alphaNum4.writeDigitAscii(1, '-');
    alphaNum4.writeDigitAscii(2, '-');
    alphaNum4.writeDigitAscii(3, '-');
    alphaNum4.writeDisplay();

    selectChannel(CHANNEL_KEYBOARD);
    keypad->begin();
}

void WiringManager::selectChannel(const uint8_t channel) {
    Wire.beginTransmission(ADDR_MULTIPLEXER);
    Wire.write(1 << channel);
    Wire.endTransmission();
}

uint8_t WiringManager::pcfRead8(const uint8_t channel, const uint8_t index) {
    std::lock_guard<std::mutex> lg(mutex);
    auto pcf = getPcf(channel, index);
    return pcf.read8();
}

uint8_t WiringManager::pcfRead(const uint8_t channel, const uint8_t index, const uint8_t pin) {
    std::lock_guard<std::mutex> lg(mutex);
    auto pcf = getPcf(channel, index);
    return pcf.read(pin);
}

void WiringManager::pcfWrite8(const uint8_t channel, const uint8_t index, const uint8_t value) {
    std::lock_guard<std::mutex> lg(mutex);
    auto pcf = getPcf(channel, index);
    pcf.write8(value);
}

void WiringManager::pcfWrite(const uint8_t channel, const uint8_t index, const uint8_t pin, const uint8_t value) {
    std::lock_guard<std::mutex> lg(mutex);
    auto pcf = getPcf(channel, index);
    pcf.write(pin, value);
}

PCF8574 &WiringManager::getPcf(const uint8_t channel, const uint8_t index) {
    selectChannel(channel);
    return pcfs.at(channel).at(index);
}

void WiringManager::registerPcf(const uint8_t channel, const uint8_t index, const uint8_t addr) {
    std::lock_guard<std::mutex> lg(mutex);
    selectChannel(channel);

    PCF8574 pcf(addr);
    pcf.begin();

    Serial.print("Adding new PCF: channel=");
    Serial.print(channel);
    Serial.print(" index=");
    Serial.print(index);
    Serial.print(" addr=");
    Serial.println(addr, HEX);

    if (pcfs.count(channel) == 0) {
        pcfs.emplace(channel, MapInner{});
    }

    MapInner &inner = pcfs.at(channel);

    inner.emplace(index, std::move(pcf));
}

void WiringManager::alphaNumWrite(const char segments[4]) {
    std::lock_guard<std::mutex> lg(mutex);

    selectChannel(CHANNEL_ALPHANUM4);
    alphaNum4.clear();

    for (uint8_t i = 0; i <= 3; i++) {
        // TODO align to right?

        if (isPrintable(segments[i])) {
            alphaNum4.writeDigitAscii(i, segments[i]);
        }
    }

    alphaNum4.writeDisplay();
}

char WiringManager::keyboardRead() {
    std::lock_guard<std::mutex> lg(mutex);

    selectChannel(CHANNEL_KEYBOARD);

    // TODO great, but doesn't work

    auto value = this->keypad->getKey();

//    Serial.print("Keyboard value: ");
//    Serial.println(value);

    return value; // TODO
}
