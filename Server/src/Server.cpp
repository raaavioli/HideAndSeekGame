#include "Server.h"
#include <sstream>

#include "Protocol.h"
#include "Utils/MazeGenerator.h"
#include "GameObjects/Player.h"
#include "GameObjects/Collision/Collider.h"

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

	GroundPlane &m_Plane = GroundPlane(60, 40);
	Collider::Add(&m_Plane, MovementType::STATIC);

	//Outer walls
	Wall *w = new Wall(m_Plane, -1, 0, glm::vec3(1, (int)m_Plane.GetHeight(), 4));

	std::string s = w->ToProtocolString();
	Protocol protocol = Protocol(&s);
	/*
	Fix so that the values from the protocol are sent to each client.
	*/


	Collider::Add(w, MovementType::STATIC);
	Collider::Add(new Wall(m_Plane, (int)m_Plane.GetWidth(), 0, glm::vec3(1, (int)m_Plane.GetHeight(), 4)), MovementType::STATIC);
	Collider::Add(new Wall(m_Plane, 0, -1, glm::vec3((int)m_Plane.GetWidth(), 1, 4)), MovementType::STATIC);
	Collider::Add(new Wall(m_Plane, 0, (int)m_Plane.GetHeight(), glm::vec3((int)m_Plane.GetWidth(), 1, 4)), MovementType::STATIC);

	MazeGenerator mg(12, 8);
	mg.GenerateMaze();
	mg.CutLongerWalls(3);
	mg.PrintMaze();


	for (Wall* w : mg.GetGameWalls(m_Plane)) {
		Collider::Add(w, MovementType::STATIC);
		w->Update();
	}

	Player *m_Player = new Player();
	Collider::Add(m_Player, MovementType::DYNAMIC);

	while (m_Running)
	{
		for (SOCKET client : m_Clients)
		{

		}
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
	for (int i = 0; i < m_ClientCount; i++) 
	{
		sockaddr_in client;
		int clientSize = sizeof(client);
		std::cout << "Trying to accept a client..." << std::endl;
		m_Clients.push_back(accept(m_ServerFD, (sockaddr*)&client, &clientSize));
		std::cout << "Client was accepted successfully" << std::endl;
	}
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

void Server::Send(SOCKET &client, std::string message)
{
	message.append('\0');
	send(client, message.c_str(), message.size(), 0);
}

std::string Server::Receive(SOCKET client)
{
	int valread = 0, accum = 0, buffersize = 4096;
	char buffer[4096] = { 0 };

	if (recv(client, buffer, buffersize, 0) > 0)
		return buffer;
	else
		return "";
}
