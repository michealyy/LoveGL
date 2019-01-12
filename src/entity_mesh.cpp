#include "entity_mesh.h"
#include <glad/glad.h>

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
    glDisable(GL_CULL_FACE);

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

    Entity::Setup();
}

void Mesh::Update(float deltaTime)
{
    if (material == nullptr)
    {
        return;
    }

    material->Bind();

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //TODO: 绘制线框
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);

    Entity::Update(deltaTime);
}

namespace va
{
}

} // namespace kd