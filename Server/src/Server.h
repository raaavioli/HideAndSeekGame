#pragma once
#ifdef _WIN32
	#define _WINSOCK_DEPRECATED_NO_WARNINGS
	/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
	#define _WIN32_WINNT 0x0501 
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

#include <iostream>

class Server
{
public:
	Server(std::string ipAddress, int port)
		: m_Address(ipAddress), m_Port(port) 
	{
		m_Opt = 1;
		Init();
	};
	~Server()
	{
		Shutdown();
	};

	void Run();


private:
	//The server socket
	SOCKET m_ServerFD;
	SOCKET m_ClientFD;
	//The 4 byte address.
	std::string m_Address;
	//The port
	int m_Port;
	//The opt for set/get-sockopt
	char m_Opt;
	bool m_Running;

	void Init();
	void Shutdown();
	SOCKET CreateSocket();
	void Wait();
	int CloseSocket();
};

