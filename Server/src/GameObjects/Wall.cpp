#include "Wall.h"

#include "OBJLoader.h"

Wall::Wall(GroundPlane &gp, const double x, const double y, const glm::vec3 scale)
	: Entity("unitcube", false, false)
{
	DoScale(scale);

	double xPos = -gp.GetWidth() / 2 + x + scale.x / 2;
	double yPos = gp.GetHeight() / 2 - y - scale.y / 2;
	double zPos = scale.z / 2;

	v_Transition = glm::vec3(xPos, yPos, zPos);
	v_Color = glm::vec3(0.25, 0, 0);
}

Wall::~Wall()
{
}


