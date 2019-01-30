#include "render_target.h"
#include <glad/glad.h>
#include <cstdio>

namespace kd
{

RenderTarget::RenderTarget()
{
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLFWRenderWindow::GLFWRenderWindow()
{
}

GLFWRenderWindow::~GLFWRenderWindow()
{
}

RenderTexture::RenderTexture(unsigned width, unsigned height)
{
    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    //渲染到贴图
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    //Renderbuffer作为深度模板缓冲区
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTexture::~RenderTexture()
{
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &renderBuffer);
}

void RenderTexture::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

MultiRenderTarget::MultiRenderTarget()
{
}

MultiRenderTarget::~MultiRenderTarget()
{
}

}