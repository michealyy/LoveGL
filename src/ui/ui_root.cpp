#include "ui_root.h"
#include <glm/glm.hpp>
#include "ui_button.h"
#include "font_manager.h"

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

    //查看字体图集
    auto font_texture = new UIRect();
    font_texture->name = "font_texture";
    font_texture->width = static_cast<float>(FontManager::GetInstance()->TEXTURE_WIDTH);
    font_texture->height = static_cast<float>(FontManager::GetInstance()->TEXTURE_HEIGHT);
    font_texture->material = FontManager::GetInstance()->GetMaterial(0);
    font_texture->position = vec3(0, 0, 1);
    AddChild(font_texture);

    auto char_rect_info = FontManager::GetInstance()->GetGlyphInfo('+', 0, 0);
    auto char_rect = new UIRect();
    char_rect->material = FontManager::GetInstance()->GetMaterial(0);
    char_rect->width = char_rect_info.width;
    char_rect->height = char_rect_info.height;
    char_rect->uv_right_bottom = char_rect_info.uv_right_bottom;
    char_rect->uv_right_top = char_rect_info.uv_right_top;
    char_rect->uv_left_top = char_rect_info.uv_left_top;
    char_rect->uv_left_bottom = char_rect_info.uv_left_bottom;
    char_rect->position = char_rect_info.position;
    AddChild(char_rect);

    auto ui1 = new Button();
    ui1->name = "ui1";
    ui1->SetImage("demo");
    ui1->position = vec3(0, 200, 0);
    AddChild(ui1);

    auto ui2 = new Button();
    ui2->name = "ui2";
    ui2->SetImage("demo");
    ui2->position = vec3(200, 0, 0);
    ui1->AddChild(ui2);
}

void UIRoot::Update(float deltaTime)
{
    Entity::Update(deltaTime);
    for (auto child : children_)
    {
        child->Update(deltaTime);
    }
}

} // namespace ui
} // namespace kd
