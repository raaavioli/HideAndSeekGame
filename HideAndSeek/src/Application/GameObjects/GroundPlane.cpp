#include "GroundPlane.h"

#include "Engine/Objects/OBJLoader.h"

GroundPlane::GroundPlane(const float width, const float height)
	: Entity(Engine::OBJLoader::GetModel("unitcube", false, false))
{
	float thickness = 0.1;
	DoScale(glm::vec3(width, height, thickness));
	SetPosition(glm::vec3(0, 0, -thickness));

}

GroundPlane::GroundPlane()
	: Entity(Engine::OBJLoader::GetModel("unitcube", false, false))
{
	v_Color = glm::vec3(0, 0.35, 0);
}

GroundPlane::~GroundPlane()
{
}
