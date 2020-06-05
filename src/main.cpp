#include <Arduino.h>

#include <Constants.h>
#include <networking/WebServer.h>
#include <networking/JsonConnector.h>
#include <networking/WifiClient.h>

WebServer webServer;
JsonConnector jsonConnector;

void setup() {
    Serial.begin(9600);
    FileSystem.begin();

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
}

void loop() {
    // no op - everything is handled by native tasks through Tasker
}
