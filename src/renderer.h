#pragma once

#include <vector>
#include "common.h"
#include "entity_mesh.h"

namespace kd
{

class Renderer final : public Singleton<Renderer>
{
  //friend Singleton<Renderer>;
public:
  explicit Renderer();
  ~Renderer();
  inline void AddOpaque(Mesh *mesh) { opaque_meshes_.push_back(mesh); }
  inline void AddTransparent(Mesh *mesh) { transparent_meshes_.push_back(mesh); }
  void Render();

private:
  std::vector<Mesh *> opaque_meshes_;
  std::vector<Mesh *> transparent_meshes_;
  void DrawMesh(Mesh *mesh);
  void SortTransparent();

  DISALLOW_COPY_AND_ASSIGN(Renderer)
};

} // namespace kd