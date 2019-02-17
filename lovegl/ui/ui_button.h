#pragma once

#include <functional>
#include "ui_rect.h"
#include <core/material.h>

namespace kd
{

namespace ui
{

class Button : public UIRect
{
public:
  explicit Button();
  virtual ~Button();

  virtual void Setup() override;
  virtual void Update(float deltaTime) override;
  virtual void OnMouseLeftButtonPress() override;
  virtual void OnMouseLeftButtonRelease() override;
  virtual void OnMouseHover() override;
  inline void SetClickCallback(const std::function<void(void)> &func) { click_callback_ = func; }
  
  Material *normal_material = nullptr;
  Material *hover_material = nullptr;

private:
  bool is_pressed_ = false;
  bool is_hover_ = false;
  std::function<void(void)> click_callback_;

  DISALLOW_COPY_AND_ASSIGN(Button)
};

} // namespace ui

} // namespace kd