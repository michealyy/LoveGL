#include "scene_manager.h"
#include <algorithm>
#include <filesystem>
#include <glad/glad.h>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include "engine.h"

using namespace std;

namespace kd
{

SceneManager::SceneManager()
{
    root_ = new Entity();
    root_->name = "Root";
    entities_.push_back(root_);
}

SceneManager::~SceneManager()
{
    for (auto entity : entities_)
    {
        SafeDelete(entity);
    }
}

void SceneManager::Setup()
{
    for (auto entity : entities_)
    {
        entity->Setup();
    }
}

void SceneManager::Update(float deltaTime)
{
    if (Engine::GetInstance()->mainCamera == nullptr)
    {
        fprintf(stderr, "not find mainCamera. pls set Engine::GetInstance()->mainCamera!");
    }

    for (auto entity : entities_)
    {
        if (entity->visible)
            entity->Update(deltaTime);
    }

    Render();
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

    for (unsigned i = 0; i < model.bufferViews.size(); i++)
    {
        auto bufferView = model.bufferViews[i];
        if (bufferView.target == 0)
        {
            fprintf(stderr, "[WARNING][SceneManager]: bufferView.target is zero");
            continue;
        }

        auto buffer = model.buffers[bufferView.buffer];
        unsigned vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(bufferView.target, vbo);
        glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
        glBindBuffer(bufferView.target, 0);
        gltf_vbos_.push_back(vbo);
    }

    //贴图

    //材质

    //场景节点树
    auto scene = model.scenes[model.defaultScene];
    for (int i = 0; i < scene.nodes.size(); i++)
    {
        LoadGLTFNode(model, model.nodes[scene.nodes[i]]);
    }
}

void SceneManager::LoadGLTFNode(tinygltf::Model &model, tinygltf::Node &node)
{
    if (node.mesh < 0)
        return;
    
    auto _mesh = new Mesh();
    entities_.push_back(_mesh);
    //给一个丢失材质，没有材质信息时候能快速视觉反馈
    _mesh->material = Engine::GetInstance()->GetMaterial("miss");

    //读取仿射变换信息
    if (node.translation.size() > 0)
        _mesh->position = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
    if (node.scale.size() > 0)
        _mesh->scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
    if (node.rotation.size() > 0)
    {
        auto quat = glm::quat((float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2], (float)node.rotation[3]);
        auto euler = glm::eulerAngles(quat);
        _mesh->eulerAngles = glm::vec3(glm::degrees(euler.z), glm::degrees(euler.y), glm::degrees(euler.x));
    }

    //读取mesh的primitives，里面存放顶点和材质信息
    auto mesh = model.meshes[node.mesh];
    for (int i = 0; i < mesh.primitives.size(); i++)
    {
        auto primitive = mesh.primitives[i];
        auto submesh = new SubMesh();
        submesh->SetupFromGLTF(model, primitive);
        _mesh->submeshes.push_back(submesh);
    }

    //递归找子节点
    for (auto node_index : node.children)
        LoadGLTFNode(model, model.nodes[node_index]);
}

//TODO:用BSP树做排序，剔除和碰撞也可以
//https://www.cnblogs.com/hibernate6/archive/2012/01/18/2521941.html
void SceneManager::Render()
{
    //分开透明和不透明，然后给透明做排序
    vector<Mesh *> opaque_meshes;
    vector<Mesh *> transparent_meshes;
    for (auto entity : entities_)
    {
        auto mesh = dynamic_cast<Mesh *>(entity);
        if (mesh != nullptr && mesh->material != nullptr)
        {
            if (mesh->material->isTransparent)
                transparent_meshes.push_back(mesh);
            else
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
    
    for (auto subMesh : mesh->submeshes)
    {
        auto material = subMesh->material;
        if (material == nullptr || material->GetShader() == nullptr)
            material = Engine::GetInstance()->GetMaterial("miss");

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
        for(int i = 0; i < pointLights_.size(); i++)
        {
            auto pointLight = pointLights_[i];
            shader->SetVector3(string("pointLights[0].color").replace(12, 1, to_string(i)).c_str(), pointLight->color);
            shader->SetVector3(string("pointLights[0].position").replace(12, 1, to_string(i)).c_str(), pointLight->position);
            shader->SetFloat(string("pointLights[0].constant").replace(12, 1, to_string(i)).c_str(), pointLight->constant);
            shader->SetFloat(string("pointLights[0].linear").replace(12, 1, to_string(i)).c_str(), pointLight->linear);
            shader->SetFloat(string("pointLights[0].quadratic").replace(12, 1, to_string(i)).c_str(), pointLight->quadratic);
        }
        //聚光灯源
        shader->SetInt("spotLightsCount", (int)spotLights_.size());
        for(int i = 0; i < spotLights_.size(); i++)
        {
            auto spotLights = spotLights_[i];
            shader->SetVector3(string("spotLights[0].color").replace(11, 1, to_string(i)).c_str(), spotLights->color);
            shader->SetVector3(string("spotLights[0].position").replace(11, 1, to_string(i)).c_str(), spotLights->position);
            shader->SetVector3(string("spotLights[0].direction").replace(11, 1, to_string(i)).c_str(), spotLights->direction);
            shader->SetFloat(string("spotLights[0].innerAngle").replace(11, 1, to_string(i)).c_str(), spotLights->innerAngle);
            shader->SetFloat(string("spotLights[0].outerAngle").replace(11, 1, to_string(i)).c_str(), spotLights->outerAngle);
        }

        subMesh->Draw();
        Engine::GetInstance()->draw_call++;
    }

    // glBindVertexArray(mesh->vao);
    // //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    // glDrawElements(GL_TRIANGLES, (int)mesh->indices.size(), GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0);

    
}

} // namespace kd