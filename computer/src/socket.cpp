/**
 * @file socket.cpp
 * @author Thomas DUCLOS
 * @brief This file contain all the function relative to the Socket Namespace
 * @version 1.0
 * @date 2022-02-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "socket.hpp"

/**
 * @brief This function initiates use of the Winsock DLL
 * 
 * @return true if the use of the Winsock DLL is sucessfull for windows. Always true for Linux.
 * @return false if the use of the Winsock DLL is unsucessfull for windows.
 */
bool Socket::Start()
{
#ifdef _WIN32
	WSAData wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
	return true;
#endif
}

/**
 * @brief This function terminates use of the Winsock 2 DLL for Windows
 * 
 */
void Socket::Release()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

/**
 * @brief This function returns the latest socket error that happened
 * 
 * @return int error code
 */
int Socket::GetError()
{
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

/**
 * @brief This function closes the socket
 * 
 * @param s the socket to close
 */
void Socket::CloseSocket(SOCKET s)
{
#ifdef _WIN32
	closesocket(s);
#else
	close(s);
#endif
}