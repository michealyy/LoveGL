#pragma once

#include <glm/glm.hpp>

struct RectData
{
    glm::vec3 right_bottom;
    glm::vec3 right_top;
    glm::vec3 left_top;
    glm::vec3 left_bottom;
    glm::vec2 uv_right_bottom;
    glm::vec2 uv_right_top;
    glm::vec2 uv_left_top;
    glm::vec2 uv_left_bottom;
    unsigned material_id;
};