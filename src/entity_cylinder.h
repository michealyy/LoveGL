#pragma once

#include <vector>
#include "common.h"
#include "entity_mesh.h"

namespace kd
{

class Cylinder : public Mesh
{
  public:
    /*
    *@param segments must > 8
    */
    explicit Cylinder(Material *mat, float radius = 0.5f, int segments = 8, int height = 1);
    virtual ~Cylinder();

    virtual void AddVertices() override;
    float radius = 0.f;
    int segments = 0;
    int height = 0;
    glm::vec3 color{1};

  private:
    void GenerateCap(bool isTop, int startIndex);
    DISALLOW_COPY_AND_ASSIGN(Cylinder)
};

} // namespace kd