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
		auto move = [&](MouseMovedEvent &e)->bool {
			APP_INFO("MouseMove: {0},{1}", e.GetX(), e.GetY());
			return true;
		};

		auto mclick = [&](MouseButtonPressedEvent &e)->bool {
			APP_INFO("MousePress: {0}", e.GetMouseButton());
			return true;
		};
		auto mrel = [&](MouseButtonReleasedEvent &e)->bool {
			APP_INFO("MouseRelease: {0}", e.GetMouseButton());
			return true;
		};

		auto scroll = [&](MouseScrolledEvent &e)->bool {
			APP_INFO("MouseScroll: {0},{1}", e.GetXOffset(), e.GetYOffset());
			return true;
		};

		dispatcher.Dispatch<MouseMovedEvent>(move);
		dispatcher.Dispatch<MouseButtonPressedEvent>(mclick);
		dispatcher.Dispatch<MouseButtonReleasedEvent>(mrel);
		dispatcher.Dispatch<MouseScrolledEvent>(scroll);
		
	}

	bool Application::OnWindowClose(WindowClosedEvent &e)
	{
		m_Running = false;
		return true;
	}
}