#include "Renderer.h"

namespace Engine {

	Renderer* Renderer::s_Renderer = nullptr;

	Renderer::~Renderer()
	{
	}

	void Renderer::Init()
	{
		CORE_ASSERT(!s_Renderer, "Renderer already exists!");
		s_Renderer = new Renderer();
	}

	void Renderer::Shutdown()
	{
		if(s_Renderer)
			delete s_Renderer;
	}

	void Renderer::Render(std::vector<Model*> &objects)
	{
		for (auto model : objects)
			model->Draw();
	}

}
