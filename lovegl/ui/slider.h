#pragma once
#include "ui_rect.h"
#include <functional>
#include <glm/glm.hpp>
#include "simple_button.h"

namespace kd
{

namespace ui
{

class Slider : public UIRect
{
  public:
    explicit Slider();
    virtual ~Slider();
    virtual void Setup() override;
    virtual void Update(float deltaTime) override;
    virtual void OnMouseLeftButtonPress() override;
    virtual void OnMouseLeftButtonRelease() override;
    virtual void OnMouseHover() override;
    inline void SetClickCallback(const std::function<void(void)> &func) { click_callback_ = func; }

    float rectWidth = 14.f;
    float rectPadding = 3.f;

  private:
    SimpleButton *rect_ = nullptr;
    bool is_pressed_ = false;
    bool is_hover_ = false;
    std::function<void(void)> click_callback_;
    DISALLOW_COPY_AND_ASSIGN(Slider)
};

} // namespace ui

} // namespace kd