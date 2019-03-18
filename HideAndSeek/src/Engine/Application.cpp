#include "Application.h"

#include "Objects/Model.h"
#include "Objects/Collision/Collider.h"

namespace Engine {

#define BIND_EVENT_FN(fn) std::bind(&Application::fn, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		
		m_Camera = std::unique_ptr<Camera>(new Camera(
				glm::vec3(0.0f, 0.0f, 45.0f), 
				-glm::pi<float>(), 0.0f, 0.0f, 60.0f, 0.1f, 1000.0f,
				m_Window->GetWidth() / m_Window->GetHeight()
		));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{

		while (m_Running) {
			for (auto it = m_LayerStack.StackEnd(); it != m_LayerStack.StackBegin(); )
			{
				(*--it)->OnUpdate();
			}

			Engine::Collider::Interact();

			for (auto it = m_LayerStack.StackBegin(); it != m_LayerStack.StackEnd(); )
			{
				(*it++)->OnRender();
			}


			m_Window->OnUpdate();

			m_Window->ClearWindow();
		}
	}

	void Application::PushLayer(Layer *layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer *overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer *layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer *overlay)
	{
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
	}

	void Application::OnEvent(Event &e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(OnWindowClose));
		
		//Loop through all layers from top to bottom and break whenever the event is handled
		for (auto it = m_LayerStack.StackEnd(); it != m_LayerStack.StackBegin(); )
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}

	bool Application::OnWindowClose(WindowClosedEvent &e)
	{
		m_Running = false;
		return true;
	}
}