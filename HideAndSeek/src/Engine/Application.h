#pragma once

#include "Core.h"

#include "Window.h"
#include "Event/Event.h"
#include "Event/WindowEvent.h"

namespace Engine {

	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowClosedEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}
