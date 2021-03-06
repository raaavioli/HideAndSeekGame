#include "Player.h"

Player::Player()
	: Entity(Engine::OBJLoader::GetModel("character", true, true)), m_Action(InstructionType::OBJERROR)
{
	v_Color = glm::vec3(0.1, 0.1, 0.1);
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
	pChar n{ 2 };
	int entity_id = GetId();
	pInt i{ entity_id };
	glm::vec3 &entity_velocity = GetVelocity();
	pVector3 v{ entity_velocity.x, entity_velocity.y, entity_velocity.z };

	m_ProtocolString.clear();
	m_ProtocolString.reserve(sizeof(pChar) + 2 * sizeof(pVector3) + sizeof(int));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::NUMATTRIBS,	&n));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::ID,			&i));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::DIRECTION, &v));

	return m_ProtocolString;
};

/*******
* An action string consists of an a
********/
const std::string &Player::BuildActionString()
{
	if (m_Action == PICKUP || m_Action == DROP || m_Action == ATTACK)
	{
		pChar n{ 1 };
		int entity_id = GetId();
		pInt i{ entity_id };

		m_ProtocolString.clear();
		m_ProtocolString.reserve(sizeof(pChar) + sizeof(int));
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
