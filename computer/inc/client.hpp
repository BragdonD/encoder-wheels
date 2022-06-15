/**
 * @file client.hpp
 * @author Thomas DUCLOS
 * @brief This file is the header file of the TCPSocket Class
 * @version 1.0
 * @date 2022-02-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#if !defined(CLIENT_HPP)
#define CLIENT_HPP

#include <exception>
#include <string>
#include "socket.hpp"

/**
 * @brief Class which handle the connection to the ESP8266 Card and serve as a client socket.
 * 
 */
class TCPSocket {
    private:
        SOCKET m_Socket;
    
    public:
        TCPSocket();
        ~TCPSocket();
        
        bool Connect_IP(const std::string& ipadress, unsigned short port);
        bool Connect_MDNS(const std::string& mdnsadress, unsigned short port);

        void sendData(char data);
};

#endif // CLIENT_HPP