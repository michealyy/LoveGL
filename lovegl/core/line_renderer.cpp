#include "line_renderer.h"
#include <GL/glew.h>
#include <engine.h>
#include <resource_manager.h>
#include <glm/glm.hpp>


using namespace glm;

namespace kd
{

LineRenderer::LineRenderer()
{
}

LineRenderer::~LineRenderer()
{
}

void LineRenderer::Setup()
{
    glGenVertexArrays(1, &line_vao_);
    glBindVertexArray(line_vao_);
    glGenBuffers(1, &line_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, line_vbo_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
}

void LineRenderer::Render()
{
}

void LineRenderer::DrawDebugLine(const float vertices[6])
{
    //TODO:支持多个线段一起提交绘制
    auto camera = Engine::GetInstance()->mainCamera;
    auto shader = ResourceManager::GetInstance()->GetShader("unlit_pos");
    shader->Bind();
    shader->SetMatrix("mvp", camera->projectMatrix * camera->GetViewMatrix());
    shader->SetFloat("alpha", 1);
    shader->SetVector3("color", glm::vec3(0, 1, 0));

    glBindVertexArray(line_vao_);

    //float _vertices[] = {0,0,0, 0,2,0};
    glBindBuffer(GL_ARRAY_BUFFER, line_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertices, GL_STATIC_DRAW);
    
    glDrawArrays(GL_LINE_LOOP, 0, 2);
}

}