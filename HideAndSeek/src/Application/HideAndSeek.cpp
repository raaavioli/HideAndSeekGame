#include "HideAndSeek.h"

#include "GameLayer.h"

HideAndSeek::HideAndSeek() 
{
	PushLayer(new GameLayer(Engine::Layer::LayerType::LAYER));	
};