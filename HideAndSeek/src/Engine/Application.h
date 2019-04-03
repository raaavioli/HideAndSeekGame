#pragma once

#include <glad/glad.h>

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Event/Event.h"
#include "Engine/Event/WindowEvent.h"
#include "Engine/Event/MouseEvent.h"
#include "Engine/Event/KeyEvent.h"
#include "Objects/Camera.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);
		void PopLayer(Layer *layer);
		void PopOverlay(Layer *overlay);

		inline Camera& GetCamera() { return *m_Camera; }
		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowClosedEvent& e);

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Camera> m_Camera;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
