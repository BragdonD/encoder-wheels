#if !defined(WS_H)
#define WS_H

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
class WebServer
{
    public:
        WebServer(uint8_t _port);
        ~WebServer();

        void setup();
        void run();
        static String processor(const String& var);

        void notifyClients(String &data);
        void initWS();

    private:
        uint8_t m_port;
        AsyncWebServer m_server;
        AsyncWebSocket m_ws;
};

void handleWSMessage(void* arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);


#endif // WEBSERVER_H
