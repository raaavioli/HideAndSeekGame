#include "Client.h"

#include "GameObjects/Collision/Collider.h"

Client::Client(SOCKET s, float playerX, float playerY)
	: m_Socket(s), m_Player(new Player(s, playerX, playerY, 3))
{
	Collider::Add(m_Player, MovementType::DYNAMIC);
}

Client::~Client()
{
	delete m_Player;
}
