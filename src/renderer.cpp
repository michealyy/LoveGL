﻿#include "renderer.h"
#include <algorithm>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
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

void Renderer::SetupUIBatchRender()
{
    //2DUI投影矩阵
    int width, height;
    glfwGetWindowSize(Engine::GetInstance()->GetMainWindow(), &width, &height);
    //坐下为原点，分辨率为大小
    glm::mat4 projectionMatrix = glm::ortho(0.f, (float)width, 0.f, (float)height, -10.f, 10.f);
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    ui_project_view_matrix_ = projectionMatrix * viewMatrix;

    //批渲染支持。先申请一个buffer，后面累积完顶点数据再复制到这个buffer
    glGenVertexArrays(1, &ui_vao_);
    glBindVertexArray(ui_vao_);
    glGenBuffers(2, ui_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, ui_vbo_[0]);
    //Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(va::Pos_Tex), (void *)0);
    //Texture Coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(va::Pos_Tex), (void *)offsetof(va::Pos_Tex, TexCoords));
    //element
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_vbo_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui_indices_[0]) * INDEX_VBO_SIZE, ui_indices_, GL_STATIC_DRAW);
    //unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::AddMesh(Mesh *mesh)
{
    if (mesh != nullptr && mesh->material != nullptr)
    {
        if (mesh->material->isTransparent)
            AddTransparent(mesh);
        else
            AddOpaque(mesh);
    }
}

void Renderer::Render()
{
    RenderSkyBox();
    Render3DObjects();
    BatchRenderUI();
}

void Renderer::RenderSkyBox()
{
}

void Renderer::Render3DObjects()
{
    //开启深度测试，自动抛弃离摄像机远的片段
    glEnable(GL_DEPTH_TEST);

    for (auto opaque_mesh : opaque_meshes_)
    {
        DrawMesh(opaque_mesh);
    }

    //渲染透明物体，开启混合，需要自己处理渲染次序不一样造成不一样的效果
    SortTransparent();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto transparent_mesh : transparent_meshes_)
    {
        DrawMesh(transparent_mesh);
    }
    glDisable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);

    opaque_meshes_.clear();
    transparent_meshes_.clear();
}

void Renderer::DrawMesh(Mesh *mesh)
{
    //glEnable(GL_CULL_FACE);

    auto material = mesh->material;
    auto camera = Engine::GetInstance()->mainCamera;

    if (material == nullptr || camera == nullptr)
    {
        return;
    }

    material->Bind();
    material->TransferUniformsToShader();
    material->GetShader()->SetMatrix("mvp", camera->projectMatrix * mesh->localToCameraTransform);

    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glDrawElements(GL_TRIANGLES, (int)mesh->indices.size(), GL_UNSIGNED_INT, 0);

    Engine::GetInstance()->draw_call++;
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

void Renderer::BatchRenderUI()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    bool first_flag = true;
    int last_rect_index = -1;
    for (auto rect : ui_rect_list_)
    {
        //不同材质生成一个DrawCall
        unsigned current_material_id = rect->material->GetID();
        if (!first_flag && ui_last_material_id_ != current_material_id)
        {
            GenerateUIDrawCall(last_rect_index);
        }
        //Buffer满了
        if (ui_vertex_index_ + sizeof(va::Pos_Tex) * 4 >= VBO_SIZE)
        {
            GenerateUIDrawCall(last_rect_index);
        }
        //叠加rect顶点信息
        ui_vertices_[ui_vertex_index_].Position = rect->right_bottom;
        ui_vertices_[ui_vertex_index_].TexCoords = rect->uv_right_bottom;
        ui_vertices_[ui_vertex_index_ + 1].Position = rect->right_top;
        ui_vertices_[ui_vertex_index_ + 1].TexCoords = rect->uv_right_top;
        ui_vertices_[ui_vertex_index_ + 2].Position = rect->left_top;
        ui_vertices_[ui_vertex_index_ + 2].TexCoords = rect->uv_left_top;
        ui_vertices_[ui_vertex_index_ + 3].Position = rect->left_bottom;
        ui_vertices_[ui_vertex_index_ + 3].TexCoords = rect->uv_left_bottom;
        //叠加顶点绘制索引
        ui_indices_[ui_element_index_] = ui_vertex_index_;
        ui_indices_[ui_element_index_ + 1] = ui_vertex_index_ + 1;
        ui_indices_[ui_element_index_ + 2] = ui_vertex_index_ + 2;
        ui_indices_[ui_element_index_ + 3] = ui_vertex_index_;
        ui_indices_[ui_element_index_ + 4] = ui_vertex_index_ + 2;
        ui_indices_[ui_element_index_ + 5] = ui_vertex_index_ + 3;

        ui_vertex_index_ = ui_vertex_index_ + 4;
        ui_element_index_ = ui_element_index_ + 6;
        ui_last_material_id_ = current_material_id;
        first_flag = false;

        last_rect_index++;
    }
    //绘制最后剩余的rects
    if (ui_rect_list_.size() > 0)
        GenerateUIDrawCall(last_rect_index);

    ui_rect_list_.clear();

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

void Renderer::GenerateUIDrawCall(unsigned last_rect_index)
{
    //取累积最后rect的材质
    auto rect = ui_rect_list_[last_rect_index];
    auto material = rect->material;
    material->Bind();
    material->GetShader()->SetMatrix("mvp", ui_project_view_matrix_);
    material->GetShader()->SetVector3("color", rect->color);
    material->GetShader()->SetFloat("alpha", rect->alpha);

    glBindVertexArray(ui_vao_);

    //复制顶点数据到Buffer
    glBindBuffer(GL_ARRAY_BUFFER, ui_vbo_[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ui_vertices_[0]) * ui_vertex_index_, nullptr, GL_STATIC_DRAW);
    void *buf = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(buf, ui_vertices_, sizeof(ui_vertices_[0]) * ui_vertex_index_);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_vbo_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui_indices_[0]) * ui_element_index_, ui_indices_, GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, ui_element_index_, GL_UNSIGNED_SHORT, 0);

    Engine::GetInstance()->ui_vertices = Engine::GetInstance()->ui_vertices + ui_vertex_index_;
    Engine::GetInstance()->ui_draw_call++;

    ui_vertex_index_ = 0;
    ui_element_index_ = 0;
}

} // namespace kd