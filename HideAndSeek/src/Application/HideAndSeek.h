#pragma once

#include "Engine.h"
#include "ServerHandler/ServerHandler.h"

class HideAndSeek : public Engine::Application
{
public:
	HideAndSeek();

	~HideAndSeek();

private: 
	std::vector<Engine::Entity*>models;
};


Engine::Application* Engine::CreateApplication()
{
	return new HideAndSeek();
}