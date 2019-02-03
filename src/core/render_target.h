#pragma once

#include "common.h"

namespace kd
{

class RenderTarget
{
public:
	explicit RenderTarget();
	virtual ~RenderTarget();
	virtual void Bind();

	unsigned width = 0;
	unsigned height = 0;
private:

	DISALLOW_COPY_AND_ASSIGN(RenderTarget)
};

class GLFWRenderWindow : public RenderTarget
{
public:
	explicit GLFWRenderWindow();
	virtual ~GLFWRenderWindow();

private:

	DISALLOW_COPY_AND_ASSIGN(GLFWRenderWindow)
};

class RenderTexture : public RenderTarget
{
public:
	explicit RenderTexture(unsigned width, unsigned height);
	virtual ~RenderTexture();
	virtual void Bind() override;

	unsigned frameBuffer = 0;
	unsigned texture = 0;
	unsigned renderBuffer = 0;

private:

	DISALLOW_COPY_AND_ASSIGN(RenderTexture)
};

class MultiRenderTarget : public RenderTarget
{
public:
	explicit MultiRenderTarget();
	virtual ~MultiRenderTarget();

private:

	DISALLOW_COPY_AND_ASSIGN(MultiRenderTarget)
};

} // namespace kd