#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine/Window.h"


namespace Engine {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
		void OnUpdate() override;
		inline void ClearWindow() override { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };

		double GetWidth() const override { return m_Data.Width; }
		double GetHeight() const override { return m_Data.Height; }

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
			double Width, Height;

			EventCallbackFunction EventCallback;
		};

		WindowData m_Data;
	};
}