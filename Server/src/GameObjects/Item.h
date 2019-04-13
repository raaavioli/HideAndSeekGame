#pragma once

#include "Entity.h"
#include "GroundPlane.h"
#include "OBJLoader.h"

class Item : public Entity 
{
public:
	enum ItemStatus
	{
		OWNED = 1,
		ACTIVE = 2,
		USABLE = 4,
		DROPPABLE = 8,
	};

	Item(const char* model, float weight, char status);
	~Item();

	inline bool isOwned() { return m_Status & OWNED; }
	inline bool isActive() { return m_Status & ACTIVE; }
	inline bool isUsable() { return m_Status & USABLE; }
	inline bool isDroppable() { return m_Status & DROPPABLE; }
	inline float GetWeight() { return m_Weight; }
	inline void SetStatus(ItemStatus is) { m_Status |= is; }
	inline void RemoveStatus(ItemStatus is) { m_Status &= ~is; }
	inline int GetTimeCarried() { return m_SecondsCarried; }
	inline void IncrementSecondsCarried() { m_SecondsCarried++; }
	inline void ResetSecondsCarried() { m_SecondsCarried = 0; }

private:
	float m_Weight;
	char m_Status;
	int m_SecondsCarried;
};