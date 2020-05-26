#ifndef BOMBA_CONSTANTS_H
#define BOMBA_CONSTANTS_H

#include <FS.h>

#include "Looper.h"

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define STATIC_FILES_PREFIX "/w"
#define HTTP_PORT 80
#define DNS_PORT 53
#define WEBSOCKET_PATH "/ws"

static FS FileSystem = SPIFFS;

#endif //BOMBA_CONSTANTS_H
