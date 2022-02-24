#include "ws.h"

/**
 * @brief Construct a new Web Server:: Web Server object
 * 
 * @param _port 
 */
WebServer::WebServer(uint8_t _port) : m_port(_port) ,m_server(_port), m_ws("/ws"), m_encoder(SECRET_KEY) {

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
    LittleFS.begin();

    initWS();

    //Setup all the path
    m_server.on("/login", HTTP_GET, [this](AsyncWebServerRequest *request) {
        Serial.println("get /login.html asked");
        request->send(LittleFS, "./login.html", String(), false);
    });

    m_server.on("/login.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("get /login.css asked");
        request->send(LittleFS, "./login.css", "text/css");
    });

    m_server.on("/login.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("get /login.js asked");
        request->send(LittleFS, "./login.js", "text/javascript");
    });

    m_server.on("/", [this](AsyncWebServerRequest *request) {
        Serial.println("get /index.html asked");
        request->send(LittleFS, "./index.html", String(), false);
    });

    m_server.onNotFound([](AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS) {
            request->send(200);
        } else {
            request->send(404);
        }
    });

    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/login", [this](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonObject jsonObj = json.as<JsonObject>();
        if( !jsonObj.containsKey("ssid") || !jsonObj.containsKey("password") ) {
            request->send(404, "text/plain", "wrong password/ssid");
        }
        else {
            String ssid = jsonObj["ssid"];
            String password = jsonObj["password"];

            if( checkSecurity(ssid, password) ) {
                AsyncWebServerResponse *response = request->beginResponse(201, "text/plain", "accepted");
                String clientIp = request->client()->localIP().toString();
                String Cookie = ( "ACCESS=" + createJWT( clientIp ) + ";SameSite=Strict" );
                response->addHeader( "Set-Cookie", Cookie );
                request->send( response );
            }
            else {
                request->send(404, "text/plain", "wrong password/ssid");
            }
        }
        
        
    });

    m_server.addHandler(handler);

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    m_server.begin();
}

/**
 * @brief 
 * 
 */
void WebServer::run() {
    m_ws.cleanupClients();
}

String WebServer::processor(const String& var) {
    return "temp";
}

String WebServer::createJWT(String &Ip) {
    String JWT = m_encoder.encodeJWT(Ip);
    m_JWT.push_back(JWT);
    return JWT;
}

bool WebServer::checkJWT(String &JWT) {
    for(const auto &it : m_JWT) {
        if( strcmp( JWT.c_str(), it.c_str() ) == 0 ) {
            return true;
        }
    }
    return false;
}

bool WebServer::checkSecurity(String &ssid, String &password) {
    return ( strcmp( ssid.c_str(), SERVER_SSID) == 0 && strcmp( password.c_str(), SERVER_PASSWORD ) == 0 );
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
