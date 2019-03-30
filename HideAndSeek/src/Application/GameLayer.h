#pragma once

#include "ServerHandler/ServerHandler.h"
#include "ServerHandler/Protocol.h"

#include "Engine/Layer.h"
#include "Engine/Event/KeyEvent.h"
#include "GameObjects/GroundPlane.h"
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
	GroundPlane *m_Plane;
	Player *m_Player;
	glm::vec3 m_LightSource;

	bool toggleCamera(Engine::KeyPressedEvent &e);
	unsigned char getWASDDirection();
	void handleCameraMovement(unsigned char dir, float mouseX, float mouseY);
	void setWindowsMouseCenter();
	Engine::Entity* getNewEntityPointerFromType(ObjectType ot);
	bool parseNextEntity(Protocol &protocol);

};