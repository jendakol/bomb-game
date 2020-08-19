#include <WiFi.h>

#include <Constants.h>
#include "AccessPoint.h"

void AccessPoint::begin(const char *ssid, const char *password) {
    WiFi.disconnect(true); // for case it was configured as wifi client  in last run...

    IPAddress apIP(8, 8, 8, 8);
    IPAddress netMsk(255, 255, 255, 0);

    Serial.print(F("Configuring access point...\n"));
    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.softAP(ssid, password);

    auto *dnsServer = new DNSServer();

    /* Setup the DNS server redirecting all the domains to the apIP */
    dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer->start(DNS_PORT, "*", apIP);

    NetworkTasker.loop([dnsServer] {
        dnsServer->processNextRequest();
        taskYIELD();
    });

    delay(500);
    Serial.println("AP ready");
}

String AccessPoint::serialNumberForMac() {
    Serial.printf("Mac address: %s\n", WiFi.macAddress().c_str());

    if ((String) "24:6F:28:81:57:FC" == WiFi.macAddress()) {
        return "23456789";
    } else {
        return "12345678";
    }
}
