#pragma once

#include "Entity.h"
#include "GroundPlane.h"
#include "OBJLoader.h"

class Flag : public Entity 
{
public:
	enum ItemStatus
	{
		OWNED = 1,
		ACTIVE = 2,
		USABLE = 4,
	};

	Flag(GroundPlane &gp, char team, char status);
	~Flag();

	inline bool isOwned() { return m_Status & OWNED; }
	inline bool isActive() { return m_Status & ACTIVE; }
	inline bool isUsable() { return m_Status & USABLE; }
	inline void SetStatus(ItemStatus is) { m_Status |= is; }
	inline void RemoveStatus(ItemStatus is) { m_Status &= ~is; }

private:
	char m_Status;
	char m_Team;
};