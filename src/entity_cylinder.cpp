#include "entity_cylinder.h"
#include <cmath>

using namespace glm;
using namespace kd::va;

namespace kd
{

Cylinder::Cylinder(Material *mat, float radius, int segments, int height)
    : Mesh(mat), radius(radius), segments(segments), height(height)
{
}

Cylinder::~Cylinder()
{
}

void Cylinder::AddVertices()
{
    GenerateCap(true, 0);
    GenerateCap(false, segments + 2);

    int a = 0;
}

void Cylinder::GenerateCap(bool isTop, int startIndex)
{
    float halfHeight = 0.f;
    if (isTop)
        halfHeight = height / 2.0f;
    else
        halfHeight = -height / 2.0f;

    //center point
    P_C vertex0;
    vertex0.Position = vec3(0, halfHeight, 0);
    vertex0.Color = color;
    this->vertices_p_c.push_back(vertex0);

    for (int i = 0; i <= segments; i++)
    {
        float segmentAngle = i * (PI_FLOAT * 2 / segments);
        float x = radius * cos(segmentAngle);
        float y = radius * sin(segmentAngle);

        P_C vertex;
        vertex.Position = vec3(x, halfHeight, y);
        vertex.Color = color;
        this->vertices_p_c.push_back(vertex);
    }

    for (int i = startIndex + 1; i <= startIndex + segments; i++)
    {
        if (isTop)
        {
            this->indices.push_back(i);
            this->indices.push_back(i + 1);
            this->indices.push_back(startIndex);
        }
        else
        {
            this->indices.push_back(i + 1);
            this->indices.push_back(i);
            this->indices.push_back(startIndex);
        }
    }
}

} // namespace kd