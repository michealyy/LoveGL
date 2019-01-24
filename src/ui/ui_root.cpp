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
}

UIRoot::~UIRoot()
{
}

void UIRoot::Setup()
{
    UIRect::Setup();

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

    // auto ui1 = new Button();
    // ui1->name = "ui1";
    // ui1->SetImage("demo");
    // ui1->position = vec3(0, 200, 0);
    // AddChild(ui1);

    //标题栏
    auto ui_common_title_mat = new Material("ui_common_title");
    ui_common_title_mat->SetShader("unlit_pos_tex");
    ui_common_title_mat->SetTexture(0, "white");
    auto title = new UIRect();
    title->width = 100;
    title->height = 30;
    title->material = ui_common_title_mat;
    title->color = vec3(1, 0, 0);
    title->position = vec3(0, height - 30, 0);
    AddChild(title);

    //背景
    auto ui_common_bg_mat = new Material("ui_common_bg");
    ui_common_bg_mat->SetShader("unlit_pos_tex");
    ui_common_bg_mat->SetTexture(0, "white");
    auto bg = new UIRect();
    bg->width = 100;
    bg->height = (float)height - 30;
    bg->material = ui_common_bg_mat;
    bg->color = vec3(0.5, 0.5, 0.5);
    bg->alpha = 0.8f;
    AddChild(bg);
}

void UIRoot::Update(float deltaTime)
{
    Entity::Update(deltaTime);
    for (auto child : children_)
    {
        child->Update(deltaTime);
    }

    if (show_stat_label)
    {
        fps_label_->SetText(std::string("FPS: ").append(std::to_string(Engine::GetInstance()->fps)));
        draw_call_label_->SetText(std::string("Draw call: ").append(std::to_string(Engine::GetInstance()->draw_call)));
        ui_draw_call_label_->SetText(std::string("UI Draw call: ").append(std::to_string(Engine::GetInstance()->ui_draw_call)));
    }
}

} // namespace ui
} // namespace kd
