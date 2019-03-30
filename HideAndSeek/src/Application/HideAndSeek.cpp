#include "HideAndSeek.h"

#include "GameLayer.h"

HideAndSeek::HideAndSeek() 
{
	ServerHandler::Init("127.0.0.1", 8080);
	PushLayer(new GameLayer(Engine::Layer::LayerType::LAYER));	
}
HideAndSeek::~HideAndSeek()
{
	ServerHandler::Shutdown();
}