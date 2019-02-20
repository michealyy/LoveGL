#include "engine.h"
#include <resource_manager.h>
#include <core/ui_batch_renderer.h>
#include <core/line_renderer.h>
#include <ui/font_manager.h>

using namespace std;

namespace kd
{

Engine::Engine()
{
}

Engine::~Engine()
{
	SafeDelete(app_);
	SafeDelete(uiRoot);
}

void Engine::Setup()
{
	ResourceManager::GetInstance()->Setup();
	FontManager::GetInstance()->Setup("FreeSans.ttf");
	UIBatchRenderer::GetInstance()->Setup();
	LineRenderer::GetInstance()->Setup();

	if (app_)
		app_->Setup();
	else
		fprintf(stderr, "[Engine]app == nullptr");
	
	if (sceneManager)
		sceneManager->Setup();
	else
		fprintf(stderr, "[Engine]sceneManager == nullptr");

	uiRoot = new ui::UIRoot();
	uiRoot->Setup();
}

void Engine::Update()
{
	float _time = (float)glfwGetTime();
	float deltaTime = _time - lastTime;
	lastTime = _time;
	
	fps = (int)(1.f / deltaTime);
	drawCall = 0;
	
	if (app_)
		app_->Update(deltaTime);
	
	if (sceneManager)
		sceneManager->Update(deltaTime);
	
	if (uiRoot)
		uiRoot->Update(deltaTime);
	
	uiDrawCall = 0;
	uiVertices = 0;
	UIBatchRenderer::GetInstance()->Render();

	PickEntity();
}

void Engine::PickEntity()
{
	if (!mainCamera || !sceneManager)
		return;
	
	//场景拾取
	double x, y;
	glfwGetCursorPos(main_window_, &x, &y);

	//主摄像机鼠标点转换到世界空间射线
	auto ray = mainCamera->MousePointToRay(glm::vec2(x, y));
	//测试屏幕射线是否正确
	auto p1 = ray.GetPoint(1);
	float line[] = {p1.x, p1.y, p1.z, 0, 0, 0};
	LineRenderer::GetInstance()->DrawDebugLine(line);

	if (glfwGetMouseButton(main_window_, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && mainCamera)
	{
		for (auto nodes : sceneManager->GetNodes())
		{
			auto mesh = dynamic_cast<Mesh *>(nodes);
			if (mesh)
			{
				RayCastHit rayCastHit;
				if (mesh->Raycast(ray, rayCastHit))
				{
					selectedEntity = rayCastHit.node;
					break;
				}
			}
		}
	}
}

} // namespace kd