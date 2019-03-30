#pragma once

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#pragma comment (lib, "ws2_32.lib")
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <winsock.h>
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
typedef int SOCKET
#endif

#include <string>

class ServerHandler
{
public:
	static void Init(std::string addr, int port);
	static void Send(const std::string message);
	static std::string Recieve();
	static void Shutdown();

private:
	static void CreateSocket();
	static void CloseSocket();
	
	static ServerHandler *s_Instance;
	static SOCKET m_Socket;
	static std::string m_Addr;
	static int m_Port;
	static char m_Opt;
};