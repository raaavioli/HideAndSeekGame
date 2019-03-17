#pragma once

#include "Engine.h"

class HUDLayer : public Engine::Layer
{
public:
	HUDLayer(Layer::LayerType type, std::vector<Engine::Entity*> &objects)
		: Layer(type, objects) {};

	virtual void OnAttach() {

	}

	virtual void OnDetach() {}

	void OnUpdate() override
	{

	}

	void OnEvent(Engine::Event &e) override
	{

	}
private:
};