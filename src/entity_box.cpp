#include "entity_box.h"

using namespace glm;
using namespace kd::va;

namespace kd
{

Box::Box(Material *mat, float width, float height, float depth)
    : width(width), height(height), depth(depth)
{
    this->material = mat;
}

Box::~Box()
{
}

void Box::AddVertices()
{
    float halfWidth = width/2.0f;
    float halfHeight = height / 2.0f;
    float halfDepth = depth / 2.0f;

    Pos_Tex vertex0;
    vertex0.Position = vec3(halfWidth, halfHeight, halfDepth);
    vertex0.TexCoords = vec2(1.0f, 1.0f);
    this->vertices_pos_tex.push_back(vertex0);

    Pos_Tex vertex1;
    vertex1.Position = vec3(halfWidth, -halfHeight, halfDepth);
    vertex1.TexCoords = vec2(1.0f, 0.0f);
    this->vertices_pos_tex.push_back(vertex1);

    Pos_Tex vertex2;
    vertex2.Position = vec3(-halfWidth, -halfHeight, halfDepth);
    vertex2.TexCoords = vec2(0.0f, 0.0f);
    this->vertices_pos_tex.push_back(vertex2);

    Pos_Tex vertex3;
    vertex3.Position = vec3(-halfWidth, halfHeight, halfDepth);
    vertex3.TexCoords = vec2(0.0f, 1.0f);
    this->vertices_pos_tex.push_back(vertex3);

    Pos_Tex vertex4;
    vertex4.Position = vec3(halfWidth, halfHeight, -halfDepth);
    vertex4.TexCoords = vec2(1.0f, 1.0f);
    this->vertices_pos_tex.push_back(vertex4);

    Pos_Tex vertex5;
    vertex5.Position = vec3(halfWidth, -halfHeight, -halfDepth);
    vertex5.TexCoords = vec2(1.0f, 0.0f);
    this->vertices_pos_tex.push_back(vertex5);

    Pos_Tex vertex6;
    vertex6.Position = vec3(-halfWidth, -halfHeight, -halfDepth);
    vertex6.TexCoords = vec2(0.0f, 0.0f);
    this->vertices_pos_tex.push_back(vertex6);

    Pos_Tex vertex7;
    vertex7.Position = vec3(-halfWidth, halfHeight, -halfDepth);
    vertex7.TexCoords = vec2(0.0f, 1.0f);
    this->vertices_pos_tex.push_back(vertex7);

    //front
    this->indices.push_back(0);
    this->indices.push_back(3);
    this->indices.push_back(1);

    this->indices.push_back(1);
    this->indices.push_back(3);
    this->indices.push_back(2);

    //back
    this->indices.push_back(4);
    this->indices.push_back(7);
    this->indices.push_back(5);

    this->indices.push_back(5);
    this->indices.push_back(7);
    this->indices.push_back(6);

    //left
    this->indices.push_back(2);
    this->indices.push_back(7);
    this->indices.push_back(3);

    this->indices.push_back(2);
    this->indices.push_back(6);
    this->indices.push_back(7);

    //right
    this->indices.push_back(0);
    this->indices.push_back(5);
    this->indices.push_back(4);

    this->indices.push_back(0);
    this->indices.push_back(1);
    this->indices.push_back(5);

    //top
    this->indices.push_back(0);
    this->indices.push_back(7);
    this->indices.push_back(4);

    this->indices.push_back(0);
    this->indices.push_back(3);
    this->indices.push_back(7);

    //botoom
    this->indices.push_back(5);
    this->indices.push_back(2);
    this->indices.push_back(1);

    this->indices.push_back(5);
    this->indices.push_back(6);
    this->indices.push_back(2);
}

} // namespace kd