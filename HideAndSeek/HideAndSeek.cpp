#include <Engine.h>

class HideAndSeek : public Engine::Application
{
public:
	HideAndSeek() 
	{
	
	};
	
	~HideAndSeek() 
	{
	
	};
};

Engine::Application* Engine::CreateApplication()
{
	return new HideAndSeek();
}