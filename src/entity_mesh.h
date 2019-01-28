#pragma once

#include <vector>
#include <ofbx.h>
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
  explicit Mesh();
  virtual ~Mesh();

  virtual void Setup() override;
  virtual void Update(float deltaTime) override;
  virtual bool Raycast(Ray ray, RayCastHit &rayCastHit);
  bool LoadFromFbx(const std::string& path);

  Material *material = nullptr;
  unsigned vao = 0;
  unsigned ebo = 0;
  std::vector<unsigned int> indices;

public:
  virtual void AddVertices() = 0;
  void LoadNodeRecursively(const ofbx::Object& object);
  void ProcessNode();

  unsigned vbo = 0;
  std::vector<va::Pos> vertices_pos;
  std::vector<va::Pos_Tex> vertices_pos_tex;

private:
  DISALLOW_COPY_AND_ASSIGN(Mesh)
};

} // namespace kd