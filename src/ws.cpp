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
 * @brief This function setup the webServer and all the route which can be called. 
 * It can handle post request with json body.
 * It can handle CORS request.
 * It inits the system file LittleFS.
 * It inits the webSocket.
 * This function need to be call once inside the setup function.
 */
void WebServer::setup() {
    ///Start the file system LittleFS
    LittleFS.begin();

    ///Start the WebSocket
    initWS();

    ///Setup all the path of the server
    ///Login path
    m_server.on("/login", HTTP_GET, [this](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /login.html asked");
        #endif
        request->send(LittleFS, "./login.html", String(), false);
    });

    ///Login css file path
    m_server.on("/login.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /login.css asked");
        #endif
        request->send(LittleFS, "./login.css", "text/css");
    });

    ///Login css.map file path
    m_server.on("/login.css.map", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /login.css.map asked");
        #endif
        request->send(LittleFS, "./login.css.map", "text/css");
    });

    ///Login scss file path
    m_server.on("/login.scss", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /login.scss asked");
        #endif
        request->send(LittleFS, "./login.scss", "text/css");
    });

    ///Login js file path
    m_server.on("/login.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /login.js asked");
        #endif
        request->send(LittleFS, "./login.js", "text/javascript");
    });

    ///Login js.map file path
    m_server.on("/login.js.map", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /login.js.map asked");
        #endif
        request->send(LittleFS, "./login.js.map", "text/javascript");
    });

    ///Login ts file path
    m_server.on("/login.ts", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /login.ts asked");
        #endif
        request->send(LittleFS, "./login.ts", "text/javascript");
    });

    ///Root path
    m_server.on("/", [this](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /index.html asked");
        #endif

        String token = request->header( "Cookie" );
        int index = token.indexOf( "ACCESS=" );

        if( index != -1) {
            token.remove( index, strlen("ACCESS=") );
            if( checkJWT(token) ) {
                #if DEBUG
                    Serial.printf("Accord access at /index.html to %s\n", request->client()->localIP().toString().c_str());
                #endif
                request->send(LittleFS, "./index.html", String(), false);
            }
            else {
                request->redirect("/login");
            }
        }
        else {
            request->redirect("/login");
        }
    });

     ///index css file path
    m_server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /index.css asked");
        #endif
        request->send(LittleFS, "./index.css", "text/css");
    });

    ///index css.map file path
    m_server.on("/index.css.map", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /index.css.map asked");
        #endif
        request->send(LittleFS, "./index.css.map", "text/css");
    });

    ///index scss file path
    m_server.on("/index.scss", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /index.scss asked");
        #endif
        request->send(LittleFS, "./index.scss", "text/css");
    });

    ///index js file path
    m_server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /index.js asked");
        #endif
        request->send(LittleFS, "./index.js", "text/javascript");
    });

    ///index js.map file path
    m_server.on("/index.js.map", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /index.js.map asked");
        #endif
        request->send(LittleFS, "./index.js.map", "text/javascript");
    });

    ///index ts file path
    m_server.on("/index.ts", HTTP_GET, [](AsyncWebServerRequest *request) {
        #if DEBUG
            Serial.println("get /index.ts asked");
        #endif
        request->send(LittleFS, "./index.ts", "text/javascript");
    });

    ///Handle Cors request
    m_server.onNotFound([](AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS) {
            request->send(200);
        } else {
            request->send(404);
        }
    });

    ///Handler Json for the post method on login path
    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/login", [this](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonObject jsonObj = json.as<JsonObject>(); ///transform the data in a Json Object
        ///Check if the two key exist for authentification
        if( !jsonObj.containsKey("ssid") || !jsonObj.containsKey("password") ) {
            ///bad request
            request->send(404, "text/plain", "wrong password/ssid");
        }
        else {
            ///get both key value
            String ssid = jsonObj["ssid"];
            String password = jsonObj["password"];

            ///check the ssid and the password
            if( checkSecurity(ssid, password) ) {
                ///correct password and ssid

                ///create a response
                AsyncWebServerResponse *response = request->beginResponse(201, "text/plain", "accepted");
                ///create a cookie for authentification purpose
                String clientIp = request->client()->localIP().toString();
                String Cookie = ( "ACCESS=" + createJWT( clientIp ) + ";SameSite=Strict" );
                ///set the cookie
                response->addHeader( "Set-Cookie", Cookie );
                ///send the response
                request->send( response );
            }
            else {
                ///bad password/ssid
                request->send(404, "text/plain", "wrong password/ssid");
            }
        }
        
        
    });

    ///add the handler function
    m_server.addHandler(handler);
    ///default header to handle Cors
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    ///Start web server
    m_server.begin();
}

/**
 * @brief This function run the webserver and websocket. It needs to be called inside the loop function.
 * 
 */
void WebServer::run() {
    m_ws.cleanupClients();
}

/**
 * @brief Function to swap variable value inside html file.
 * 
 * @param var 
 * @return String 
 */
[[deprecated("This function is in here developping stage do not use it")]]
String WebServer::processor(const String& var) {
    return "temp";
}

/**
 * @brief This function create a JWT from an Ip as payload. It use ArduinoJWT
 * 
 * @param Ip Client local Ip
 * @return String containing the JWT
 */
String WebServer::createJWT(String &Ip) {
    String JWT = m_encoder.encodeJWT(Ip);
    m_JWT.push_back(JWT); ///add the JWT inside the JWT storage
    return JWT;
}

/**
 * @brief Function which check if the JWT is contain inside the list of already existing JWT in circulation.
 * 
 * @param JWT String to test
 * @return true if find else false
 */
bool WebServer::checkJWT(String &JWT) {
    for(const auto &it : m_JWT) {
        if( strcmp( JWT.c_str(), it.c_str() ) == 0 ) {
            return true;
        }
    }
    return false;
}

/**
 * @brief check if the login is matching with secret.h
 * 
 * @param ssid String
 * @param password String
 * @return true if matching else false
 */
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
void handleWSMessage(void* arg, uint8_t *data, size_t len, AsyncWebSocket *socket) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;

    if( info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;

        DynamicJsonDocument doc(1024);
        deserializeJson(doc, (char*)data);
        JsonObject json = doc.as<JsonObject>();

        if(json["motorA"]["speed"]) {
            #if DEBUG
                Serial.println("modif speed motor A");
            #endif
            motorA->wantedSpeed = json["motorA"]["speed"];
        }
        else if(json["motorA"]["state"]) {
            #if DEBUG
                Serial.println("modif state motor A");
            #endif
            if(json["motorA"]["state"] == "on") {
                motorA->state = ON;
            }
            else {
                motorA->state = OFF;
            }
        }
        else if(json["motorB"]["speed"]) {
            #if DEBUG
                Serial.println("modif speed motor B");
            #endif
            motorB->wantedSpeed = json["motorB"]["speed"];
        }
        else if(json["motorB"]["state"]) {
            #if DEBUG
                Serial.println("modif state motor B");
            #endif
            if(json["motorB"]["state"] == "on") {
                motorB->state = ON;
            }
            else {
                motorB->state = OFF;
            }
        }

        String Data;
        serializeJson(json, Data);
        notifyClients(Data, socket);
    }
}

/**
 * @brief 
 * 
 * @param data 
 * @param ws 
 */
void notifyClients(String &data, AsyncWebSocket *ws) {
    ws->textAll(data);
}

/**
 * @brief 
 * 
 * @param ws 
 * @param client 
 * @param type 
 * @param arg 
 * @param data 
 * @param len 
 */
void onEvent(AsyncWebSocket *ws, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    switch (type)
    {
    case WS_EVT_CONNECT:
        #if DEBUG
            Serial.printf( "New client %u connected from %s\n", client->id(), client->remoteIP().toString().c_str() );
        #endif
        break;
    case WS_EVT_DISCONNECT:
        #if DEBUG
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
        #endif
        break;
    case WS_EVT_DATA:
        handleWSMessage(arg, data, len, ws);
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
