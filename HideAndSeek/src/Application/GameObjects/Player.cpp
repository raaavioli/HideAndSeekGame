#include "Player.h"

Player::Player()
	: Entity(Engine::OBJLoader::GetModel("character", true, true)), m_Action(InstructionType::OBJERROR)
{
	float charScale = 3;
	DoScale(charScale);
	float depth = charScale * m_Model->GetMaxPos().z;
	SetPosition(glm::vec3(-27.5, -18.5, depth));
	SetId(UNDEFINED);
}


Player::~Player()
{
}

void Player::ChangeVelocity(unsigned int directions)
{
	using Direction = Engine::Direction;
	static auto inDir = [&](Direction d) { return directions & d; };

	glm::vec3 direction(0.0);
	if (inDir(Direction::FORWARD))
		direction.y += 1;
	if (inDir(Direction::BACKWARD))
		direction.y -= 1;
	if (inDir(Direction::RIGHT))
		direction.x += 1;
	if (inDir(Direction::LEFT))
		direction.x -= 1;

	SetVelocity(direction);
}

const std::string &Player::BuildProtocolString()
{
	static InstructionType ot = InstructionType::PLAYER;
	Numattribs n{ 4 };
	int entity_id = GetId();
	Id i{ entity_id };
	glm::vec3 &entity_scale = GetScale();
	Scale s{ entity_scale.x, entity_scale.y, entity_scale.z };
	glm::vec3 &entity_pos = GetPosition();
	Position p{ entity_pos.x, entity_pos.y, entity_pos.z };
	glm::vec3 &entity_velocity = GetVelocity();
	Position v{ entity_velocity.x, entity_velocity.y, entity_velocity.z };

	m_ProtocolString.clear();
	m_ProtocolString.reserve(sizeof(Numattribs) + sizeof(Position) + sizeof(Scale) + 3 * sizeof(int));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::NUMATTRIBS,	&n));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::ID,			&i));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::POSITION,	&p));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::SCALE,		&s));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::DIRECTION, &v));
	/*
	Send a message of the items a player is carrying.
	My thought is to make items map to a binary string, where
	i.e 000101 means a player is currently carrying item 0 and 2. Etc.
	*/

	return m_ProtocolString;
};

const std::string &Player::BuildActionString()
{
	if (m_Action == PICKUP || m_Action == DROP)
	{
		Numattribs n{ 1 };
		int entity_id = GetId();
		Id i{ entity_id };

		m_ProtocolString.clear();
		m_ProtocolString.reserve(sizeof(Numattribs) + sizeof(int));
		m_ProtocolString.append(Protocol::Stringify(m_Action, Attribute::NUMATTRIBS, &n));
		m_ProtocolString.append(Protocol::Stringify(m_Action, Attribute::ID, &i));

		return m_ProtocolString;
	}
	else
	{
		m_ProtocolString = "";
		return m_ProtocolString;
	}
}
