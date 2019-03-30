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
	std::string received = ServerHandler::Recieve();
	APP_INFO("Received {0} bytes of data from server", received.size());

	Protocol protocol(&received);
	bool validEntity = true;
	do {
		validEntity = parseNextEntity(protocol);
	} while (validEntity && protocol.Next());

	for (Engine::Entity* e : m_Objects) {
		Engine::Collider::Add(e, Engine::STATIC);
		e->Update();
	}
		
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

Engine::Entity* GameLayer::getNewEntityPointerFromType(ObjectType ot)
{
	switch (ot)
	{
	case PLANE:
		return new GroundPlane();
	case WALL:
		return new Wall();
	case ITEM:
		//To be implemented
	default:
		return nullptr;
	}
}

bool GameLayer::parseNextEntity(Protocol &protocol)
{
	ObjectType ot = protocol.GetObjectType();
	Attribute attrib = protocol.GetAttribute();
	APP_ASSERT(attrib == NUMATTRIBS, "First attribute of object is not NUMATTRIBS.");
	Numattribs na;
	protocol.GetData(&na);
	APP_ASSERT(na.Value > 0 && na.Value <= NUMATTRIBS, "Invalid number of attributes");

	Engine::Entity *entity;
	if ((entity = getNewEntityPointerFromType(ot)) != nullptr)
	{
		for (int i = 0; i < na.Value; i++)
		{
			if (!protocol.HasNext()) {
				delete entity;
				entity = nullptr;
				return false;
			}
			protocol.Next();

			Attribute attribToSet = protocol.GetAttribute();
			if (attribToSet == POSITION) {
				Position p;
				protocol.GetData(&p);
				entity->SetPosition(glm::vec3(p.X, p.Y, p.Z));
			}
			else if (attribToSet == SCALE) {
				Scale s;
				protocol.GetData(&s);
				entity->SetScale(glm::vec3(s.X, s.Y, s.Z));
			}
			else {
				APP_ERROR("There should not be anything but scaled and translated entities here...");
				delete entity;
				entity = nullptr;
				return false;
			}
		}
		PushModel(entity);
	}
	else
	{
		return false;
	}
	return true;
}
