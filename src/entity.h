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

	void AddChild(Entity *child);
	Entity *FindChild(const std::string &name);
	void RemoveChild(const std::string &name);
	void RemoveAllChild();

	void UpdateModelMatrix();

	std::string name;

	//切场景不删除
	bool notDeleteByScene = false;

	glm::vec3 position{0.0f};
	glm::vec3 scale{1.0f};
	glm::vec3 eulerAngles{0.0f};
	glm::vec3 worldPosition{0.0f};
	glm::vec3 worldScale{0.0f};
	glm::mat4 localTransform{1.0f};
	glm::mat4 worldTransform{1.0f};
	//给顶点着色器传mvp矩阵时候用到；在透明物体排序时候用到，取离摄像机距离。
	glm::mat4 localToCameraTransform{1.0f};
	
	//静态实体，不会发生变换，不需要每帧运算变换矩阵
	bool isStatic = false;
	//是否显示
	bool visible = true;

  protected:
	std::vector<Entity *> children_;
	Entity *parent_ = nullptr;

private:
	
	DISALLOW_COPY_AND_ASSIGN(Entity)
};

} // namespace kd