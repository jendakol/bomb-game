#ifndef BOMBA_WIFICLIENT_H
#define BOMBA_WIFICLIENT_H

#include <Arduino.h>
#include <WiFi.h>

class WifiClient {
public:
    static void begin(const char *ssid, const char *password) {
        WiFi.softAPdisconnect(true); // for case it was configured as access point in last run...

        Serial.print(F("Connecting to "));
        Serial.println(ssid);

        WiFi.begin(ssid, password);

        int i = 0;
        while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
            delay(1000);
            Serial.print(++i);
            Serial.print(' ');
        }

        Serial.println('\n');
        Serial.println(F("Connection established!"));
        Serial.print(F("IP address:\t"));
        Serial.println(WiFi.localIP());
    }
};

#endif //BOMBA_WIFICLIENT_H
