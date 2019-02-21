#include "slider.h"
#include <glm/glm.hpp>
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
    rect_->width = 10;
    rect_->height = this->height - 6;
	rect_->alpha = 0.8f;
    rect_->position = vec3(0, 3, 0);
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
    float mouseX = static_cast<float>(x);
    float mouseY = static_cast<float>(height - y);

	rect_->position = vec3(mouseX, 3, 0);
	
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