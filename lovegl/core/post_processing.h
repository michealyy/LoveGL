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

class ToneMapping : public PostProcessing
{
public:
  explicit ToneMapping();
  virtual ~ToneMapping();
  virtual void Setup() override;
  virtual void Draw() override;

private:
  DISALLOW_COPY_AND_ASSIGN(ToneMapping)
};

class Bloom : public PostProcessing
{
public:
  explicit Bloom();
  virtual ~Bloom();

  virtual void Setup() override;
  virtual void Draw() override;

  //混合因子
  float factor = 1.f;
  //必须为偶数
  unsigned blurCount = 10;

private:
  MultiRenderTarget *multiRenderTarget_ = nullptr;
  RenderTexture *blur_rtt_ = nullptr;
  RenderTexture *blur_temp_rtt_ = nullptr;

  DISALLOW_COPY_AND_ASSIGN(Bloom)
};

} // namespace kd