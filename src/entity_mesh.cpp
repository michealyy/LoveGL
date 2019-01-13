#include "entity_mesh.h"
#include <glad/glad.h>
#include "engine.h"

using namespace std;

namespace kd
{

Mesh::Mesh(Material *mat) : material(mat)
{
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Mesh::Setup()
{
    Entity::Setup();

    AddVertices();

    if (vertices_p_c.size() < 2)
    {
        fprintf(stderr, "[mesh] vertices size < 2");
        return;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    if (vertices_p_c.size() > 1)
    {
        GLsizei stride = sizeof(va::P_C);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_p_c.size() * stride, &vertices_p_c[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(va::P_C, Color));
    }
}

void Mesh::Update(float deltaTime)
{
    Entity::Update(deltaTime);

    auto camera = Engine::GetInstance()->mainCamera;

    if (material == nullptr || camera == nullptr)
    {
        return;
    }

    material->Bind();
    material->SetMatrix("mvp", camera->projectMatrix * camera->GetViewMatrix() * worldTransform);

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
}

} // namespace kd