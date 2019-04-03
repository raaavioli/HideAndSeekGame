#include "GameLayer.h"

#include <glm/gtc/constants.hpp>

#include "Engine/Application.h"
#include "Engine/Renderer/ShaderProgram.h"
#include <Engine/Objects/Camera.h>
#include "GameObjects/Wall.h"

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
	//Needs to be rewritten, should not have to do a handshake for playerData
	ServerHandler::Send("handshake");
		
	m_Player = new Player();
	std::string playerData = ServerHandler::Recieve();
	Protocol playerProtocol(&playerData);
	do {
		updatePlayer(playerProtocol);
	} while (playerProtocol.Next());
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

	handlePlayerVelocity();

	//Send local player attributes (and action(s)) and wait for server to respond
	std::string playerSend = m_Player->BuildProtocolString();
	playerSend.append(m_Player->BuildActionString());
	ServerHandler::Send(playerSend);
	m_Player->SetAction(OBJERROR);
	std::string playerData = ServerHandler::Recieve();
	Protocol playerProtocol(&playerData);
	do {
		updatePlayer(playerProtocol);
	} while (playerProtocol.Next());

	
	Engine::Camera& cam = Engine::Application::Get().GetCamera();
	//Set camera position to player's head
	*cam.GetPosition() = m_Player->GetPosition() * glm::vec3(1,1,1.9);
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
			else if (attribToSet == ID) {
				Id i;
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
	InstructionType ot = protocol.GetInstructionType();
	Attribute at = protocol.GetAttribute();
	if (ot != InstructionType::PLAYER || at != Attribute::NUMATTRIBS)
		return;

	Numattribs na;
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
			Id id;
			protocol.GetData(&id);
			if (m_Items.find(id.Value) != m_Items.end())
				items.push_back(id.Value);
			continue;
		}
		else if (ot != InstructionType::PLAYER)
			return;

		if (at == Attribute::ID)
		{
			Id i;
			protocol.GetData(&i);
			player_id = i.Value;
			//Assuming the first player updated is m_Player, otherwise game will not work properly.
			if (m_Player->GetId() == UNDEFINED || m_Player->GetId() == player_id)
			{
				if (m_Player->GetId() == UNDEFINED)
				{
					PushModel(m_Player);
				}
				player = m_Player;
				player->SetId(player_id);
			}
			else if (m_Opponents.find(player_id) == m_Opponents.end())
			{
				player = new Player();
				m_Opponents.insert(std::make_pair(player_id, player));
				PushModel(player);
				player->SetId(player_id);
			}
			else
			{
				player = m_Opponents.at(player_id);
			}
		}
		else if (at == Attribute::POSITION)
		{
			Position p;
			protocol.GetData(&p);
			position = glm::vec3(p.X, p.Y, p.Z);
		}
		else if (at == Attribute::SCALE)
		{
			Scale s;
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
}