#pragma once

#include <vector>
#include "common.h"
#include "vertex_attribute.h"
#include "entity.h"
#include "material.h"
#include "ray.h"

namespace kd
{

struct RayCastHit
{
  Entity *entity = nullptr;
  glm::vec2 point;
  float distance = 0.f;
};

class Mesh : public Entity
{
public:
  explicit Mesh(Material *mat = nullptr);
  virtual ~Mesh();

  virtual void Setup() override;
  virtual void Update(float deltaTime) override;
  virtual bool Raycast(Ray ray, RayCastHit &rayCastHit);
  void SetMesh(const std::string &name);

  Material *material = nullptr;
  unsigned vao = 0;
  unsigned ebo = 0;
  std::vector<unsigned int> indices;

public:
  virtual void AddVertices();

  unsigned vbo = 0;
  std::vector<va::P_T_N_T_B> vertices;
  std::vector<va::Pos> vertices_pos;
  std::vector<va::Pos_Tex> vertices_pos_tex;

private:
  DISALLOW_COPY_AND_ASSIGN(Mesh)
};

} // namespace kd