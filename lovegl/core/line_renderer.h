#pragma once

#include <common.h>

namespace kd
{

class LineRenderer final : public Singleton<LineRenderer>
{
  public:
    explicit LineRenderer();
    ~LineRenderer();
    void Setup();
    void Render();
    void DrawDebugLine(const float vertices[6]);

  private:
    unsigned line_vao_ = 0;
    unsigned line_vbo_ = 0;
    DISALLOW_COPY_AND_ASSIGN(LineRenderer)
};

} // namespace kd