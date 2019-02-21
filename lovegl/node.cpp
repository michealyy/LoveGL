#include "node.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include "engine.h"

using namespace std;
using namespace glm;

namespace kd
{

Node::Node()
{
}

Node::~Node()
{
}

void Node::Setup()
{
	UpdateModelMatrix();
}

void Node::Update(float deltaTime)
{
	if (!isStatic)
	{
		UpdateModelMatrix();
	}

	//TODO 支持多摄像机
	if (Engine::GetInstance()->mainCamera)
	{
    	localToCameraTransform = Engine::GetInstance()->mainCamera->GetViewMatrix() * worldTransform;
	}
}

void Node::AddChild(Node *child)
{
	if (child == nullptr)
	{
		return;
	}
	child->parent = this;
	children_.push_back(child);
}

Node *Node::FindChild(const string &name)
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

void Node::RemoveChild(const string &name)
{
}

void Node::RemoveAllChild()
{
	// for (auto child : children_)
	// {
	// 	SafeDelete(child);
	// }
	// children_.clear();
	// Remove from SceneManager
}

void Node::UpdateModelMatrix()
{
	mat4 temp(1.0f);
	localTransform = translate(temp, position) * glm::scale(temp, scale) * mat4_cast(quat(glm::radians(eulerAngles)));

	if (parent != nullptr)
	{
		worldTransform = parent->worldTransform * localTransform;
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