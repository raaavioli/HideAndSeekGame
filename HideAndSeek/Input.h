#pragma once

#include <utility>

#include "Engine/Core.h"
#include "GLFW/glfw3.h"

namespace Engine {
	class ENGINE_API Input
	{
	public:
		inline bool IsKeyPressed(int keycode)				{ return s_Instance->IsKeyPressedImpl(keycode); }
		inline bool IsMouseButtonPressed(int button)		{ return s_Instance->IsMouseButtonPressedImpl(button); }
		inline double GetMouseX()							{ return s_Instance->GetMouseXImpl(); }
		inline double GetMouseY()							{ return s_Instance->GetMouseYImpl(); }
		inline std::pair<double,double> GetMousePosition()	{ return s_Instance->GetMousePositionImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode);
		virtual bool IsMouseButtonPressedImpl(int button);
		virtual double GetMouseXImpl();
		virtual double GetMouseYImpl();
		virtual std::pair<double,double> GetMousePositionImpl();
	private:
		static Input *s_Instance;
	};
}