#include "Application.h"
#include <memory>

namespace Engine {

#define BIND_EVENT_FN(fn) std::bind(&Application::fn, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (m_Running) {

		}
	}
	void Application::OnEvent(Event &e)
	{
		
	}

	bool Application::OnWindowClose(WindowClosedEvent & e)
	{
		return false;
	}
}