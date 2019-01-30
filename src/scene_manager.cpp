#include "scene_manager.h"
#include <glad/glad.h>
#include <algorithm>
#include <glm/gtc/matrix_access.hpp>
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
        glEnable(GL_CULL_FACE);
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
    auto material = mesh->material;
    if (material == nullptr || camera == nullptr)
    {
        return;
    }

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
        shader->SetVector3("pointLights[0].color", pointLight->color);
        shader->SetVector3("pointLights[0].position", pointLight->position);
        shader->SetFloat("pointLights[0].constant", pointLight->constant);
        shader->SetFloat("pointLights[0].linear", pointLight->linear);
        shader->SetFloat("pointLights[0].quadratic", pointLight->quadratic);
    }
    //聚光灯源
    shader->SetInt("spotLightsCount", (int)spotLights_.size());
    for(int i = 0; i < spotLights_.size(); i++)
    {
        auto spotLights = spotLights_[i];
        shader->SetVector3("spotLights[0].color", spotLights->color);
        shader->SetVector3("spotLights[0].position", spotLights->position);
        shader->SetVector3("spotLights[0].direction", spotLights->direction);
        shader->SetFloat("spotLights[0].angle", spotLights->angle);
    }

    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glDrawElements(GL_TRIANGLES, (int)mesh->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    Engine::GetInstance()->draw_call++;
}

} // namespace kd