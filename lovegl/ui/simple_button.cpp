#include "simple_button.h"

namespace kd
{

namespace ui
{
SimpleButton::SimpleButton()
{
}

SimpleButton::~SimpleButton()
{
}

void SimpleButton::OnMouseLeftButtonPress()
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

void SimpleButton::OnMouseLeftButtonRelease()
{
    is_pressed_ = false;
}

} // namespace ui

} // namespace kd