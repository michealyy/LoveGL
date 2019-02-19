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

    shader = ResourceManager::GetInstance()->GetShader("post_gray");
    texture = renderTexture->texture;
}

Bloom::Bloom()
{}

Bloom::~Bloom()
{}

void Bloom::Setup()
{
    if (camera == nullptr)
        return;

    int width, height;
    glfwGetWindowSize(Engine::GetInstance()->GetMainWindow(), &width, &height);

    auto multiRenderTarget = new MultiRenderTarget(width, height, 2);
    camera->renderTarget = multiRenderTarget;

    // shader = ResourceManager::GetInstance()->GetShader("post_gray");
    // texture = renderTexture->texture;
}

void Bloom::Draw()
{
    // bind blur shader
    //Draw

    // combin shader

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // if (shader)
    //     shader->Bind();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture);
    // glBindVertexArray(vao_);
    // glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
}

} // namespace kd