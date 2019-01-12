#pragma once

#include <vector>
#include <glm.hpp>
#include "common.h"
#include "entity.h"
#include "material.h"

namespace kd
{

namespace va
{

struct P_C
{
    glm::vec3 Position;
    glm::vec3 Color;
};

struct P_T_C
{
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec3 Color;
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

class Mesh : public Entity
{
  public:
    explicit Mesh(Material *mat);
    virtual ~Mesh();

    virtual void Setup() override;
    virtual void Update(float deltaTime) override;
    virtual void AddVertices() = 0;

  protected:
    unsigned vao = 0;
    unsigned vbo = 0;
    unsigned ebo = 0;
    std::vector<va::P_C> vertices_p_c;
    std::vector<unsigned int> indices;

    Material *material = nullptr;

  private:
    DISALLOW_COPY_AND_ASSIGN(Mesh)
};
} // namespace kd