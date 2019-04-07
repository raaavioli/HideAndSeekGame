#include "Client.h"

#include "GameObjects/Collision/Collider.h"

Client::Client(SOCKET s)
	: m_Socket(s)
{}

Client::~Client()
{}
