#pragma once

#include "Engine/Renderer/ShaderProgram.h"
#include "Engine/Objects/OBJLoader.h"
#include "Engine/Objects/Collision/Collider.h"

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv) 
{
	Engine::Log::Init();
	Engine::Renderer::Init();
	Engine::OBJLoader::Init();
	Engine::Collider::Init();
	auto app = Engine::CreateApplication();
	
	Engine::ShaderProgram::Init("src/Engine/Renderer/Shaders/vertex3d.glsl", "src/Engine/Renderer/Shaders/fragment3d.glsl");
	app->Run();
	delete app;

	Engine::Collider::Shutdown();
	Engine::OBJLoader::Shutdown();
	Engine::ShaderProgram::Shutdown();
	Engine::Renderer::Shutdown();
}

#endif