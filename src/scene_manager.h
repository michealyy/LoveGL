#pragma once

#include <vector>
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

	inline Entity *GetRoot() { return root_; }
	inline std::vector<Entity *> &GetEntities() { return entities_; };
	// inline std::vector<Camera *> &GetCameras() { return cameras_; };
	// inline std::vector<Light *> &GetLights() { return lights_; };
	//void RemoveEntity();
	void Render();

  protected:
	Entity *root_ = nullptr;
	std::vector<Entity *> entities_;
	std::vector<Camera *> cameras_;
	DirectionalLight* directionalLight_ = nullptr;
	std::vector<PointLight *> pointLights_;
	std::vector<SpotLight *> spotLights_;

  private:
	void DrawMesh(Camera *camera, Mesh *mesh);

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