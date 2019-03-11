#pragma once

#include <utility>

#include "Engine/Core.h"
#include "GLFW/glfw3.h"

namespace Engine {
	class ENGINE_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode)				{ return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(int button)		{ return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static double GetMouseX()							{ return s_Instance->GetMouseXImpl(); }
		inline static double GetMouseY()							{ return s_Instance->GetMouseYImpl(); }
		inline static std::pair<double,double> GetMousePosition()	{ return s_Instance->GetMousePositionImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<double, double> GetMousePositionImpl() = 0;
		virtual double GetMouseXImpl() = 0;
		virtual double GetMouseYImpl() = 0;
	private:
		static Input *s_Instance;
	};
}