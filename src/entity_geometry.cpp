#include "entity_geometry.h"
#include <cmath>

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

Cylinder::Cylinder(Material *mat, float radius, int segments, int height)
    : radius(radius), segments(segments), height(height)
{
    this->material = mat;
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
    va::Pos vertex0;
    vertex0.Position = vec3(0, halfHeight, 0);
    this->vertices_pos.push_back(vertex0);

    for (int i = 0; i <= segments; i++)
    {
        float segmentAngle = i * (PI_FLOAT * 2 / segments);
        float x = radius * cos(segmentAngle);
        float y = radius * sin(segmentAngle);

        va::Pos vertex;
        vertex.Position = vec3(x, halfHeight, y);
        this->vertices_pos.push_back(vertex);
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