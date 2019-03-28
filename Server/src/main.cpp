#include <iostream>

#include "GameObjects/OBJLoader.h"
#include "GameObjects/Collision/Collider.h"
#include "Server.h"

void main() 
{
	OBJLoader::Init();
	Collider::Init();
	Server server = Server("127.0.0.1", 8080, 2);
	
	server.Run();

	Collider::Shutdown();
	OBJLoader::Shutdown();
}