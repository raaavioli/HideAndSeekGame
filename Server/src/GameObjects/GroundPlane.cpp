#include "GroundPlane.h"

#include "OBJLoader.h"

GroundPlane::GroundPlane(const float width, const float height)
	: m_Width(width), m_Height(height),
	Entity("unitcube", false, false)
{
	m_Thickness = 0.1;
	
	DoScale(glm::vec3(width, height, m_Thickness));
	SetPosition(glm::vec3(0, 0, -m_Thickness));

}

GroundPlane::~GroundPlane()
{
}
