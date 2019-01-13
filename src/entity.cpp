#include "entity.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace std;
using namespace glm;

namespace kd
{

Entity::Entity()
{
}

Entity::~Entity()
{
	for (auto child : children_)
	{
		SafeDelete(child);
	}
}

void Entity::Setup()
{
	for (auto child : children_)
	{
		child->Setup();
	}
}

void Entity::Update(float deltaTime)
{
	for (auto child : children_)
	{
		child->Update(deltaTime);
	}
}

void Entity::AddChild(Entity *child)
{
	if (child == nullptr)
	{
		return;
	}
	child->parent_ = this;
	children_.push_back(child);

	//dirty_ = true;
}

Entity *Entity::FindChild(const string &name)
{
	for (auto child : children_)
	{
		if (child->GetName() == name)
		{
			return child;
		}
	}

	return nullptr;
}

void Entity::RemoveChild(const string &name)
{
}

void Entity::RemoveAllChild()
{
	children_.clear();
}

void Entity::UpdateModelMatrix()
{
	//return translate(mat4(1.0f), position) * scale(mat4(1.0f), scale) * mat4_cast(quat(glm::radians(eulerAngles)));

	translate(mat4(1.0f), position);
	glm::scale(mat4(1.0f), this->scale);
	//mat4_cast(quat(glm::radians(eulerAngles)));

	//return translate(mat4(1.0f), position) * glm::scale(mat4(1.0f), this->scale);
	localTransform = mat4(1.0f);
}

 }// namespace kdpace kd