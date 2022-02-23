#include "webServer.h"

/**
 * @brief Construct a new Web Server:: Web Server object
 * 
 * @param _port 
 */
WebServer::WebServer(uint8_t _port) : m_port(_port) ,m_server(_port), m_ws("/ws") {

}

/**
 * @brief Destroy the Web Server:: Web Server object
 * 
 */
WebServer::~WebServer() {

}

/**
 * @brief 
 * 
 */
void WebServer::setup() {
    initWS();

    //Setup all the path
    

    m_server.begin();
}

/**
 * @brief 
 * 
 */
void WebServer::run() {
    m_ws.cleanupClients();
}

/**
 * @brief 
 * 
 * @param arg 
 * @param data 
 * @param len 
 */
void handleWSMessage(void* arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;

    if( info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        ///A changer mais test comment le json est handle
        Serial.println((char*)data);
    }
}

/**
 * @brief 
 * 
 */
void WebServer::notifyClients(String &data) {
    m_ws.textAll(data);
}

/**
 * @brief 
 * 
 * @param server 
 * @param client 
 * @param type 
 * @param arg 
 * @param data 
 * @param len 
 */
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type)
    {
    case WS_EVT_CONNECT:
        Serial.printf( "New client %u connected from %s\n", client->id(), client->remoteIP().toString().c_str() );
        break;
    case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
    case WS_EVT_DATA:
        handleWSMessage(arg, data, len);
        break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        break;
    
    default:
        break;
    }
}

/**
 * @brief 
 * 
 */
void WebServer::initWS() {
    m_ws.onEvent(onEvent);
    m_server.addHandler(&m_ws);
}
