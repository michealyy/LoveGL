#include "entity_box.h"

using namespace glm;
using namespace kd::va;

namespace kd
{

Box::Box(Material *mat, float width, float height, float depth, glm::vec3 color)
    : Mesh(mat), width(width), height(height), depth(depth), color(color)
{
}

Box::~Box()
{
}

void Box::AddVertices()
{
    float halfWidth = width/2.0f;
    float halfHeight = height / 2.0f;
    float halfDepth = depth / 2.0f;

    P_C vertex0;
    vertex0.Position = vec3(halfWidth, halfHeight, halfDepth);
    vertex0.Color = color;
    this->vertices_p_c.push_back(vertex0);

    P_C vertex1;
    vertex1.Position = vec3(halfWidth, -halfHeight, halfDepth);
    vertex1.Color = color;
    this->vertices_p_c.push_back(vertex1);

    P_C vertex2;
    vertex2.Position = vec3(-halfWidth, -halfHeight, halfDepth);
    vertex2.Color = color;
    this->vertices_p_c.push_back(vertex2);

    P_C vertex3;
    vertex3.Position = vec3(-halfWidth, halfHeight, halfDepth);
    vertex3.Color = color;
    this->vertices_p_c.push_back(vertex3);

    P_C vertex4;
    vertex4.Position = vec3(halfWidth, halfHeight, -halfDepth);
    vertex4.Color = color;
    this->vertices_p_c.push_back(vertex4);

    P_C vertex5;
    vertex5.Position = vec3(halfWidth, -halfHeight, -halfDepth);
    vertex5.Color = color;
    this->vertices_p_c.push_back(vertex5);

    P_C vertex6;
    vertex6.Position = vec3(-halfWidth, -halfHeight, -halfDepth);
    vertex6.Color = color;
    this->vertices_p_c.push_back(vertex6);

    P_C vertex7;
    vertex7.Position = vec3(-halfWidth, halfHeight, -halfDepth);
    vertex7.Color = color;
    this->vertices_p_c.push_back(vertex7);

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