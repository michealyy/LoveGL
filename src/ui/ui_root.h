#pragma once

#include "ui_rect.h"
#include "ui_label.h"

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

  bool show_stat_label = true;

private:
  Label *fps_label_;
  Label *draw_call_label_;
  Label *ui_draw_call_label_;

  DISALLOW_COPY_AND_ASSIGN(UIRoot)
};

} // namespace ui

} // namespace kd