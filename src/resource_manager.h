#pragma once

#include "common.h"
#include <core/shader.h>
#include <core/texture.h>
#include <core/material.h>
#include "vertex_attribute.h"

namespace kd
{

class ResourceManager final : public Singleton<ResourceManager>
{
public:
	explicit ResourceManager();
	~ResourceManager();
	
	void Setup();
	void Update(float deltaTime);

	Shader *GetShader(const std::string &name);
	Texture *GetTexture(const std::string &name);
	Material *GetMaterial(const std::string &name);
	MeshData *GetMesh(const std::string &name);
	inline void AddTexture(const std::string &name, Texture *texture) { textures_[name] = texture; }
	inline void AddMaterial(const std::string &name, Material *material) { materials_[name] = material; }
	
private:
	std::map<std::string, Texture *> textures_;
	std::map<std::string, Shader *> shaders_;
	std::map<std::string, Material *> materials_;
	std::map<std::string, MeshData *> meshes_;

	void LoadTextures();
	void LoadShaders();
	void LoadMaterials();
	void LoadMeshes();

	DISALLOW_COPY_AND_ASSIGN(ResourceManager)
};

} // namespace kd