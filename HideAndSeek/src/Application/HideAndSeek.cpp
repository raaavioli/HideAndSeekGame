#include "HideAndSeek.h"

#include "GameLayer.h"

HideAndSeek::HideAndSeek() 
{
	models = std::vector<Engine::Entity*>();

	Engine::Model *stall = Engine::OBJLoader::GetModel("stall", false, false);
	Engine::Entity *ent = new Engine::Entity(stall);
	Engine::Entity *ent2 = new Engine::Entity(stall);
	
	ent2->SetPosition(glm::vec3(0, 6, 0));
	ent2->Scale(0.5);
	models.push_back(ent);
	models.push_back(ent2);

	PushLayer(new GameLayer(Engine::Layer::LayerType::LAYER, models));	
};