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
		virtual Model *GetStaticUnitModel() = 0;
		virtual void Draw() = 0;
	};


	class ENGINE_API AABB : public BoundingBox
	{
	public:
		AABB(){};
		~AABB();

		inline bool CollidesWith(BoundingBox& other) override 
		{
			return false;
		}

		inline void Draw() override 
		{
			if (!AABBModel)
				GetStaticUnitModel();
			AABBModel->DrawOutline();
		}

		Model *GetStaticUnitModel() override;

	private:


		static Model *AABBModel;
	};

}

