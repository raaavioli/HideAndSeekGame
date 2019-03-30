#pragma once

#include "Engine/Objects/Entity.h"

class GroundPlane : public Engine::Entity
{
public:
	GroundPlane();
	GroundPlane(const float width, const float height);
	~GroundPlane();

	inline const float GetWidth() { return GetScale().x; }
	inline const float GetHeight() { return GetScale().y; }
	//inline std::vector<Engine::Entity*> &GetPlane() { return m_Planes; }

private:
	//std::vector<Engine::Entity*> m_Planes;
};

