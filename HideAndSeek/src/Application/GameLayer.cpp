#include "GameLayer.h"

#include <glm/gtc/constants.hpp>
#include "Engine/Window.h"
#include "Engine/Application.h"
#include "Engine/Renderer/ShaderProgram.h"
#include <Engine/Objects/Camera.h>
#include "Engine/Objects/Collision/BoundingBox.h"

void GameLayer::OnAttach() {}

void GameLayer::OnDetach() {}

void GameLayer::OnUpdate()
{
	//Check mouse movement
	Engine::Window *window = &Engine::Application::Get().GetWindow();
	double windowHalfWidth = window->GetWidth() / 2;
	double windowHalfHeight = window->GetHeight() / 2;
	auto[mouseX, mouseY] = Engine::Input::GetMousePosition();

	mouseX -= windowHalfWidth;
	mouseY -= windowHalfHeight;

	//Check key pressed
	unsigned char dir = getWASDDirection();
	handleCameraMovement(dir, (float)mouseX, (float)mouseY);

	float speed = 0.1f;
	if (Engine::Input::IsKeyPressed(GLFW_KEY_X)) {
		m_Objects.at(0)->Move(glm::vec3(mouseX, 0, 0), speed);
	}
	else if (Engine::Input::IsKeyPressed(GLFW_KEY_Y)) {
		m_Objects.at(0)->Move(glm::vec3(0, mouseY, 0), speed);
	}
	else if (Engine::Input::IsKeyPressed(GLFW_KEY_Z)) {
		m_Objects.at(0)->Move(glm::vec3(0, 0, mouseX), speed);
	}

	for (auto entity : m_Objects)
		entity->Update();

	setWindowsMouseCenter();

	for (int i = 0; i < m_Objects.size(); i++) {
		auto &o1 = *m_Objects.at(i);
		for (int j = i + 1; j < m_Objects.size(); j++) {
			auto &o2 = *m_Objects.at(j);

			if (o1.CollidesWith(o2)) {
				if (Engine::Input::IsKeyPressed(GLFW_KEY_X)) {
					m_Objects.at(0)->Move(glm::vec3(-mouseX, 0, 0), speed);
				}
				else if (Engine::Input::IsKeyPressed(GLFW_KEY_Y)) {
					m_Objects.at(0)->Move(glm::vec3(0, -mouseY, 0), speed);
				}
				else if (Engine::Input::IsKeyPressed(GLFW_KEY_Z)) {
					m_Objects.at(0)->Move(glm::vec3(0, 0, -mouseX), speed);
				}
			}
		}
	}

	
}

void GameLayer::OnEvent(Engine::Event &e)
{
	Engine::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Engine::KeyPressedEvent>(
		std::bind(&GameLayer::toggleCamera, this, std::placeholders::_1)
	);
}

bool GameLayer::toggleCamera(Engine::KeyPressedEvent &e)
{
	if (e.GetKeyCode() == GLFW_KEY_T)
	{
		setWindowsMouseCenter();
		Engine::Application::Get().GetCamera().ToggleRotatable();
		return true;
	}
	return false;
}

unsigned char GameLayer::getWASDDirection() {
	unsigned char dir = 0;
	if (Engine::Input::IsKeyPressed(GLFW_KEY_W))
		dir |= Engine::Direction::FORWARD;
	if (Engine::Input::IsKeyPressed(GLFW_KEY_S))
		dir |= Engine::Direction::BACKWARD;
	if (Engine::Input::IsKeyPressed(GLFW_KEY_A))
		dir |= Engine::Direction::LEFT;
	if (Engine::Input::IsKeyPressed(GLFW_KEY_D))
		dir |= Engine::Direction::RIGHT;
	return dir;
}

void GameLayer::handleCameraMovement(unsigned char dir, float mouseX, float mouseY) {
	Engine::Camera& cam = Engine::Application::Get().GetCamera();
	if (cam.IsRotatable()) {
		if (dir > 0)
			cam.Move(dir, 0.1f);
		setWindowsMouseCenter();

		if (Engine::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
			cam.Rotate(0, 0, mouseX, 0.001f);
		else
			cam.Rotate(mouseY, mouseX, 0, 0.001f);
	}
	Engine::ShaderProgram::Get().BindViewProjectionMatrices(&cam);
}

void GameLayer::setWindowsMouseCenter()
{
	Engine::Window *window = &Engine::Application::Get().GetWindow();
	glfwSetCursorPos(
		(GLFWwindow*)window->GetNativeWindow(),
		window->GetWidth() / 2,
		window->GetHeight() / 2
	);
}
