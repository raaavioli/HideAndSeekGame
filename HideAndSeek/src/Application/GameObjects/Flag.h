#pragma once

#include "Engine/Objects/Entity.h"
#include "Engine/Objects/OBJLoader.h"
#include "Player.h"
#include "GroundPlane.h"

class Flag : public Engine::Entity
{
public:
	Flag();
	Flag(GroundPlane &gp, const int x, const int y, const glm::vec3 scale);
	~Flag();

private:
};

