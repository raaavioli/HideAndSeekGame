#pragma once

#include "Engine/Objects/Entity.h"

class GroundPlane : public Engine::Entity
{
public:
	GroundPlane(const float width, const float height);
	~GroundPlane();

	inline const float GetWidth() { return m_Width; }
	inline const float GetHeight() { return m_Height; }

private:
	float m_Height;
	float m_Width;
	float m_Thickness;
};

