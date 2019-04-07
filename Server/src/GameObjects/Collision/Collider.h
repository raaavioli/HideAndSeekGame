#pragma once

#include <vector>

class Entity;

enum MovementType {
	DYNAMIC,
	STATIC,
	LOOTABLE,
};

class Collider {
public:
	static void Init();
	static void Shutdown();

	static void Interact();

	static void Add(Entity*, MovementType);
	static void Remove(Entity*);

private:
	Collider() {};
	static Collider *s_Instance;

	static void AABBvsAABB(Entity *e1, Entity *e2);

	static std::vector<Entity*> dynamicEntities;
	static std::vector<Entity*> staticEntities;
	static std::vector<Entity*> lootableEntities;
};
