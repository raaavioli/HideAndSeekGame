#include <iostream>

#include "Server.h"

void main() 
{
	Server server = Server("127.0.0.1", 8080);
	
	server.Run();
}