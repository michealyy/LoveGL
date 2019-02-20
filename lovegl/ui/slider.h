#pragma once
#include "ui_rect.h"

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

  private:
    DISALLOW_COPY_AND_ASSIGN(Slider)
};

} // namespace ui

} // namespace kd