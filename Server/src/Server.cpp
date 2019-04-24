#include "Server.h"
#include <iostream>
#include <thread>

#include "GameObjects/Collision/Collider.h"
#include "Protocol.h"

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
		Client *c = new Client(accept(m_ServerFD, (sockaddr*)&client, &clientSize));
		m_Clients.push_back(c);
		std::cout << "Client was accepted successfully" << std::endl;
	}
}

void Server::Run()
{
	m_Running = true;

	int w, h, density;
	std::cout << "Walls in width: ";
	std::cin >> w;
	std::cout << "\nWalls in height: ";
	std::cin >> h;
	std::cout << "\nDensity: ";
	std::cin >> density;

	KeepTheFlag ktf(60, 40, w, h, density);

	std::vector<std::thread*> threads;

	auto threadFunc = [&](Client* client) {
		while (m_Running)
		{
			processClientReceived(ktf, client);
			std::this_thread::sleep_for(std::chrono::microseconds(33));
			processClientSend(ktf, client);
		}
	};

	for (Client* client : m_Clients)
	{
		sendSetupData(ktf, client);
		processClientSend(ktf, client);

		threads.push_back(new std::thread(
			threadFunc, client
		));
	}

	while (m_Running)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(33));
		m_Running = ktf.Update();
	}

	for (auto thread : threads)
	{
		thread->join();
	}

	//Temporary, we don't want to sleep here. But if I don't, the game crashes
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	Shutdown();
	std::cout << "The server got shut down!" << std::endl;
}

void Server::processClientReceived(KeepTheFlag &ktf, Client* client)
{
	std::string playerData = Receive(client->GetSocket());
	Protocol protocol(&playerData);
	ktf.GetPlayer(client->GetSocket())->UpdatePlayerData(protocol);
}

void Server::processClientSend(KeepTheFlag &ktf, Client* client)
{
	SOCKET s = client->GetSocket();
	std::string clientData = ktf.GetGameState((int)s);
	Send(s, clientData);
}

void Server::sendSetupData(KeepTheFlag &ktf, Client* client)
{
	SOCKET s = client->GetSocket();
	std::string initialDataTransfer = ktf.GetGameMap();
	Send(s, initialDataTransfer);
}

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
