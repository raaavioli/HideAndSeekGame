#pragma once
#include <set>

#include "Entity.h"
#include "Flag.h"

static const double m_NormalSpeed = 0.1f;
static const double m_FlagSpeed = 0.15f;

class Player : public Entity
{
public:
	Player();
	Player(int id, double xPos, double yPos, float scale);
	~Player();

	void UpdatePlayerData(Protocol &protocol);
	const std::string &ToProtocolString() override;

	std::set<Flag*>::iterator DropItem(Flag* f);
	void PushItem(Flag* f);
	bool HasItem(Flag* f);
	void Move() override;
	void SetFlying();
	inline bool IsFlying() { return m_IsFlying;  }
	inline int GetScore() { return m_Score; }
	inline void AddScore(int score) { m_Score += score; }
	inline void SetAction(InstructionType it) { m_Action = it; }
	inline InstructionType GetAction() { return m_Action; }

private:
	int m_Score;
	float m_Speed;
	bool m_IsFlying;
	InstructionType m_Action;
	std::set<Flag*> m_Items;

	void ParsePlayerAttribs(Protocol &protocol);
	void ParsePlayerAction(Protocol &protocol);
};

