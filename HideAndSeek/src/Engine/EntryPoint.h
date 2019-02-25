#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv) 
{
	Engine::Log::Init();

	CORE_LOG_ERROR("OVERHEATING!! {0} {1}", "Hello", 15);
	APP_INFO("Maybe you should take a break!");

	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif