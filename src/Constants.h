#ifndef BOMBA_CONSTANTS_H
#define BOMBA_CONSTANTS_H

#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <map>

#include "Tasker.h"

#define WIFI_SSID_PREFIX "BMBSTV2020WOQ_"

#define STATIC_FILES_PREFIX "/w"
#define HTTP_PORT 80
#define DNS_PORT 53
#define WEBSOCKET_PATH "/ws"

#define FileSystem SPIFFS

#define PIN_LED_RING 4

#define ADDR_MULTIPLEXER 0x70
#define ADDR_ALPHANUM4 0x71
#define ADDR_KEYBOARD 0x20
#define ADDR_CABLES 0x20

#define CHANNEL_ALPHANUM4 6
#define CHANNEL_KEYBOARD 0
#define CHANNEL_CABLES 3

#define TIME_TO_DEFUSE 60 * 60 + 15
#define BAD_ANSWER_PENALIZATION 5 * 60

#define ANSWERS_PATH "/answers.json"

#define MODULE_KEYBOARD 0
#define MODULE_CABLES 1

#define LEDS_RING_COUNT 24
#define LEDS_RING_BRIGHTNESS 5

static std::map<int, String> MODULE_NAME_MAPPING = {
        {MODULE_KEYBOARD, "keyboard"},
        {MODULE_CABLES,   "cables"}
};

#endif //BOMBA_CONSTANTS_H
