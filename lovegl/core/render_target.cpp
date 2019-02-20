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

RenderTexture::RenderTexture(unsigned width, unsigned height, unsigned msaa_sample)
{
    this->width = width;
    this->height = height;
    this->msaa_sample_ = msaa_sample;

    if (msaa_sample == 0)
    {
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        //渲染到贴图
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //HDR GL_RGB16F
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
        //Renderbuffer作为深度缓冲区。暂时没有用到模板，不需要模板缓冲区
        glGenRenderbuffers(1, &depthRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");
    }
    else
    {
        glGenFramebuffers(1, &msaaFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);
        glGenTextures(1, &msaaTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaTexture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa_sample, GL_RGB16F, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaaTexture, 0);
        glGenRenderbuffers(1, &depthRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa_sample, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");
        
        //用来复制有msaa的fbo
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");
    }
}

RenderTexture::~RenderTexture()
{
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &depthRbo);
}

void RenderTexture::Bind()
{
    if (msaa_sample_ == 0)
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    else
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);
}

void RenderTexture::BlitMSAAFBO()
{
    if (msaa_sample_ == 0)
        return;
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

MultiRenderTarget::MultiRenderTarget(unsigned width, unsigned height, unsigned count, unsigned msaa_sample)
{
    this->width = width;
    this->height = height;
    this->msaa_sample_ = msaa_sample;

    if (msaa_sample == 0)
    {
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        //多张贴图绑定到片段多个输出
        unsigned attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        for (unsigned i = 0; i < count; i++)
        {
            unsigned texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            //HDR GL_RGB16F
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);
            textures.push_back(texture);
        }
        //必须调用，默认shader只会渲染到第一个
        glDrawBuffers(count, &attachments[0]);
        //深度缓冲区
        glGenRenderbuffers(1, &depthRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");
    }
    else
    {
        glGenFramebuffers(1, &msaaFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);
        unsigned attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        for (unsigned i = 0; i < count; i++)
        {
            unsigned msaaTexture;
            glGenTextures(1, &msaaTexture);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaTexture);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa_sample, GL_RGB16F, width, height, GL_TRUE);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, msaaTexture, 0);
            msaaTextures.push_back(msaaTexture);
        }
        glDrawBuffers(count, &attachments[0]);
        glGenRenderbuffers(1, &depthRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa_sample, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");

        //用来复制有msaa的fbo
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        for (unsigned i = 0; i < count; i++)
        {
            unsigned texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);
            textures.push_back(texture);
        }
        //必须调用，默认shader只会渲染到第一个
        glDrawBuffers(count, &attachments[0]);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");
    }
}

MultiRenderTarget::~MultiRenderTarget()
{
    glDeleteFramebuffers(1, &frameBuffer);
    for (auto texture : textures)
        glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &depthRbo);
}

void MultiRenderTarget::Bind()
{
    if (msaa_sample_ == 0)
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    else
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);
}

void MultiRenderTarget::BlitMSAAFBO()
{
    if (msaa_sample_ == 0)
        return;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

} // namespace kd