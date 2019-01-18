#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "common.h"

namespace kd
{

class Scene;

class Entity
{
public:
	explicit Entity();
	virtual ~Entity();

	virtual void Setup();
	virtual void Update(float deltaTime);

	void AddChild(Entity *child);
	Entity *FindChild(const std::string &name);
	void RemoveChild(const std::string &name);
	void RemoveAllChild();

	void UpdateModelMatrix();

	std::string name;

	bool notDeleteByScene = false;
	Scene *scene = nullptr;

	bool isStatic = false;
	glm::vec3 position{0.0f};
	glm::vec3 scale{1.0f};
	glm::vec3 eulerAngles{0.0f};
	glm::mat4 localTransform{1.0f};
	glm::mat4 worldTransform{1.0f};

	bool isTransparent = false;

protected:
	std::vector<Entity *> children_;
	Entity *parent_ = nullptr;

private:
	DISALLOW_COPY_AND_ASSIGN(Entity)
};

} // namespace kd