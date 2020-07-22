#ifndef BOMBA_CONSTANTS_H
#define BOMBA_CONSTANTS_H

#include <FS.h>
#include <SPIFFS.h>

#include "Tasker.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define STATIC_FILES_PREFIX "/w"
#define HTTP_PORT 80
#define DNS_PORT 53
#define WEBSOCKET_PATH "/ws"

#define FileSystem SPIFFS

#define ADDR_MULTIPLEXER 0x70
#define ADDR_ALPHANUM4 0x71

#define CHANNEL_ALPHANUM4 6
#define CHANNEL_KEYBOARD 5


#endif //BOMBA_CONSTANTS_H
