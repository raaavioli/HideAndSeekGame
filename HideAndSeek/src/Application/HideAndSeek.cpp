#include "HideAndSeek.h"

#include "GameLayer.h"

HideAndSeek::HideAndSeek() 
{
	//hompc 80.216.76.19
	ServerHandler::Init("127.0.0.1", 8080);
	PushLayer(new GameLayer(Engine::Layer::LayerType::LAYER));	
}
HideAndSeek::~HideAndSeek()
{
	ServerHandler::Shutdown();
}