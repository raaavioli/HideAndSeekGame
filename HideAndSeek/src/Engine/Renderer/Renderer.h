#pragma once

#include <vector>

#include "Engine/Core.h"
#include "Engine/Log.h"
#include "Engine/Objects/Model.h"

namespace Engine {
	class ENGINE_API Renderer
	{
		Renderer() {};
	public:
		
		~Renderer();

		static void Init();
		static void Shutdown();

		void Render(std::vector<Model*> &objects);

		inline static Renderer &GetRenderer() { return *s_Renderer; }

	private:
		static Renderer *s_Renderer;
	};

}

