#include "resource_manager.h"
#include <filesystem>
#include <obj_loader.h>
#include "config.h"

using namespace std;

namespace kd
{

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Setup()
{
    LoadTextures();
    LoadShaders();
    LoadMaterials();
    //LoadMeshes();
}
void ResourceManager::Update(float deltaTime)
{
}

Shader *ResourceManager::GetShader(const std::string &name)
{
    auto it = shaders_.find(name);
    if (it != shaders_.end())
    {
        return (*it).second;
    }
    return nullptr;
}

Texture *ResourceManager::GetTexture(const std::string &name)
{
    auto it = textures_.find(name);
    if (it != textures_.end())
    {
        return (*it).second;
    }
    return nullptr;
}

Material *ResourceManager::GetMaterial(const std::string &name)
{
    auto it = materials_.find(name);
    if (it != materials_.end())
    {
        return (*it).second;
    }
    return nullptr;
}

MeshData *ResourceManager::GetMesh(const std::string &name)
{
	auto it = meshes_.find(name);
	if (it != meshes_.end())
	{
		return (*it).second;
	}
	return nullptr;
}


void ResourceManager::LoadTextures()
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

void ResourceManager::LoadShaders()
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
                if (_path.extension().string() == ".glsl")
                {
                    auto shader = new Shader(_path.stem().string(), _path.string());
                    shaders_[shader->GetName()] = shader;
                }
            }
        }
    }
}

void ResourceManager::LoadMaterials()
{
    auto ui_default = new Material("ui_default");
    ui_default->SetShader("unlit_pos_tex");
    ui_default->SetTexture("white");

    auto miss = new Material("miss");
    miss->SetShader("unlit_pos_tex");
    miss->SetTexture("miss");
}

void ResourceManager::LoadMeshes()
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
						auto curMesh = loader.LoadedMeshes[i];
						for (int j = 0; j < curMesh.Vertices.size(); j++)
						{
							auto vertices = curMesh.Vertices[j];
							va::P_T_N_T_B vertex;
							auto pos = vertices.Position;
							vertex.Position = glm::vec3(pos.X, pos.Y, pos.Z);
							auto uv = vertices.TextureCoordinate;
							vertex.TexCoords = glm::vec2(uv.X, uv.Y);
							auto normal = vertices.Normal;
							vertex.Normal = glm::vec3(normal.X, normal.Y, normal.Z);
							mesh->vertices.push_back(vertex);
							//TODO: Tangent + Bitangent
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