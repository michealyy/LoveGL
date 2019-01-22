#include "entity_mesh.h"
#include <glad/glad.h>
#include "engine.h"
#include "renderer.h"

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

    if (vertices_p_c.size() < 2 && vertices_pos_tex.size() < 2)
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
    else if(vertices_pos_tex.size() > 1)
    {
        GLsizei stride = sizeof(va::Pos_Tex);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_pos_tex.size() * stride, &vertices_pos_tex[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(va::Pos_Tex, TexCoords));
    }
}

void Mesh::Update(float deltaTime)
{
    Entity::Update(deltaTime);

    if (isTransparent)
    {
        Renderer::GetInstance()->AddTransparent(this);
    }
    else
    {
        Renderer::GetInstance()->AddOpaque(this);
    }
}

} // namespace kd