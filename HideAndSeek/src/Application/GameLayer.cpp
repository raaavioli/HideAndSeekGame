#include "GameLayer.h"
#include <ctime>
#include <stdlib.h>

#include <glm/gtc/constants.hpp>

#include "Engine/Application.h"
#include "Engine/Renderer/ShaderProgram.h"
#include <Engine/Objects/Camera.h>
#include "GameObjects/Wall.h"

void GameLayer::OnAttach() 
{
	m_GameRunning = true;
	std::string received = ServerHandler::Recieve();
	APP_INFO("Received {0} bytes of data from server (Game map data)", received.size());

	m_Player = new Player();

	Protocol protocol(&received);
	bool validEntity = true;
	do {
		validEntity = parseNextEntity(protocol);
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
		Protocol playerProtocol(&playerData);
		do {
			updatePlayer(playerProtocol);
		} while (playerProtocol.Next());

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

Engine::Entity* GameLayer::getNewEntityPointerFromType(InstructionType ot)
{
	switch (ot)
	{
	case PLANE:
		return new GroundPlane();
	case WALL:
		return new Wall();
	case ITEM: 
		return new Flag();
	default:
		return nullptr;
	}
}

bool GameLayer::parseNextEntity(Protocol &protocol)
{
	InstructionType ot = protocol.GetInstructionType();
	Attribute attrib = protocol.GetAttribute();
	if (ot == PLAYER)
	{
		updatePlayer(protocol);
		return parseNextEntity(protocol);
	}
	else if ((ot == MESSAGE || ot == ENDGAME) && attrib == Attribute::STATUS)
	{
		setStatusMessage(protocol);
		if (ot == ENDGAME)
		{
			m_GameRunning = false;
			return false;
		}
		return parseNextEntity(protocol);
	}

	if (attrib != NUMATTRIBS || ot == OBJERROR) return false;
	pChar na;
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
				pVector3 p;
				protocol.GetData(&p);
				entity->SetPosition(glm::vec3(p.X, p.Y, p.Z));
			}
			else if (attribToSet == SCALE) {
				pVector3 s;
				protocol.GetData(&s);
				entity->SetScale(glm::vec3(s.X, s.Y, s.Z));
			}
			else if (attribToSet == ID) {
				pInt i;
				protocol.GetData(&i);

				if (ot == ITEM && m_Items.find(i.Value) == m_Items.end())
				{
					m_Items.insert(std::make_pair(i.Value, (Flag*)entity));
				}
				
				entity->SetId(i.Value);
			}
			else {
				APP_ERROR("Data from server must be wrong. Or something is not working properly");
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

void GameLayer::updatePlayer(Protocol &protocol)
{
	do {
		InstructionType ot = protocol.GetInstructionType();
		Attribute at = protocol.GetAttribute();
		if (ot != InstructionType::PLAYER || at != Attribute::NUMATTRIBS)
		{
			parseNextEntity(protocol);
			return;
		}

		pChar na; //number attribs, change name later
		protocol.GetData(&na);

		int player_id = UNDEFINED;
		std::vector<int> items;
		Player* player = nullptr;
		glm::vec3 position;
		glm::vec3 scale;
		for (int i = 0; i < na.Value; i++)
		{
			if (!protocol.HasNext()) {
				return;
			}
			protocol.Next();
			ot = protocol.GetInstructionType();
			at = protocol.GetAttribute();
			if (ot == ITEM && at == ID)
			{
				pInt id;
				protocol.GetData(&id);
				if (m_Items.find(id.Value) != m_Items.end())
					items.push_back(id.Value);
				continue;
			}
			else if (ot != InstructionType::PLAYER)
				return;

			if (at == Attribute::ID)
			{
				pInt i;
				protocol.GetData(&i);
				player_id = i.Value;
				player = getPlayerFromID(player_id);
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
			//Ignore any other attributes
		}

		if (player_id != UNDEFINED && player != nullptr) 
		{
			player->SetScale(scale);
			player->SetPosition(position);
			player->Update();
			for (int id : items) {
				Flag *item = (*m_Items.find(id)).second;
				item->SetPosition(glm::vec3(position.x, position.y, 1.5));
				item->Update();
			}
		}
	} while (protocol.Next());
}

Player *GameLayer::getPlayerFromID(int player_id)
{
	if (m_Player->GetId() == UNDEFINED || m_Player->GetId() == player_id)
	{
		if (m_Player->GetId() == UNDEFINED)
		{
			m_Player->SetId(player_id);
		}
		return m_Player;
	}
	else if (m_Opponents.find(player_id) == m_Opponents.end())
	{
		Player* player = new Player();
		m_Opponents.insert(std::make_pair(player_id, player));
		PushModel(player);
		player->SetId(player_id);
		return player;
	}
	else
	{
		return m_Opponents.at(player_id);
	}
}

void GameLayer::setStatusMessage(Protocol& protocol)
{
	InstructionType ot = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();
	if ((ot == MESSAGE || ot == ENDGAME) && at == STATUS)
	{
		pString message; 
		protocol.GetData(&message);
		m_GameStatus = message.Message;
	}
	protocol.Next();
}