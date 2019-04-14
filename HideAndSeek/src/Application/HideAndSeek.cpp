#include "HideAndSeek.h"

#include "GameLayer.h"

HideAndSeek::HideAndSeek() 
{
	//hompc 
	ServerHandler::Init("80.216.76.19", 8080);
	PushLayer(new GameLayer(Engine::Layer::LayerType::LAYER));	
}
HideAndSeek::~HideAndSeek()
{
	ServerHandler::Shutdown();
}