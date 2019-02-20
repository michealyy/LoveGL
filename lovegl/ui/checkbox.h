#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "ui_rect.h"
#include "label.h"
#include "simple_button.h"

namespace kd
{

namespace ui
{

class CheckBox : public UIRect
{
  public:
    explicit CheckBox(const std::string &str, bool selected);
    virtual ~CheckBox();
    virtual void Setup() override;
    virtual void Update(float deltaTime) override;
    virtual void OnMouseLeftButtonPress() override;

    bool selected = false;

  private:
    UIRect *rect_bg_ = nullptr;
    SimpleButton *rect_ = nullptr;
    Label *label_ = nullptr;

    DISALLOW_COPY_AND_ASSIGN(CheckBox)
};

} // namespace ui

} // namespace kd