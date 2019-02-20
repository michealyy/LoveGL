#pragma once

#include "ui_rect.h"
#include "label.h"
#include <core/material.h>

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

    Material *ui_button_normal_mat = nullptr;
    Material *ui_button_hover_mat = nullptr;
    Material *ui_checkbox_rect_bg_mat = nullptr;
    Material *ui_checkbox_rect_mat = nullptr;
    
  private:
    Label *fps_label_;
    Label *draw_call_label_;
    Label *ui_draw_call_label_;
    Label *gl_renderer_label_;
    Label *gl_version_label_;
    Label *selected_node_label_;

    DISALLOW_COPY_AND_ASSIGN(UIRoot)
};

} // namespace ui

} // namespace kd