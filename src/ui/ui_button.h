#pragma once

#include <functional>
#include "ui_rect.h"

namespace kd
{

namespace ui
{

class Button : public UIRect
{
  public:
    explicit Button();
    virtual ~Button();
    // virtual void OnSetup() override;
    // virtual void OnUpdate() override;
    virtual void OnMouseLeftButtonPress() override;
    virtual void OnMouseLeftButtonRelease() override;

    void SetClickCallback(const std::function<void(void)> &func) { click_callback_ = func; }

  private:
    bool is_pressed_ = false;
    std::function<void(void)> click_callback_;

    DISALLOW_COPY_AND_ASSIGN(Button)
};

} // namespace ui

} // namespace kd