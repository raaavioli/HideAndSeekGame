#include "GameLayer.h"

#include <glm/gtc/constants.hpp>
#include "Engine/Application.h"
#include "Engine/Renderer/ShaderProgram.h"
#include <Engine/Objects/Camera.h>
#include "GameObjects/Wall.h"
#include "GameObjects/Player.h"
#include "Engine/Objects/Collision/Collider.h"
#include "Utils/MazeGenerator.h"

void GameLayer::OnAttach() 
{
	//GroundPlane &gp = GroundPlane(60, 40);
	m_Plane = new GroundPlane(60, 40);
	PushModel(m_Plane);
	
	//Outer walls
	PushModel(new Wall(*m_Plane, -1, 0, glm::vec3(1, (int)m_Plane->GetHeight(), 4)));
	PushModel(new Wall(*m_Plane, (int)m_Plane->GetWidth(), 0, glm::vec3(1, (int)m_Plane->GetHeight(), 4)));
	PushModel(new Wall(*m_Plane, 0, -1, glm::vec3((int)m_Plane->GetWidth(), 1, 4)));
	PushModel(new Wall(*m_Plane, 0, (int)m_Plane->GetHeight(), glm::vec3((int)m_Plane->GetWidth(), 1, 4)));
	
	MazeGenerator mg(12, 8);
	mg.GenerateMaze();
	mg.PrintMaze();
	mg.CutLongerWalls(3);
	std::cout << "\n";
	mg.PrintMaze();

	std::vector<Wall*> walls = mg.GetGameWalls(*m_Plane);
	for (Engine::Entity* e : m_Objects) {
		Engine::Collider::Add(e, Engine::STATIC);
		e->Update();
	}

	for (Wall* w : walls) {
		PushModel(w);
		Engine::Collider::Add(w, Engine::STATIC);
		w->Update();
	}

	//Push background plane into layer but not collider when we have floor as smaller units
	/*for (Engine::Entity* plane : gp.GetPlane())
	{
		PushModel(plane);
		plane->Update();
	}*/
		
	m_Player = new Player();
	PushModel(m_Player);
	Engine::Collider::Add(m_Player, Engine::DYNAMIC);
}

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

	//Check WASD-keys pressed
	unsigned char dir = getWASDDirection();

	//Move and rotate camera based on mouse-movement and key-presses
	handleCameraMovement(dir, (float)mouseX, (float)mouseY);

	//Set player velocity based on input
	float speed = 0.1f;
	m_Player->Move(dir, speed);
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

/*
	Currently also binds light source. Camera position is source of light
*/
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

		setWindowsMouseCenter();
	}

	Engine::ShaderProgram::Get().BindViewProjectionMatrices(&cam);
	Engine::ShaderProgram::Get().BindLightSource(cam.GetPosition());
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
