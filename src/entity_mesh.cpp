#include "entity_mesh.h"
#include <filesystem>
#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
#include "engine.h"
#include "renderer.h"
#include "config.h"

using namespace std;
using namespace glm;

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

    if (vertices_pos.size() < 2 && vertices_pos_tex.size() < 2)
    {
        fprintf(stderr, "[mesh] vertices size < 2");
        return;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    if (vertices_pos.size() > 1)
    {
        GLsizei stride = sizeof(va::Pos);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_pos.size() * stride, &vertices_pos[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    }
    else if (vertices_pos_tex.size() > 1)
    {
        GLsizei stride = sizeof(va::Pos_Tex);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices_pos_tex.size() * stride, &vertices_pos_tex[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(va::Pos_Tex, TexCoords));
    }
}

void Mesh::Update(float deltaTime)
{
    Entity::Update(deltaTime);

    Renderer::GetInstance()->AddMesh(this);
}

void Mesh::AddVertices()
{
}

bool Mesh::Raycast(Ray ray, RayCastHit &rayCastHit)
{
    if (vertices_pos_tex.size() <= 0)
        return false;
    
    //遍历几何体所有三角面进行射线碰撞检测
    for (int i = 0; i < indices.size() - 3; i = i + 3)
    {
        
        auto va_index_1 = indices[i];
        auto va_index_2 = indices[i + 1];
        auto va_index_3 = indices[i + 2];
        
        vec3 va_1 = worldTransform * vec4(vertices_pos_tex[va_index_1].Position, 1.f);
        vec3 va_2 = worldTransform * vec4(vertices_pos_tex[va_index_2].Position, 1.f);
        vec3 va_3 = worldTransform * vec4(vertices_pos_tex[va_index_3].Position, 1.f);

        vec2 bary_pos;
        float distance;
        if (intersectRayTriangle(ray.origin, ray.direction, va_1, va_2, va_3, bary_pos, distance))
        {
            rayCastHit.entity = this;
            rayCastHit.point = bary_pos;
            rayCastHit.distance = distance;
            return true;
        }
    }

    //遍历子节点
    for (auto child : children_)
	{
        auto _child = dynamic_cast<Mesh*>(child);
        if (_child)
            return _child->Raycast(ray, rayCastHit);
	}

    return false;
}

void Mesh::SetMesh(const std::string &name)
{
    auto mesh = Engine::GetInstance()->GetMesh(name);
    if (mesh)
    {
        this->vertices_pos_tex = mesh->vertices;
        this->indices = mesh->indices;
    }
}

} // namespace kd