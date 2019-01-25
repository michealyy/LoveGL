#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "common.h"
#include "entity_mesh.h"
#include "vertex_attribute.h"
#include "ui/ui_rect.h"

namespace kd
{

class Renderer final : public Singleton<Renderer>
{
  //friend Singleton<Renderer>;
public:
  explicit Renderer();
  ~Renderer();

  void Setup();
  void AddMesh(Mesh *mesh);
  inline void AddOpaque(Mesh *mesh) { opaque_meshes_.push_back(mesh); }
  inline void AddTransparent(Mesh *mesh) { transparent_meshes_.push_back(mesh); }
  inline void AddUIRect(ui::UIRect *rect) { ui_rect_list_.push_back(rect); }
  void Render();

private:
  void SetupUIBatchRender();
  void SetupPostProcessingRenderTexture();
  void RenderSkyBox();
  void Render3DObjects();
  void DrawMesh(Mesh *mesh);
  void SortTransparent();
  void SortUIRectByDepthAndHandleInput();
  void BatchRenderUI();
  void GenerateUIDrawCall(unsigned last_rect_index);

  std::vector<Mesh *> opaque_meshes_;
  std::vector<Mesh *> transparent_meshes_;

  //post processing
  unsigned post_processing_framebuffer_ = 0;
  unsigned post_processing_texture_ = 0;
  unsigned post_processing_render_buffer_ = 0;
  unsigned post_processing_vao_ = 0;
  unsigned post_processing_vbo_ = 0;

  //ui batch control
  std::vector<ui::UIRect *> ui_rect_list_;
  unsigned ui_vao_ = 0;
  unsigned ui_vbo_[2];
  static const int VBO_SIZE = 65536;
  va::Pos_Tex ui_vertices_[VBO_SIZE];
  static const int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;
  unsigned short ui_indices_[INDEX_VBO_SIZE];
  unsigned ui_vertex_index_ = 0;
  unsigned ui_element_index_ = 0;
  unsigned ui_last_material_id_ = 0;
  glm::mat4 ui_project_view_matrix_{1.0f};

  //ui input control
  bool is_left_mouse_btn_press = false;

  DISALLOW_COPY_AND_ASSIGN(Renderer)
};

} // namespace kd