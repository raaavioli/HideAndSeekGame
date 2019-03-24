#include "Server.h"
#include <sstream>

void Server::Init()
{
	WSADATA wsa_data;
	int wsInt = 0;
#ifdef _WIN32
	wsInt = WSAStartup(MAKEWORD(2, 2), &wsa_data);
#endif
	if (wsInt != 0)
	{
		std::cerr << "Wsa_data not successfully initialized" << std::endl;
		return;
	}

	if (CreateSocket() == INVALID_SOCKET) std::cerr << "Socket was not successfully created" << std::endl;
}

void Server::Run()
{
	m_Running = true;

	int valread;
	char buffer[4096] = { 0 };
	int buffersize = 4096;

	while (m_Running)
	{
		int accum = 0;
		while ((valread = recv(m_ClientFD, buffer, buffersize, 0)) > 0) 
		{
			accum += valread;
			if (accum > buffersize) break;

			printf("From Client: %.*s, %d bytes data \n", valread, buffer+accum-valread, valread);

			if (accum%buffersize != 0) break;
		}
		printf("All string: %s", buffer);

		send(m_ClientFD, "Gotya!!!", buffersize, 0);
		if (strcmp(buffer, "quit") == 0)
			m_Running = false;

	}
	std::cout << "The server got shut down!" << std::endl;
}

SOCKET Server::CreateSocket()
{
	//Create a socket
	m_ServerFD = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ServerFD != INVALID_SOCKET) {
		//Attach socket to port


		//Bind ip and port to socket
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_Port);
		hint.sin_addr.S_un.S_addr = inet_addr(m_Address.c_str());
		if (setsockopt(m_ServerFD, SOL_SOCKET, SO_REUSEADDR, &m_Opt, sizeof(m_Opt)) < 0)
		{
			std::cerr << "Socket opt could not be set!" << std::endl;
		}
		bind(m_ServerFD, (sockaddr*)&hint, sizeof(hint));
		listen(m_ServerFD, SOMAXCONN);

		Wait();
	}
	return m_ServerFD;
}

void Server::Wait()
{
	sockaddr_in client;
	int clientSize = sizeof(client);
	std::cout << "Trying to accept a client..." << std::endl;
	m_ClientFD = accept(m_ServerFD, (sockaddr*)&client, &clientSize);
	std::cout << "Client was accepted successfully" << std::endl;
}

/* Note: For POSIX, typedef SOCKET as an int. */
int Server::CloseSocket()
{
	int status = 0;

#ifdef _WIN32
	status = shutdown(m_ServerFD, SD_BOTH);
	if (status == 0) { status = closesocket(m_ServerFD); }
#else
	status = shutdown(sock, SHUT_RDWR);
	if (status == 0) { status = close(sock); }
#endif

	return status;

}

void Server::Shutdown()
{
	CloseSocket();
#ifdef _WIN32
	WSACleanup();
#endif
}
