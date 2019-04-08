#pragma once
#include <set>
#include <vector>

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
	void Hit(Player* player);
	inline bool IsFlying() { return m_IsFlying;  }
	inline void IncrementFlagTime() { m_FlagTime++; }
	inline void IncrementHitsTaken() { m_HitsTaken++; }
	inline int GetScore() { return m_FlagTime + 5 * m_HitsGiven - 3 * m_HitsTaken + 10 * m_TakeDowns; };
	inline void SetAction(InstructionType it) { m_Action = it; }
	inline InstructionType GetAction() { return m_Action; }
	inline int GetFlagTime() { return m_FlagTime; }
	inline int GetHitsTaken() { return m_HitsTaken; }
	inline int GetHitsGiven() { return m_HitsGiven; }
	inline int GetTakeDowns() { return m_TakeDowns; }
	inline std::vector<Player*> &GetPlayersHit() { return m_PlayersHit; }

private:
	int m_FlagTime;
	int m_HitsGiven;
	int m_HitsTaken;
	int m_TakeDowns;
	float m_Speed;
	bool m_IsFlying;
	InstructionType m_Action;
	std::set<Flag*> m_Items;
	std::vector<Player*> m_PlayersHit;

	void ParsePlayerAttribs(Protocol &protocol);
	void ParsePlayerAction(Protocol &protocol);
};

