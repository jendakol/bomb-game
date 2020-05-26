#ifndef BOMBA_ACCESSPOINT_H
#define BOMBA_ACCESSPOINT_H

#include <DNSServer.h>

class AccessPoint {
public:
    static void begin(char *ssid, char *password);

private:
    static DNSServer dnsServer;
};


#endif //BOMBA_ACCESSPOINT_H
