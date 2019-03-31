#pragma once

#include "Engine/Objects/Entity.h"
#include "Engine/Objects/OBJLoader.h"
#include "GroundPlane.h"

class Wall : public Engine::Entity
{
public:
	Wall();
	Wall(GroundPlane &gp, const int x, const int y, const glm::vec3 scale);
	~Wall();

private:
};

