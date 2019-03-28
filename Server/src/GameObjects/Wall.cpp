#include "Wall.h"

#include "OBJLoader.h"

Wall::Wall(GroundPlane &gp, const int x, const int y, const glm::vec3 scale)
	: Entity(OBJLoader::GetAABB("unitcube", false, false))
{
	DoScale(scale);

	float xPos = -gp.GetWidth() / 2 + x + scale.x / 2;
	float yPos = gp.GetHeight() / 2 - y - scale.y / 2;
	float zPos = scale.z / 2;

	SetPosition(glm::vec3(xPos, yPos, zPos));
}

Wall::~Wall()
{
}

const std::string &Wall::ToProtocolString()
{
	static ObjectType ot = ObjectType::WALL;
	static Attribute scale = Attribute::SCALE;
	static Attribute pos = Attribute::POSITION;
	glm::vec3 &entity_scale = GetScale();
	Scale s{ entity_scale.x, entity_scale.y, entity_scale.z };
	glm::vec3 &entity_pos = GetPosition();
	Position p{ entity_pos.x, entity_pos.y, entity_pos.z };

	m_ProtocolString.reserve(sizeof(Position) + sizeof(Scale) + 2 * sizeof(int));
	m_ProtocolString.append(Protocol::Stringify(ot, pos, &p));
	m_ProtocolString.append(Protocol::Stringify(ot, scale, &s));

	return m_ProtocolString;
};

