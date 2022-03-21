#include <stdexcept>
#include "connection.h"

/**
 * @brief Construct a new ESP8266WiFi::ESP8266WiFi object. 
 * 
 * @param _useMDNS boolean to use the MDNS for the network
 * @param _mdnsName string to store the local name for mdns
 */
ESP8266WiFi::ESP8266WiFi(bool _useMDNS, String _mdnsName) : m_alive(false), m_useMDNS(_useMDNS), m_mdnsName(_mdnsName) {

}

/**
 * @brief Destroy the ESP8266WiFi::ESP8266WiFi object
 * 
 */
ESP8266WiFi::~ESP8266WiFi() {

}

/**
 * @brief Set the wifi state
 * 
 * @param state boolean
 */
void ESP8266WiFi::setState(bool state) {
    m_alive = state;
}

/**
 * @brief Get the wifi state
 * 
 * @return true if connected
 * @return false if disconnected
 */
bool ESP8266WiFi::getState() {
    return m_alive;
}

/**
 * @brief Set if MDNS need to be use.
 * 
 * @param use boolean true if you wanna use it
 */
void ESP8266WiFi::setMDNS(bool use) {
    m_useMDNS = use;
}

/**
 * @brief Get if MDNS need to be use
 * 
 * @return if it need to be use
 */
bool ESP8266WiFi::getMDNS(){
    return m_useMDNS;
}

/**
 * @brief Set the MDNS Name 
 * 
 * @param name String which contain the name. Need to be minimum more than 0 lentgh
 */
void ESP8266WiFi::setMdnsName(String name) {
    if(name.length() > 0UL) {
        m_mdnsName = name;
    }
    else {
        #if DEBUG
            throw std::invalid_argument("error mdns name can't be null string.");
        #endif
    }
    
}

/**
 * @brief Get the MDNS Name
 * 
 * @return String which contain the MDNS name.
 */
String ESP8266WiFi::getMdnsName() {
    return m_mdnsName;
}

/**
 * @brief Setup function which init the wifi multi with the ssids and passwords given in secret.h.
 * And it init the MDNS.
 * This function needs to be called inside the setup function only once.
 * 
 */
void ESP8266WiFi::setup() {
    ///Init variables with the define inside secret.h
    const String wifiNames[] = WIFI_SSID;
    const String wifiPassword[] = WIFI_PASSWORD;
    ///Add all the wifis
    for(int i = 0; i < WIFI_NB; i++) {
        m_wifis.addAP( wifiNames[i].c_str(), wifiPassword[i].c_str() );
    }
    ///Init MDNS
    if( !MDNS.begin( getMdnsName() ) ) {
        #if DEBUG
            Serial.println("Error setting up MDNS.");
        #endif
        setMDNS( false );
    }
}

/**
 * @brief Run function which reconnect to the next wifi if the connection is lost.
 * It runs the MDNS.
 * This function needs to be called inside the loop function to run the wifi and the MDNS.
 * 
 */
void ESP8266WiFi::run() {
    ///Reconnect to the next wifi if the connection is lost
    if( m_wifis.run() != WL_CONNECTED) {
        if ( getState() ) {
            setState( false );
            #if DEBUG
                Serial.print("Looking for WiFi ");
            #endif
        }
        #if DEBUG
            Serial.print(".");
        #endif
        delay(500);
    }
    else if( !getState() ) {
        setState( true );
        #if DEBUG
            Serial.printf("Connected to %s\nLocal Ip : %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str() );
        #endif
    }
    ///Run MDNS
    if( getMDNS() ) {
        MDNS.update();
    }
}