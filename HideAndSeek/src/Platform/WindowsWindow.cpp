#include "WindowsWindow.h"

namespace Engine {

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
	}


	void WindowsWindow::OnUpdate()
	{
	}

	void WindowsWindow::Init(const WindowProps & props)
	{
		//Initialize window data
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		//Set callback functions
	}

	void WindowsWindow::Shutdow()
	{
	}

}