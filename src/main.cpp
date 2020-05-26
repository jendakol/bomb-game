#include <Arduino.h>

#include <Constants.h>
#include <networking/WebServer.h>
#include <networking/WifiClient.h>

WebServer webServer;

void setup() {
    Serial.begin(9600);
    FileSystem.begin();

    WifiClient::begin(WIFI_SSID, WIFI_PASSWORD);

    webServer.begin();
}

void loop() {
    DefaultLooper.tick();
}
