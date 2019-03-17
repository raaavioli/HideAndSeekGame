#pragma once

#include "Engine/Objects/Entity.h"
#include "Engine/Objects/OBJLoader.h"
#include "Engine/Objects/Camera.h"

class Player : public Engine::Entity
{
public:
	Player();
	~Player();

	void Move(unsigned int dir, float speed);

private:
	int score;
};

