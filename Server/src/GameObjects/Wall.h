#pragma once

#include "Entity.h"

#include "GroundPlane.h"

class Wall : public Entity
{
public:
	Wall(GroundPlane &gp, const int x, const int y, const glm::vec3 scale);
	~Wall();

	const std::string &ToProtocolString() override;

private:
};

