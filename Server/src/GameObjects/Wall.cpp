#include "Wall.h"

#include "OBJLoader.h"

Wall::Wall(GroundPlane &gp, const double x, const double y, const glm::vec3 scale)
	: Entity("unitcube", false, false)
{
	DoScale(scale);

	double xPos = -gp.GetWidth() / 2 + x + scale.x / 2;
	double yPos = gp.GetHeight() / 2 - y - scale.y / 2;
	double zPos = scale.z / 2;

	SetPosition(glm::vec3(xPos, yPos, zPos));
}

Wall::~Wall()
{
}

const std::string &Wall::ToProtocolString()
{
	static InstructionType ot = InstructionType::WALL;
	static Attribute attrcount = Attribute::NUMATTRIBS;
	static Attribute scale = Attribute::SCALE;
	static Attribute pos = Attribute::POSITION;
	pChar n{ 2 };
	glm::vec3 &entity_scale = GetScale();
	pVector3 s{ entity_scale.x, entity_scale.y, entity_scale.z };
	glm::vec3 &entity_pos = GetPosition();
	pVector3 p{ entity_pos.x, entity_pos.y, entity_pos.z };

	m_ProtocolString.clear();
	m_ProtocolString.reserve(sizeof(pChar) + 2 * sizeof(pVector3) + 3 * sizeof(int));
	m_ProtocolString.append(Protocol::Stringify(ot, attrcount, &n));
	m_ProtocolString.append(Protocol::Stringify(ot, pos, &p));
	m_ProtocolString.append(Protocol::Stringify(ot, scale, &s));

	return m_ProtocolString;
};

