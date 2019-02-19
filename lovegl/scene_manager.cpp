#include "scene_manager.h"
#include <algorithm>
#include <filesystem>
#include <gli/gli.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include <resource_manager.h>
#include <config.h>
#include <engine.h>

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
    SafeDelete(skybox_);
    for (auto node : nodes_)
    {
        SafeDelete(node);
    }
}

void SceneManager::Setup()
{
    if (skybox_)
        skybox_->Setup();

    for (auto node : nodes_)
    {
        node->Setup();
    }

    if (Engine::GetInstance()->mainCamera == nullptr)
        Engine::GetInstance()->mainCamera = cameras_[0];
}

void SceneManager::Update(float deltaTime)
{
    for (auto node : nodes_)
    {
        node->Update(deltaTime);
    }

    if (skybox_)
        skybox_->Render();

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
        if (mesh != nullptr)
        {
            bool isTransparent = false;
            for (auto subMesh : mesh->subMeshes)
            {
                if (subMesh->material && subMesh->material->isTransparent)
                    isTransparent = true;
            }
            if (isTransparent)
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
        //支持rtt。没有就直接绘制到颜色缓冲区
        if (camera->renderTarget)
        {
            camera->renderTarget->Bind();
            glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //三角面模式，线或者填充
        if (camera->isLinePolygon)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //可选择不叠加摄像机渲染结果
        if (!camera->isOverlay)
        {
            glClearColor(camera->clearColor.x, camera->clearColor.y, camera->clearColor.z, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        //面剔除
        //glCullFace(GL_FRONT);
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

        //后处理
        if (camera->GetPostProcessing())
            camera->GetPostProcessing()->Draw();
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

        //给pbr shader传送IBL信息
        if(shader->GetName() == "pbr")
        {
            shader->SetInt("irradianceMap", 0);
            shader->SetInt("prefilterMap", 1);
            shader->SetInt("brdfLUT", 2);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, diffuseCubemap_);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, specularCubemap_);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, brdfLUT_);
        }

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
        mat->SetShader("pbr");
        mat->SetVector3("albedo", glm::vec3(0.f, 0.f, 0.f));
        mat->SetFloat("roughness", 0.f);
        mat->SetFloat("metallic", 0.f);
        //基础颜色
        if (material.values.find("baseColorFactor") != material.values.end())
        {
            auto baseColor = material.values["baseColorFactor"].ColorFactor();
            if (baseColor.size() >= 3)
                mat->SetVector3("albedo", glm::vec3(baseColor[0], baseColor[1], baseColor[2]));
            if (baseColor.size() == 4)
                mat->SetAlpha((float)baseColor[3]);
        }
        //粗糙度
        if (material.values.find("roughnessFactor") != material.values.end())
            mat->SetFloat("roughness", (float)material.values["roughnessFactor"].number_value);
        //金属度
        if (material.values.find("metallicFactor") != material.values.end())
            mat->SetFloat("metallic", (float)material.values["metallicFactor"].number_value);
        //基础贴图
        auto baseColorTextureJson = material.values["baseColorTexture"].json_double_value;
        if (baseColorTextureJson.find("index") != baseColorTextureJson.end())
        {
            //TODO: 极少情况下，多张贴图拥有独立多个uv，这里固定使用TEXCOORD_0作为所以贴图uv坐标
            auto uv_channel = baseColorTextureJson.find("texCoord")->second;

            auto texture = model.textures[(size_t)baseColorTextureJson.find("index")->second];
            auto image = model.images[texture.source];
            mat->SetTexture(image.name, 3, "albedoTexture");
        }
        //金属度B通道+粗糙度G通道贴图
        auto metallicRoughnessTextureJson = material.values["metallicRoughnessTexture"].json_double_value;
        if (metallicRoughnessTextureJson.find("index") != metallicRoughnessTextureJson.end())
        {
            auto texture = model.textures[(size_t)metallicRoughnessTextureJson.find("index")->second];
            auto image = model.images[texture.source];
            mat->SetTexture(image.name, 4, "metallicRoughnessTexture");
        }
        //法线贴图
        auto normalTextureJson = material.additionalValues["normalTexture"].json_double_value;
        if (normalTextureJson.find("index") != normalTextureJson.end())
        {
            auto texture = model.textures[(size_t)normalTextureJson.find("index")->second];
            auto image = model.images[texture.source];
            //mat->SetTexture(image.name, 7, "normalMap");
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
        auto x = glm::degrees(euler.x);
        auto y = glm::degrees(euler.y);
        auto z = glm::degrees(euler.z);
        _node->eulerAngles = glm::vec3(z, y, x);
    }

    //递归找子节点
    for (auto node_index : node.children)
        LoadGLTFNode(model, model.nodes[node_index], _node);
}

void SceneManager::LoadIBL(const std::string &path)
{
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    filesystem::path aPath("assets/ibl/");

    //brdfLUT
    auto brdfLUT = gli::load_dds((aPath / path / "brdf.dds").string());
    if (brdfLUT.empty())
    {
        fprintf(stderr, "[SceneManager] load brdf.dds error: %s\n", path.c_str());
        return;
    }
    glGenTextures(1, &brdfLUT_);
    glBindTexture(GL_TEXTURE_2D, brdfLUT_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, brdfLUT.extent().x, brdfLUT.extent().y, 0, GL_RGB, GL_UNSIGNED_BYTE, brdfLUT.data());

    //diffuse
    auto diffuse = gli::load_dds((aPath / path / "diffuse.dds").string());
    if (diffuse.empty())
    {
        fprintf(stderr, "[SceneManager] load diffuse.dds error: %s\n", path.c_str());
        return;
    }
    glGenTextures(1, &diffuseCubemap_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, diffuseCubemap_);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (unsigned i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, diffuse.extent().x, diffuse.extent().y, 0, GL_RGB, GL_UNSIGNED_BYTE, diffuse.data(0, i, 0));

    //specular
    specularCubemap_ = GLILoadCreateGLTexture((aPath / path / "specular.dds").string());

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    if (skybox_)
        SafeDelete(skybox_);

    skybox_ = new SkyBox();
    skybox_->cubeMap = specularCubemap_;
}

} // namespace kd