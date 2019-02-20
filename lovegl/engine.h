#pragma once

#include <string>
#include <map>
#include <GLFW/glfw3.h>
#include <core/texture.h>
#include <core/material.h>

#include <entity/camera.h>
#include "ui/ui_root.h"
#include "scene_manager.h"
#include "app.h"

namespace kd
{

class Engine final : public Singleton<Engine>
{
  public:
	explicit Engine();
	~Engine();

	inline void SetApp(App *app) { app_ = app; }
	inline void SetMainWindow(GLFWwindow *wnd) { main_window_ = wnd; }
	inline GLFWwindow *GetMainWindow() { return main_window_; }
	void Setup();
	void Update();

	//global setting
	float gamma = 2.2f;
	float exposure = 1.f;
	unsigned msaaSample = 8;

	//scene
	SceneManager *sceneManager = nullptr;
	Camera *mainCamera = nullptr;
	ui::UIRoot *uiRoot = nullptr;
	Node *selectedEntity = nullptr;
	
	//statistics
	int fps = 0;
	unsigned drawCall = 0;
	unsigned uiDrawCall = 0;
	unsigned uiVertices = 0;

  private:
	App *app_ = nullptr;
	GLFWwindow *main_window_;
	float lastTime = 0.0f;
	void PickEntity();

	DISALLOW_COPY_AND_ASSIGN(Engine)
};

} // namespace kd