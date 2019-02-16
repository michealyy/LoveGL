#include "mesh.h"
#include <filesystem>
#include <glad/glad.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
#include <engine.h>
#include <core/renderer.h>
#include <resource_manager.h>
#include "config.h"
#include <tiny_gltf.h>

using namespace std;
using namespace glm;

namespace kd
{

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    for (auto subMesh : subMeshes)
    {
        SafeDelete(subMesh);
    }
}

bool Mesh::Raycast(Ray ray, RayCastHit &rayCastHit)
{
    for (auto subMesh : subMeshes)
    {
        if (subMesh->Raycast(ray, rayCastHit))
            return true;
    }

    //遍历子节点
    for (auto child : children_)
    {
        auto _child = dynamic_cast<Mesh *>(child);
        if (_child)
            return _child->Raycast(ray, rayCastHit);
    }

    return false;
}

void Mesh::AddSubMesh(SubMesh *subMesh)
{
    subMeshes.push_back(subMesh);
    subMesh->mesh = this;
}

void Mesh::SetMesh(const std::string &name)
{
    auto meshData = ResourceManager::GetInstance()->GetMesh(name);
    if (meshData)
    {
        auto subMesh = new SubMesh();
        //subMesh->Setup(meshData);
    }
}

SubMesh::SubMesh()
{
}

SubMesh::~SubMesh()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}

void SubMesh::Setup()
{
    if (indices.size() < 1)
        return;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao);

    GLsizei stride = sizeof(va::P_T_N_T_B);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * stride, &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

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

    glBindVertexArray(0);
}

void SubMesh::SetupFromGLTF(tinygltf::Model &model, tinygltf::Primitive &primitive)
{
    //顶点属性
    auto pos_accessor = model.accessors[primitive.attributes["POSITION"]];
    auto pos_bufferView = model.bufferViews[pos_accessor.bufferView];
    auto pos_bufferData = model.buffers[pos_bufferView.buffer].data;
    auto pos_index = pos_bufferView.byteOffset;
    assert(pos_accessor.byteOffset == 0);

    auto uv_accessor = model.accessors[primitive.attributes["TEXCOORD_0"]];
    auto uv_bufferView = model.bufferViews[uv_accessor.bufferView];
    auto uv_bufferData = model.buffers[uv_bufferView.buffer].data;
    auto uv_index = uv_bufferView.byteOffset;
    assert(uv_accessor.byteOffset == 0);

    auto normal_accessor = model.accessors[primitive.attributes["NORMAL"]];
    auto normal_bufferView = model.bufferViews[normal_accessor.bufferView];
    auto normal_bufferData = model.buffers[normal_bufferView.buffer].data;
    auto normal_index = normal_bufferView.byteOffset;
    assert(normal_accessor.byteOffset == 0);

    assert(pos_accessor.count == uv_accessor.count && pos_accessor.count == normal_accessor.count);
    for (unsigned i = 0; i < pos_accessor.count; i++)
    {
        va::P_T_N_T_B vertex;
        {
            float p1 = *(float *)&pos_bufferData[pos_index];
            float p2 = *(float *)&pos_bufferData[pos_index + sizeof(float)];
            float p3 = *(float *)&pos_bufferData[pos_index + sizeof(float) * 2];
            vertex.Position = glm::vec3(p1, p2, p3);
            pos_index = pos_index + sizeof(float) * 3;
        }
        {
            float p1 = *(float *)&uv_bufferData[uv_index];
            float p2 = *(float *)&uv_bufferData[uv_index + sizeof(float)];
            vertex.TexCoords = glm::vec2(p1, p2);
            uv_index = uv_index + sizeof(float) * 2;
        }
        {
            float p1 = *(float *)&normal_bufferData[normal_index];
            float p2 = *(float *)&normal_bufferData[normal_index + sizeof(float)];
            float p3 = *(float *)&normal_bufferData[normal_index + sizeof(float) * 2];
            vertex.Normal = glm::vec3(p1, p2, p3);
            normal_index = normal_index + sizeof(float) * 3;
        }
        vertex.CalculateTB();
        vertices.push_back(vertex);
    }

    //顶点绘制索引
    auto index_accessor = model.accessors[primitive.indices];
    auto index_bufferView = model.bufferViews[index_accessor.bufferView];
    auto index_bufferData = model.buffers[index_bufferView.buffer].data;
    auto index_index = index_bufferView.byteOffset;
    assert(index_accessor.byteOffset == 0);
    for (unsigned i = 0; i < index_accessor.count; i++)
    {
        if(index_accessor.componentType == GL_UNSIGNED_SHORT)
        {
            unsigned short index = *(unsigned short *)&index_bufferData[index_index];
            indices.push_back(index);
            index_index = index_index + sizeof(unsigned short);
        }
    }

    /*****解析材质信息*****/
    if (primitive.material > 0)
    {
        auto gltf_mat = model.materials[primitive.material];
        this->material = ResourceManager::GetInstance()->GetMaterial(gltf_mat.name);
    }

    Setup();
}

void SubMesh::Draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (unsigned)indices.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

bool SubMesh::Raycast(Ray ray, RayCastHit &rayCastHit)
{
    if (mesh == nullptr || vertices.size() < 1)
        return false;

    //遍历几何体所有三角面进行射线碰撞检测
    for (int i = 0; i < indices.size() - 3; i = i + 3)
    {
        auto va_index_1 = indices[i];
        auto va_index_2 = indices[i + 1];
        auto va_index_3 = indices[i + 2];

        vec3 va_1 = mesh->worldTransform * vec4(vertices[va_index_1].Position, 1.f);
        vec3 va_2 = mesh->worldTransform * vec4(vertices[va_index_2].Position, 1.f);
        vec3 va_3 = mesh->worldTransform * vec4(vertices[va_index_3].Position, 1.f);

        vec2 bary_pos;
        float distance;
        if (intersectRayTriangle(ray.origin, ray.direction, va_1, va_2, va_3, bary_pos, distance))
        {
            rayCastHit.node = mesh;
            rayCastHit.point = bary_pos;
            rayCastHit.distance = distance;
            return true;
        }
    }

    return false;
}

} // namespace kd