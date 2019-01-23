#include "ui_rect.h"
#include "rect_data.h"
#include "../renderer.h"
#include "../engine.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

namespace kd
{

UIRect::UIRect(float width, float height)
    : width(width), height(height)
{
    notDeleteByScene = true;
}

UIRect::~UIRect()
{
}

void UIRect::Update(float deltaTime)
{
    Entity::Update(deltaTime);
    
    auto a= translate(mat4(1.f), vec3(100.f,100.f,0)) * vec4(100.f, 100.f, 0.f, 0.f);

    RectData rect;
    rect.right_bottom = worldTransform * vec4(width, 0, 0, 0);
    rect.right_top = worldTransform * vec4(width, height, 0, 0);
    rect.left_top = worldTransform * vec4(0, height, 0, 0);
    rect.left_bottom = worldTransform * vec4(0, 0, 0, 0);
    
    rect.depth = depth;
    
    if (material && material->GetShader())
    {
        rect.material_id = material->GetID();
        material->GetShader()->SetVector3("color", this->color);
        material->GetShader()->SetFloat("alpha", this->alpha);
    }
    else
    {
        rect.material_id = Engine::GetInstance()->GetMaterial("ui_default")->GetID();
    }
    
    Renderer::GetInstance()->AddUIRect(rect);
}

} // namespace kd