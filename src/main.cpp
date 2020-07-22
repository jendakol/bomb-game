#include <Arduino.h>

#include <Constants.h>
#include <networking/WebServer.h>
#include <networking/JsonConnector.h>
#include <networking/WifiClient.h>

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
    modulesManager.begin();

    NetworkTasker.once([] {
        WifiClient::begin(WIFI_SSID, WIFI_PASSWORD);
        webServer.begin(jsonConnector);
    });

    // TODO this is just example code
    long c = 0;
    DefaultTasker.loopEvery(10, [c]() mutable {
        c++;

        StaticJsonDocument<1024> json;

        if (c % 1000 == 0) {
            json["op"] = "msg";
            json["text"] = "Ahoj :-)";
            jsonConnector.send(json);
            json.clear();

            json["op"] = "uptime";

            char buff[16]{0};
            ltoa(millis(), buff, 10);

            json["value"] = buff;
            jsonConnector.send(json);
            json.clear();
        }

        if (c % 10 == 0) {
            json["op"] = "color";
            json["value"] = c % 255;
            jsonConnector.send(json);
            json.clear();
        }
    });

    jsonConnector.subscribe([](const JsonDocument &json) {
        Serial.print("JSON received: msg=");
        Serial.println(json["msg"].as<char *>());
    });

    // wiring example:

    wiringManager.registerPcf(3, 0, 0x20);

    DefaultTasker.loopEvery(1000, [] {
        byte value = wiringManager.pcfRead8(3, 0);

        char buff[16]{0};
        ltoa(value, buff, 10);

        wiringManager.alphaNumWrite(buff);
    });
}

void loop() {
    // no op - everything is handled by native tasks through Tasker
}
