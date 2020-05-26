#include "WebServer.h"

WebServer::WebServer() {
    webServer = new AsyncWebServer(HTTP_PORT);
    webSocket = new AsyncWebSocket(WEBSOCKET_PATH);
}

void WebServer::begin() {
    installCaptivePortalRedirects();

    webServer->onNotFound([](AsyncWebServerRequest *request) {
        if (WebServer::handleStaticFile(request)) return;

        request->send(404);
    });

    webServer->addHandler(webSocket);
    webSocket->onEvent(
            [this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
                onWsEvent(server, client, type, data, len);
            }
    );

    webServer->begin();
}

void WebServer::installCaptivePortalRedirects() {
    webServer->addRewrite(new AsyncWebRewrite("/generate_204", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/gen_204", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/fwlink", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/connecttest.txt", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/hotspot-detect.html", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/library/test/success.html", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/kindle-wifi/wifistub.html", "/index.html"));
}

void WebServer::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, uint8_t *data, size_t len) {
    // TODO implement meaningfully

    if (type == WS_EVT_CONNECT) {
        Serial.println(F("Websocket client connection received"));

//        client->text("hello"); // handshake

        if (!send) {
            send = true;

            long c = 0;
            DefaultLooper.add([c, server]() mutable {

                if (c++ % 10 == 0) {
                    server->printfAll("%ld", c++ % 255);
                }

                delay(10);
            });
        }


    } else if (type == WS_EVT_DISCONNECT) {
        Serial.println(F("Websocket client disconnected"));
    } else if (type == WS_EVT_DATA) {
        Serial.print(F("Websocket data received: "));

        for (uint i = 0; i < len; i++) {
            Serial.print((char) data[i]);
        }

        Serial.println();
    }
}
