#include "BoundingBox.h"

#include <map>

BoundingBox::~BoundingBox()
{
}

bool BoundingBox::CollidesWith(BoundingBox &other)
{
	if (other.GetColliderType() == ColliderType::tAABB && this->GetColliderType() == ColliderType::tAABB) {
		AABB &b1 = (AABB&)*this;
		AABB &b2 = (AABB&)other;

		bool colX = (b1.GetColliderMin().x < b2.GetColliderMax().x && b1.GetColliderMax().x > b2.GetColliderMin().x);
		bool colY = (b1.GetColliderMin().y < b2.GetColliderMax().y && b1.GetColliderMax().y > b2.GetColliderMin().y);
		bool colZ = (b1.GetColliderMin().z < b2.GetColliderMax().z && b1.GetColliderMax().z > b2.GetColliderMin().z);

		return colX && colY && colZ;
	}
	return false;
}

//--------------------------------------------------------//
//  AABB                                                  //
//--------------------------------------------------------//

AABB::~AABB()
{
}


void AABB::Update(glm::mat4 & matrix)
{
	m_ColliderMax = glm::vec3(matrix * glm::vec4(m_ModelMax, 1.0));
	m_ColliderMin = glm::vec3(matrix * glm::vec4(m_ModelMin, 1.0));
}

