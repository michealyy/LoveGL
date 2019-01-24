#pragma once

#include <string>
#include "ui_rect.h"

namespace kd
{

namespace ui
{

class Label : public UIRect
{
  public:
    explicit Label();
    virtual ~Label();

    virtual void Update(float deltaTime) override;
    void SetText(const std::string &text);

  private:
    int font_size_ = 24;
    bool is_update_text_ = false;
    std::string text_;

    void GenCharNode();

    DISALLOW_COPY_AND_ASSIGN(Label)
};

} // namespace ui

} // namespace kd