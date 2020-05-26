#ifndef BOMBA_WEBSERVER_H
#define BOMBA_WEBSERVER_H

#include <vector>
#include <ESPAsyncWebServer.h> // this is universal over ESP32/ESP8266

#include <Constants.h>

class WebServer {
public:
    WebServer();

    void begin();

    // TODO send and receive ws

private:
    AsyncWebServer *webServer;
    AsyncWebSocket *webSocket;

    bool send = false;

    void installCaptivePortalRedirects();

    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, uint8_t *data, size_t len);

    static bool handleStaticFile(AsyncWebServerRequest *request) {
        String path = STATIC_FILES_PREFIX + request->url();

        if (path.endsWith("/")) path += F("index.html");

        String contentType = getContentType(path);
        String pathWithGz = path + ".gz";

        if (FileSystem.exists(pathWithGz) || FileSystem.exists(path)) {
            bool gzipped = false;

            if (FileSystem.exists(pathWithGz)) {
                gzipped = true;
                path += ".gz";
            }

            File file = FileSystem.open(path, "r");

            Serial.println("Serving static file, path=" + path + " size=" + file.size() + " content-type=" + contentType);

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

            request->send(response);

            return true;
        }

        return false;
    }

    static String getContentType(const String &filename) {
        if (filename.endsWith(F(".htm"))) return F("text/html");
        else if (filename.endsWith(F(".html"))) return F("text/html");
        else if (filename.endsWith(F(".css"))) return F("text/css");
        else if (filename.endsWith(F(".js"))) return F("application/javascript");
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
