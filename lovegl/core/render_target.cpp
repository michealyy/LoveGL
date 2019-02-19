#include "render_target.h"
#include <GL/glew.h>
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
    
    //Renderbuffer作为深度缓冲区。暂时没有用到模板，不需要模板缓冲区
    glGenRenderbuffers(1, &depth_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbo);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTexture::~RenderTexture()
{
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &depth_rbo);
}

void RenderTexture::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

MultiRenderTarget::MultiRenderTarget(unsigned width, unsigned height, unsigned count)
{
    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    unsigned attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    for (unsigned i = 0; i < count; i++)
    {
        unsigned texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);

        textures.push_back(texture);
    }
    //必须调用，默认只会渲染到一个
    glDrawBuffers(count, &attachments[0]);

    //深度缓冲区
    glGenRenderbuffers(1, &depth_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

MultiRenderTarget::~MultiRenderTarget()
{
    glDeleteFramebuffers(1, &frameBuffer);
    for (auto texture : textures)
        glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &depth_rbo);
}

void MultiRenderTarget::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

} // namespace kd