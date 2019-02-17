#pragma once

#include <glm/glm.hpp>
#include <node.h>

namespace kd
{

struct RayCastHit
{
    Node *node = nullptr;
    glm::vec2 point;
    float distance = 0.f;
};

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
    inline glm::vec3 GetPoint(float distance)
    {
        return origin + (direction * distance);
    }
};

} // namespace kd