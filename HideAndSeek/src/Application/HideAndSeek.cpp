#include "HideAndSeek.h"

#include "GameLayer.h"

HideAndSeek::HideAndSeek() 
{
	ServerHandler::Init("127.0.0.1", 8080);
	//std::string received = ServerHandler::Recieve();
	//APP_INFO("Received from Server: {0}", received);
	PushLayer(new GameLayer(Engine::Layer::LayerType::LAYER));	
}
HideAndSeek::~HideAndSeek()
{
	ServerHandler::Shutdown();
}