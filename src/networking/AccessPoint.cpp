#include <ESP8266WiFi.h> // TODO ESP32

#include <Constants.h>
#include "AccessPoint.h"

void AccessPoint::begin(const char *ssid, const char *password) {
    WiFi.disconnect(true); // for case it was configured as wifi client  in last run...

    IPAddress apIP(8, 8, 8, 8);
    IPAddress netMsk(255, 255, 255, 0);

    Serial.print(F("Configuring access point…"));
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(ssid, password);

    auto *dnsServer = new DNSServer();

    /* Setup the DNS server redirecting all the domains to the apIP */
    dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer->start(DNS_PORT, "*", apIP);

    DefaultLooper.add([dnsServer] { dnsServer->processNextRequest(); });

    delay(500);
    Serial.print(F("AP IP address: "));
    Serial.println(WiFi.softAPIP());
}
