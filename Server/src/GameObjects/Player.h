#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	Player(int id, float xPos, float yPos, float scale);
	~Player();

	void UpdatePlayerData(std::string &pData);
	const std::string &ToProtocolString() override;

private:
	int m_Score;
	float m_Speed;
};

