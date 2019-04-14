#pragma once
#include <set>
#include <vector>

#include "Entity.h"
#include "Item.h"

static const float m_NormalSpeed = 0.015f;
static const float m_PlayerWeight = 75.0f;

class Player : public Entity
{
public:
	Player();
	Player(int id, double xPos, double yPos, float scale);
	~Player();

	void UpdatePlayerData(Protocol &protocol);
	const std::string ToProtocolString(InstructionType it) override;

	std::set<Item*>::iterator DropItem(Item* f);
	void PushItem(Item* f);
	bool HasItem(Item* f);
	void Move() override;
	void SetFlying();
	void UpdateItems(int timemillis);
	void Hit(Player* player);
	inline void IncrementFlagTime() { m_FlagTime++; }
	inline void IncrementHitsTaken() { m_HitsTaken++; }
	inline int GetScore() { return m_FlagTime + 5 * m_HitsGiven - 3 * m_HitsTaken + 10 * m_TakeDowns; };
	inline void SetAction(InstructionType it) { m_Action = it; }
	inline InstructionType GetAction() { return m_Action; }
	inline int GetFlagTime() { return m_FlagTime; }
	inline int GetHitsTaken() { return m_HitsTaken; }
	inline int GetHitsGiven() { return m_HitsGiven; }
	inline int GetTakeDowns() { return m_TakeDowns; }
	inline std::set<Player*> &GetUniquePlayersHit() { return m_PlayersHit; }

private:
	int m_FlagTime;
	int m_HitsGiven;
	int m_HitsTaken;
	int m_TakeDowns;
	float m_Speed;
	InstructionType m_Action;
	std::set<Item*> m_Items;
	std::set<Player*> m_PlayersHit;

	void ParsePlayerAttribs(Protocol &protocol);
	void ParsePlayerAction(Protocol &protocol);
};

