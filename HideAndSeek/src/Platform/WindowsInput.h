#pragma once

#include <utility>

#include <Engine/Core.h>
#include <GLFW/glfw3.h>

#include "Engine/Application.h"
#include "Engine/Input.h"

namespace Engine {
	class ENGINE_API WindowsInput : public Input
	{
	protected:
		bool IsKeyPressedImpl(int keycode) override;
		bool IsMouseButtonPressedImpl(int button) override;
		std::pair<double, double> GetMousePositionImpl() override;
		double GetMouseXImpl() override;
		double GetMouseYImpl() override;
	};
}