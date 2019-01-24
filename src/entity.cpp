#include "entity.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include "scene.h"
#include "engine.h"

using namespace std;
using namespace glm;

namespace kd
{

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::Setup()
{
	UpdateModelMatrix();
}

void Entity::Update(float deltaTime)
{
	if (!isStatic)
	{
		UpdateModelMatrix();
	}

	if (Engine::GetInstance()->mainCamera)
	{
    	localToCameraTransform = Engine::GetInstance()->mainCamera->GetViewMatrix() * worldTransform;
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

	if (scene != nullptr)
	{
		scene->AddEntity(child);
	}
}

Entity *Entity::FindChild(const string &name)
{
	for (auto child : children_)
	{
		if (child->name == name)
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
	// for (auto child : children_)
	// {
	// 	SafeDelete(child);
	// }
	// children_.clear();
	// Remove from Scene
}

void Entity::UpdateModelMatrix()
{
	mat4 temp(1.0f);
	localTransform = translate(temp, position) * glm::scale(temp, scale) * mat4_cast(quat(glm::radians(-eulerAngles)));

	if (parent_ != nullptr)
	{
		worldTransform = parent_->worldTransform * localTransform;
	}
	else
	{
		worldTransform = localTransform;
	}
	
	quat rotation;
	vec3 skew;
	vec4 perspective;
	decompose(worldTransform, worldScale, rotation, worldPosition, skew, perspective);
	//TODO: worldEulerAngles
}

} // namespace kd