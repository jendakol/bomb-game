#ifndef BOMBA_JSONCONNECTOR_H
#define BOMBA_JSONCONNECTOR_H

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

class JsonConnector {
public:
    JsonConnector();
    void bind(AsyncWebServer &webServer);
    void subscribe(const std::function<void(const JsonDocument&)> &callback);
    void send(const JsonDocument& json);
private:
    AsyncWebSocket *webSocket;
    char buff[1024];
    std::vector<std::function<void(const JsonDocument&)>> callbacks;

    void onWsEvent(AwsEventType type, uint8_t *data, size_t len);
};

#endif //BOMBA_JSONCONNECTOR_H
