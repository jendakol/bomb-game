#ifndef BOMBA_ACCESSPOINT_H
#define BOMBA_ACCESSPOINT_H

#include <DNSServer.h>

class AccessPoint {
public:
    static void begin(const char *ssid, const char *password);
};

#endif //BOMBA_ACCESSPOINT_H
