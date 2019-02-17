#pragma once

#include "common.h"

namespace kd
{

class SkyBox final
{
  public:
    explicit SkyBox();
    ~SkyBox();
    void Setup();
    void Render();
    
    unsigned cubeMap = 0;

  private:
    unsigned vao_ = 0;
    unsigned vbo_ = 0;

    DISALLOW_COPY_AND_ASSIGN(SkyBox)
};

} // namespace kd