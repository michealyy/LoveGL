#include "ui_button.h"

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

void Button::OnMouseLeftButtonPress()
{
    if (!is_pressed_) {
		if (click_callback_) {
			click_callback_();
		}
		fprintf(stderr, "%d\n", this);
	}
	is_pressed_ = true;
}

void Button::OnMouseLeftButtonRelease()
{
    is_pressed_ = false;
}

} // namespace ui

} // namespace kd
