#pragma once

#include <vector>
#include "common.h"
#include "entity_mesh.h"

namespace kd
{

class Cone : public Mesh
{
  public:
    /*
    *@param segments must > 8
    */
    explicit Cone(Material *mat, float radius = 0.5f, int segments = 12, int height = 1);
    virtual ~Cone();

    virtual void AddVertices() override;
    float radius = 0.f;
    int segments = 0;
    int height = 0;
    glm::vec3 color{1};

  private:
    DISALLOW_COPY_AND_ASSIGN(Cone)
};

} // namespace kd