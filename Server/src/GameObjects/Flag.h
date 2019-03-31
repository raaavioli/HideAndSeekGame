#pragma once

#include "Entity.h"
#include "GroundPlane.h"
#include "OBJLoader.h"

class Flag : public Entity 
{
public:
	Flag(GroundPlane &gp, char team);
	~Flag();

	const std::string &ToProtocolString() override;

private:
	char m_Team;
};