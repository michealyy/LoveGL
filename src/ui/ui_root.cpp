#include "ui_root.h"
#include <glm/glm.hpp>
#include "ui_button.h"

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

    auto ui1 = new Button();
    ui1->name = "ui1";
    ui1->SetImage("demo");
    ui1->position = vec3(0, 200, 0);
    ui1->alpha = 0.5;
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
