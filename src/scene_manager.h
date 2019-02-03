#pragma once

#include <string>
#include <vector>
#include <tiny_gltf.h>
#include "common.h"
#include "entity_camera.h"
#include "entity_light.h"
#include "entity_mesh.h"

namespace kd
{

class SceneManager
{
  public:
	explicit SceneManager();
	virtual ~SceneManager();

	virtual void Setup();
	virtual void Update(float deltaTime);
	void LoadGLTF(const std::string &path);
	inline Entity *GetRoot() { return root_; }
	inline std::vector<Entity *> &GetEntities() { return entities_; };
	//void RemoveEntity();

	inline std::vector<unsigned> &GetGltfVbos() { return gltf_vbos_; };
	void Render();

  protected:
	Entity *root_ = nullptr;
	std::vector<Entity *> entities_;
	std::vector<Camera *> cameras_;
	DirectionalLight *directionalLight_ = nullptr;
	std::vector<PointLight *> pointLights_;
	std::vector<SpotLight *> spotLights_;

	std::vector<unsigned> gltf_vbos_;
	//tinygltf::Model gltf_model_;

  private:
	void DrawMesh(Camera *camera, Mesh *mesh);
	/*
	 * 递归解析glft节点
	*/
	void LoadGLTFNode(tinygltf::Model &model, tinygltf::Node &node);

	DISALLOW_COPY_AND_ASSIGN(SceneManager)

  public:
	template <typename ClassType>
	ClassType *CreateEntity(Entity *parent = nullptr)
	{
		if (parent == nullptr)
			parent = root_;

		auto entity = new ClassType();
		parent->AddChild(entity);
		entities_.push_back(entity);

		if (typeid(ClassType) == typeid(Camera))
			cameras_.push_back(dynamic_cast<Camera *>(entity));
		else if (typeid(ClassType) == typeid(DirectionalLight))
		{
			SafeDelete(directionalLight_);
			directionalLight_ = dynamic_cast<DirectionalLight *>(entity);
		}
		else if (typeid(ClassType) == typeid(PointLight))
			pointLights_.push_back(dynamic_cast<PointLight *>(entity));
		else if (typeid(ClassType) == typeid(SpotLight))
			spotLights_.push_back(dynamic_cast<SpotLight *>(entity));

		return entity;
	}
};

} // namespace kd