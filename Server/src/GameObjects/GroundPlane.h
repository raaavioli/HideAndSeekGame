#pragma once

#include "Entity.h"

class GroundPlane : public Entity
{
public:
	GroundPlane(const float width, const float height);
	~GroundPlane();

	inline const float GetWidth() { return m_Width; }
	inline const float GetHeight() { return m_Height; }

	const std::string &ToProtocolString() override;
	//inline std::vector<Engine::Entity*> &GetPlane() { return m_Planes; }

private:
	float m_Height;
	float m_Width;
	float m_Thickness;
	//std::vector<Engine::Entity*> m_Planes;
};

