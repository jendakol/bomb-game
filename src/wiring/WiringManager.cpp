#include "WiringManager.h"

#include <Wire.h>
#include <PCF8574.h>

// TODO handle errors

char keyboardKeys[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
};

#define INDEX_KEYBOARD 0

WiringManager::WiringManager() {
    this->strip = new NeoPixelBrightnessBus<NeoGrbFeature, Neo800KbpsMethod>(LEDS_RING_COUNT, PIN_LED_RING);
}

void WiringManager::begin() {
    std::lock_guard<std::mutex> lg(mutex);

    Wire.begin();

    // init alphanum display:
    selectChannel(CHANNEL_ALPHANUM4);
    alphaNum4.begin(ADDR_ALPHANUM4);

    alphaNum4.writeDigitAscii(0, '-');
    alphaNum4.writeDigitAscii(1, '-');
    alphaNum4.writeDigitAscii(2, '-');
    alphaNum4.writeDigitAscii(3, '-');
    alphaNum4.writeDisplay();

    // for keyboard:
    addPcf(CHANNEL_KEYBOARD, INDEX_KEYBOARD, ADDR_KEYBOARD);

    // init LED ring:
    strip->Begin();
    strip->Show();
    strip->SetBrightness(LEDS_RING_BRIGHTNESS);

    for (int i = 0; i < LEDS_RING_COUNT; i++) {
        delay(RING_LED_ANIM_DELAY);
        strip->SetPixelColor(i, RING_COLOR_BLUE);
        strip->Show();
    }

    delay(RING_LED_ANIM_DELAY * 2);

    strip->ClearTo(RgbColor(0, 0, 0));

    strip->Show();
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

    addPcf(channel, index, addr);
}

void WiringManager::addPcf(const uint8_t channel, const uint8_t index, const uint8_t addr) {
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
    auto pcf = getPcf(CHANNEL_KEYBOARD, INDEX_KEYBOARD);

    char key = 0;

    for (int row = 0; row <= 3; row++) {
        pcf.write8(0xff ^ (1 << row));

        const auto value = (pcf.read8() & 0xf0) >> 4;
        int col = 0;
        for (; col <= 3; col++) {
            if (value + (1 << col) == 0xf) break;
        }

        if (col > 3) continue;

        key = keyboardKeys[3 - row][3 - col];
    }

    return key;
}
