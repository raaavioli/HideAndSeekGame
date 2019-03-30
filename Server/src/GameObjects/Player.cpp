#include "Player.h"

#include <iostream>
#include "OBJLoader.h"

Player::Player()
	: Entity(OBJLoader::GetAABB("monkey", true, true)), m_Score(0), m_Speed(0.1)
{
	float charScale = 3;
	DoScale(charScale);
	float depth = charScale * ((AABB*)m_ColliderBox)->GetColliderMax().z;
	SetPosition(glm::vec3(-27.5, -18.5, depth));
	SetId(-1);
}

Player::Player(int id, float xPos, float yPos, float scale)
	: Entity(OBJLoader::GetAABB("monkey", true, true)), m_Score(0), m_Speed(0.1)
{
	DoScale(scale);
	float depth = scale * ((AABB*)m_ColliderBox)->GetColliderMax().z;
	SetPosition(glm::vec3(xPos, yPos, scale));
	SetId(id);
}


Player::~Player()
{
}

void Player::UpdatePlayerData(std::string & pData)
{
	Protocol protocol(&pData);
	ObjectType ot = protocol.GetObjectType();
	Attribute at = protocol.GetAttribute();
	if (ot != ObjectType::PLAYER || at != Attribute::NUMATTRIBS)
		return;

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
		ot = protocol.GetObjectType();
		at = protocol.GetAttribute();
		if (ot != ObjectType::PLAYER)
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
}

const std::string &Player::ToProtocolString()
{
	static ObjectType ot = ObjectType::PLAYER;
	Numattribs n{ 3 };
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
	/*
	Send a message of the items a player is carrying.
	My thought is to make items map to a binary string, where
	i.e 000101 means a player is currently carrying item 0 and 2. Etc.
	*/

	return m_ProtocolString;
};
