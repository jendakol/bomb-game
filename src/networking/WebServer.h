#ifndef BOMBA_WEBSERVER_H
#define BOMBA_WEBSERVER_H

#include <vector>
#include <ESPAsyncWebServer.h> // this is universal over ESP32/ESP8266

#include "JsonConnector.h"
#include <Constants.h>

class WebServer {
public:
    WebServer();

    void begin(JsonConnector &jsonConnector);

private:
    AsyncWebServer *webServer;
    bool send = false;

    void installCaptivePortalRedirects();

    static void handleStaticFile(AsyncWebServerRequest *request) {
        String path = STATIC_FILES_PREFIX + request->url();

        // Serial.printf("Requested URL: %s\n", request->url().c_str());

        if (path.endsWith("/")) path += F("index.html");

        String pathWithGz = path + ".gz";

        if (!(FileSystem.exists(pathWithGz) || FileSystem.exists(path))) {
            // some application page?
            path = String(STATIC_FILES_PREFIX) + "/index.html";
        }

        String contentType = getContentType(path);

        bool gzipped = false;

        if (FileSystem.exists(pathWithGz)) {
            gzipped = true;
            path += ".gz";
        }

        File file = FileSystem.open(path, "r");

//            Serial.println("Serving static file, path=" + path + " size=" + file.size() + " content-type=" + contentType);

        AsyncWebServerResponse *response = request->beginResponse(
                contentType,
                file.size(),
                [file](uint8_t *buffer, size_t maxLen, size_t total) mutable -> size_t {
                    int bytes = file.read(buffer, maxLen);

                    // close file at the end
                    if (bytes + total == file.size()) file.close();

                    return max(0, bytes); // return 0 even when no bytes were loaded
                }
        );

        if (gzipped) {
            response->addHeader(F("Content-Encoding"), F("gzip"));
        }

        response->addHeader(F("Access-Control-Allow-Origin"), F("*"));

        request->send(response);
    }

    static String getContentType(const String &filename) {
        if (filename.endsWith(F(".htm"))) return F("text/html");
        else if (filename.endsWith(F(".html"))) return F("text/html");
        else if (filename.endsWith(F(".css"))) return F("text/css");
        else if (filename.endsWith(F(".js"))) return F("application/javascript");
        else if (filename.endsWith(F(".json"))) return F("application/json");
        else if (filename.endsWith(F(".png"))) return F("image/png");
        else if (filename.endsWith(F(".gif"))) return F("image/gif");
        else if (filename.endsWith(F(".jpg"))) return F("image/jpeg");
        else if (filename.endsWith(F(".ico"))) return F("image/x-icon");
        else if (filename.endsWith(F(".xml"))) return F("text/xml");
        else if (filename.endsWith(F(".pdf"))) return F("application/x-pdf");
        else if (filename.endsWith(F(".zip"))) return F("application/x-zip");
        else if (filename.endsWith(F(".gz"))) return F("application/x-gzip");

        return F("text/plain");
    }
};

#endif //BOMBA_WEBSERVER_H
