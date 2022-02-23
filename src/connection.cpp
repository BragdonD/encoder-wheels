#include <stdexcept>
#include "connection.h"

/**
 * @brief Construct a new ESP8266WiFi::ESP8266WiFi object. 
 * 
 * @param _useMDNS boolean to use the MDNS for the network
 * @param _mdnsName string to store the local name for mdns
 */
ESP8266WiFi::ESP8266WiFi(bool _useMDNS, String _mdnsName) : m_useMDNS(_useMDNS), m_mdnsName(_mdnsName), m_alive(false) {

}

/**
 * @brief Destroy the ESP8266WiFi::ESP8266WiFi object
 * 
 */
ESP8266WiFi::~ESP8266WiFi() {

}

/**
 * @brief 
 * 
 * @param state 
 */
void ESP8266WiFi::setState(bool state) {
    m_alive = state;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool ESP8266WiFi::getState() {
    return m_alive;
}

void ESP8266WiFi::setMDNS(bool use) {
    m_useMDNS = use;
}

bool ESP8266WiFi::getMDNS(){
    return m_useMDNS;
}

void ESP8266WiFi::setMdnsName(String name) {
    if(name.length() > 0UL) {
        m_mdnsName = name;
    }
    else {
        throw std::invalid_argument("error mdns name can't be null string.");
    }
    
}

String ESP8266WiFi::getMdnsName() {
    return m_mdnsName;
}

/**
 * @brief 
 * 
 */
void ESP8266WiFi::setup() {
    const String wifiNames[] = WIFI_SSID;
    const String wifiPassword[] = WIFI_PASSWORD;

    for(int i = 0; i < WIFI_NB; i++) {
        m_wifis.addAP( wifiNames[i].c_str(), wifiPassword[i].c_str() );
    }

    if( !MDNS.begin( getMdnsName() ) ) {
        Serial.println("Error setting up MDNS.");
        setMDNS( false );
    }
}

/**
 * @brief 
 * 
 */
void ESP8266WiFi::run() {
    if( m_wifis.run() != WL_CONNECTED) {
        if ( getState() ) {
            setState( false );
            Serial.print("Looking for WiFi ");
        }
        Serial.print(".");
        delay(500);
    }
    else if( !getState() ) {
        setState( true );
        Serial.printf("Connected to %s\nLocal Ip : %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str() );
    }

    if( getMDNS() ) {
        MDNS.update();
    }
}