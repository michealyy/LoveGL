#pragma once

#include "ui_rect.h"

namespace kd
{

namespace ui
{

class UIRoot : public UIRect
{
  public:
    explicit UIRoot();
    virtual ~UIRoot();
    virtual void Setup() override;
    virtual void Update(float deltaTime) override;
  
  private:
    DISALLOW_COPY_AND_ASSIGN(UIRoot)
};

} // namespace ui

} // namespace kd