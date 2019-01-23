#include "entity_cone.h"
#include <cmath>

using namespace glm;

namespace kd
{

Cone::Cone(Material *mat, float radius, int segments, int height)
    : radius(radius), segments(segments), height(height)
{
    this->material = mat;
}

Cone::~Cone()
{
}

void Cone::AddVertices()
{
    if (segments < 8)
        segments = 8;

    float halfHeight = height / 2.0f;

    //circle center point
    va::Pos vertex0;
    vertex0.Position = vec3(0, -halfHeight, 0);
    this->vertices_pos.push_back(vertex0);

    for (int i = 0; i <= segments; i++)
    {
        float segmentAngle = i * (PI_FLOAT * 2 / segments);
        float x = radius * cos(segmentAngle);
        float y = radius * sin(segmentAngle);

        va::Pos vertex;
        vertex.Position = vec3(x, -halfHeight, y);
        this->vertices_pos.push_back(vertex);
    }

    for (int i = 1; i <= segments; i++)
    {
        this->indices.push_back(i + 1);
        this->indices.push_back(i);
        this->indices.push_back(0);
    }

    //top point
    va::Pos vertex1;
    vertex1.Position = vec3(0, halfHeight, 0);
    this->vertices_pos.push_back(vertex1);

    for (int i = 1; i <= segments; i++)
    {
        this->indices.push_back(i + 1);
        this->indices.push_back(i);
        this->indices.push_back(segments + 2);
    }
}

} // namespace kd