#pragma once

#include <glm/glm.hpp>
#include <vector>

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

struct MeshData
{
    std::vector<va::P_T_N_T_B> vertices;
    std::vector<unsigned> indices;
};

}