#pragma once

#include <vector>
#include "common.h"
#include "entity_mesh.h"

namespace kd
{

class Box : public Mesh
{
  public:
    explicit Box(Material *mat);
    virtual ~Box();
    
    virtual void AddVertices() override;

  private:
    DISALLOW_COPY_AND_ASSIGN(Box)
};

} // namespace kd