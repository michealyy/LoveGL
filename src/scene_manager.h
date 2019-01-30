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
	inline std::vector<Camera *> &GetCameras() { return cameras_; };
	inline std::vector<Light *> &GetLights() { return lights_; };
	//void RemoveEntity();

protected:
	Entity *root_ = nullptr;
	std::vector<Entity *> entities_;
	std::vector<Camera *> cameras_;
	std::vector<Light *> lights_;

private:
	DISALLOW_COPY_AND_ASSIGN(SceneManager)

public:
	template <typename ClassType>
	ClassType *CreateEntity(Entity *parent = nullptr)
	{
		if (parent == nullptr)
			parent = root_;

		if (typeid(ClassType) == typeid(Mesh))
		{
			auto mesh = new Mesh();
			parent->AddChild(mesh);
			entities_.push_back(mesh);
			return dynamic_cast<ClassType *>(mesh);
		}
		else if (typeid(ClassType) == typeid(Camera))
		{
			auto camera = new Camera();
			parent->AddChild(camera);
			cameras_.push_back(camera);
			entities_.push_back(camera);
			return dynamic_cast<ClassType *>(camera);
		}
		else if (typeid(ClassType) == typeid(Light))
		{
			auto light = new Light();
			parent->AddChild(light);
			lights_.push_back(light);
			entities_.push_back(light);
			return dynamic_cast<ClassType *>(light);
		}
		else
		{
			auto entity = new Entity();
			parent->AddChild(entity);
			entities_.push_back(entity);
			return dynamic_cast<ClassType *>(entity);
		}
	}
};

} // namespace kd