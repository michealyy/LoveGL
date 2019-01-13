#include "entity_box.h"

using namespace glm;
using namespace kd::va;

namespace kd
{

Box::Box(Material *mat, float width, float height, float length, glm::vec3 color, float alpha)
    : Mesh(mat), width(width), height(height), length(length), color(color), alpha(alpha)
{
}

Box::~Box()
{
}

void Box::AddVertices()
{
    float halfWidth = width/2.0f;
    float halfHeight = height / 2.0f;
    float halfLength = length / 2.0f;

    P_C vertex0;
    vertex0.Position = vec3(halfWidth, halfHeight, 0);
    vertex0.Color = color;
    this->vertices_p_c.push_back(vertex0);

    P_C vertex1;
    vertex1.Position = vec3(halfWidth, -halfHeight, 0);
    vertex1.Color = color;
    this->vertices_p_c.push_back(vertex1);

    P_C vertex2;
    vertex2.Position = vec3(-halfWidth, -halfHeight, 0);
    vertex2.Color = color;
    this->vertices_p_c.push_back(vertex2);

    P_C vertex3;
    vertex3.Position = vec3(-halfWidth, halfHeight, 0);
    vertex3.Color = color;
    this->vertices_p_c.push_back(vertex3);

    this->indices.push_back(0);
    this->indices.push_back(1);
    this->indices.push_back(3);

    this->indices.push_back(1);
    this->indices.push_back(2);
    this->indices.push_back(3);
}

} // namespace kd