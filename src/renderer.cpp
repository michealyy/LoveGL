#include "renderer.h"
#include <algorithm>
#include <glad/glad.h>
#include <glm/gtc/matrix_access.hpp>
#include "engine.h"

using namespace std;

namespace kd
{

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Render()
{
    //渲染非透明物体，开启深度测试，自动抛弃离摄像机远的片段
    glEnable(GL_DEPTH_TEST);
    for (auto opaque_mesh : opaque_meshes_)
    {
        DrawMesh(opaque_mesh);
    }
    glDisable(GL_DEPTH_TEST);

    //渲染透明物体，开启混合，需要自己处理渲染次序不一样造成不一样的效果
    SortTransparent();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto transparent_mesh : transparent_meshes_)
    {
        DrawMesh(transparent_mesh);
    }
    glDisable(GL_BLEND);

    opaque_meshes_.clear();
    transparent_meshes_.clear();
}

void Renderer::DrawMesh(Mesh *mesh)
{
    auto material = mesh->material;
    auto camera = Engine::GetInstance()->mainCamera;

    if (material == nullptr || camera == nullptr)
    {
        return;
    }

    material->Bind();
    material->SetMatrix("mvp", camera->projectMatrix * mesh->localToCameraTransform);

    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glDrawElements(GL_TRIANGLES, (int)mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::SortTransparent()
{
    //取离摄像机距离，位移变换的的Z轴作为排序依据
    sort(transparent_meshes_.begin(), transparent_meshes_.end(), [](Mesh *a, Mesh *b) -> bool {
        auto a_z = glm::column(a->localToCameraTransform, 3).z;
        auto b_z = glm::column(b->localToCameraTransform, 3).z;
        return a_z < b_z;
    });
}

} // namespace kd