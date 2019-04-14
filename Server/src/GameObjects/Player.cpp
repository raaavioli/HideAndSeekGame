#include "Player.h"

#include <iostream>
#include "OBJLoader.h"
#include "ItemSpawner.h"

Player::Player(int id, double xPos, double yPos, float scale)
	: Entity("character", true, true)
{
	DoScale(scale);
	SetId(id);
	v_Transition = glm::vec3(xPos, yPos, 30);
	v_Velocity = glm::vec3(0);
	v_Color = glm::vec3(0.1, 0.1, 0.1);

	m_FlagTime = 0;
	m_HitsGiven = 0;
	m_HitsTaken = 0;
	m_TakeDowns = 0;
	m_Speed = m_NormalSpeed;
}


Player::~Player()
{
}

void Player::UpdatePlayerData(Protocol &protocol)
{
	InstructionType ot = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();

	if (ot == PLAYER)
	{
		ParsePlayerAttribs(protocol);
	}
	else if (ot == PICKUP || ot == ATTACK || ot == DROP)
	{
		ParsePlayerAction(protocol);
	}
}

void Player::ParsePlayerAttribs(Protocol &protocol)
{
	InstructionType ot = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();
	pChar na;
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
			pInt i;
			protocol.GetData(&i);
			player_id = i.Value;
			if (GetId() != player_id && player_id > 0)
			{
				std::cout << "Player " << GetId() << " tries to move Player " << player_id << std::endl;
				return;
			}
		}
		if (at == Attribute::DIRECTION)
		{
			pVector3 v;
			protocol.GetData(&v);
			velocity = glm::vec3(v.X * m_Speed, v.Y * m_Speed, velocity.z);
		}
		//Ignore any other attributes
	}
	if (player_id > 0)
	{
		SetVelocity(velocity);
	}
	protocol.Next();
	UpdatePlayerData(protocol);
}

void Player::ParsePlayerAction(Protocol & protocol)
{
	int player_id = -1;
	if (!protocol.Next()) return;

	InstructionType ot = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();
	if (ot != InstructionType::PICKUP &&  ot != InstructionType::ATTACK && ot != InstructionType::DROP)
		return;

	if (at == Attribute::ID)
	{
		pInt i;
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
		if (ot == DROP)
		{
			for (auto item : m_Items)
			{
				if (item->isDroppable())
				{
					DropItem(item);
					break;
				}
				else continue;
			}
			m_Action = InstructionType::OBJERROR;
		}
		else if (ot == PICKUP || ot == ATTACK)
		{
 			m_Action = ot;
		}
	}
	protocol.Next();
	UpdatePlayerData(protocol);
}


const std::string Player::ToProtocolString(InstructionType it)
{
	std::string string = Entity::ToProtocolString(it);

	for (auto item : m_Items)
	{
		string.append(item->ToProtocolString(ITEM));
	}
	return string;
}

void Player::Move()
{
	v_Transition += v_Velocity; 
	if (v_Transition.z < -50)
	{
		m_HitsTaken += 5;
		ItemSpawner::RandomRespawn(this);
		v_Transition.z = 10;
	}
	for (auto item : m_Items)
	{
		item->GetPosition().x = v_Transition.x;
		item->GetPosition().y = v_Transition.y;
		item->Update();
	}
}

void Player::SetFlying()
{
	v_Velocity.z = 0.20;
	for (auto it = m_Items.begin(); it != m_Items.end(); )
	{
		it = DropItem(*it);
	}
}

void Player::UpdateItems(int timemillis)
{
	for (auto it = m_Items.begin(); it != m_Items.end(); )
	{
		if ((*it)->isUsable())
		{
			(*it)->IncrementSecondsCarried();
			if ((*it)->GetTimeCarried() > 15)
			{
				Item *item = *it;
				ItemSpawner::RandomRespawn(item);
				it = DropItem(item);
				continue;
			}
		}
		it++;
	}
}

void Player::Hit(Player * player)
{
	m_HitsGiven++;
	player->IncrementHitsTaken();
	player->SetFlying();
	m_PlayersHit.insert(player);

}

std::set<Item*>::iterator Player::DropItem(Item * item)
{
	if (m_Items.size() > 0) 
	{
		for (auto it = m_Items.begin(); it != m_Items.end(); it++) 
		{
			if ((*it) == item)
			{
				item->RemoveStatus(Item::OWNED);
				//Reset Item to ground position;
				item->ResetSecondsCarried();
				item->GetPosition().z = item->GetHeight() / 2;
				item->Update();
				m_Speed -= (-item->GetWeight() / m_PlayerWeight) * m_NormalSpeed;
				return m_Items.erase(it);
			}
		}
	}
	return m_Items.end();
}

void Player::PushItem(Item *item)
{
	if (!item->isOwned())
	{
		m_Items.insert(item);
		item->SetStatus(Item::OWNED);
		//Raise Item from floor position
		if (!item->isUsable())
			item->GetPosition() += glm::vec3(0, 0, 1.5);
		else item->GetPosition().z = 4;
		item->Update();
		//Random way for calculating speed based upon player-weight and normal speed
		m_Speed += (-item->GetWeight() / m_PlayerWeight) * m_NormalSpeed;
	}
}

bool Player::HasItem(Item * f)
{
	return m_Items.find(f) != m_Items.end();
};
