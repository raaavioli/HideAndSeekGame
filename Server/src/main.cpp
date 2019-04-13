#include <iostream>

#include "GameObjects/OBJLoader.h"
#include "GameObjects/Collision/Collider.h"
#include "GameObjects/ItemSpawner.h"
#include "Server.h"

void main() 
{
	OBJLoader::Init();
	Collider::Init();
	ItemSpawner::Init();
	int clients;
	std::cout << "Enter number of participating clients: ";
	std::cin >> clients;
	Server server = Server("127.0.0.1", 8080, clients);
	
	server.Run();

	ItemSpawner::Shutdown();
	Collider::Shutdown();
	OBJLoader::Shutdown();
}