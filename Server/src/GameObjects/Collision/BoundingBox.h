#pragma once

#include <vector>

#include "glm/glm/glm.hpp"

enum ColliderType {
	tAABB,
	tSPHERE,
	tCOMPLEX,
};

class BoundingBox
{
public:
	~BoundingBox();

	virtual ColliderType GetColliderType() = 0;
	virtual void Update(glm::mat4 &matrix) = 0;

	bool CollidesWith(BoundingBox &other);
protected:
};


class AABB : public BoundingBox
{
public:
	AABB(const glm::vec3 modelMin, const glm::vec3 modelMax)
		: m_ColliderMax(modelMax), m_ColliderMin(modelMin),
		m_ModelMax(modelMax), m_ModelMin(modelMin) {};
	~AABB();

	inline ColliderType GetColliderType() override { return GetStaticColliderType(); }
	inline static ColliderType GetStaticColliderType() { return ColliderType::tAABB; }

	void Update(glm::mat4 &matrix);

	inline glm::vec3 &GetColliderMin() { return m_ColliderMin; }
	inline glm::vec3 &GetColliderMax() { return m_ColliderMax; }

private:
	glm::vec3 m_ColliderMax;
	glm::vec3 m_ColliderMin;
	glm::vec3 m_ModelMin;
	glm::vec3 m_ModelMax;
};



