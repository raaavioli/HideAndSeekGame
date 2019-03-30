#include "ServerHandler.h"
#include "Engine/Log.h"

ServerHandler *ServerHandler::s_Instance = nullptr;
SOCKET ServerHandler::m_Socket;
std::string ServerHandler::m_Addr;
int ServerHandler::m_Port;
char ServerHandler::m_Opt;

void ServerHandler::Init(std::string addr, int port)
{
	APP_ASSERT(!s_Instance, "ServerHandler already instantiated");
	s_Instance = new ServerHandler();
	m_Addr = addr;
	m_Port = port;
	m_Opt = 1;

	WSADATA wsa_data;
	int wsInt = 0;
#ifdef _WIN32
	wsInt = WSAStartup(MAKEWORD(2, 2), &wsa_data);
#endif
	APP_ASSERT(wsInt == 0, "WSAStartup was not done successfully");

	CreateSocket();
}

void ServerHandler::Send(const std::string message)
{
	send(m_Socket, message.c_str(), message.size(), 0);
}

std::string ServerHandler::Recieve()
{
	int valread = 0, accum = 0, buffersize = 4096;
	char buffer[4096] = { 0 };

	if (valread = recv(m_Socket, buffer, buffersize, 0))
		return std::string(buffer, valread);
	else
		return "";
}

void ServerHandler::Shutdown()
{
	delete s_Instance;
	CloseSocket();
#ifdef _WIN32
	WSACleanup();
#endif
}

void ServerHandler::CreateSocket()
{
	//Create a socket
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket != INVALID_SOCKET) {
		sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(m_Port);
		server_addr.sin_addr.S_un.S_addr = inet_addr(m_Addr.c_str());

		APP_ASSERT(connect(m_Socket, (sockaddr*)&server_addr, sizeof(server_addr)) >= 0, "Could not connect to server...");
		APP_INFO("Successfully connected to server: IP {0}, Port {1}", m_Addr, m_Port);
		return;
	}
	APP_ERROR("Socket was not successfully created!");
}

/* Note: For POSIX, typedef SOCKET as an int. */
void ServerHandler::CloseSocket()
{
	int status = 0;

#ifdef _WIN32
	status = shutdown(m_Socket, SD_BOTH);
	if (status == 0) { status = closesocket(m_Socket); }
#else
	status = shutdown(sock, SHUT_RDWR);
	if (status == 0) { status = close(sock); }
#endif
	APP_ASSERT(status == 0, "Socket was not successfully closed!");
}
