#pragma once
#ifdef _WIN32
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

#include "GameObjects/Player.h"

class Client {
public:
	Client(SOCKET s, float playerX, float playerY);
	~Client();

	inline SOCKET GetSocket() { return m_Socket; }
	inline Player *GetPlayer() { return m_Player; }

private:
	SOCKET m_Socket;
	Player *m_Player;
};