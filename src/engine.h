#pragma once

#include <string>
#include <map>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "material.h"
#include "texture.h"

#include "entity_box.h"

namespace kd {
	class Engine final : public Singleton<Engine> {
		//friend Singleton<Engine>;
		//friend UIBatchRender;
	public:
		explicit Engine();
		~Engine();

		inline void SetMainWindow(GLFWwindow* wnd) { main_window_ = wnd; }
		void OnSetup();
		void OnUpdate();
		
		Shader* GetShader(const std::string& name);
		Texture* GetTexture(const std::string& name);
		Material* GetMaterial(const std::string& name);
		Material* GetMaterialById(unsigned id);
		inline void AddTexture(const std::string& name, Texture* texture) { textures_[name] = texture; }
		inline void AddMaterial(const std::string& name, Material* material) { materials_[name] = material; }

	private:
		GLFWwindow* main_window_;
		std::map<std::string, Texture*> textures_;
		std::map<std::string, Shader*> shaders_;
		std::map<std::string, Material*> materials_;

		Box* box1 = nullptr;

		void LoadTextures();
		void LoadShaders();
		void LoadMaterials();
		void HandleUIRootInput();
		DISALLOW_COPY_AND_ASSIGN(Engine)
	};
}