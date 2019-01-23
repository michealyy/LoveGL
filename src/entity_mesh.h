#pragma once

#include <vector>
#include "common.h"
#include "vertex_attribute.h"
#include "entity.h"
#include "material.h"

namespace kd
{

class Mesh : public Entity
{
  public:
    explicit Mesh();
    virtual ~Mesh();

    virtual void Setup() override;
    virtual void Update(float deltaTime) override;

    Material *material = nullptr;
    unsigned vao = 0;
    unsigned ebo = 0;
    std::vector<unsigned int> indices;

  protected:
    virtual void AddVertices() = 0;

    unsigned vbo = 0;
    std::vector<va::Pos> vertices_pos;
    std::vector<va::Pos_Tex> vertices_pos_tex;

  private:
    DISALLOW_COPY_AND_ASSIGN(Mesh)
};

} // namespace kd