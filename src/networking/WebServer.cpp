#include "WebServer.h"

WebServer::WebServer() {
    webServer = new AsyncWebServer(HTTP_PORT);
}

void WebServer::begin(JsonConnector &jsonConnector) {
    installCaptivePortalRedirects();

    webServer->onNotFound([](AsyncWebServerRequest *request) {
        WebServer::handleStaticFile(request);
    });

    jsonConnector.bind(*webServer);

    webServer->begin();
}

void WebServer::installCaptivePortalRedirects() {
    webServer->addRewrite(new AsyncWebRewrite("/home", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/generate_204", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/gen_204", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/fwlink", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/connecttest.txt", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/hotspot-detect.html", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/library/test/success.html", "/index.html"));
    webServer->addRewrite(new AsyncWebRewrite("/kindle-wifi/wifistub.html", "/index.html"));
}
