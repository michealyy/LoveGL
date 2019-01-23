#include <filesystem>
#include "config.h"
#include "engine.h"
#include "renderer.h"
#include "scene/scene_geometry.h"

using namespace std;

namespace kd {
	
Engine::Engine()
{
}

Engine::~Engine() {
	SafeDelete(currentScene);
}

void Engine::OnSetup() {
	LoadTextures();
	LoadShaders();
	LoadMaterials();
	
	Renderer::GetInstance()->SetupUIBatchRender();

	this->currentScene =  new SceneGeometry();
	this->currentScene->Setup();
}

void Engine::OnUpdate(){
	float _time = (float)glfwGetTime();
	float deltaTime = _time - lastTime;
	lastTime = _time;
	
	this->currentScene->Update(deltaTime);
}

Shader *Engine::GetShader(const std::string &name)
{
	auto it = shaders_.find(name);
	if (it != shaders_.end()) {
		return (*it).second;
	}
	return nullptr;
}

Texture* Engine::GetTexture(const std::string& name) {
	auto it = textures_.find(name);
	if (it != textures_.end()) {
		return (*it).second;
	}
	return nullptr;
}

Material* Engine::GetMaterial(const std::string& name) {
	auto it = materials_.find(name);
	if (it != materials_.end()) {
		return (*it).second;
	}
	return nullptr;
}

Material* Engine::GetMaterialById(unsigned id) {
	for (auto material : materials_) {
		if (id == material.second->GetID()) {
			return material.second;
		}
	}
	return nullptr;
}

void Engine::LoadTextures() {
	filesystem::path aPath(ASSET_PATH);
	auto path = aPath / "textures";
	if (filesystem::exists(path) && filesystem::is_directory(path)) {
		for (auto entry : filesystem::directory_iterator(path)) {
			auto _path = entry.path();
			if (!filesystem::is_directory(_path)) {
				auto texture = new Texture(_path.stem().string());
				texture->LoadFormFile(_path.string());
			}
		}
	}
}

void Engine::LoadShaders() {
	filesystem::path aPath(ASSET_PATH);
	auto path = aPath / "shaders";
	if (filesystem::exists(path) && filesystem::is_directory(path)) {
		for (auto entry : filesystem::directory_iterator(path)) {
			auto _path = entry.path();
			if (!filesystem::is_directory(_path)) {
				if (_path.extension().string() == ".shader") {
					auto shader = new Shader(_path.stem().string(), _path.string());
					shaders_[shader->GetName()] = shader;
				}
			}
		}
	}
}

void Engine::LoadMaterials() {
	auto ui_default = new Material("ui_default");
    ui_default->SetShader("unlit_pos_tex");
	ui_default->SetTexture(0, "white");
}

void Engine::HandleUIRootInput() {
	// auto left_mouse_btn_stat = glfwGetMouseButton(main_window_, GLFW_MOUSE_BUTTON_1);
	// if (left_mouse_btn_stat == GLFW_PRESS) {
	// 	double x, y;
	// 	glfwGetCursorPos(main_window_, &x, &y);
	// 	auto ui_control = ui_root_->FindChildControl(static_cast<float>(x), static_cast<float>(y));
	// 	if (ui_control) {
	// 		ui_control->OnMouseLeftButtonPress();
	// 	}
	// }
	// else if (left_mouse_btn_stat == GLFW_RELEASE) {
	// 	double x, y;
	// 	glfwGetCursorPos(main_window_, &x, &y);
	// 	auto ui_control = ui_root_->FindChildControl(static_cast<float>(x), static_cast<float>(y));
	// 	if (ui_control) {
	// 		ui_control->OnMouseLeftButtonRelease();
	// 	}
	// }
}

}