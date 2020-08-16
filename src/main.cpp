#include <Arduino.h>

#include <Constants.h>
#include <networking/WebServer.h>
#include <networking/JsonConnector.h>
#include <networking/AccessPoint.h>

#include <logic/StateManager.h>
#include <logic/ModulesManager.h>
#include <wiring/WiringManager.h>

WiringManager wiringManager;

WebServer webServer;
JsonConnector jsonConnector;
StateManager stateManager(&jsonConnector, &wiringManager);
ModulesManager modulesManager(&stateManager, &wiringManager);

void setup() {
    Serial.begin(9600);
    FileSystem.begin();
    wiringManager.begin();
    stateManager.begin();
    modulesManager.begin();

    NetworkTasker.once([] {
        AccessPoint::begin(WIFI_SSID, WIFI_PASSWORD);
        webServer.begin(jsonConnector);
    });
}

void loop() {
    // no op - everything is handled by native tasks through Tasker
}
