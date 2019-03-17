#pragma once

#include <glm/glm.hpp>

#include "Engine/Core.h"
#include "Engine/Objects/Model.h"

namespace Engine {

	enum ColliderType {
		tAABB,
		tSPHERE,
		tCOMPLEX,
	};

	class ENGINE_API BoundingBox
	{
	public:
		~BoundingBox();

		virtual ColliderType GetColliderType() = 0;
		virtual Model *GetDrawableModel() = 0;
		virtual void Update(glm::mat4 &matrix) = 0;
		virtual void Draw() = 0;

		bool CollidesWith(BoundingBox &other);
	protected:
		Model *m_Model;
	};


	class ENGINE_API AABB : public BoundingBox
	{
	public:
		AABB(const glm::vec3 modelMin, const glm::vec3 modelMax)
			: m_MaxPosition(modelMax), m_MinPosition(modelMin) {
			Init(modelMin, modelMax);
		};
		~AABB();

		inline ColliderType GetColliderType() override { return GetStaticColliderType(); }
		inline static ColliderType GetStaticColliderType() { return ColliderType::tAABB; }

		void Update(glm::mat4 &matrix);
		inline void Draw() override { m_Model->DrawOutline(); }

		inline Model *GetDrawableModel() override { return m_Model; }
		inline glm::vec3 &GetMinPos() { return m_MinPosition; }
		inline glm::vec3 &GetMaxPos() { return m_MaxPosition; }
		glm::vec3 GetModelCenter();

	private:
		glm::vec3 m_MaxPosition;
		glm::vec3 m_MinPosition;

		void Init(const glm::vec3 modelMin, const glm::vec3 modelMax);

		
	};

}

