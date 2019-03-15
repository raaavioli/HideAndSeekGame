#pragma once

#include <glm/glm.hpp>

#include "Engine/Core.h"
#include "Engine/Objects/Model.h"

namespace Engine {

	class ENGINE_API BoundingBox
	{
	public:
		~BoundingBox();

		virtual bool CollidesWith(BoundingBox& other) = 0;
		virtual Model *GetDrawableModel() = 0;
		virtual void Draw() = 0;
	};


	class ENGINE_API AABB : public BoundingBox
	{
	public:
		AABB(const glm::vec3 modelMin, const glm::vec3 modelMax){
			Init(modelMin, modelMax);
		};
		~AABB();

		inline bool CollidesWith(BoundingBox& other) override 
		{
			return false;
		}

		inline void Draw() override 
		{
			AABBModel->DrawOutline();
		}

		inline Model *GetDrawableModel() override { return AABBModel; }

	private:

		void Init(const glm::vec3 modelMin, const glm::vec3 modelMax);

		Model *AABBModel;
	};

}

