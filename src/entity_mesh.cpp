#include "entity_mesh.h"
#include <filesystem>
#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
#include "engine.h"
#include "renderer.h"
#include "config.h"
#include <tiny_gltf.h>

using namespace std;
using namespace glm;

namespace kd
{

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
void SubMesh::SetupFromGLTF(tinygltf::Model &model, tinygltf::Primitive &primitive)
{
    //TODO: 材质
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    auto gltf_vbos = Engine::GetInstance()->sceneManager->GetGltfVbos();
    //顶点属性绑定
    for (auto attribute : primitive.attributes)
    {
        auto accessor = model.accessors[attribute.second];

        //每个顶点数据单元大小
        int size = 1;
        if (accessor.type == TINYGLTF_TYPE_SCALAR)
            size = 1;
        else if (accessor.type == TINYGLTF_TYPE_VEC2)
            size = 2;
        else if (accessor.type == TINYGLTF_TYPE_VEC3)
            size = 3;
        else if (accessor.type == TINYGLTF_TYPE_VEC4)
            size = 4;
        else
            fprintf(stderr, "[WARNING][SubMesh]: not support vertex attribute");

        int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
        glBindBuffer(GL_ARRAY_BUFFER, gltf_vbos[accessor.bufferView]);
        
        //绑定不同顶点属性到对应通道，让顶点着色器接收到
        if (attribute.first == "POSITION")
        {
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, size, accessor.componentType, accessor.normalized ? GL_TRUE : GL_FALSE, byteStride, BUFFER_OFFSET(accessor.byteOffset));
        }
        else if (attribute.first == "TEXCOORD_0")
        {
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, size, accessor.componentType, accessor.normalized ? GL_TRUE : GL_FALSE, byteStride, BUFFER_OFFSET(accessor.byteOffset));
        }
        else if (attribute.first == "NORMAL")
        {
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, size, accessor.componentType, accessor.normalized ? GL_TRUE : GL_FALSE, byteStride, BUFFER_OFFSET(accessor.byteOffset));
        }
        else
        {
            fprintf(stderr, "[WARNING][SubMesh]: not support vertex attribute %s", attribute.first.c_str());
        }
    }
    //顶点绘制索引
    auto indexAccessor = model.accessors[primitive.indices];
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gltf_vbos[indexAccessor.bufferView]);
    
    glBindVertexArray(0);

    draw_mode = GL_TRIANGLES;
    draw_count = (unsigned)indexAccessor.count;
    draw_type = indexAccessor.componentType;
}

void SubMesh::Draw()
{
    glBindVertexArray(vao);
    //glDrawElements(mode, indexAccessor.count, indexAccessor.componentType, BUFFER_OFFSET(indexAccessor.byteOffset));
    glDrawElements(draw_mode, draw_count, draw_type, 0);
    glBindVertexArray(0);
}

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

    if (vertices.size() < 2 && vertices_pos.size() < 2 && vertices_pos_tex.size() < 2)
    {
        fprintf(stderr, "[mesh] vertices size < 2");
        return;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    if (vertices.size() > 1)
    {
        GLsizei stride = sizeof(va::P_T_N_T_B);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * stride, &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        //positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
        //texture coords
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(va::P_T_N_T_B, TexCoords));
        //normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(va::P_T_N_T_B, Normal));
        //tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(va::P_T_N_T_B, Tangent));
        //bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(va::P_T_N_T_B, Bitangent));
    }

    // if (vertices_pos.size() > 1)
    // {
    //     GLsizei stride = sizeof(va::Pos);
    //     glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //     glBufferData(GL_ARRAY_BUFFER, vertices_pos.size() * stride, &vertices_pos[0], GL_STATIC_DRAW);

    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //     glEnableVertexAttribArray(0);
    //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    // }
    // else if (vertices_pos_tex.size() > 1)
    // {
    //     GLsizei stride = sizeof(va::Pos_Tex);
    //     glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //     glBufferData(GL_ARRAY_BUFFER, vertices_pos_tex.size() * stride, &vertices_pos_tex[0], GL_STATIC_DRAW);

    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    //     glEnableVertexAttribArray(0);
    //     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    //     glEnableVertexAttribArray(1);
    //     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)offsetof(va::Pos_Tex, TexCoords));
    // }
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
    if (vertices.size() <= 0)
        return false;
    
    //遍历几何体所有三角面进行射线碰撞检测
    for (int i = 0; i < indices.size() - 3; i = i + 3)
    {
        
        auto va_index_1 = indices[i];
        auto va_index_2 = indices[i + 1];
        auto va_index_3 = indices[i + 2];
        
        vec3 va_1 = worldTransform * vec4(vertices[va_index_1].Position, 1.f);
        vec3 va_2 = worldTransform * vec4(vertices[va_index_2].Position, 1.f);
        vec3 va_3 = worldTransform * vec4(vertices[va_index_3].Position, 1.f);

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
        this->vertices = mesh->vertices;
        this->indices = mesh->indices;
    }
}

} // namespace kd