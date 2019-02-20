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

	SceneManager *sceneManager = nullptr;

	Camera *mainCamera = nullptr;
	ui::UIRoot *ui_root = nullptr;
	Node *selected_node = nullptr;
	//global setting
	float gamma = 2.2f;
	float exposure = 1.f;
	unsigned msaaSample = 8;
	
	//statistics
	int fps = 0;
	unsigned draw_call = 0;
	unsigned ui_draw_call = 0;
	unsigned ui_vertices = 0;

  private:
	App *app_ = nullptr;
	GLFWwindow *main_window_;
	float lastTime = 0.0f;

	DISALLOW_COPY_AND_ASSIGN(Engine)
};

} // namespace kd