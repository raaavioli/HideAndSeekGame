#include "Collider.h"

namespace Engine {
	Collider *Collider::s_Instance = nullptr;
	std::vector<Entity*> Collider::dynamicEntities;
	std::vector<Entity*>  Collider::staticEntities;

	void Engine::Collider::Init()
	{
		CORE_ASSERT(!s_Instance, "Collider already initialized");
		s_Instance = new Collider();
		dynamicEntities = std::vector<Entity*>();
		staticEntities = std::vector<Entity*>();
	}

	void Engine::Collider::Shutdown()
	{
		delete s_Instance;
	}

	void Collider::Interact()
	{

		for (int i = 0; i < dynamicEntities.size(); i++) 
		{
			Entity dynamicCopy = *dynamicEntities.at(i);
			dynamicCopy.Move();

			//Probably don't want to update this for every object every time.
			dynamicCopy.Update();

			for (int j = i+1; j < dynamicEntities.size(); j++) {
				Entity dynamicOtherCopy = *dynamicEntities.at(j);
				dynamicOtherCopy.Move();

				//Probably don't want to update this for every object every time.
				dynamicOtherCopy.Update();

				if (dynamicCopy.CollidesWith(dynamicOtherCopy)) {
					if (dynamicCopy.GetColliderBox().GetColliderType() == tAABB
						&& dynamicOtherCopy.GetColliderBox().GetColliderType() == tAABB) {

						

					}
				}
					
			}
			for (int j = 0; j < staticEntities.size(); j++) {
				auto &staticOther = staticEntities.at(i);
			}
		}
	}

	/*
	* Taken from https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
	*/
	void Collider::AABBvsAABB(Entity *a, Entity *b)
	{
		// Vector from A to B
		glm::vec3 v_Between = *b->GetTransition() - *a->GetTransition();

		AABB &abox = (AABB&) a->GetColliderBox();
		AABB &bbox = (AABB&) b->GetColliderBox();

		// Calculate half extents along x axis for each object
		float a_extent = (abox.GetMaxPos().x - abox.GetMinPos().x) / 2;
		float b_extent = (bbox.GetMaxPos().x - bbox.GetMinPos().x) / 2;

		// Calculate overlap on x axis
		float x_overlap = a_extent + b_extent - abs(v_Between.x);

		// SAT test on x axis
		if (x_overlap > 0)
		{
			// Calculate half extents along y axis for each object
			float a_extent = (abox.GetMaxPos().y - abox.GetMinPos().y) / 2;
			float b_extent = (bbox.GetMaxPos().y - bbox.GetMinPos().y) / 2;

			// Calculate overlap on y axis
			float y_overlap = a_extent + b_extent - abs(v_Between.y);

			// SAT test on y axis
			if (y_overlap > 0)
			{
				
			}
		}
	}

	void Engine::Collider::Add(Entity *entity, ColliderType c_Type)
	{
	}

	void Engine::Collider::Remove(Entity *entity)
	{
	}

}