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

    fps_label_ = new Label();
    fps_label_->position = vec3(0, 40, 0);
    AddChild(fps_label_);
    draw_call_label_ = new Label();
    draw_call_label_->position = vec3(0, 20, 0);
    AddChild(draw_call_label_);
    ui_draw_call_label_ = new Label();
    AddChild(ui_draw_call_label_);

    //查看字体图集
    // auto font_texture = new UIRect();
    // font_texture->name = "font_texture";
    // font_texture->width = static_cast<float>(FontManager::GetInstance()->TEXTURE_WIDTH);
    // font_texture->height = static_cast<float>(FontManager::GetInstance()->TEXTURE_HEIGHT);
    // font_texture->material = FontManager::GetInstance()->GetMaterial(0);
    // AddChild(font_texture);

    auto ui1 = new Button();
    ui1->name = "ui1";
    ui1->SetImage("demo");
    ui1->position = vec3(0, 200, 0);
    AddChild(ui1);

    // auto ui2 = new Button();
    // ui2->name = "ui2";
    // ui2->SetImage("demo");
    // ui2->position = vec3(200, 0, 0);
    // ui1->AddChild(ui2);
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
