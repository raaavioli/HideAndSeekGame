#include "GroundPlane.h"

#include "Engine/Objects/OBJLoader.h"

GroundPlane::GroundPlane(const float width, const float height)
	: Entity(Engine::OBJLoader::GetModel("unitcube", false, false))
{
	float thickness = 0.1;
	//If we want to divide floor into smaller units
	/*for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Engine::Entity* plane = new Engine::Entity(Engine::OBJLoader::GetModel("unitcube", false, false));
			plane->Scale(glm::vec3(1, 1, m_Thickness));
			plane->SetPosition(glm::vec3(-width / 2 + x + 0.5, -height / 2 + y + 0.5, -m_Thickness));
			m_Planes.push_back(plane);
		}
	}*/
	
	Scale(glm::vec3(width, height, thickness));
	SetPosition(glm::vec3(0, 0, -thickness));

}

GroundPlane::GroundPlane()
	: Entity(Engine::OBJLoader::GetModel("unitcube", false, false))
{}

GroundPlane::~GroundPlane()
{
}
