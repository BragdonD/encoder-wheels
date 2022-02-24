#if !defined(WS_H)
#define WS_H

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <ArduinoJWT.h>
#include <vector>
#include "secret.h"

class WebServer
{
    public:
        WebServer(uint8_t _port);
        ~WebServer();

        void setup();
        void run();
        static String processor(const String& var);

        String createJWT(String &Ip);
        bool checkJWT(String &JWT);
        bool checkSecurity(String &ssid, String &password);

        void notifyClients(String &data);
        void initWS();

    private:
        uint8_t m_port;
        AsyncWebServer m_server;
        AsyncWebSocket m_ws;
        ArduinoJWT m_encoder;
        std::vector<String> m_JWT;       
};

void handleWSMessage(void* arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);


#endif // WEBSERVER_H
