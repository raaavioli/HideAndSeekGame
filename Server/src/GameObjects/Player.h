#pragma once
#include <set>

#include "Entity.h"
#include "Flag.h"

class Player : public Entity
{
public:
	Player();
	Player(int id, int team, float xPos, float yPos, float scale);
	~Player();

	void UpdatePlayerData(Protocol &protocol);
	const std::string &ToProtocolString() override;

	void Move() override;
	InstructionType GetAction() { return m_Action; }
	int GetTeam() { return m_Team; }
	void DropItem(Flag* f);
	inline void PushItem(Flag* f) { m_Items.insert(f); }
	inline void SetAction(InstructionType it) { m_Action = it; }

private:
	int m_Score;
	int m_Team;
	float m_Speed;
	InstructionType m_Action;
	std::set<Flag*> m_Items;

	void ParsePlayerAttribs(Protocol &protocol);
	void ParsePlayerPickup(Protocol &protocol);
	void ParsePlayerDrop(Protocol &protocol);
};

