#include "Server.h"
#include <sstream>
#include <iostream>

#include "GameObjects/Collision/Collider.h"
#include "Protocol.h"
#include "Utils/MazeGenerator.h"
#include "GameObjects/GroundPlane.h"
#include "GameObjects/Wall.h"
#include "GameObjects/Flag.h"

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
	std::string gameMapProtocolString = std::string();

	GroundPlane &m_Plane = GroundPlane(60, 40);
	Collider::Add(&m_Plane, MovementType::STATIC);
	gameMapProtocolString.append(m_Plane.ToProtocolString());

	MazeGenerator mg(20, 10);
	mg.GenerateMaze();
	mg.CutLongerWalls(3);
	mg.PrintMaze();

	for (Wall* w : mg.GetGameWalls(m_Plane)) {
		Collider::Add(w, MovementType::STATIC);
		gameMapProtocolString.append(w->ToProtocolString());
		w->Update();
	}

	Flag *red = new Flag(m_Plane, 0);
	gameMapProtocolString.append(red->ToProtocolString());
	Flag *blue = new Flag(m_Plane, 1);
	gameMapProtocolString.append(blue->ToProtocolString());
	Collider::Add(red, MovementType::STATIC);
	Collider::Add(blue, MovementType::STATIC);

	for (Client* client : m_Clients)
	{
		SOCKET s = client->GetSocket();
		Send(s, gameMapProtocolString);

		//Receive handshake that client is done reading map
		Receive(client->GetSocket());

		//Send player details.
		//First player sent is the client's main player, the rest are sent for player positions
		std::string playerData = client->GetPlayer()->ToProtocolString();
		for (Client* other : m_Clients)
		{
			if (other->GetSocket() == client->GetSocket()) continue;
			playerData.append(other->GetPlayer()->ToProtocolString());
		}
		Send(s, playerData);
	}

	while (m_Running)
	{
		//Receive instructions from each client
		for (Client* client : m_Clients)
		{
			std::string playerData = Receive(client->GetSocket());
			client->GetPlayer()->UpdatePlayerData(playerData);
			/*TODO
			
				Make sure UpdatePlayerData not only updates player data, 
				but also parses any Action-requests.
			*/
		}

		Collider::Interact();

		for (Client* client : m_Clients)
		{
			std::string playerData = client->GetPlayer()->ToProtocolString();
			for (Client* other : m_Clients)
			{
				if (other->GetSocket() == client->GetSocket()) continue;
				playerData.append(other->GetPlayer()->ToProtocolString());
			}
			SOCKET s = client->GetSocket();
			Send(s, playerData);
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
		hint.sin_addr.S_un.S_addr = ADDR_ANY;
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
		//CHANGE PLAYER POSITION LATER, NOW IT WILL NOT WORK FOR MORE THAN TWO PLAYERS
		Client *c = new Client(accept(m_ServerFD, (sockaddr*)&client, &clientSize), -27.5 + i * 20, -18.5);
		m_Clients.push_back(c);
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

void Server::Send(SOCKET &client, std::string &message)
{
	int data = send(client, message.c_str(), message.size(), 0);
	std::cout << data << " bytes were sent to client " << client << std::endl;
}

std::string Server::Receive(SOCKET client)
{
	int valread = 0, accum = 0, buffersize = 16384;
	char buffer[16384] = { 0 };

	if (valread = recv(client, buffer, buffersize, 0))
		return std::string(buffer, valread);
	else
		return "";
}
