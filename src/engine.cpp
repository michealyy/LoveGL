#include "engine.h"
#include <filesystem>
#include <glm/glm.hpp>
#include "config.h"
#include "renderer.h"
#include "scene/scene_geometry.h"
#include "ui/font_manager.h"
#include "obj_loader.h"

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
	LoadMeshes();
	
	FontManager::GetInstance()->Setup("FreeSans.ttf");

	Renderer::GetInstance()->Setup();

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

	if (currentScene)
		this->currentScene->Update(deltaTime);

	if (ui_root)
		this->ui_root->Update(deltaTime);

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

MeshData *Engine::GetMesh(const std::string &name)
{
	auto it = meshes_.find(name);
	if (it != meshes_.end())
	{
		return (*it).second;
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
	ui_default->SetTexture("white");
}

void Engine::LoadMeshes()
{
	filesystem::path aPath(ASSET_PATH);
	auto path = aPath / "objs";
	if (filesystem::exists(path) && filesystem::is_directory(path))
	{
		for (auto entry : filesystem::directory_iterator(path))
		{
			auto _path = entry.path();
			if (!filesystem::is_directory(_path))
			{
				if (_path.extension().string() == ".obj")
				{
					objl::Loader loader;
					if (!loader.LoadFile(_path.string()))
						continue;

					auto mesh = new MeshData();
					for (int i = 0; i < loader.LoadedMeshes.size(); i++)
					{
						objl::Mesh curMesh = loader.LoadedMeshes[i];
						for (int j = 0; j < curMesh.Vertices.size(); j++)
						{
							va::Pos_Tex vertex;
							auto pos = curMesh.Vertices[j].Position;
							vertex.Position = glm::vec3(pos.X, pos.Y, pos.Z);
							auto uv = curMesh.Vertices[j].TextureCoordinate;
							vertex.TexCoords = glm::vec2(uv.X, uv.Y);
							mesh->vertices.push_back(vertex);
						}
						for (int j = 0; j < curMesh.Indices.size(); j++)
						{
							mesh->indices.push_back(curMesh.Indices[j]);
						}
					}
					meshes_[_path.stem().string()] = mesh;
				}
			}
		}
	}
}

} // namespace kd