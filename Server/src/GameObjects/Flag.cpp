#include "Flag.h"

Flag::Flag(GroundPlane &gp, char team, char status)
	: Entity("flag", false, false), m_Status(status)
{
	if (team == 1) 
	{
		SetPosition(glm::vec3(- gp.GetWidth() / 2 + 2, -gp.GetHeight() / 2 + 1, 0.1));
	}
	else 
	{
		SetPosition(glm::vec3(gp.GetWidth() / 2 - 1, gp.GetHeight() / 2 - 2, 0.1));
	}
	DoScale(glm::vec3(3, 3, 3));
	SetId(team);
}

Flag::~Flag()
{
}
