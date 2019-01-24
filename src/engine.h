#pragma once

#include <string>
#include <map>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "material.h"
#include "texture.h"
#include "scene.h"
#include "entity_camera.h"
#include "ui/ui_root.h"

namespace kd
{

class Engine final : public Singleton<Engine>
{
	//friend Singleton<Engine>;
  public:
	explicit Engine();
	~Engine();

	inline void SetMainWindow(GLFWwindow *wnd) { main_window_ = wnd; }
	inline GLFWwindow *GetMainWindow() { return main_window_; }
	void OnSetup();
	void OnUpdate();

	Shader *GetShader(const std::string &name);
	Texture *GetTexture(const std::string &name);
	Material *GetMaterial(const std::string &name);
	Material *GetMaterialById(unsigned id);
	inline void AddTexture(const std::string &name, Texture *texture) { textures_[name] = texture; }
	inline void AddMaterial(const std::string &name, Material *material) { materials_[name] = material; }

	Scene *currentScene = nullptr;
	Camera *mainCamera = nullptr;

	ui::UIRoot *ui_root = nullptr;;
	//statistics
	int fps = 0;
	unsigned draw_call = 0;
	unsigned ui_draw_call = 0;
	unsigned ui_vertices = 0;

  private:
	GLFWwindow *main_window_;
	float lastTime = 0.0f;
	std::map<std::string, Texture *> textures_;
	std::map<std::string, Shader *> shaders_;
	std::map<std::string, Material *> materials_;

	void LoadTextures();
	void LoadShaders();
	void LoadMaterials();
	
	DISALLOW_COPY_AND_ASSIGN(Engine)
};

} // namespace kd