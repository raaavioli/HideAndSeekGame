#include "GroundPlane.h"

#include "OBJLoader.h"

GroundPlane::GroundPlane(const float width, const float height)
	: m_Width(width), m_Height(height),
	Entity("unitcube", false, false)
{
	m_Thickness = 10;
	
	v_Scale = glm::vec3(width, height, m_Thickness);
	v_Transition = glm::vec3(0, 0, -m_Thickness/2);
	v_Color = glm::vec3(0, 0.35, 0);
}

GroundPlane::~GroundPlane()
{
}
