#pragma once

#include <string>
#include <map>
#include <GLFW/glfw3.h>
#include <core/texture.h>
#include <core/material.h>
#include "vertex_attribute.h"
#include "entity_camera.h"
#include "ui/ui_root.h"
#include "scene_manager.h"
#include "app.h"

namespace kd
{

class Engine final : public Singleton<Engine>
{
	//friend Singleton<Engine>;
  public:
	explicit Engine();
	~Engine();

	inline void SetApp(App *app) { app_ = app; }
	inline void SetMainWindow(GLFWwindow *wnd) { main_window_ = wnd; }
	inline GLFWwindow *GetMainWindow() { return main_window_; }
	void OnSetup();
	void OnUpdate();

	Shader *GetShader(const std::string &name);
	Texture *GetTexture(const std::string &name);
	Material *GetMaterial(const std::string &name);
	Material *GetMaterialById(unsigned id);
	MeshData *GetMesh(const std::string &name);
	inline void AddTexture(const std::string &name, Texture *texture) { textures_[name] = texture; }
	inline void AddMaterial(const std::string &name, Material *material) { materials_[name] = material; }

	SceneManager* sceneManager = nullptr;
	
	Camera *mainCamera = nullptr;
	ui::UIRoot *ui_root = nullptr;
	Entity *selected_entity = nullptr;
	//statistics
	int fps = 0;
	unsigned draw_call = 0;
	unsigned ui_draw_call = 0;
	unsigned ui_vertices = 0;

  private:
  	App* app_ = nullptr;
	GLFWwindow *main_window_;
	float lastTime = 0.0f;
	std::map<std::string, Texture *> textures_;
	std::map<std::string, Shader *> shaders_;
	std::map<std::string, Material *> materials_;
	std::map<std::string, MeshData *> meshes_;

	void LoadTextures();
	void LoadShaders();
	void LoadMaterials();
	void LoadMeshes();

	DISALLOW_COPY_AND_ASSIGN(Engine)
};

} // namespace kd