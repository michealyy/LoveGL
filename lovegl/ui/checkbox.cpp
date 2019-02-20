#include "checkbox.h"
#include "button.h"
#include "../engine.h"

using namespace glm;

namespace kd
{

namespace ui
{

CheckBox::CheckBox(const std::string &str, bool _selected) : selected(_selected)
{
    rect_bg_ = new UIRect();
    rect_bg_->material = Engine::GetInstance()->uiRoot->ui_checkbox_rect_bg_mat;
    rect_bg_->width = 26;
    rect_bg_->height = 26;
    rect_bg_->color = vec3(1, 1, 1);
    rect_bg_->alpha = 0.6f;
    rect_bg_->position = vec3(0, 0, 0);
    AddChild(rect_bg_);

    rect_ = new SimpleButton();
    rect_->material = Engine::GetInstance()->uiRoot->ui_checkbox_rect_mat;
    rect_->width = 20;
    rect_->height = 20;
    rect_->color = vec3(0.9, 0.133, 0.415);
    rect_->alpha = selected ? 1.f : 0.f;
    rect_->position = vec3(3, 3, 0);
    AddChild(rect_);
    rect_->SetClickCallback([this]() {
        selected = !selected;
        if (selected)
            rect_->alpha = 1;
        else
            rect_->alpha = 0;
    });

    label_ = new Label();
    label_->SetText(str);
    label_->position = vec3(40, 8, 0);
    AddChild(label_);
}

CheckBox::~CheckBox()
{
}

void CheckBox::Setup()
{
    UIRect::Setup();
}

void CheckBox::Update(float deltaTime)
{
    //跳过自身渲染
    Node::Update(deltaTime);
    for (auto child : children_)
    {
        child->Update(deltaTime);
    }
}

void CheckBox::OnMouseLeftButtonPress()
{
    selected = !selected;
    if (selected)
        rect_->color = vec3(0.9, 0.133, 0.415);
    else
        rect_->color = vec3(0, 0.133, 0.415);
}

} // namespace ui
} // namespace kd