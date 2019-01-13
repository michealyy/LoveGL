#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "common.h"

namespace kd
{

class Entity
{
public:
	explicit Entity();
	virtual ~Entity();

	virtual void Setup();
	virtual void Update(float deltaTime);
	inline std::string GetName() { return name_; }

	virtual void AddChild(Entity *child);
	Entity *FindChild(const std::string &name);
	void RemoveChild(const std::string &name);
	void RemoveAllChild();

	void UpdateModelMatrix();
	glm::mat4 localTransform{1.0f};

protected:
	std::vector<Entity *> children_;
	std::string name_;
	Entity *parent_;
	//bool dirty_ = true;

	glm::vec3 position{0.0f};
	glm::vec3 scale{1.0f};
	glm::vec3 eulerAngles{0.0f};

private:
	DISALLOW_COPY_AND_ASSIGN(Entity)
};

} // namespace kd