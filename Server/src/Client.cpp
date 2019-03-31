#include "Client.h"

#include "GameObjects/Collision/Collider.h"

Client::Client(SOCKET s, int team, float playerX, float playerY)
	: m_Socket(s), m_Player(new Player(s, team, playerX, playerY, 2))
{
	Collider::Add(m_Player, MovementType::DYNAMIC);
}

Client::~Client()
{
	delete m_Player;
}
