#pragma once

#include "common.h"

namespace kd
{

class RenderTarget
{
public:
	explicit RenderTarget();
	virtual ~RenderTarget();

private:

	DISALLOW_COPY_AND_ASSIGN(RenderTarget)
};

class RenderTexture : public RenderTarget
{
public:
	explicit RenderTexture();
	virtual ~RenderTexture();

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

class GLFWRenderWindow : public RenderTarget
{
public:
	explicit GLFWRenderWindow();
	virtual ~GLFWRenderWindow();

private:

	DISALLOW_COPY_AND_ASSIGN(GLFWRenderWindow)
};

} // namespace kd