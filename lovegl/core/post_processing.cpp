#include "post_processing.h"
#include <GL/glew.h>
#include <engine.h>
#include <core/render_target.h>
#include <resource_manager.h>

namespace kd
{

PostProcessing::PostProcessing()
{
    //普铺满屏幕的矩形
    float vertices[] = {
        -1.0,
        1.0,
        0.0,
        1.0,
        -1.0,
        -1.0,
        0.0,
        0.0,
        1.0,
        -1.0,
        1.0,
        0.0,
        -1.0,
        1.0,
        0.0,
        1.0,
        1.0,
        -1.0,
        1.0,
        0.0,
        1.0,
        1.0,
        1.0,
        1.0,
    };
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(sizeof(float) * 2));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

PostProcessing::~PostProcessing()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

void PostProcessing::Setup()
{
}

void PostProcessing::Draw()
{
    if (camera == nullptr)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (shader)
        shader->Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostGray::Setup()
{
    if (camera == nullptr)
        return;

    int width, height;
    glfwGetWindowSize(Engine::GetInstance()->GetMainWindow(), &width, &height);

    auto renderTexture = new RenderTexture(width, height);
    camera->renderTarget = renderTexture;

    shader = ResourceManager::GetInstance()->GetShader("post_normal");
    texture = renderTexture->texture;
}

Bloom::Bloom()
{}

Bloom::~Bloom()
{
    SafeDelete(multiRenderTarget_);
    SafeDelete(blur_rtt_);
    SafeDelete(blur_temp_rtt_);
}

void Bloom::Setup()
{
    if (camera == nullptr)
        return;

    int width, height;
    glfwGetWindowSize(Engine::GetInstance()->GetMainWindow(), &width, &height);

    multiRenderTarget_ = new MultiRenderTarget(width, height, 2);
    camera->renderTarget = multiRenderTarget_;

    blur_rtt_ = new RenderTexture(width, height);
    blur_temp_rtt_ = new RenderTexture(width, height);
}

void Bloom::Draw()
{
    if (camera == nullptr || multiRenderTarget_ == nullptr)
        return;

    auto mrt = multiRenderTarget_->textures;
    assert(mrt.size() == 2);

    //低亮度
    auto lowTexture = mrt[0];
    //高亮度
    auto highTexture = mrt[1];
    
    /**********高亮度贴图进行多次模糊，使用一个临时缓冲区，来回模糊***********/
    auto blurShader = ResourceManager::GetInstance()->GetShader("bloom_blur");
    blurShader->Bind();
    //第一次绘制到temp
    glBindFramebuffer(GL_FRAMEBUFFER, blur_temp_rtt_->frameBuffer);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, highTexture);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //轮回绘制
    bool isTemp = false;
    for (unsigned i = 0; i < blurCount; i++)
    {
        if (i < blurCount/2)
            blurShader->SetInt("horizontal", 1);
        else
            blurShader->SetInt("horizontal", 0);
        unsigned fbo = 0;
        if (isTemp)
            fbo = blur_temp_rtt_->frameBuffer;
        else
            fbo = blur_rtt_->frameBuffer;
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glActiveTexture(GL_TEXTURE0);
        unsigned tex = 0;
        if (isTemp)
            tex = blur_rtt_->texture;
        else
            tex = blur_temp_rtt_->texture;
        glBindTexture(GL_TEXTURE_2D, tex);
        glBindVertexArray(vao_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        isTemp = !isTemp;
    }

    /***********合并低亮度和模糊高亮度图*****************/
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    auto shader = ResourceManager::GetInstance()->GetShader("bloom");
    shader->Bind();
    shader->SetFloat("factor", factor);
    shader->SetFloat("exposure", camera->exposure);
    shader->SetInt("image", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lowTexture);
    shader->SetInt("blur", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, blur_temp_rtt_->texture);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

} // namespace kd