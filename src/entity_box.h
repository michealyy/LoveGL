#pragma once

#include <vector>
#include "common.h"
#include "entity_mesh.h"

namespace kd
{

class Box : public Mesh
{
  public:
    explicit Box(Material *mat, float width, float height, float length, glm::vec3 color, float alpha = 1.0f);
    virtual ~Box();

    virtual void AddVertices() override;
    float width = 1.0f;
    float height = 1.0f;
    float length = 1.0f;
    float alpha = 1.0f;
    glm::vec3 color{1};

  private:
    DISALLOW_COPY_AND_ASSIGN(Box)
};

} // namespace kd