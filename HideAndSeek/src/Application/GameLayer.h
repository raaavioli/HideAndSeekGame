#pragma once

#include <map>

#include "Engine/Layer.h"
#include "Engine/Event/KeyEvent.h"
#include "Engine/Objects/Collision/Collider.h"

#include "Application/ServerHandler/ServerHandler.h"
#include "Application/ServerHandler/Protocol.h"
#include "GameObjects/GroundPlane.h"
#include "GameObjects/Player.h"
#include "GameObjects/Flag.h"

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
	GroundPlane *m_Plane;
	Player *m_Player;
	std::map<int, Player*> m_Opponents;
	std::map<int, Flag*> m_Items;
	glm::vec3 m_LightSource;

	bool gameKeyEvent(Engine::KeyPressedEvent &e);
	unsigned char getWASDDirection();
	void handleCameraMovement(unsigned char dir, float mouseX, float mouseY);
	void handlePlayerVelocity();
	void setWindowsMouseCenter();
	Engine::Entity* getNewEntityPointerFromType(InstructionType ot);
	bool parseNextEntity(Protocol &protocol);
	void updatePlayer(Protocol &protocol);
	Player *getPlayerFromID(int);

};