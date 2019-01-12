#include "entity_box.h"

using namespace glm;
using namespace kd::va;

namespace kd
{

Box::Box(Material *mat) : Mesh(mat)
{
}

Box::~Box()
{
}

void Box::AddVertices()
{
    P_C vertex0;
    vertex0.Position = vec3(0.5f, 0.5f, 0);
    vertex0.Color = vec3(0, 1, 0);
    this->vertices_p_c.push_back(vertex0);

    P_C vertex1;
    vertex1.Position = vec3(0.5f, -0.5f, 0);
    vertex1.Color = vec3(1, 0, 0);
    this->vertices_p_c.push_back(vertex1);

    P_C vertex2;
    vertex2.Position = vec3(-0.5f, -0.5f, 0);
    vertex2.Color = vec3(1, 0, 0);
    this->vertices_p_c.push_back(vertex2);

    P_C vertex3;
    vertex3.Position = vec3(-0.5f, 0.5f, 0);
    vertex3.Color = vec3(1, 0, 0);
    this->vertices_p_c.push_back(vertex3);

    this->indices.push_back(0);
    this->indices.push_back(1);
    this->indices.push_back(3);

    this->indices.push_back(1);
    this->indices.push_back(2);
    this->indices.push_back(3);
}

} // namespace kd