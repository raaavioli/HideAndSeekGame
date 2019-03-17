#pragma once

#include "Engine/Layer.h"
#include "Engine/Event/KeyEvent.h"

class GameLayer : public Engine::Layer
{

public:
	GameLayer(Layer::LayerType type, std::vector<Engine::Entity*> &objects)
		: Layer(type, objects) {};

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(Engine::Event &e) override;

private:
	bool toggleCamera(Engine::KeyPressedEvent &e);
	unsigned char getWASDDirection();
	void handleCameraMovement(unsigned char dir, float mouseX, float mouseY);
	void setWindowsMouseCenter();

};