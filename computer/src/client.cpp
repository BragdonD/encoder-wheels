/**
 * @file client.cpp
 * @author Thomas DUCLOS
 * @brief this file contain all the relative function to the TCPSocket Class.
 * @version 1.0
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <ws2tcpip.h> 
#include "client.hpp"

/**
 * @brief Construct a new TCPSocket::TCPSocket object. The socket is create with the TCP Protocol and can use IPV4-IPV6 Ip Adress.
 * In case the creation failed for an X Reason there will be an error throw with the Error Code associated. You will maybe need to wrap this function inside a try ... catch ...
 */
TCPSocket::TCPSocket() {
	m_Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);	///Create a socket with the basic configuration for a TCP Socket
	///Basic test to know if the Socket is valid.
	if( m_Socket == INVALID_SOCKET ) {
		std::string error = "Erreur initialisation socket [" + std::to_string(Socket::GetError()) + "]";
		throw std::runtime_error(error);
	}
}

/**
 * @brief Destroy the TCPSocket::TCPSocket object
 * 
 */
TCPSocket::~TCPSocket() {
    	Socket::CloseSocket(m_Socket);
}

/**
 * @brief Method of the TCPSocket Class to connect the socket to a server with an ipadress
 * 
 * @param ipadress string wich contain the Ip Adress of the server 
 * @param port server port
 * @return true if the connection suceed
 * @return false if the connection failed
 */
bool TCPSocket::Connect_IP(const std::string& ipadress, unsigned short port) {
	sockaddr_in server;	///Create a variable with the information to connect to the server
	inet_pton(AF_INET, ipadress.c_str(), &server.sin_addr.s_addr); ///converts the src string to an af-family network address structure
	///Fill the variable with the connection informations
	server.sin_family = AF_INET;
	server.sin_port = htons(port); 
	return connect(m_Socket, (struct sockaddr*)&server, sizeof(server)) == 0; ///Return the result of the connection.
}

/**
 * @brief Method of the TCPSocket Class to connect the socket to a server with a MDNS name
 * 
 * @param mdnsadress string wich contain the Ip Adress of the server. You need to put .local at the end of the name
 * @param port server port
 * @return true if the connection suceed 
 * @return false if the connection failed 
 */
bool TCPSocket::Connect_MDNS(const std::string& mdnsadress, unsigned short port) {
	struct addrinfo *result = NULL; ///Create a variable with the information to connect to the server
	struct addrinfo hints; 			///Create a variable with the information to connect to the server
	ZeroMemory(&hints, sizeof(hints));   
	///Fill the variable with the connection informations
	hints.ai_family = AF_INET;  
	hints.ai_socktype = SOCK_STREAM;   
	hints.ai_protocol = IPPROTO_TCP; 
	///Try to get the address information to test if it exists
	if(getaddrinfo(mdnsadress.c_str(), std::to_string(port).c_str(), &hints, &result) != 0) {
		std::cout << "Unable to find : " << mdnsadress << std::endl;
		return false;
	}
	return connect(m_Socket, result->ai_addr, (int)result->ai_addrlen) == 0; ///Return the result of the connection.
}

void TCPSocket::sendData(char data)
{
	char buff[2];
	buff[1] = data;
	send(m_Socket, buff, 2, 0);
}