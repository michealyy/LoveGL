#pragma once

#include <functional>
#include "ui_rect.h"

namespace kd
{

namespace ui
{

class SimpleButton : public UIRect
{
  public:
    explicit SimpleButton();
    virtual ~SimpleButton();
    virtual void OnMouseLeftButtonPress() override;
    virtual void OnMouseLeftButtonRelease() override;
    inline void SetClickCallback(const std::function<void(void)> &func) { click_callback_ = func; }

  private:
    bool is_pressed_ = false;
    std::function<void(void)> click_callback_;
    DISALLOW_COPY_AND_ASSIGN(SimpleButton)
};

} // namespace ui

} // namespace kd