#pragma once
#include <map>
#include "Entity.h"
#include "Utils/MazeGenerator.h"
#include "Item.h"

class ItemSpawner {
public:
	~ItemSpawner();

	static void Init();
	static void Shutdown();
	static void SetGroundDimensions(int mazeX, int mazeY, int groundX, int groundY);
	static Item* Spawn(const char* modelName);
	static Item* Get(unsigned int id);
	static void RandomRespawn(Entity* entity);

private:
	ItemSpawner() {};
	static ItemSpawner *s_Instance;

	static int m_MazeX, m_MazeY, m_GroundX, m_GroundY;
	static unsigned int m_IDCounter;
	static std::map<unsigned int, Item*> m_SpawnedItems;
};