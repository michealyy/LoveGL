#include "render_target.h"

namespace kd
{

RenderTarget::RenderTarget()
{
}

RenderTarget::~RenderTarget()
{
}

RenderTexture::RenderTexture()
{
    // glGenFramebuffers(1, &post_processing_framebuffer_);
    // glBindFramebuffer(GL_FRAMEBUFFER, post_processing_framebuffer_);
    // //渲染到贴图
    // glGenTextures(1, &post_processing_texture_);
    // glBindTexture(GL_TEXTURE_2D, post_processing_texture_);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, post_processing_texture_, 0);
    // //Renderbuffer作为深度模板缓冲区
    // glGenRenderbuffers(1, &post_processing_render_buffer_);
    // glBindRenderbuffer(GL_RENDERBUFFER, post_processing_render_buffer_);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, post_processing_render_buffer_);
    // glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    //     fprintf(stderr, "[Render] Post Processing Framebuffer is not complete");
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderTexture::~RenderTexture()
{
}

MultiRenderTarget::MultiRenderTarget()
{
}

MultiRenderTarget::~MultiRenderTarget()
{
}

GLFWRenderWindow::GLFWRenderWindow()
{
}

GLFWRenderWindow::~GLFWRenderWindow()
{
}

}