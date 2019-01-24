#include "ui_rect.h"
#include "rect_data.h"
#include "../renderer.h"
#include "../engine.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace kd
{

namespace ui
{

UIRect::UIRect(float width, float height)
    : width(width), height(height)
{
}

UIRect::~UIRect()
{
    for (auto child : children_)
    {
        SafeDelete(child);
    }
}

void UIRect::SetImage(const std::string &name)
{
    std::string mat_name("ui_");
    mat_name.append(name);

    material = Engine::GetInstance()->GetMaterial(mat_name);
    if (material == nullptr)
    {
        material = new Material(mat_name);
        material->SetShader("unlit_pos_tex");
        material->SetTexture(0, name);
    }
}

void UIRect::Setup()
{
    Entity::Setup();
    for (auto child : children_)
    {
        child->Setup();
    }
}

void UIRect::Update(float deltaTime)
{
    Entity::Update(deltaTime);
    for (auto child : children_)
    {
        child->Update(deltaTime);
    }
    
    RectData rect;
    rect.right_bottom = worldTransform * vec4(width, 0, 0, 1);
    rect.right_top = worldTransform * vec4(width, height, 0, 1);
    rect.left_top = worldTransform * vec4(0, height, 0, 1);
    rect.left_bottom = worldTransform * vec4(0, 0, 0, 1);

    if (material == nullptr || material->GetShader() == nullptr)
        material = Engine::GetInstance()->GetMaterial("ui_default");

    rect.material_id = material->GetID();
    material->GetShader()->SetVector3("color", this->color);
    material->GetShader()->SetFloat("alpha", this->alpha);

    Renderer::GetInstance()->AddUIRect(rect);
}

void UIRect::OnMouseLeftButtonPress()
{}

void UIRect::OnMouseLeftButtonRelease()
{}

UIRect *UIRect::FindChild(float x, float y)
{
    for (auto _child : children_)
    {
        auto child = dynamic_cast<UIRect *>(_child);
        if (child)
        {
            auto posX= child->worldPosition.x;
            auto posY= child->worldPosition.y;
            auto width = child->width * child->worldScale.x;
            auto height = child->height * child->worldScale.y;
            if (x > posX && x < posX + width && y > posY && y < posY + height)
            {
                return child;
            }
            else
            {
                auto __child = child->FindChild(x, y);
                if (__child)
                {
                    return __child;
                }
            }
        }
    }

    return nullptr;
}

} // namespace ui

} // namespace kd
