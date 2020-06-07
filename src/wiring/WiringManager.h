#ifndef BOMBA_WIRING_MANAGER_H
#define BOMBA_WIRING_MANAGER_H

#include <map>
#include <mutex>

#include <PCF8574.h>
#include "Adafruit_LEDBackpack.h"

#include <Constants.h>

typedef std::map<uint8_t, PCF8574> MapInner;
typedef std::map<uint8_t, MapInner> PcfMap;

class WiringManager {
public:
    void begin();

    void registerPcf(const uint8_t channel, const uint8_t index, uint8_t addr);

    uint8_t pcfRead8(const uint8_t channel, const uint8_t index);

    uint8_t pcfRead(const uint8_t channel, const uint8_t index, uint8_t pin);

    void pcfWrite8(const uint8_t channel, const uint8_t index, const uint8_t value);

    void pcfWrite(const uint8_t channel, const uint8_t index, const uint8_t pin, const uint8_t value);

    void alphaNumWrite(const char segments[4]);

    void selectChannel(const uint8_t channel);

private:
    std::mutex mutex;
    PcfMap pcfs;
    Adafruit_AlphaNum4 alphaNum4;

    PCF8574 &getPcf(uint8_t channel, uint8_t index);
};


#endif //BOMBA_WIRING_MANAGER_H
