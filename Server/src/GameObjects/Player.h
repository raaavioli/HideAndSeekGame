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

	void DropItem(Flag* f);
	void PushItem(Flag* f);
	void Move() override;
	inline int GetTeam() { return m_Team; }
	inline int GetScore() { return m_Score; }
	inline void AddScore(int score) { m_Score += score; }
	inline void SetAction(InstructionType it) { m_Action = it; }
	inline InstructionType GetAction() { return m_Action; }

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

