#pragma once

#include <glm/glm.hpp>

namespace kd
{

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
    inline glm::vec3 GetPoint(float distance)
    {
        return origin + (direction * distance);
    }
};

}