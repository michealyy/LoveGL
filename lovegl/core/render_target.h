#pragma once

#include <common.h>
#include <vector>

namespace kd
{

class RenderTarget
{
  public:
	explicit RenderTarget();
	virtual ~RenderTarget();
	virtual void Bind() = 0;
	virtual void BlitMSAAFBO() = 0;

	unsigned width = 0;
	unsigned height = 0;

  private:
	DISALLOW_COPY_AND_ASSIGN(RenderTarget)
};

class RenderTexture : public RenderTarget
{
  public:
		explicit RenderTexture(unsigned width, unsigned height, unsigned msaa_sample = 0);
		virtual ~RenderTexture();
		virtual void Bind() override;
		virtual void BlitMSAAFBO() override;

		unsigned frameBuffer = 0;
		unsigned texture = 0;
		unsigned depthRbo = 0;
		unsigned msaaFrameBuffer = 0;
		unsigned msaaTexture = 0;

	private:
		unsigned msaa_sample_ = 0;
		DISALLOW_COPY_AND_ASSIGN(RenderTexture)
};

class MultiRenderTarget : public RenderTarget
{
  public:
		explicit MultiRenderTarget(unsigned width, unsigned height, unsigned count, unsigned msaa_sample = 0);
		virtual ~MultiRenderTarget();
		virtual void Bind() override;
		virtual void BlitMSAAFBO() override;

		unsigned frameBuffer = 0;
		std::vector<unsigned> textures;
		unsigned depthRbo = 0;
		unsigned msaaFrameBuffer = 0;
		std::vector<unsigned> msaaTextures;

	private:
		unsigned msaa_sample_ = 0;
		DISALLOW_COPY_AND_ASSIGN(MultiRenderTarget)
};

} // namespace kd