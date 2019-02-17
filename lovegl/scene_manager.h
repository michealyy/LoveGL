#pragma once

#include <string>
#include <vector>
#include <common.h>
#include <core/skybox.h>
#include <entity/camera.h>
#include <entity/light.h>
#include <entity/mesh.h>
#include <tiny_gltf.h>

namespace kd
{

class SceneManager
{
  public:
	explicit SceneManager();
	virtual ~SceneManager();

	virtual void Setup();
	virtual void Update(float deltaTime);

	inline Node *GetRoot() { return root_; }
	inline std::vector<Node *> &GetNodes() { return nodes_; };
	void RemoveNode(const std::string &name);
	Node *GetNode(const std::string &name);
	Camera *GetCamera(const std::string &name);
	void Render();

	void LoadGLTF(const std::string &path);
	void LoadIBL(const std::string &path);

  protected:
	Node *root_ = nullptr;
	std::vector<Node *> nodes_;
	std::vector<Camera *> cameras_;
	DirectionalLight *directionalLight_ = nullptr;
	std::vector<PointLight *> pointLights_;
	std::vector<SpotLight *> spotLights_;

	//skybox
	SkyBox *skybox_ = nullptr;
	
	//IBL
	unsigned brdfLUT_;
	unsigned diffuseCubemap_;
	unsigned specularCubemap_;

  private:
	void DrawMesh(Camera *camera, Mesh *mesh);

	// 递归解析glft节点
	void LoadGLTFNode(tinygltf::Model &model, tinygltf::Node &node, Node *parent);

	DISALLOW_COPY_AND_ASSIGN(SceneManager)

  public:
	template <typename ClassType>
	ClassType *CreateNode(Node *parent = nullptr)
	{
		if (parent == nullptr)
			parent = root_;

		auto node = new ClassType();
		parent->AddChild(node);
		nodes_.push_back(node);

		if (typeid(ClassType) == typeid(Camera))
			cameras_.push_back(dynamic_cast<Camera *>(node));
		else if (typeid(ClassType) == typeid(DirectionalLight))
		{
			SafeDelete(directionalLight_);
			directionalLight_ = dynamic_cast<DirectionalLight *>(node);
		}
		else if (typeid(ClassType) == typeid(PointLight))
			pointLights_.push_back(dynamic_cast<PointLight *>(node));
		else if (typeid(ClassType) == typeid(SpotLight))
			spotLights_.push_back(dynamic_cast<SpotLight *>(node));

		return node;
	}
};

} // namespace kd