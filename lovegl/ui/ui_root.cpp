#include "ui_root.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "font_manager.h"
#include "ui_button.h"
#include "ui_label.h"
#include "../engine.h"

using namespace glm;

namespace kd
{
namespace ui
{

UIRoot::UIRoot()
{
    //按钮悬停变色用
    ui_button_normal_mat = new Material("ui_button_normal");
    ui_button_normal_mat->SetShader("unlit_pos_tex");
    ui_button_normal_mat->SetTexture("white");
    ui_button_hover_mat = new Material("ui_button_hover");
    ui_button_hover_mat->SetShader("unlit_pos_tex");
    ui_button_hover_mat->SetTexture("white");
}

UIRoot::~UIRoot()
{
    SafeDelete(ui_button_normal_mat);
    SafeDelete(ui_button_hover_mat);
}

void UIRoot::Setup()
{
    int width, height;
    glfwGetWindowSize(Engine::GetInstance()->GetMainWindow(), &width, &height);

    //查看字体图集
    // auto font_texture = new UIRect();
    // font_texture->name = "font_texture";
    // font_texture->width = static_cast<float>(FontManager::GetInstance()->TEXTURE_WIDTH);
    // font_texture->height = static_cast<float>(FontManager::GetInstance()->TEXTURE_HEIGHT);
    // font_texture->material = FontManager::GetInstance()->GetMaterial(0);
    // AddChild(font_texture);

    //fps状态等
    fps_label_ = new Label();
    fps_label_->position = vec3(width - 80, height - 12, 0);
    AddChild(fps_label_);
    draw_call_label_ = new Label();
    draw_call_label_->position = vec3(width - 80, height - 28, 0);
    AddChild(draw_call_label_);
    ui_draw_call_label_ = new Label();
    ui_draw_call_label_->position = vec3(width - 80, height - 45, 0);
    AddChild(ui_draw_call_label_);
    gl_renderer_label_ = new Label();
    gl_renderer_label_->position = vec3(width - 200, height - 62, 0);
    AddChild(gl_renderer_label_);
    gl_version_label_ = new Label();
    gl_version_label_->position = vec3(width - 150, height - 80, 0);
    AddChild(gl_version_label_);

    //选中物体
    selected_node_label_ = new Label();
    selected_node_label_->position = vec3(width / 2, height - 20, 0);
    AddChild(selected_node_label_);

    //检视器头
    auto ui_common_tile_mat = new Material("ui_common_tile");
    ui_common_tile_mat->SetShader("unlit_pos_tex");
    ui_common_tile_mat->SetTexture("white");
    auto title = new UIRect();
    title->material = ui_common_tile_mat;
    title->width = 100;
    title->height = 30;
    title->position = vec3(0, height - 30, 0);
    title->color = vec3(0.2, 0.2, 0.2);
    AddChild(title);
    auto inspector_label = new Label();
    inspector_label->SetText("Inspector");
    inspector_label->position = vec3(20, 10, 1);
    inspector_label->depth = 1;
    title->AddChild(inspector_label);

    //左背景
    auto ui_common_bg_mat = new Material("ui_common_bg");
    ui_common_bg_mat->SetShader("unlit_pos_tex");
    ui_common_bg_mat->SetTexture("white");
    auto bg = new UIRect();
    bg->material = ui_common_bg_mat;
    bg->width = 100;
    bg->height = (float)height;
    bg->color = vec3(0, 0, 0);
    bg->alpha = 0.8f;
    bg->depth = -1;
    AddChild(bg);

    //测试按钮
    auto btn_1 = new Button();
    btn_1->position = vec3(10, height - 100, -1);
    AddChild(btn_1);

    auto btn_2 = new Button();
    btn_2->position = vec3(10, height - 150, 0);
    AddChild(btn_2);

    auto btn_3 = new Button();
    btn_3->position = vec3(10, height - 200, 0);
    AddChild(btn_3);

    UIRect::Setup();
}

void UIRoot::Update(float deltaTime)
{
    Node::Update(deltaTime);
    for (auto child : children_)
    {
        if (child->visible)
            child->Update(deltaTime);
    }

    if (show_stat_label)
    {
        fps_label_->SetText(std::string("FPS: ").append(std::to_string(Engine::GetInstance()->fps)));
        draw_call_label_->SetText(std::string("Draw call: ").append(std::to_string(Engine::GetInstance()->drawCall)));
        ui_draw_call_label_->SetText(std::string("UI Batch: ").append(std::to_string(Engine::GetInstance()->uiDrawCall)));

        gl_renderer_label_->SetText((char *)glGetString(GL_RENDERER));
        gl_version_label_->SetText((char *)glGetString(GL_VERSION));
    }

    if (Engine::GetInstance()->selectedEntity)
    {
        selected_node_label_->SetText(std::string("Selected: ").append(Engine::GetInstance()->selectedEntity->name));
    }
}

} // namespace ui

} // namespace kd
