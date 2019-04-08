#pragma once

#include "Core.h"
#include "Event/Event.h"

#include "Log.h"
#include <string>
#include <functional>

namespace Engine {

	struct WindowProps
	{
		std::string Title;
		double Width;
		double Height;

		WindowProps(
			const std::string& title = "Engine Window",
			double width = 1280.0f,
			double height = 720.0f)
			: Title(title), Width(width), Height(height) {}
	};

	class ENGINE_API Window
	{
	public:
		using EventCallbackFunction = std::function<void(Event&)>;

		virtual ~Window() {};
		virtual void OnUpdate() {};
		virtual void ClearWindow() {};
		virtual void ClearConsole() {};

		virtual double GetWidth() const = 0;
		virtual double GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;

		// Returns a pointer to the native platform based window
		// In the case of WindowsWindow, the NativeWindow is
		// a GLFWWindow*.
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}