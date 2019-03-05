#include "Application.h"

namespace Engine {

#define BIND_EVENT_FN(fn) std::bind(&Application::fn, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		ENGINE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (m_Running) {
			m_Window->OnUpdate();
		}
	}
	void Application::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(OnWindowClose));
		/*
		after layerstack + layer implementation

		for(auto *layer : Application.layerStack)
			layer->OnEvent(e)
		*/
	}

	bool Application::OnWindowClose(WindowClosedEvent &e)
	{
		m_Running = false;
		return true;
	}
}