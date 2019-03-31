#include "GroundPlane.h"

#include "OBJLoader.h"

GroundPlane::GroundPlane(const float width, const float height)
	: m_Width(width), m_Height(height),
	Entity(OBJLoader::GetAABB("unitcube", false, false))
{
	m_Thickness = 0.1;
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
	
	DoScale(glm::vec3(width, height, m_Thickness));
	SetPosition(glm::vec3(0, 0, -m_Thickness));

}

GroundPlane::~GroundPlane()
{
}

const std::string &GroundPlane::ToProtocolString()
{
	static InstructionType ot = InstructionType::PLANE;
	static Attribute attrcount = Attribute::NUMATTRIBS;
	static Attribute scale = Attribute::SCALE;
	static Attribute pos = Attribute::POSITION;
	Numattribs n{ 2 };
	glm::vec3 &entity_scale = GetScale();
	Scale s{ entity_scale.x, entity_scale.y, entity_scale.z };
	glm::vec3 &entity_pos = GetPosition();
	Position p{ entity_pos.x, entity_pos.y, entity_pos.z };

	m_ProtocolString.clear();
	m_ProtocolString.reserve(sizeof(Numattribs) + sizeof(Position) + sizeof(Scale) + 3 * sizeof(int));
	m_ProtocolString.append(Protocol::Stringify(ot, attrcount, &n));
	m_ProtocolString.append(Protocol::Stringify(ot, pos, &p));
	m_ProtocolString.append(Protocol::Stringify(ot, scale, &s));

	return m_ProtocolString;
};
