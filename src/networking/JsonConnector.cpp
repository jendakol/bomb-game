#include "JsonConnector.h"
#include <Constants.h>

JsonConnector::JsonConnector() {
    webSocket = new AsyncWebSocket(WEBSOCKET_PATH);

    ulong eventC = 0;

    webSocket->onEvent(
            [this, eventC](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
                           size_t len) mutable {
                onWsEvent(type, data, len);
                if (eventC++ % 10 == 0) {
                    server->cleanupClients();
                }
            }
    );
}

void JsonConnector::bind(AsyncWebServer &webServer) {
    webServer.addHandler(webSocket);
}

void JsonConnector::onWsEvent(AwsEventType type, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.println(F("Websocket client connection received"));
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.println(F("Websocket client disconnected"));
    } else if (type == WS_EVT_DATA) {
//        Serial.print(F("Websocket data received: "));
//        auto command = new String((const char *)data);
//        Serial.println(*command);

        StaticJsonDocument<1024> json;
        deserializeJson(json, data, len);

        for (auto &callback : callbacks)
            callback(json);
    }
}

void JsonConnector::subscribe(const std::function<void(const JsonDocument &)> &callback) {
    callbacks.push_back(callback);
}

void JsonConnector::send(const JsonDocument &json) {
    size_t len = serializeJson(json, buff);
    webSocket->textAll(buff, len);
}
