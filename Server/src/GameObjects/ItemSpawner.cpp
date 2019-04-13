#include "ItemSpawner.h"
#include "Collision/Collider.h"
#include "Collision/BoundingBox.h"
#include <random>
#include <ctime>

ItemSpawner *ItemSpawner::s_Instance = nullptr;
int ItemSpawner::m_MazeX = 0;
int ItemSpawner::m_MazeY = 0;
int ItemSpawner::m_GroundY = 0;
int ItemSpawner::m_GroundX = 0; 
unsigned int ItemSpawner::m_IDCounter = 0;
std::map<unsigned int, Item*> ItemSpawner::m_SpawnedItems;

ItemSpawner::~ItemSpawner()
{
}

void ItemSpawner::Init()
{
	if (!s_Instance) {
		s_Instance = new ItemSpawner();
		m_IDCounter = 1000;
		m_SpawnedItems.clear();
		std::srand(std::time(0));
	}
}

void ItemSpawner::Shutdown()
{
	delete s_Instance;
}

void ItemSpawner::SetGroundDimensions(int mazeX, int mazeY, int groundX, int groundY)
{
	m_MazeX = mazeX;
	m_MazeY = mazeY;
	m_GroundX = groundX;
	m_GroundY = groundY;
}

Item* ItemSpawner::Spawn(const char* modelName)
{
	Item *i;
	if (strcmp(modelName, "flag") == 0)
		i = new Item(modelName, -40, Item::DROPPABLE);
	if (strcmp(modelName, "wingboots") == 0)
		i = new Item(modelName, -100, Item::USABLE);
	i->Update();
	RandomRespawn(i);

	i->SetId(m_IDCounter);
	m_IDCounter++;
	m_SpawnedItems.insert(std::make_pair(i->GetId(), i));
	Collider::Add(i, LOOTABLE);
	return i;
}

Item* ItemSpawner::Get(unsigned int id)
{
	if (m_SpawnedItems.find(id) != m_SpawnedItems.end())
		return m_SpawnedItems.at(id);
	return nullptr;
}

void ItemSpawner::RandomRespawn(Entity * entity)
{
	float distPerWallX = m_GroundX / m_MazeX;
	float distPerWallY = m_GroundY / m_MazeY;

	int randx = std::rand() % m_MazeX;
	int randy = std::rand() % m_MazeY;
	float posX = -m_GroundX / 2 + randx * distPerWallX + distPerWallX / 2;
	float posY = -m_GroundY / 2 + randy * distPerWallY + distPerWallY / 2;
	float posZ = entity->GetHeight() / 2;

	entity->SetPosition(glm::vec3(posX, posY, posZ));
	entity->Update();
}
