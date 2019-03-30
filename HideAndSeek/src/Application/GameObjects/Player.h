#pragma once

#include "Engine/Objects/Entity.h"
#include "Engine/Objects/OBJLoader.h"
#include "Engine/Objects/Camera.h"

#define UNDEFINED -1

class Player : public Engine::Entity
{
public:
	Player();
	~Player();

	void ChangeVelocity(unsigned int dir);
	const std::string &BuildProtocolString();

private:
	int m_Score;
	std::string m_ProtocolString;
};

