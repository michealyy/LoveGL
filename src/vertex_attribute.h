#pragma once

#include <glm/glm.hpp>

namespace kd
{

namespace va
{

struct Pos
{
    glm::vec3 Position;
};

struct Pos_Tex
{
    glm::vec3 Position;
    glm::vec2 TexCoords;
};

struct P_T_N_T_B
{
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

} // namespace va

}