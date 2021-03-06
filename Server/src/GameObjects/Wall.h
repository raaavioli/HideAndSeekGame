#pragma once

#include "Entity.h"

#include "GroundPlane.h"

class Wall : public Entity
{
public:
	Wall(GroundPlane &gp, const double x, const double y, const glm::vec3 scale);
	~Wall();

private:
};

