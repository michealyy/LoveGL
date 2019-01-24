#include "ui_root.h"
#include <glm/glm.hpp>
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
    ui_button_normal_mat->SetTexture(0, "white");
    ui_button_hover_mat = new Material("ui_button_hover");
    ui_button_hover_mat->SetShader("unlit_pos_tex");
    ui_button_hover_mat->SetTexture(0, "white");
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
    fps_label_->position = vec3(width - 100, height - 12, 0);
    AddChild(fps_label_);
    draw_call_label_ = new Label();
    draw_call_label_->position = vec3(width - 100, height - 28, 0);
    AddChild(draw_call_label_);
    ui_draw_call_label_ = new Label();
    ui_draw_call_label_->position = vec3(width - 100, height - 45, 0);
    AddChild(ui_draw_call_label_);

    //检视器
    auto ui_common_tile_mat = new Material("ui_common_tile");
    ui_common_tile_mat->SetShader("unlit_pos_tex");
    ui_common_tile_mat->SetTexture(0, "white");
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
    title->AddChild(inspector_label);

    //左背景
    auto ui_common_bg_mat = new Material("ui_common_bg");
    ui_common_bg_mat->SetShader("unlit_pos_tex");
    ui_common_bg_mat->SetTexture(0, "white");
    auto bg = new UIRect();
    bg->material = ui_common_bg_mat;
    bg->width = 100;
    bg->height = (float)height;
    bg->color = vec3(0, 0, 0);
    bg->alpha = 0.8f;
    bg->position = vec3(0, 0, -1);
    AddChild(bg);

    // //切换检视器显示
    // title_btn->SetClickCallback([this, title_btn, bg] {
    //     show_inspector = !show_inspector;
    //     if (show_inspector)
    //     {
    //         title_btn->color = vec3(0.2, 0.2, 0.2);
    //         title_btn->alpha = 1.f;
    //         bg->visible = true;
    //     }
    //     else
    //     {
    //         title_btn->color = vec3(0, 0, 0);
    //         title_btn->alpha = 0.6f;
    //         bg->visible = false;
    //     }
    // });

    //测试按钮
    auto btn_1 = new Button();
    btn_1->position = vec3(10, height - 100, 0);
    bg->AddChild(btn_1);

    auto btn_2 = new Button();
    btn_2->position = vec3(10, height - 150, 0);
    bg->AddChild(btn_2);

    auto btn_3 = new Button();
    btn_3->position = vec3(10, height - 200, 0);
    bg->AddChild(btn_3);

    UIRect::Setup();
}

void UIRoot::Update(float deltaTime)
{
    Entity::Update(deltaTime);
    for (auto child : children_)
    {
        if (child->visible)
            child->Update(deltaTime);
    }
    HandleInput();

    if (show_stat_label)
    {
        fps_label_->SetText(std::string("FPS: ").append(std::to_string(Engine::GetInstance()->fps)));
        draw_call_label_->SetText(std::string("Draw call: ").append(std::to_string(Engine::GetInstance()->draw_call)));
        ui_draw_call_label_->SetText(std::string("UI Draw call: ").append(std::to_string(Engine::GetInstance()->ui_draw_call)));
    }
}

void UIRoot::HandleInput()
{
    auto main_window = Engine::GetInstance()->GetMainWindow();
    double x, y;
    glfwGetCursorPos(main_window, &x, &y);
    int width, height;
    glfwGetWindowSize(main_window, &width, &height);

    float mouseX = static_cast<float>(x);
    float mouseY = static_cast<float>(height - y);

    auto button = dynamic_cast<ui::Button *>(this->FindButton(mouseX, mouseY));
    if (button)
    {
        if (glfwGetMouseButton(main_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
        {
            is_left_mouse_btn_press = true;
            button->OnMouseLeftButtonPress();
        }
        else
        {
            if (is_left_mouse_btn_press)
            {
                is_left_mouse_btn_press = false;
                button->OnMouseLeftButtonRelease();
            }
            else
            {
                button->OnMouseHover();
            }
        }
    }
}

} // namespace ui

} // namespace kd
