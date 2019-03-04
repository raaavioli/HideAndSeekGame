#pragma once

#include "Engine/Window.h"
#include <GLFW/glfw3.h>

namespace Engine {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFunction& callback) override { m_Data.EventCallback = callback; }

		inline void* GetNativeWindow() const override { return m_Window; }

		inline void EnableVSync(bool b) { if (b) glfwSwapInterval(1); else glfwSwapInterval(0); }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdow();

		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;

			EventCallbackFunction EventCallback;
		};

		WindowData m_Data;
	};
}