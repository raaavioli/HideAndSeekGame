#include "Flag.h"


Flag::Flag(GroundPlane &gp, const int x, const int y, const glm::vec3 scale)
	: Entity(Engine::OBJLoader::GetModel("character", false, false))
{
	DoScale(scale);

	float xPos = -gp.GetWidth() / 2 + x + scale.x / 2;
	float yPos = gp.GetHeight() / 2 - y - scale.y / 2;
	float zPos = scale.z / 2;

	SetPosition(glm::vec3(xPos, yPos, zPos));
}


Flag::Flag()
	: Entity(Engine::OBJLoader::GetModel("flag", false, false))
{
	v_Color = glm::vec3(1, 1, 1);
};

Flag::~Flag()
{
}


