#if !defined(WEBSERVER_H)
#define WEBSERVER_H

#include <ESPAsyncWebServer.h>

class WebServer
{
    public:
        WebServer(uint8_t _port);
        ~WebServer();

        void setup();
        void run();

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
