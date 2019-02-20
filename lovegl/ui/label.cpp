#include "label.h"
#include "font_manager.h"
#include <glm/glm.hpp>

using namespace glm;

namespace kd
{

namespace ui
{

Label::Label()
{
}

Label::~Label()
{
}

void Label::SetText(const std::string &text)
{
    if (text_ == text)
        return;

    text_ = text;
    is_update_text_ = true;
}

void Label::Update(float deltaTime)
{
    if (is_update_text_)
    {
        for (auto child : children_)
            SafeDelete(child);
        children_.clear();

        GenCharNode();
        is_update_text_ = false;
    }

    Node::Update(deltaTime);
    for (auto child : children_)
    {
        child->Update(deltaTime);
    }
}

void Label::GenCharNode()
{
    float offsetX = 0, offsetY = 0;
    for (auto c = text_.begin(); c != text_.end(); c++)
    {
        auto char_rect_info = FontManager::GetInstance()->GetGlyphInfo(*c, offsetX, offsetY);
        offsetX = char_rect_info.offsetX;
		offsetY = char_rect_info.offsetY;
        auto char_rect = new UIRect();
        char_rect->depth = depth;
        char_rect->material = FontManager::GetInstance()->GetMaterial(char_rect_info.material_index);
        char_rect->width = char_rect_info.width;
        char_rect->height = char_rect_info.height;
        char_rect->uv_right_bottom = char_rect_info.uv_right_bottom;
        char_rect->uv_right_top = char_rect_info.uv_right_top;
        char_rect->uv_left_top = char_rect_info.uv_left_top;
        char_rect->uv_left_bottom = char_rect_info.uv_left_bottom;
        char_rect->position = char_rect_info.position;
        AddChild(char_rect);
    }
}

} // namespace ui

} // namespace kd
