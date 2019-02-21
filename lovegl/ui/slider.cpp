#include "slider.h"

#include "../engine.h"

using namespace glm;

namespace kd
{

namespace ui
{

Slider::Slider()
{
}

Slider::~Slider()
{
}

void Slider::Setup()
{
    UIRect::Setup();

	this->alpha = 0.3f;

	rect_ = new SimpleButton();
    rect_->material = Engine::GetInstance()->uiRoot->ui_slider_bg_mat;
    rect_->width = rectWidth;
    rect_->height = this->height - rectPadding * 2;
    rect_->alpha = 0.8f;
    rect_->position = vec3(rectPadding, rectPadding, 0);
    AddChild(rect_);
    // rect_->SetClickCallback([this]() {
    //     selected = !selected;
    //     if (selected)
    //         rect_->alpha = 1;
    //     else
    //         rect_->alpha = 0;
    // });
}

void Slider::Update(float deltaTime)
{
    UIRect::Update(deltaTime);
}

void Slider::OnMouseLeftButtonPress()
{
	auto main_window = Engine::GetInstance()->GetMainWindow();
    double x, y;
    glfwGetCursorPos(main_window, &x, &y);
    int width, height;
    glfwGetWindowSize(main_window, &width, &height);
    float _mouseX = static_cast<float>(x);
    float mouseY = static_cast<float>(height - y);

    //if (rect_->parent)

    auto mousePos = inverse(rect_->parent->worldTransform) * vec4(_mouseX, 0, 0, 1);
    auto mouseX = mousePos.x;
    if (mouseX > rectPadding && mouseX < (this->width - rectPadding - rect_->width))
        rect_->position = vec3(mouseX, rectPadding, 0);

    if (!is_pressed_)
	{
		// if (click_callback_)
		// {
		// 	click_callback_();
		// }
	}
	is_pressed_ = true;
}

void Slider::OnMouseLeftButtonRelease()
{
	is_pressed_ = false;
}

void Slider::OnMouseHover()
{
	is_hover_ = true;
}


} // namespace ui
} // namespace kd