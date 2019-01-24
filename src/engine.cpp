#include "engine.h"
#include <filesystem>
#include "config.h"
#include "renderer.h"
#include "scene/scene_geometry.h"
#include "ui/font_manager.h"

using namespace std;

namespace kd
{

Engine::Engine()
{
}

Engine::~Engine()
{
	SafeDelete(currentScene);
	SafeDelete(ui_root);
}

void Engine::OnSetup()
{
	LoadTextures();
	LoadShaders();
	LoadMaterials();

	FontManager::GetInstance()->Setup("FreeSans.ttf");

	Renderer::GetInstance()->SetupUIBatchRender();

	this->currentScene = new SceneGeometry();
	this->currentScene->Setup();

	ui_root = new ui::UIRoot();
	ui_root->Setup();
}

void Engine::OnUpdate()
{
	float _time = (float)glfwGetTime();
	float deltaTime = _time - lastTime;
	lastTime = _time;
	fps = (int)(1.f / deltaTime);

	this->currentScene->Update(deltaTime);

	if (ui_root)
	{
		HandleUIRootInput();
		ui_root->Update(deltaTime);
	}

	draw_call = 0;
	ui_draw_call = 0;
	ui_vertices = 0;
	Renderer::GetInstance()->Render();
}

Shader *Engine::GetShader(const std::string &name)
{
	auto it = shaders_.find(name);
	if (it != shaders_.end())
	{
		return (*it).second;
	}
	return nullptr;
}

Texture *Engine::GetTexture(const std::string &name)
{
	auto it = textures_.find(name);
	if (it != textures_.end())
	{
		return (*it).second;
	}
	return nullptr;
}

Material *Engine::GetMaterial(const std::string &name)
{
	auto it = materials_.find(name);
	if (it != materials_.end())
	{
		return (*it).second;
	}
	return nullptr;
}

Material *Engine::GetMaterialById(unsigned id)
{
	for (auto material : materials_)
	{
		if (id == material.second->GetID())
		{
			return material.second;
		}
	}
	return nullptr;
}

void Engine::LoadTextures()
{
	filesystem::path aPath(ASSET_PATH);
	auto path = aPath / "textures";
	if (filesystem::exists(path) && filesystem::is_directory(path))
	{
		for (auto entry : filesystem::directory_iterator(path))
		{
			auto _path = entry.path();
			if (!filesystem::is_directory(_path))
			{
				auto texture = new Texture(_path.stem().string());
				texture->LoadFormFile(_path.string());
			}
		}
	}
}

void Engine::LoadShaders()
{
	filesystem::path aPath(ASSET_PATH);
	auto path = aPath / "shaders";
	if (filesystem::exists(path) && filesystem::is_directory(path))
	{
		for (auto entry : filesystem::directory_iterator(path))
		{
			auto _path = entry.path();
			if (!filesystem::is_directory(_path))
			{
				if (_path.extension().string() == ".shader")
				{
					auto shader = new Shader(_path.stem().string(), _path.string());
					shaders_[shader->GetName()] = shader;
				}
			}
		}
	}
}

void Engine::LoadMaterials()
{
	auto ui_default = new Material("ui_default");
	ui_default->SetShader("unlit_pos_tex");
	ui_default->SetTexture(0, "white");
}

void Engine::HandleUIRootInput()
{
	double x, y;
	glfwGetCursorPos(main_window_, &x, &y);
	int width, height;
	glfwGetWindowSize(main_window_, &width, &height);
	//fprintf(stderr, "mouse pos %f,%f\n", x, y);

	float mouseX = static_cast<float>(x);
	float mouseY = static_cast<float>(height - y);

	//left mouse btn
	auto left_mouse_btn_stat = glfwGetMouseButton(main_window_, GLFW_MOUSE_BUTTON_1);
	if (left_mouse_btn_stat == GLFW_PRESS)
	{
		is_left_mouse_btn_press = true;
		auto ui_rect = ui_root->FindRect(mouseX, mouseY);
		if (ui_rect)
		{
			ui_rect->OnMouseLeftButtonPress();
		}
	}
	else
	{
		auto ui_rect = ui_root->FindRect(mouseX, mouseY);
		if (is_left_mouse_btn_press)
		{
			is_left_mouse_btn_press = false;
			if (ui_rect)
			{
				ui_rect->OnMouseLeftButtonRelease();
			}
		}
		else
		{
			if (ui_rect)
			{
				ui_rect->OnMouseHover();
			}
		}
	}
}

} // namespace kd