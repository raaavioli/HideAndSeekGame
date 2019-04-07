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

const std::string & Flag::ToProtocolString()
{
	static InstructionType ot = InstructionType::ITEM;
	pChar n{ 3 };
	int entity_id = GetId();
	pInt i{ entity_id };
	glm::vec3 &entity_scale = GetScale();
	pVector3 s{ entity_scale.x, entity_scale.y, entity_scale.z };
	glm::vec3 &entity_pos = GetPosition();
	pVector3 p{ entity_pos.x, entity_pos.y, entity_pos.z };

	m_ProtocolString.clear();
	m_ProtocolString.reserve(sizeof(pChar) + 2 * sizeof(pVector3) + 3 * sizeof(int));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::NUMATTRIBS, &n));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::ID, &i));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::POSITION, &p));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::SCALE, &s));
	/*
	Send a message of the items a player is carrying.
	My thought is to make items map to a binary string, where
	i.e 000101 means a player is currently carrying item 0 and 2. Etc.
	*/

	return m_ProtocolString;
}
