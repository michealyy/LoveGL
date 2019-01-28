#include "renderer.h"
#include <algorithm>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include "engine.h"
#include "config.h"

using namespace std;

namespace kd
{

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
    glDeleteFramebuffers(1, &post_processing_framebuffer_);
    glDeleteTextures(1, &post_processing_texture_);
    glDeleteRenderbuffers(1, &post_processing_render_buffer_);
    glDeleteVertexArrays(1, &post_processing_vao_);
    glDeleteBuffers(1, &post_processing_vbo_);

    glDeleteVertexArrays(1, &ui_vao_);
    glDeleteBuffers(2, ui_vbo_);
}

void Renderer::Setup()
{
    SetupPostProcessingRenderTexture();
    SetupUIBatchRender();
    SetupDebugLines();
}

void Renderer::SetupPostProcessingRenderTexture()
{
    int width, height;
    glfwGetWindowSize(Engine::GetInstance()->GetMainWindow(), &width, &height);

    glGenFramebuffers(1, &post_processing_framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, post_processing_framebuffer_);
    //渲染到贴图
    glGenTextures(1, &post_processing_texture_);
    glBindTexture(GL_TEXTURE_2D, post_processing_texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, post_processing_texture_, 0);
    //Renderbuffer作为深度模板缓冲区
    glGenRenderbuffers(1, &post_processing_render_buffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, post_processing_render_buffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, post_processing_render_buffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //屏幕大小的矩形
    float vertices[] = {
        -1.0,
        1.0,
        0.0,
        1.0,
        -1.0,
        -1.0,
        0.0,
        0.0,
        1.0,
        -1.0,
        1.0,
        0.0,
        -1.0,
        1.0,
        0.0,
        1.0,
        1.0,
        -1.0,
        1.0,
        0.0,
        1.0,
        1.0,
        1.0,
        1.0,
    };
    glGenVertexArrays(1, &post_processing_vao_);
    glBindVertexArray(post_processing_vao_);
    glGenBuffers(1, &post_processing_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, post_processing_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(sizeof(float) * 2));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::SetupUIBatchRender()
{
    //2DUI投影矩阵
    int width, height;
    glfwGetWindowSize(Engine::GetInstance()->GetMainWindow(), &width, &height);
    //坐下为原点，分辨率为大小
    glm::mat4 projectionMatrix = glm::ortho(0.f, (float)width, 0.f, (float)height, -100.f, 100.f);
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

void Renderer::SetupDebugLines()
{
    glGenVertexArrays(1, &line_vao_);
    glBindVertexArray(line_vao_);
    glGenBuffers(1, &line_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, line_vbo_);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
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
    //渲染到后处理贴图 TODO:FBO实现抗锯齿
    if (open_post_processing)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, post_processing_framebuffer_);
        glClearColor(UI_CLEAR_COLOR.r, UI_CLEAR_COLOR.g, UI_CLEAR_COLOR.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

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

    //关闭场景渲染到贴图，开始渲染成品的贴图
    if (open_post_processing)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //Engine::GetInstance()->GetShader("post_blur")->Bind();
        Engine::GetInstance()->GetShader("post_normal")->Bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, post_processing_texture_);
        glBindVertexArray(post_processing_vao_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

void Renderer::DrawMesh(Mesh *mesh)
{
    glEnable(GL_CULL_FACE);

    auto material = mesh->material;
    auto camera = Engine::GetInstance()->mainCamera;

    if (material == nullptr || camera == nullptr)
    {
        return;
    }

    material->Bind();
    auto shader = material->GetShader();
    shader->SetMatrix("mvp", camera->projectMatrix * mesh->localToCameraTransform);
    shader->SetMatrix("model", mesh->worldTransform);
    shader->SetVector3("cameraPos", camera->worldPosition);
    shader->SetVector3("lightPosition", glm::vec3(-10,0,0));

    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glDrawElements(GL_TRIANGLES, (int)mesh->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

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

void Renderer::DrawDebugLine(const float vertices[6])
{
    //TODO:支持多个线段
    auto camera = Engine::GetInstance()->mainCamera;
    auto shader = Engine::GetInstance()->GetShader("unlit_pos");
    shader->Bind();
    shader->SetMatrix("mvp", camera->projectMatrix * camera->GetViewMatrix());
    shader->SetFloat("alpha", 1);
    shader->SetVector3("color", glm::vec3(0,1,0));
    
    glBindVertexArray(line_vao_);

    //float _vertices[] = {0,0,0, 0,2,0};
    glBindBuffer(GL_ARRAY_BUFFER, line_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6, vertices, GL_STATIC_DRAW);

    glDrawArrays(GL_LINE_LOOP, 0, 2);
}

void Renderer::SortUIRectByDepthAndHandleInput()
{
    //按z轴排序。影响渲染和事件响应先后
    sort(ui_rect_list_.begin(), ui_rect_list_.end(), [](ui::UIRect *a, ui::UIRect *b) -> bool {
        return a->depth < b->depth;
    });

    //处理UI事件 TODO:解耦render负责渲染
    auto main_window = Engine::GetInstance()->GetMainWindow();
    double x, y;
    glfwGetCursorPos(main_window, &x, &y);
    int width, height;
    glfwGetWindowSize(main_window, &width, &height);
    float mouseX = static_cast<float>(x);
    float mouseY = static_cast<float>(height - y);
    //反向遍历，z越近越早接受事件
    ui::UIRect *first_rect = nullptr;
    for (auto iter = ui_rect_list_.rbegin(); iter != ui_rect_list_.rend(); ++iter)
    {
        auto ui_rect = *iter;
        auto posX = ui_rect->worldPosition.x;
        auto posY = ui_rect->worldPosition.y;
        auto width = ui_rect->width * ui_rect->worldScale.x;
        auto height = ui_rect->height * ui_rect->worldScale.y;
        if (mouseX > posX && mouseX < posX + width && mouseY > posY && mouseY < posY + height)
        {
            first_rect = ui_rect;
            break;
        }
    }
    if (first_rect)
    {
        if (glfwGetMouseButton(main_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
        {
            is_left_mouse_btn_press = true;
            first_rect->OnMouseLeftButtonPress();
        }
        else
        {
            if (is_left_mouse_btn_press)
            {
                is_left_mouse_btn_press = false;
                first_rect->OnMouseLeftButtonRelease();
            }
            else
            {
                first_rect->OnMouseHover();
            }
        }
    }
}

void Renderer::BatchRenderUI()
{
    SortUIRectByDepthAndHandleInput();

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
    material->SetColor(rect->color);
    material->SetAlpha(rect->alpha);
    material->Bind();
    material->GetShader()->SetMatrix("mvp", ui_project_view_matrix_);

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
    glBindVertexArray(0);

    Engine::GetInstance()->ui_vertices = Engine::GetInstance()->ui_vertices + ui_vertex_index_;
    Engine::GetInstance()->ui_draw_call++;

    ui_vertex_index_ = 0;
    ui_element_index_ = 0;
}

} // namespace kd