#include "Wall.h"


Wall::Wall(GroundPlane &gp, const int x, const int y, const glm::vec3 scale)
	: Entity(Engine::OBJLoader::GetModel("unitcube", false, false)),
	m_Scale(scale), xStart(x), yStart(y)
{
	DoScale(scale);

	float xPos = -gp.GetWidth() / 2 + x + scale.x / 2;
	float yPos = gp.GetHeight() / 2 - y - scale.y / 2;
	float zPos = scale.z / 2;

	SetPosition(glm::vec3(xPos, yPos, zPos));
}


Wall::Wall()
	: Entity(Engine::OBJLoader::GetModel("unitcube", false, false)), 
	xStart(0), yStart(0), m_Scale(glm::vec3(0))
{};

Wall::~Wall()
{
}

