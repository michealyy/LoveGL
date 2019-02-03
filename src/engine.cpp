#include "engine.h"
#include <core/renderer.h>
#include <resource_manager.h>
#include "ui/font_manager.h"

using namespace std;

namespace kd
{

Engine::Engine()
{
}

Engine::~Engine()
{
	SafeDelete(app_);
	SafeDelete(ui_root);
}

void Engine::Setup()
{
	ResourceManager::GetInstance()->Setup();
	FontManager::GetInstance()->Setup("FreeSans.ttf");
	Renderer::GetInstance()->Setup();

	if (app_)
		app_->Setup();
	else
		fprintf(stderr, "[Engine]app == nullptr");
	
	if (sceneManager)
		sceneManager->Setup();
	else
		fprintf(stderr, "[Engine]sceneManager == nullptr");

	ui_root = new ui::UIRoot();
	ui_root->Setup();
}

void Engine::Update()
{
	float _time = (float)glfwGetTime();
	float deltaTime = _time - lastTime;
	lastTime = _time;
	
	fps = (int)(1.f / deltaTime);
	draw_call = 0;
	

	if (app_)
		app_->Update(deltaTime);
	
	if (sceneManager)
		sceneManager->Update(deltaTime);
	
	if (ui_root)
		this->ui_root->Update(deltaTime);
	
	ui_draw_call = 0;
	ui_vertices = 0;
	Renderer::GetInstance()->Render();
}

} // namespace kd