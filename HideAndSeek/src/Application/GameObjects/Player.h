#pragma once

#include "Application/ServerHandler/Protocol.h"
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
	const std::string &BuildActionString();

	inline void SetAction(InstructionType i) { m_Action = i; }

private:
	int m_Score;
	std::string m_ProtocolString;
	InstructionType m_Action;
};

