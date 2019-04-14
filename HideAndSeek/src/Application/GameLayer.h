#pragma once

#include <map>

#include "Engine/Layer.h"
#include "Engine/Event/KeyEvent.h"
#include "Engine/Objects/Collision/Collider.h"

#include "Application/ServerHandler/ServerHandler.h"
#include "Application/ServerHandler/Protocol.h"
#include "GameObjects/Player.h"

class GameLayer : public Engine::Layer
{

public:
	GameLayer(Layer::LayerType type, const std::string& name = "GameLayer")
		: Layer(type, name) {};

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(Engine::Event &e) override;

private:
	Player *m_Player;
	std::map<int, Engine::Entity*> m_Entities;
	glm::vec3 m_LightSource;
	std::string m_GameStatus;
	bool m_GameRunning;

	long currentTime;
	long elapsed;
	int frameCounter;

	bool gameKeyEvent(Engine::KeyPressedEvent &e);
	unsigned char getWASDDirection();
	void updateConsole();
	void handleCameraMovement();
	void handlePlayerVelocity();
	void setWindowsMouseCenter();

	bool parseNextInstruction(Protocol &protocol);
	bool parseEntity(Protocol &protocol);

	Engine::Entity* getEntity(int entity_id, InstructionType ot);
	Engine::Entity *getEntityFromID(int, InstructionType);

	bool GameLayer::updateEntity(Engine::Entity * entity, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 color, bool visibility, char * modelName);

	bool setStatusMessage(Protocol& protocol);

};