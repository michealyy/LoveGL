#include "ui_button.h"
#include "../engine.h"

namespace kd
{

namespace ui
{

Button::Button()
{
}

Button::~Button()
{
}

void Button::Setup()
{
	UIRect::Setup();

	width = 80;
	height = 30;

	normal_material = Engine::GetInstance()->uiRoot->ui_button_normal_mat;
	hover_material = Engine::GetInstance()->uiRoot->ui_button_hover_mat;
}

void Button::Update(float deltaTime)
{
	UIRect::Update(deltaTime);

	if (!is_hover_ && normal_material)
	{
		material = normal_material;
		color = {0.11, 0.66, 0.85};
		//color = {1, 1, 1};
	}
	is_hover_ = false;
}

void Button::OnMouseLeftButtonPress()
{
	if (!is_pressed_)
	{
		if (click_callback_)
		{
			click_callback_();
		}
	}
	is_pressed_ = true;
}

void Button::OnMouseLeftButtonRelease()
{
	is_pressed_ = false;
}

void Button::OnMouseHover()
{
	is_hover_ = true;
	if (hover_material)
	{
		material = hover_material;
		color = {0.9, 0.133, 0.415};
	}
}

} // namespace ui

} // namespace kd
