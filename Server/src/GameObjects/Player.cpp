#include "Player.h"

#include "OBJLoader.h"

Player::Player()
	: Entity(OBJLoader::GetAABB("monkey", true, true))
{
	float charScale = 3;
	DoScale(charScale);
	float depth = charScale * ((AABB*)m_ColliderBox)->GetColliderMax().z;
	SetPosition(glm::vec3(-27.5, -18.5, depth));
}


Player::~Player()
{
}

void Player::Move(unsigned int directions, float speed)
{
	static auto inDir = [&](Player::Direction d) { return directions & d; };

	glm::vec3 direction(0.0);
	if (inDir(Player::Direction::FORWARD))
		direction.y += 1;
	if (inDir(Player::Direction::BACKWARD))
		direction.y -= 1;
	if (inDir(Player::Direction::RIGHT))
		direction.x += 1;
	if (inDir(Player::Direction::LEFT))
		direction.x -= 1;

	SetVelocity(speed * direction);
}

const std::string &Player::ToProtocolString()
{
	static ObjectType ot = ObjectType::PLAYER;
	static Attribute attrcount = Attribute::NUMATTRIBS;
	static Attribute scale = Attribute::SCALE;
	static Attribute pos = Attribute::POSITION;
	Numattribs n{ 2 }; // 3 when items are in the game
	glm::vec3 &entity_scale = GetScale();
	Scale s{ entity_scale.x, entity_scale.y, entity_scale.z };
	glm::vec3 &entity_pos = GetPosition();
	Position p{ entity_pos.x, entity_pos.y, entity_pos.z };

	m_ProtocolString.clear();
	m_ProtocolString.reserve(sizeof(Numattribs) + sizeof(Position) + sizeof(Scale) + 3 * sizeof(int));
	m_ProtocolString.append(Protocol::Stringify(ot, attrcount, &n));
	m_ProtocolString.append(Protocol::Stringify(ot, pos, &p));
	m_ProtocolString.append(Protocol::Stringify(ot, scale, &s));
	/*
	Send a message of the items a player is carrying.
	My thought is to make items map to a binary string, where
	i.e 000101 means a player is currently carrying item 0 and 2. Etc.
	*/

	return m_ProtocolString;
};
