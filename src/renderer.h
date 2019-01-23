#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "common.h"
#include "entity_mesh.h"
#include "vertex_attribute.h"
#include "ui/rect_data.h"

namespace kd
{

class Renderer final : public Singleton<Renderer>
{
  //friend Singleton<Renderer>;
public:
  explicit Renderer();
  ~Renderer();

  void SetupUIBatchRender();
  void AddMesh(Mesh *mesh);
  inline void AddOpaque(Mesh *mesh) { opaque_meshes_.push_back(mesh); }
  inline void AddTransparent(Mesh *mesh) { transparent_meshes_.push_back(mesh); }
  inline void AddUIRect(RectData rect) { rect_list_.push_back(rect); }
  void Render();

private:
  std::vector<Mesh *> opaque_meshes_;
  std::vector<Mesh *> transparent_meshes_;

  //ui batch control
  std::vector<RectData> rect_list_;
  unsigned ui_vao_;
  unsigned ui_vbo_[2];
  static const int VBO_SIZE = 65536;
  va::Pos_Tex ui_vertices_[VBO_SIZE];
  static const int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;
  unsigned short ui_indices_[INDEX_VBO_SIZE];
  unsigned ui_vertex_index_ = 0;
  unsigned ui_element_index_ = 0;
  unsigned int ui_last_material_id_ = 0;
  glm::mat4 ui_project_view_matrix_{1.0f};

  void RenderSkyBox();
  void Render3DObjects();
  void DrawMesh(Mesh *mesh);
  void SortTransparent();
  void BatchRenderUI();
  void GenerateUIDrawCall();

  DISALLOW_COPY_AND_ASSIGN(Renderer)
};

} // namespace kd