#include <Engine.h>
#include <Engine/Objects/Camera.h>

class TestLayer : public Engine::Layer
{



public:
	TestLayer(Layer::LayerType type, std::vector<Engine::Model*> &objects)
		: Layer(type, objects) {};

	virtual void OnAttach() {
	
	}
	//virtual void OnDetach() {}
	void OnUpdate() override 
	{
		//Check mouse movement
		Engine::Window *window = &Engine::Application::Get().GetWindow();
		double windowHalfWidth = window->GetWidth() / 2;
		double windowHalfHeight = window->GetHeight() / 2;
		auto[mouseX, mouseY] = Engine::Input::GetMousePosition();

		mouseX -= windowHalfWidth;
		mouseY -= windowHalfHeight;

		//Check key pressed
		unsigned char dir = 0;
		if (Engine::Input::IsKeyPressed(GLFW_KEY_W))
			dir |= Engine::Direction::FORWARD;
		if (Engine::Input::IsKeyPressed(GLFW_KEY_S))
			dir |= Engine::Direction::BACKWARD;
		if (Engine::Input::IsKeyPressed(GLFW_KEY_A))
			dir |= Engine::Direction::LEFT;
		if (Engine::Input::IsKeyPressed(GLFW_KEY_D))
			dir |= Engine::Direction::RIGHT;

		Engine::Camera& cam = Engine::Application::Get().GetCamera();
		if(dir > 0)
			cam.Move(dir, 0.1f);

		if (cam.IsRotatable()) {
			if(Engine::Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
				cam.Rotate(0, 0, mouseX, 0.001f);
			else
				cam.Rotate(mouseY, mouseX, 0, 0.001f);
			m_setWindowsMouseCenter();
		}


		Engine::ShaderProgram::Get().BindViewProjectionMatrices(&cam);
	}
	
	void OnEvent(Engine::Event &e) override 
	{
		Engine::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Engine::KeyPressedEvent>(
			std::bind(&TestLayer::OnKeyPressed, this, std::placeholders::_1)
		);
	}

private:
	bool OnKeyPressed(Engine::KeyPressedEvent &e)
	{
		if(e.GetKeyCode() == GLFW_KEY_T) 
		{
			m_setWindowsMouseCenter();
			Engine::Application::Get().GetCamera().ToggleRotatable();
			return true;
		}
		return false;
	}

	void m_setWindowsMouseCenter() 
	{
		Engine::Window *window = &Engine::Application::Get().GetWindow();
		glfwSetCursorPos(
			(GLFWwindow*)window->GetNativeWindow(),
			window->GetWidth() / 2,
			window->GetHeight() / 2
		);
	}
};

class HUDLayer : public Engine::Layer
{



public:
	HUDLayer(Layer::LayerType type, std::vector<Engine::Model*> &objects)
		: Layer(type, objects) {};

	virtual void OnAttach() {
		
	}

	virtual void OnDetach() {}

	void OnUpdate() override
	{

	}

	void OnEvent(Engine::Event &e) override
	{

	}

private:

};

class HideAndSeek : public Engine::Application
{
public:
	std::vector<Engine::Model*>models;

	HideAndSeek() 
	{
		models = std::vector<Engine::Model*>();

		//models.push_back(Engine::OBJLoader::GetModel("stall"));

		static const GLfloat vy[] = {
		   -10.0f, -10.0f, 0.0f,
		   -10.0f, 10.0f, 0.0f,
		   10.0f,  -10.0f, 0.0f,
			10.0f,  10.0f, 0.0f,
		};

		static const GLfloat vy2[] = {
		   -1.0f, -1.0f, 1.0f,
		   -1.0f, 1.0f, 1.0f,
		   1.0f,  -1.0f, 1.0f,
			1.0f,  1.0f, 1.0f,
		};

		std::vector<float> fl;
		for (int i = 0; i < 12; i++) {
			fl.push_back(vy[i]);
		}

		std::vector<float> fl2;
		for (int i = 0; i < 12; i++) {
			fl.push_back(vy2[i]);
		}

		std::vector<unsigned int> ind;
		ind.push_back(0);
		ind.push_back(1);
		ind.push_back(2);
		ind.push_back(1);
		ind.push_back(2);
		ind.push_back(3);

		Engine::Model *mod = new Engine::Model(new Engine::VAO(), new Engine::VBO(&fl, 3), nullptr, nullptr, new Engine::VBO(&ind));
		Engine::Model *mod = new Engine::Model(new Engine::VAO(), new Engine::VBO(&fl2, 3), nullptr, nullptr, new Engine::VBO(&ind));

		models.push_back(mod);

		PushLayer(new TestLayer(Engine::Layer::LayerType::LAYER, models));

		
	};
	
	~HideAndSeek() 
	{
	
	};
};

Engine::Application* Engine::CreateApplication()
{
	return new HideAndSeek();
}