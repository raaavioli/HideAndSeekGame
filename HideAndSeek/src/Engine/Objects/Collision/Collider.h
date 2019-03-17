#pragma once

#include "Engine/Core.h"
#include "Engine/Objects/Entity.h"

namespace Engine {
	enum MovementType {
		DYNAMIC,
		STATIC
	};

	class ENGINE_API Collider {
	public:
		static void Init();
		static void Shutdown();

		static void Interact();

		static void Add(Entity*, ColliderType);
		static void Remove(Entity*);

	private:
		Collider() {};
		static Collider *s_Instance;

		static void AABBvsAABB(Entity *e1, Entity *e2);

		static std::vector<Entity*> dynamicEntities;
		static std::vector<Entity*> staticEntities;
	};
}