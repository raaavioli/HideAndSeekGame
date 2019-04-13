#include "GameLayer.h"
#include <ctime>
#include <stdlib.h>

#include <glm/gtc/constants.hpp>

#include "Engine/Application.h"
#include "Engine/Renderer/ShaderProgram.h"
#include <Engine/Objects/Camera.h>

void GameLayer::OnAttach() 
{
	m_GameRunning = true;
	std::string received = ServerHandler::Recieve();
	APP_INFO("Received {0} bytes of data from server (Game map data)", received.size());

	m_Player = new Player();
	m_Player->SetId(UNDEFINED);

	Protocol protocol(&received);
	bool validEntity = true;
	do {
		validEntity = parseNextInstruction(protocol);
	} while (validEntity && protocol.Next());

	for (Engine::Entity* e : m_Objects) {
		Engine::Collider::Add(e, Engine::STATIC);
		e->Update();
	}

	currentTime = std::clock();
	elapsed = 0;
	frameCounter = 0;
}

void GameLayer::OnDetach() {}

void GameLayer::OnUpdate()
{
	if (m_GameRunning)
	{
		//Move and rotate camera based on mouse-movement and key-presses
		handleCameraMovement();
		handlePlayerVelocity();

		//Send local player attributes (and action(s)) and wait for server to respond
		std::string playerSend = m_Player->BuildProtocolString();
		playerSend.append(m_Player->BuildActionString());
		ServerHandler::Send(playerSend);
		m_Player->SetAction(OBJERROR);

		//Receive response with new data from server
		std::string playerData = ServerHandler::Recieve();

		//Run the data through the protocol and parse the instructions
		Protocol receivedData(&playerData);
		do {
			parseNextInstruction(receivedData);
		} while (receivedData.Next());

		updateConsole();
	
		//Set camera position to player's head
		Engine::Camera& cam = Engine::Application::Get().GetCamera();
		*cam.GetPosition() = m_Player->GetPosition() * glm::vec3(1,1,1.9);
	}
}

void GameLayer::OnEvent(Engine::Event &e)
{
	Engine::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Engine::KeyPressedEvent>(
		std::bind(&GameLayer::gameKeyEvent, this, std::placeholders::_1)
	);
}

bool GameLayer::gameKeyEvent(Engine::KeyPressedEvent &e)
{
	if (e.GetKeyCode() == GLFW_KEY_T)
	{
		setWindowsMouseCenter();
		Engine::Application::Get().GetCamera().ToggleRotatable();
		return true;
	}
	else if (e.GetKeyCode() == GLFW_KEY_E)
	{
		m_Player->SetAction(InstructionType::PICKUP);
		return true;
	}
	else if (e.GetKeyCode() == GLFW_KEY_Q)
	{
		m_Player->SetAction(InstructionType::DROP);
		return true;
	}
	else if (e.GetKeyCode() == GLFW_KEY_SPACE)
	{
		m_Player->SetAction(InstructionType::ATTACK);
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

void GameLayer::updateConsole()
{
	long now = std::clock();
	long diff = now - currentTime;
	elapsed += diff;
	currentTime = now;
	if (elapsed > 100 || !m_GameRunning)
	{
		Engine::Application::Get().GetWindow().ClearConsole();
		int myID = m_Player->GetId();
		APP_INFO("--ServerMessage--\nFPS: {0}\nYou are: Player {1}\n\n{2}", 1000/diff, myID, m_GameStatus);
		elapsed %= 100;
	}
}

/*
	Currently also binds light source. Camera position is source of light
*/
void GameLayer::handleCameraMovement() {
	//Check WASD-keys pressed
	unsigned char dir = getWASDDirection();

	//Check mouse movement
	Engine::Window *window = &Engine::Application::Get().GetWindow();
	auto[mouseX, mouseY] = Engine::Input::GetMousePosition();
	mouseX -= window->GetWidth() / 2;
	mouseY -= window->GetHeight() / 2;

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

void GameLayer::handlePlayerVelocity()
{
	Engine::Camera& cam = Engine::Application::Get().GetCamera();

	/******************************************/
	glm::vec3 camfrw = cam.GetForwardDirection();
	glm::vec3 camrigh = cam.getRightDirection();

	m_Player->SetVelocity(glm::vec3(0));
	if (Engine::Input::IsKeyPressed(GLFW_KEY_W))
		m_Player->GetVelocity() += glm::vec3(camfrw.x, camfrw.y, 0);
	if (Engine::Input::IsKeyPressed(GLFW_KEY_S))
		m_Player->GetVelocity() -= glm::vec3(camfrw.x, camfrw.y, 0);
	if (Engine::Input::IsKeyPressed(GLFW_KEY_A))
		m_Player->GetVelocity() -= glm::vec3(camrigh.x, camrigh.y, 0);
	if (Engine::Input::IsKeyPressed(GLFW_KEY_D))
		m_Player->GetVelocity() += glm::vec3(camrigh.x, camrigh.y, 0);
	/****************************************/
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

Engine::Entity* GameLayer::getEntity(int entity_id, InstructionType it)
{
	Engine::Entity *entity = nullptr;
	switch (it)
	{
	case PLANE:
	case WALL:
	{
		entity = new Engine::Entity();
		PushModel(entity);
		break;
	}
	case ITEM: 
	case PLAYER:
	{
		if (entity_id > 0)
		{
			entity = getEntityFromID(entity_id, it);
			if (entity == nullptr)
			{
				if (it == PLAYER) 
					entity = new Player();
				else
					entity = new Engine::Entity();

				m_Entities.insert(std::make_pair(entity_id, entity));
				PushModel(entity);
				entity->SetId(entity_id);
			}
		}
		break;
	} 
	}
	return entity;
}

bool GameLayer::parseNextInstruction(Protocol &protocol)
{
	InstructionType it = protocol.GetInstructionType();
	switch (it)
	{
		case PLAYER:
		case ITEM:
		case PLANE:
		case WALL:
		{
			return parseEntity(protocol);
		}
		case MESSAGE:
		case ENDGAME:
		{
			return setStatusMessage(protocol);
		}
	}
	return false;
}

bool GameLayer::parseEntity(Protocol &protocol) 
{
	InstructionType it = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();
	if (it != PLAYER && it != WALL && it != PLANE && it != ITEM) return false;
	if (at != Attribute::NUMATTRIBS) return false;

	pChar na; 
	protocol.GetData(&na);
	int entity_id = UNDEFINED;
	Engine::Entity* entity = nullptr;
	glm::vec3 position = glm::vec3(FLT_MAX);
	glm::vec3 scale = glm::vec3(FLT_MAX);
	glm::vec3 rotation = glm::vec3(FLT_MAX);
	glm::vec3 color = glm::vec3(FLT_MAX);
	char* model = "";
	for (int i = 0; i < na.Value; i++)
	{
		if (!protocol.HasNext()) return false;
		protocol.Next();
		InstructionType current = protocol.GetInstructionType();
		if (current != it) return false;
		at = protocol.GetAttribute();

		if (at == Attribute::ID)
		{
			pInt i;
			protocol.GetData(&i);
			entity_id = i.Value;
			entity = getEntity(entity_id, it);
		}
		else if (at == Attribute::POSITION)
		{
			pVector3 p;
			protocol.GetData(&p);
			position = glm::vec3(p.X, p.Y, p.Z);
		}
		else if (at == Attribute::SCALE)
		{
			pVector3 s;
			protocol.GetData(&s);
			scale = glm::vec3(s.X, s.Y, s.Z);
		}
		else if (at == Attribute::COLOR)
		{
			pVector3 c;
			protocol.GetData(&c);
			color = glm::vec3(c.X, c.Y, c.Z);
		}
		else if (at == Attribute::MODEL)
		{
			pString64 m;
			protocol.GetData(&m);
			model = m.Message;
		}
		//Ignore any other attributes
	}

	return entity_id != UNDEFINED && updateEntity(entity, position, scale, rotation, color, model);
}

bool GameLayer::updateEntity(Engine::Entity * entity, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::vec3 color, char * modelName)
{
	if (entity != nullptr)
	{
		if (scale.x != FLT_MAX)
			entity->SetScale(scale);
		if (position.x != FLT_MAX)
			entity->SetPosition(position);
		if (rotation.x != FLT_MAX)
			entity->SetRotation(rotation);
		if (color.x != FLT_MAX)
			entity->SetColor(color);
		if (modelName != "")
			entity->UpdateModel(modelName);
		entity->Update();
		return true;
	}
	return false;
}

Engine::Entity *GameLayer::getEntityFromID(int entity_id, InstructionType it)
{
	if ((m_Player->GetId() == UNDEFINED && it == PLAYER) || m_Player->GetId() == entity_id)
	{
		m_Player->SetId(entity_id);
		return m_Player;
	}
	else if (m_Entities.find(entity_id) != m_Entities.end())
		return m_Entities.at(entity_id);
	else return nullptr;
}

bool GameLayer::setStatusMessage(Protocol& protocol)
{
	InstructionType ot = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();
	if ((ot == MESSAGE || ot == ENDGAME) && at == STATUS)
	{
		if (ot == ENDGAME) m_GameRunning = false;

		pString512 message; 
		protocol.GetData(&message);
		m_GameStatus = message.Message;
		return true;
	}
	return false;
}