#include "BoundingBox.h"

#include <map>

#include "Engine/Log.h"

namespace Engine {


	Engine::BoundingBox::~BoundingBox()
	{
	}


	bool BoundingBox::CollidesWith(BoundingBox &other)
	{
		if (other.GetColliderType() == ColliderType::tAABB && this->GetColliderType() == ColliderType::tAABB) {
			AABB &b1 = (AABB&)*this;
			AABB &b2 = (AABB&)other;

			bool colX = (b1.GetMinPos().x < b2.GetMaxPos().x && b1.GetMaxPos().x > b2.GetMinPos().x);
			bool colY = (b1.GetMinPos().y < b2.GetMaxPos().y && b1.GetMaxPos().y > b2.GetMinPos().y);
			bool colZ = (b1.GetMinPos().z < b2.GetMaxPos().z && b1.GetMaxPos().z > b2.GetMinPos().z);

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

	glm::vec3 AABB::GetModelCenter()
	{
		return (m_MaxPosition + m_MinPosition) / glm::vec3(2.0);
	}

	void AABB::Update(glm::mat4 & matrix)
	{
		m_MaxPosition = glm::vec3(matrix * glm::vec4(m_Model->GetMaxPos(), 1.0));
		m_MinPosition = glm::vec3(matrix * glm::vec4(m_Model->GetMinPos(), 1.0));
	}

	void AABB::Init(const glm::vec3 min, const glm::vec3 max)
	{
		const float unit_cube[24] = {
			min.x, min.y, min.z,
			min.x, min.y, max.z,
			min.x, max.y, min.z,
			min.x, max.y, max.z,
			max.x, min.y, min.z,
			max.x, min.y, max.z,
			max.x, max.y, min.z,
			max.x, max.y, max.z,
		};

		const unsigned int index_array[] = {
			1,0,2, // x negative
			2,3,1,
			1,0,4, // y negative
			4,5,1,
			4,0,2, // z negative
			2,6,4,
			5,4,6, // x positive
			6,7,5,
			3,2,6, // y positive
			6,7,3,
			5,1,3, // z positive
			3,7,5,
		};

		std::vector<float> cube_vector;
		for (int i = 0; i < 24; i++) {
			cube_vector.push_back(unit_cube[i]);
		}

		std::vector<unsigned int> index_vector;
		for (int i = 0; i < 36; i++) {
			index_vector.push_back(index_array[i]);
		}

		VAO vao = VAO();

		std::map<VertexAttrib, VBO*> vbos;
		vbos[VertexAttrib::VERTEX] = new VBO(&cube_vector, 3, VertexAttrib::VERTEX);
		vbos[VertexAttrib::INDEX] = new VBO(&index_vector, VertexAttrib::INDEX);

		m_Model = new Model("manually_created_cube",vao, vbos, min, max);
	}

}