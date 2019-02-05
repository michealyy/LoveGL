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
    glm::vec3 Position{0};
    glm::vec2 TexCoords{0};
    glm::vec3 Normal{0};
    glm::vec3 Tangent{0};
    glm::vec3 Bitangent{0};
    //根据位置纹理计算切线和副切线
    void CalculateTB();
};

} // namespace va

}