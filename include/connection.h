#if !defined(CONNECTION_H)
#define CONNECTION_H

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include "secret.h"

class ESP8266WiFi {
    public:
        ESP8266WiFi(bool _useMDNS = true, String _mdnsName = "");
        ~ESP8266WiFi();

        void setState(bool state);
        bool getState();

        void setMDNS(bool use);
        bool getMDNS();

        void setMdnsName(String name);
        String getMdnsName();

        void setup();
        void run();

    private:
        bool m_alive;
        bool m_useMDNS;
        String m_mdnsName;
        ESP8266WiFiMulti m_wifis;
};

#endif // CONNECTION_H
