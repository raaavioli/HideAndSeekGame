#pragma once

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	~Player();

	void Move(unsigned int dir, float speed);
	const std::string &ToProtocolString() override;

	enum Direction {
		FORWARD = 1,
		BACKWARD = 2,
		LEFT = 4,
		RIGHT = 8,
	};

private:
	int score;
};

