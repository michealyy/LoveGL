#include "ui_batch_renderer.h"
#include <algorithm>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <resource_manager.h>
#include "engine.h"
#include "config.h"

using namespace std;

namespace kd
{

UIBatchRenderer::UIBatchRenderer()
{
}

UIBatchRenderer::~UIBatchRenderer()
{
    glDeleteVertexArrays(1, &ui_vao_);
    glDeleteBuffers(2, ui_vbo_);
}

void UIBatchRenderer::Setup()
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

void UIBatchRenderer::Render()
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

void UIBatchRenderer::SortUIRectByDepthAndHandleInput()
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

void UIBatchRenderer::GenerateUIDrawCall(unsigned last_rect_index)
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

    Engine::GetInstance()->uiVertices = Engine::GetInstance()->uiVertices + ui_vertex_index_;
    Engine::GetInstance()->uiDrawCall++;

    ui_vertex_index_ = 0;
    ui_element_index_ = 0;
}

} // namespace kd