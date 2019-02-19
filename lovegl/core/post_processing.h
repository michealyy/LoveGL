#pragma once

#include <common.h>
#include <core/render_target.h>

namespace kd
{

class Camera;
class Shader;

class PostProcessing
{
public:
  explicit PostProcessing();
  virtual ~PostProcessing();
  virtual void Setup();
  virtual void Draw();

  Camera *camera = nullptr;
  Shader *shader = nullptr;
  unsigned texture = 0;

protected:
  unsigned vao_ = 0;
  unsigned vbo_ = 0;

private:
  DISALLOW_COPY_AND_ASSIGN(PostProcessing)
};

class PostGray : public PostProcessing
{
public:
  virtual void Setup() override;
};

class Bloom : public PostProcessing
{
public:
  explicit Bloom();
  virtual ~Bloom();

  virtual void Setup() override;
  virtual void Draw() override;

private:
  MultiRenderTarget *multiRenderTarget_ = nullptr;
  RenderTexture *blur_rtt_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(Bloom)
};

} // namespace kd