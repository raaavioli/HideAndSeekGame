#include "Player.h"

#include <iostream>
#include "OBJLoader.h"

Player::Player()
	: Entity(OBJLoader::GetAABB("monkey", true, true)), m_Score(0), m_Speed(0.1)
{
	float charScale = 1.5;
	DoScale(charScale);
	float depth = charScale * ((AABB*)m_ColliderBox)->GetColliderMax().z;
	SetPosition(glm::vec3(-27.5, -18.5, depth));
	SetId(-1);
}

Player::Player(int id, int team, float xPos, float yPos, float scale)
	: Entity(OBJLoader::GetAABB("monkey", true, true)), m_Score(0), m_Speed(0.25),
	m_Team(team)
{
	DoScale(scale);
	float depth = scale * ((AABB*)m_ColliderBox)->GetColliderMax().z;
	SetPosition(glm::vec3(xPos, yPos, scale));
	SetId(id);
}


Player::~Player()
{
}

void Player::UpdatePlayerData(Protocol &protocol)
{
	InstructionType ot = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();
	if (ot != PICKUP && ot != DROP && ot != PLAYER || at != Attribute::NUMATTRIBS)
		return;

	if (ot == PLAYER)
	{
		ParsePlayerAttribs(protocol);
	}
	else if (ot == PICKUP)
	{
		ParsePlayerPickup(protocol);
	}
	else if (ot == DROP)
	{
		ParsePlayerDrop(protocol);
	}
}

void Player::ParsePlayerAttribs(Protocol &protocol)
{
	InstructionType ot = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();
	Numattribs na;
	protocol.GetData(&na);

	int player_id = -1;
	glm::vec3 velocity = GetVelocity();
	for (int i = 0; i < na.Value; i++)
	{
		if (!protocol.HasNext()) {
			return;
		}
		protocol.Next();
		ot = protocol.GetInstructionType();
		at = protocol.GetAttribute();
		if (ot != InstructionType::PLAYER)
			return;

		if (at == Attribute::ID)
		{
			Id i;
			protocol.GetData(&i);
			player_id = i.Value;
			if (GetId() != player_id)
			{
				std::cout << "Player " << GetId() << " tries to move Player " << player_id << std::endl;
				return;
			}
		}
		if (at == Attribute::DIRECTION)
		{
			Direction v;
			protocol.GetData(&v);
			velocity = glm::vec3(v.X, v.Y, v.Z);
		}
		//Ignore any other attributes
	}
	if (player_id > 0)
	{
		SetVelocity(velocity * m_Speed);
	}
	protocol.Next();
	UpdatePlayerData(protocol);
}

void Player::ParsePlayerPickup(Protocol & protocol)
{
	int player_id = -1;
	if (!protocol.Next()) return;

	InstructionType ot = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();
	if (ot != InstructionType::PICKUP)
		return;

	if (at == Attribute::ID)
	{
		Id i;
		protocol.GetData(&i);
		player_id = i.Value;
		if (GetId() != player_id)
		{
			std::cout << "Player " << GetId() << " tries to pickup for Player " << player_id << std::endl;
			return;
		}
	}
	
	if (player_id > 0)
	{
		m_Action = PICKUP;
	}
	protocol.Next();
	UpdatePlayerData(protocol);
}

void Player::ParsePlayerDrop(Protocol & protocol)
{
	int player_id = -1;
	if (!protocol.Next()) return;

	InstructionType ot = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();
	if (ot != InstructionType::DROP)
		return;

	if (at == Attribute::ID)
	{
		Id i;
		protocol.GetData(&i);
		player_id = i.Value;
		if (GetId() != player_id)
		{
			std::cout << "Player " << GetId() << " tries to pickup for Player " << player_id << std::endl;
			return;
		}
	}

	if (player_id > 0)
	{
		//Drop the latest picked up item. Should probably be changed later to
		//Drop what the player wants.
		for (auto item : m_Items) {
			m_Items.erase(item);
			break;
		}
	}
	protocol.Next();
	UpdatePlayerData(protocol);
}

const std::string &Player::ToProtocolString()
{
	static InstructionType ot = InstructionType::PLAYER;
	Numattribs n{ 3 + m_Items.size() };
	int entity_id = GetId();
	Id i{ entity_id };
	glm::vec3 &entity_scale = GetScale();
	Scale s{ entity_scale.x, entity_scale.y, entity_scale.z };
	glm::vec3 &entity_pos = GetPosition();
	Position p{ entity_pos.x, entity_pos.y, entity_pos.z };

	m_ProtocolString.clear();
	m_ProtocolString.reserve(sizeof(Numattribs) + sizeof(Position) + sizeof(Scale) + 3 * sizeof(int));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::NUMATTRIBS, &n));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::ID, &i));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::POSITION, &p));
	m_ProtocolString.append(Protocol::Stringify(ot, Attribute::SCALE, &s));

	for (auto item : m_Items)
	{
		Id itemId{ item->GetId() };
		m_ProtocolString.append(Protocol::Stringify(ITEM, ID, &itemId));
	}

	return m_ProtocolString;
}
void Player::DropItem(Flag * f)
{
	for (auto it = m_Items.begin(); it != m_Items.end(); it++) {
		if((*it) == f)
		{
			m_Items.erase(it);
		}
	}
};
