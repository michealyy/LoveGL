#include "scene_manager.h"
#include <algorithm>
#include <filesystem>
#include <glad/glad.h>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include <resource_manager.h>
#include "engine.h"

using namespace std;

namespace kd
{

SceneManager::SceneManager()
{
    root_ = new Node();
    root_->name = "Root";
    nodes_.push_back(root_);
}

SceneManager::~SceneManager()
{
    for (auto node : nodes_)
    {
        SafeDelete(node);
    }
}

void SceneManager::Setup()
{
    for (auto node : nodes_)
    {
        node->Setup();
    }
}

void SceneManager::Update(float deltaTime)
{
    if (Engine::GetInstance()->mainCamera == nullptr)
    {
        fprintf(stderr, "not find mainCamera. pls set Engine::GetInstance()->mainCamera!");
    }

    for (auto node : nodes_)
    {
        if (node->visible)
            node->Update(deltaTime);
    }

    Render();
}

void SceneManager::RemoveNode(const std::string &name)
{
}

Node *SceneManager::GetNode(const std::string &name)
{
    for (auto node : nodes_)
    {
        if (node->name == name)
            return node;
    }

    return nullptr;
}

Camera *SceneManager::GetCamera(const std::string &name)
{
    for (auto camera : cameras_)
    {
        if (camera->name == name)
            return camera;
    }

    return nullptr;
}

//TODO:用BSP树做排序，剔除和碰撞也可以
//https://www.cnblogs.com/hibernate6/archive/2012/01/18/2521941.html
void SceneManager::Render()
{
    //分开透明和不透明，然后给透明做排序
    vector<Mesh *> opaque_meshes;
    vector<Mesh *> transparent_meshes;
    for (auto node : nodes_)
    {
        auto mesh = dynamic_cast<Mesh *>(node);
        // if (mesh != nullptr && mesh->material != nullptr)
        // {
        //     if (mesh->material->isTransparent)
        //         transparent_meshes.push_back(mesh);
        //     else
        //         opaque_meshes.push_back(mesh);
        // }
        if (mesh != nullptr)
        {
            opaque_meshes.push_back(mesh);
        }
    }

    //取离摄像机距离，位移变换的的Z轴作为排序依据
    sort(transparent_meshes.begin(), transparent_meshes.end(), [](Mesh *a, Mesh *b) -> bool {
        auto a_z = glm::column(a->localToCameraTransform, 3).z;
        auto b_z = glm::column(b->localToCameraTransform, 3).z;
        return a_z < b_z;
    });

    //多摄像机渲染
    sort(cameras_.begin(), cameras_.end(), [](Camera *a, Camera *b) -> bool { return a->depth < b->depth; });
    for (auto camera : cameras_)
    {
        if (!camera->isOverlay)
        {
            glClearColor(camera->clearColor.x, camera->clearColor.y, camera->clearColor.z, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        //glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        for (auto opaque_mesh : opaque_meshes)
        {
            DrawMesh(camera, opaque_mesh);
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        for (auto transparent_mesh : transparent_meshes)
        {
            DrawMesh(camera, transparent_mesh);
        }
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    }
}

void SceneManager::DrawMesh(Camera *camera, Mesh *mesh)
{
    if (camera == nullptr)
        return;

    for (auto subMesh : mesh->subMeshes)
    {
        auto material = subMesh->material;
        if (material == nullptr || material->GetShader() == nullptr)
            material = ResourceManager::GetInstance()->GetMaterial("miss");

        material->Bind();
        auto shader = material->GetShader();
        shader->SetMatrix("mvp", camera->projectMatrix * mesh->localToCameraTransform);
        shader->SetMatrix("model", mesh->worldTransform);
        shader->SetVector3("viewPos", camera->worldPosition);
        //平行光源
        shader->SetVector3("directionalLight.color", directionalLight_->color);
        shader->SetVector3("directionalLight.direction", directionalLight_->direction);
        //点光源
        shader->SetInt("pointLightsCount", (int)pointLights_.size());
        for (int i = 0; i < pointLights_.size(); i++)
        {
            auto pointLight = pointLights_[i];
            shader->SetVector3(string("pointLights[0].color").replace(12, 1, to_string(i)).c_str(), pointLight->color);
            shader->SetVector3(string("pointLights[0].position").replace(12, 1, to_string(i)).c_str(), pointLight->worldPosition);
            shader->SetFloat(string("pointLights[0].constant").replace(12, 1, to_string(i)).c_str(), pointLight->constant);
            shader->SetFloat(string("pointLights[0].linear").replace(12, 1, to_string(i)).c_str(), pointLight->linear);
            shader->SetFloat(string("pointLights[0].quadratic").replace(12, 1, to_string(i)).c_str(), pointLight->quadratic);
        }
        //聚光灯源
        shader->SetInt("spotLightsCount", (int)spotLights_.size());
        for (int i = 0; i < spotLights_.size(); i++)
        {
            auto spotLights = spotLights_[i];
            shader->SetVector3(string("spotLights[0].color").replace(11, 1, to_string(i)).c_str(), spotLights->color);
            shader->SetVector3(string("spotLights[0].position").replace(11, 1, to_string(i)).c_str(), spotLights->worldPosition);
            shader->SetVector3(string("spotLights[0].direction").replace(11, 1, to_string(i)).c_str(), spotLights->direction);
            shader->SetFloat(string("spotLights[0].innerAngle").replace(11, 1, to_string(i)).c_str(), spotLights->innerAngle);
            shader->SetFloat(string("spotLights[0].outerAngle").replace(11, 1, to_string(i)).c_str(), spotLights->outerAngle);
        }

        subMesh->Draw();
        Engine::GetInstance()->draw_call++;
    }
}

void SceneManager::LoadGLTF(const std::string &path)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF gltf_ctx;
    std::string err;
    std::string warn;
    filesystem::path _path(path);

    bool ret = false;
    if (_path.extension().string() == "glb")
        ret = gltf_ctx.LoadBinaryFromFile(&model, &err, &warn, path.c_str());
    else
        ret = gltf_ctx.LoadASCIIFromFile(&model, &err, &warn, path.c_str());

    if (!warn.empty())
        fprintf(stderr, "[WARNING][SceneManager]: %s\n", warn.c_str());

    if (!err.empty())
        fprintf(stderr, "[ERROR][SceneManager]: %s\n", err.c_str());

    if (!ret)
    {
        fprintf(stderr, "[ERROR][SceneManager]: Failed to parse glTF\n");
        return;
    }

    //贴图
    for (auto texture : model.textures)
    {
        auto image = model.images[texture.source];
        auto texture = new Texture(image.name);
        texture->LoadFormData(image.width, image.height, image.component, image.image);
    }

    //材质
    for (auto material : model.materials)
    {
        auto mat = new Material(material.name);
        mat->SetShader("blinn_phong_normal");
        //给一个丢失材质，没有材质信息时候能快速视觉反馈
        mat->SetTexture("miss");
        //基础颜色
        if (material.values.find("baseColorFactor") != material.values.end())
        {
            auto baseColor = material.values["baseColorFactor"].ColorFactor();
            if (baseColor.size() >= 3)
                mat->SetColor(glm::vec3(baseColor[0], baseColor[1], baseColor[2]));
            if (baseColor.size() == 4)
                mat->SetAlpha((float)baseColor[3]);
        }
        //基础贴图
        auto baseColorTextureJson = material.values["baseColorTexture"].json_double_value;
        if (baseColorTextureJson.find("index") != baseColorTextureJson.end())
        {
            //TODO: 极少情况下，多张贴图拥有独立多个uv，这里固定使用TEXCOORD_0作为所以贴图uv坐标
            auto uv_channel = baseColorTextureJson.find("texCoord")->second;

            auto texture = model.textures[(size_t)baseColorTextureJson.find("index")->second];
            auto image = model.images[texture.source];
            mat->SetTexture(image.name, 0, "mainTexture");
        }
        //法线贴图
        auto normalTextureJson = material.additionalValues["normalTexture"].json_double_value;
        if (normalTextureJson.find("index") != normalTextureJson.end())
        {
            auto texture = model.textures[(size_t)normalTextureJson.find("index")->second];
            auto image = model.images[texture.source];
            mat->SetTexture(image.name, 1, "normalMap");
        }
    }

    //场景节点树
    auto scene = model.scenes[model.defaultScene];
    for (int i = 0; i < scene.nodes.size(); i++)
    {
        LoadGLTFNode(model, model.nodes[scene.nodes[i]], root_);
    }
}

void SceneManager::LoadGLTFNode(tinygltf::Model &model, tinygltf::Node &node, Node *parent)
{
    Node *_node = nullptr;

    /*****解析摄像机*****/
    if (node.camera >= 0)
    {
        auto _camera = new Camera();
        _node = _camera;
        cameras_.push_back(_camera);

        //TODO: 解析其他摄像机信息，正交 透视 进远面
    }
    /*****解析灯光*****/
    else if (node.extensions.find("KHR_lights_punctual") != node.extensions.end() && node.extensions["KHR_lights_punctual"].Has("light"))
    {
        auto light_index = node.extensions["KHR_lights_punctual"].Get("light").Get<int>();
        auto light_info = model.extensions["KHR_lights_punctual"].Get("lights").Get(light_index);

        auto light_type = light_info.Get("type").Get<string>();
        if (light_type == "Spot")
        {
        }
        else //默认点光
        {
            auto pointLight = new PointLight();
            _node = pointLight;
            pointLights_.push_back(pointLight);

            auto color = light_info.Get("color");
            float r, g, b;
            if (color.Get(0).IsInt())
                r = (float)color.Get(0).Get<int>();
            else
                r = (float)color.Get(0).Get<double>();
            if (color.Get(1).IsInt())
                g = (float)color.Get(1).Get<int>();
            else
                g = (float)color.Get(1).Get<double>();
            if (color.Get(2).IsInt())
                b = (float)color.Get(2).Get<int>();
            else
                b = (float)color.Get(2).Get<double>();
            pointLight->color = glm::vec3(r, g, b);
        }
    }
    /*****解析网格*****/
    else if (node.mesh >= 0)
    {
        auto _mesh = new Mesh();
        _node = _mesh;

        //读取mesh的primitives，里面存放顶点和材质信息
        auto mesh = model.meshes[node.mesh];
        for (int i = 0; i < mesh.primitives.size(); i++)
        {
            auto primitive = mesh.primitives[i];
            auto submesh = new SubMesh();
            submesh->SetupFromGLTF(model, primitive);
            _mesh->AddSubMesh(submesh);
        }
    }
    else
    {
        _node = new Node();
    }

    _node->name = node.name;
    parent->AddChild(_node);
    nodes_.push_back(_node);

    //读取仿射变换信息
    if (node.translation.size() > 0)
        _node->position = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
    if (node.scale.size() > 0)
        _node->scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
    if (node.rotation.size() > 0)
    {
        auto quat = glm::quat((float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2], (float)node.rotation[3]);
        auto euler = glm::eulerAngles(quat);
        _node->eulerAngles = glm::vec3(glm::degrees(euler.z), glm::degrees(euler.y), glm::degrees(euler.x));
    }

    //递归找子节点
    for (auto node_index : node.children)
        LoadGLTFNode(model, model.nodes[node_index], _node);
}

} // namespace kd