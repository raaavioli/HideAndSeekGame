#include "GroundPlane.h"

#include "Engine/Objects/OBJLoader.h"

GroundPlane::GroundPlane(const float width, const float height)
	: m_Width(width), m_Height(height), 
	Entity(Engine::OBJLoader::GetModel("unitcube", false, false))
{
	m_Thickness = 0.1;
	SetPosition(glm::vec3(0.0, 0.0, -m_Thickness));
	Scale(glm::vec3(width, height, m_Thickness));
}

GroundPlane::~GroundPlane()
{
}
